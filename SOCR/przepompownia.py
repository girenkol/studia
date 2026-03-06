import sys
import time
import threading
from dataclasses import dataclass
from PyQt6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                             QHBoxLayout, QLabel, QProgressBar, QPushButton, 
                             QSlider, QGridLayout, QFrame)
from PyQt6.QtCore import Qt, QTimer

# ==========================================
# 1. KONFIGURACJA FIZYCZNA OBIEKTU
# ==========================================

TANK_CAPACITY = 1000.0   # Maksymalna pojemność zbiornika [L]
PUMP_SMALL_CAP = 50.0    # Wydajność małej pompy [L/s]
PUMP_BIG_CAP = 100.0     # Wydajność dużej pompy [L/s]
MAX_INFLOW = 1000.0       # Maksymalny możliwy dopływ z rury [L/s]

# Progi histerezy zaworu
VALVE_CLOSE_LEVEL = 0.8 * TANK_CAPACITY  # Zamknij przy 80% (4000L)
VALVE_OPEN_LEVEL = 0.7 * TANK_CAPACITY   # Otwórz dopiero jak spadnie do 70% (3500L)

# ==========================================
# 2. DEFINICJE STRUKTUR DANYCH
# ==========================================

@dataclass
class PompaStruct:
    id: int
    wydajnosc: float       # [L/s]
    czy_dziala: bool = False
    czas_pracy: float = 0.0
    czas_odpoczynku: float = 5.0 
    czas_odpoczynku_min: float = 5.0
    czas_pracy_max: float = 20.0
    czujnik_temp: bool = False 

# ==========================================
# 3. LOGIKA STEROWNIKA (Wątek "PLC_PRG")
# ==========================================

class PLCController(threading.Thread):
    def __init__(self):
        super().__init__()
        self.daemon = True 
        self.running = True
        self.lock = threading.Lock()

        self.pompa_tablica = [
            PompaStruct(1, PUMP_SMALL_CAP),
            PompaStruct(2, PUMP_BIG_CAP),
            PompaStruct(3, PUMP_SMALL_CAP),
            PompaStruct(4, PUMP_BIG_CAP)
        ]
        
        # Wejścia/Wyjścia
        self.start_stop = False
        self.zawor = False     # True = ZAMKNIĘTY (BLOKADA)
        self.zawor_wiz = False # True = ZAMKNIĘTY PRZEZ UŻYTKOWNIKA
        self.auto_blokada = False # Flaga wewnętrzna histerezy poziomu
        
        self.woda_in = 0.0     
        self.woda_in_wiz = 0.0 
        
        self.zapelnienie = 0.0       
        self.zapelnienie_max = TANK_CAPACITY
        self.woda_out = 0.0          
        self.woda_delta = 0.0        
        
        self.przepelnienie = False
        self.flaga_przepelnienia = False
        self.licznik = 0.0
        
        self.time_wait = 0.0
        self.time_wait_max = 0.5
        self.flaga_tab = [False] * 4 

    def sprawdz_stan_pracy(self, index):
        p = self.pompa_tablica[index-1]
        if p.czujnik_temp: return False
        
        if p.czy_dziala:
            return True if p.czas_pracy < p.czas_pracy_max else False
        else:
            return True if p.czas_odpoczynku >= p.czas_odpoczynku_min else False

    def run(self):
        last_time = time.perf_counter()
        target_cycle = 0.01

        while self.running:
            current_time = time.perf_counter()
            dt = current_time - last_time
            last_time = current_time
            if dt > 1.0: dt = target_cycle 

            with self.lock:
                # --- 1. OBSŁUGA FLAGI PRZEPEŁNIENIA ---
                if self.przepelnienie:
                    self.flaga_przepelnienia = True

                if self.flaga_przepelnienia:
                    self.licznik += dt
                    if self.licznik >= 2.0: 
                        self.licznik = 0.0
                        self.flaga_przepelnienia = False
                        self.przepelnienie = False

                # --- 2. HISTEREZA ZAWORU ---
                if self.zapelnienie >= VALVE_CLOSE_LEVEL:
                    self.auto_blokada = True 
                elif self.zapelnienie <= VALVE_OPEN_LEVEL:
                    self.auto_blokada = False 

                # --- 3. FINALNE STEROWANIE ZAWOREM ---
                if self.flaga_przepelnienia:
                    self.zawor = True
                elif self.auto_blokada:
                    self.zawor = True
                else:
                    self.zawor = self.zawor_wiz

                # --- 4. FIZYKA DOPŁYWU ---
                if self.zawor: 
                    self.woda_in = 0.0
                else:
                    self.woda_in = self.woda_in_wiz

                if self.start_stop:
                    # --- 5. BILANS WODY ---
                    self.woda_out = 0.0
                    for p in self.pompa_tablica:
                        if p.czy_dziala:
                            self.woda_out += p.wydajnosc

                    delta_vol = (self.woda_in - self.woda_out) * dt
                    self.woda_delta = delta_vol
                    new_level = self.zapelnienie + delta_vol
                    
                    if new_level >= self.zapelnienie_max:
                        self.zapelnienie = self.zapelnienie_max
                        if self.woda_delta > 0: 
                            self.przepelnienie = True
                    elif new_level <= 0:
                        self.zapelnienie = 0.0
                        self.przepelnienie = False
                    else:
                        self.zapelnienie = new_level
                        if self.przepelnienie and self.woda_delta < 0:
                            self.przepelnienie = False

                    # --- 6. LOGIKA STEROWNIKA POMP ---
                    self.flaga_tab = [False] * 4
                    pct = (self.zapelnienie / self.zapelnienie_max) * 100

                    if pct < 10.0:
                         for i in range(4): self.flaga_tab[i] = False
                    
                    elif pct > 70.0:
                        if self.sprawdz_stan_pracy(1): self.flaga_tab[0] = True
                        if self.sprawdz_stan_pracy(3): self.flaga_tab[2] = True
                        if self.sprawdz_stan_pracy(2): self.flaga_tab[1] = True
                        if self.sprawdz_stan_pracy(4): self.flaga_tab[3] = True
                    
                    else:
                        if self.woda_in <= PUMP_SMALL_CAP:
                            if self.sprawdz_stan_pracy(1): self.flaga_tab[0] = True
                            elif self.sprawdz_stan_pracy(3): self.flaga_tab[2] = True
                            elif self.sprawdz_stan_pracy(2) and self.pompa_tablica[1].czas_odpoczynku >= self.pompa_tablica[3].czas_odpoczynku:
                                self.flaga_tab[1] = True
                            elif self.sprawdz_stan_pracy(4): self.flaga_tab[3] = True

                        elif self.woda_in <= PUMP_BIG_CAP:
                            if self.sprawdz_stan_pracy(2): self.flaga_tab[1] = True
                            elif self.sprawdz_stan_pracy(4): self.flaga_tab[3] = True
                            elif self.sprawdz_stan_pracy(1): self.flaga_tab[0] = True
                            elif self.sprawdz_stan_pracy(3): self.flaga_tab[2] = True
                        
                        else:
                            if self.sprawdz_stan_pracy(1): self.flaga_tab[0] = True
                            elif self.sprawdz_stan_pracy(3): self.flaga_tab[2] = True
                            if self.sprawdz_stan_pracy(2): self.flaga_tab[1] = True
                            elif self.sprawdz_stan_pracy(4): self.flaga_tab[3] = True

                    # --- 7. AKTUALIZACJA WYJŚĆ ---
                    for i in range(4):
                        if self.flaga_tab[i] != self.pompa_tablica[i].czy_dziala:
                            if self.time_wait <= 0:
                                self.pompa_tablica[i].czy_dziala = self.flaga_tab[i]
                                self.time_wait = 2 * self.time_wait_max
                        
                        if self.pompa_tablica[i].czy_dziala:
                            self.pompa_tablica[i].czas_pracy += dt
                            self.pompa_tablica[i].czas_odpoczynku = 0
                        else:
                            self.pompa_tablica[i].czas_pracy = 0
                            self.pompa_tablica[i].czas_odpoczynku += dt

                    if self.time_wait > 0:
                        self.time_wait -= dt

            process_duration = time.perf_counter() - current_time
            sleep_needed = target_cycle - process_duration
            if sleep_needed > 0:
                time.sleep(sleep_needed)

# ==========================================
# 4. WIZUALIZACJA (GUI)
# ==========================================

class MainWindow(QMainWindow):
    def __init__(self, controller):
        super().__init__()
        self.plc = controller
        self.setWindowTitle("PRZEPOMPOWNIA WODY - SOCR")
        self.resize(1000, 700)
        
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)
        
        # --- PANEL GÓRNY ---
        top_layout = QHBoxLayout()
        
        # Lewy panel sterowania
        self.control_panel = QFrame()
        self.control_panel.setFrameShape(QFrame.Shape.StyledPanel)
        cp_layout = QVBoxLayout(self.control_panel)
        
        self.lbl_panel_title = QLabel("<b>PANEL OPERATORSKI</b>")
        
        self.btn_start = QPushButton("START / STOP SYSTEMU")
        self.btn_start.setCheckable(True)
        self.btn_start.clicked.connect(self.toggle_start)
        
        self.btn_valve = QPushButton("ZAWÓR DOPŁYWU (Ręczna Blokada)")
        self.btn_valve.setCheckable(True)
        self.btn_valve.clicked.connect(self.toggle_valve)

        self.lbl_valve_status = QLabel("STAN ZAWORU: OTWARTY")
        self.lbl_valve_status.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.lbl_flow = QLabel("Zadany dopływ: 0 L/s")
        self.slider_flow = QSlider(Qt.Orientation.Horizontal)
        self.slider_flow.setRange(0, 100)
        self.slider_flow.valueChanged.connect(self.update_flow_input)
        
        self.lbl_actual_in = QLabel("Aktualny dopływ (Rura): 0.0 L/s")
        self.lbl_actual_out = QLabel("Aktualny odpływ (Pompy): 0.0 L/s")
        
        cp_layout.addWidget(self.lbl_panel_title)
        cp_layout.addWidget(self.btn_start)
        cp_layout.addWidget(self.btn_valve)
        cp_layout.addWidget(self.lbl_valve_status)
        cp_layout.addSpacing(10)
        cp_layout.addWidget(self.lbl_flow)
        cp_layout.addWidget(self.slider_flow)
        cp_layout.addWidget(self.lbl_actual_in)
        cp_layout.addWidget(self.lbl_actual_out)
        cp_layout.addStretch()
        
        # Prawy panel - Zbiornik
        self.tank_panel = QFrame()
        tank_layout = QVBoxLayout(self.tank_panel)
        tank_layout.setAlignment(Qt.AlignmentFlag.AlignHCenter) 
        
        self.lbl_tank_title = QLabel("<b>ZBIORNIK (1000L)</b>")
        self.tank_bar = QProgressBar()
        self.tank_bar.setOrientation(Qt.Orientation.Vertical)
        self.tank_bar.setRange(0, int(TANK_CAPACITY))
        self.tank_bar.setTextVisible(False)
        
        self.lbl_tank_info = QLabel(f"Poziom: 0 / {int(TANK_CAPACITY)} Litrów")
        self.lbl_overflow = QLabel("STAN: OK")
        self.lbl_overflow.setAlignment(Qt.AlignmentFlag.AlignCenter)
        
        tank_layout.addWidget(self.lbl_tank_title)
        tank_layout.addWidget(self.tank_bar)
        tank_layout.addWidget(self.lbl_tank_info)
        tank_layout.addWidget(self.lbl_overflow)
        
        top_layout.addWidget(self.control_panel, 4)
        top_layout.addWidget(self.tank_panel, 3) 
        layout.addLayout(top_layout, 2)
        
        # --- PANEL DOLNY - POMPY ---
        self.lbl_pump_title = QLabel("<b>ZESTAW POMPOWY</b>")
        layout.addWidget(self.lbl_pump_title)
        pumps_layout = QGridLayout()
        self.pump_widgets = []
        self.pump_frames = []
        
        for i in range(4):
            frame = QFrame()
            frame.setFrameShape(QFrame.Shape.Box)
            vbox = QVBoxLayout(frame)
            
            p_obj = self.plc.pompa_tablica[i]
            p_name = f"Pompa P{i+1}"
            
            lbl_title = QLabel(f"{p_name}\n({p_obj.wydajnosc} L/s)")
            lbl_title.setAlignment(Qt.AlignmentFlag.AlignCenter)
            lbl_title.setStyleSheet("font-weight: bold;")
            
            lbl_status = QLabel("STOP")
            lbl_status.setAlignment(Qt.AlignmentFlag.AlignCenter)
            
            lbl_stats = QLabel("Praca: 0.0s\nPrzerwa: 5.0s")
            lbl_stats.setAlignment(Qt.AlignmentFlag.AlignCenter)
            
            btn_temp = QPushButton("AWARIA")
            btn_temp.setCheckable(True)
            btn_temp.clicked.connect(lambda checked, idx=i: self.set_temp_error(idx, checked))

            vbox.addWidget(lbl_title)
            vbox.addWidget(lbl_status)
            vbox.addWidget(lbl_stats)
            vbox.addWidget(btn_temp)
            
            pumps_layout.addWidget(frame, 0, i)
            self.pump_widgets.append({
                "title": lbl_title,
                "status": lbl_status,
                "stats": lbl_stats,
                "temp_btn": btn_temp
            })
            self.pump_frames.append(frame)
            
        layout.addLayout(pumps_layout, 1)

        # Inicjalne style
        self.update_styles()

        self.gui_timer = QTimer()
        self.gui_timer.timeout.connect(self.update_gui)
        self.gui_timer.start(50) 

    # --- ZDARZENIE ZMIANY ROZMIARU OKNA ---
    def resizeEvent(self, event):
        # Wywołaj aktualizację stylów przy każdej zmianie rozmiaru
        self.update_styles()
        super().resizeEvent(event)

    def update_styles(self):
        """Funkcja przeliczająca rozmiary czcionek i elementów na podstawie wielkości okna"""
        
        # Oblicz skalę (bazowa szerokość to 1000px)
        scale = self.width() / 1000.0
        # Zabezpieczenie przed zbyt małą skalą
        if scale < 0.5: scale = 0.5

        # Obliczone rozmiary czcionek
        font_norm = int(12 * scale)
        font_med = int(14 * scale)
        font_big = int(16 * scale)
        font_huge = int(18 * scale)
        padding_btn = int(10 * scale)
        padding_big = int(15 * scale)

        # 1. Start Button
        self.btn_start.setStyleSheet(f"""
            QPushButton {{ padding: {padding_big}px; font-size: {font_big}px; font-weight: bold; background-color: #cccccc; color: black; }}
            QPushButton:checked {{ background-color: #006600; color: white; }}
        """)
        
        # 2. Zawór Button
        self.btn_valve.setStyleSheet(f"""
            QPushButton {{ padding: {padding_btn}px; font-size: {font_norm}px; background-color: #ccffcc; color: black; }}
            QPushButton:checked {{ background-color: #cc0000; color: white; }}
        """)

        # 3. Status Zaworu
        status_color = "#cc0000" if "ZAMKNIĘTY" in self.lbl_valve_status.text() else ("#00cc00" if "OTWARTY" in self.lbl_valve_status.text() else "#99ff99")
        text_color = "white" if "ZAMKNIĘTY" in self.lbl_valve_status.text() else ("white" if "OTWARTY" in self.lbl_valve_status.text() else "black")
        
        # Odświeżamy kolor w update_gui, tutaj tylko rozmiar
        self.lbl_valve_status.setStyleSheet(f"""
            background-color: {status_color}; color: {text_color}; padding: {int(8*scale)}px; border: {max(1, int(2*scale))}px solid gray; font-weight: bold; font-size: {font_med}px;
        """)

        # 4. Labele informacyjne
        self.lbl_flow.setStyleSheet(f"font-size: {font_norm}px;")
        self.lbl_actual_in.setStyleSheet(f"color: blue; font-weight: bold; font-size: {font_med}px;")
        self.lbl_actual_out.setStyleSheet(f"font-size: {font_med}px;")
        self.lbl_panel_title.setStyleSheet(f"font-size: {font_med}px;")
        self.lbl_tank_title.setStyleSheet(f"font-size: {font_med}px;")
        self.lbl_pump_title.setStyleSheet(f"font-size: {font_med}px;")

        # 5. Zbiornik (Pasek) - skalowanie szerokości
        tank_width = int(300 * scale)
        self.tank_bar.setFixedWidth(tank_width)
        self.tank_bar.setStyleSheet(f"""
            QProgressBar {{ border: {max(2, int(3*scale))}px solid #555; border-radius: {int(8*scale)}px; background-color: #f0f0f0; }}
            QProgressBar::chunk {{ background-color: #0099ff; border-radius: {int(4*scale)}px; }}
        """)
        self.lbl_tank_info.setStyleSheet(f"font-size: {font_med}px;")
        
        # 6. Status Alarmu
        overflow_bg = "red" if "PRZEPEŁNIENIE" in self.lbl_overflow.text() else "transparent"
        overflow_fg = "white" if "PRZEPEŁNIENIE" in self.lbl_overflow.text() else "green"
        self.lbl_overflow.setStyleSheet(f"color: {overflow_fg}; background-color: {overflow_bg}; font-weight: bold; font-size: {font_huge}px; border: {max(1, int(2*scale))}px solid green; padding: {int(5*scale)}px;")

        # 7. Pompy (Pętla po widgetach)
        for i, widgets in enumerate(self.pump_widgets):
            widgets["title"].setStyleSheet(f"font-weight: bold; font-size: {font_norm}px;")
            widgets["stats"].setStyleSheet(f"font-size: {int(11*scale)}px;")
            
            # Status pompy (kolory odświeżane w update_gui, tu tylko rozmiar)
            current_status = widgets["status"].text()
            widgets["status"].setStyleSheet(f"""
                padding: {int(8*scale)}px; font-weight: bold; border-radius: {int(4*scale)}px; font-size: {font_norm}px;
                background-color: {'#00ff00' if current_status=='PRACA' else ('orange' if 'BŁĄD' in current_status else '#aa0000')};
                color: {'black' if current_status=='PRACA' or 'BŁĄD' in current_status else 'white'};
            """)
            
            widgets["temp_btn"].setStyleSheet(f"""
                QPushButton {{ background-color: #ffcccc; color: black; border: 1px solid red; font-size: {int(11*scale)}px; padding: {int(5*scale)}px; }}
                QPushButton:checked {{ background-color: red; color: white; font-weight: bold; }}
            """)

    def toggle_start(self, checked):
        with self.plc.lock:
            self.plc.start_stop = checked
            if checked:
                self.btn_start.setText("SYSTEM PRACUJE")
            else:
                self.btn_start.setText("SYSTEM ZATRZYMANY")

    def toggle_valve(self, checked):
        with self.plc.lock:
            self.plc.zawor_wiz = checked 
            if checked:
                self.btn_valve.setText("ŻĄDANIE: OTWÓRZ")
            else:
                self.btn_valve.setText("ŻĄDANIE: ZAMKNIJ")

    def update_flow_input(self, value):
        flow_l_s = (value / 100.0) * MAX_INFLOW
        with self.plc.lock:
            self.plc.woda_in_wiz = flow_l_s
        self.lbl_flow.setText(f"Zadany dopływ: {flow_l_s:.1f} L/s")

    def set_temp_error(self, index, checked):
        with self.plc.lock:
            self.plc.pompa_tablica[index].czujnik_temp = checked

    def update_gui(self):
        with self.plc.lock:
            # Aktualizacja poziomu
            self.tank_bar.setValue(int(self.plc.zapelnienie))
            pct = (self.plc.zapelnienie / TANK_CAPACITY) * 100
            self.lbl_tank_info.setText(f"Poziom: {self.plc.zapelnienie:.0f} L  ({pct:.1f}%)")
            
            # Aby style się nie "psuły" przy odświeżaniu tekstu, wywołujemy update_styles
            # Ale to byłoby zbyt ciężkie robić to co 50ms.
            # Dlatego tutaj aktualizujemy TYLKO te elementy, których kolor zależy od stanu logicznego
            # a rozmiar bierzemy z ostatniej znanej skali (lub wymuszamy przeliczenie przy zmianie stanu)
            
            scale = self.width() / 1000.0
            font_norm = int(12 * scale)
            font_med = int(14 * scale)
            font_huge = int(18 * scale)

            # Status zaworu
            if self.plc.zawor:
                self.lbl_valve_status.setText("ZAWÓR: ZAMKNIĘTY")
                self.lbl_valve_status.setStyleSheet(f"background-color: #cc0000; color: white; padding: {int(8*scale)}px; font-weight: bold; border: 2px solid black; font-size: {font_med}px;")
                
                if self.plc.flaga_przepelnienia:
                     self.btn_valve.setText("BLOKADA (AWARIA!)")
                elif self.plc.auto_blokada:
                     self.btn_valve.setText("BLOKADA (POZIOM > 80%)")
                     if self.plc.zapelnienie < 0.8 * TANK_CAPACITY:
                          self.btn_valve.setText("CZEKAM NA SPADEK < 70%")
                elif not self.plc.zawor_wiz:
                     pass
            else:
                self.lbl_valve_status.setText("ZAWÓR: OTWARTY")
                self.lbl_valve_status.setStyleSheet(f"background-color: #00cc00; color: white; padding: {int(8*scale)}px; font-weight: bold; border: 2px solid black; font-size: {font_med}px;")
                if not self.plc.zawor_wiz:
                    self.btn_valve.setText("ŻĄDANIE: OTWÓRZ")

            # Przepływy
            self.lbl_actual_in.setText(f"Aktualny dopływ: {self.plc.woda_in:.1f} L/s")
            self.lbl_actual_out.setText(f"Aktualny odpływ: {self.plc.woda_out:.1f} L/s")

            # Alarm
            if self.plc.flaga_przepelnienia or self.plc.przepelnienie:
                self.lbl_overflow.setText("!!! PRZEPEŁNIENIE !!!")
                self.lbl_overflow.setStyleSheet(f"color: white; background-color: red; font-weight: bold; font-size: {font_huge}px; border: 3px solid black; padding: {int(5*scale)}px;")
            else:
                self.lbl_overflow.setText("STAN: OK")
                self.lbl_overflow.setStyleSheet(f"color: green; font-weight: bold; font-size: {font_huge}px; border: 2px solid green; padding: {int(5*scale)}px;")

            # Pompy
            for i, p in enumerate(self.plc.pompa_tablica):
                widgets = self.pump_widgets[i]
                if p.czy_dziala:
                    widgets["status"].setText("PRACA")
                    widgets["status"].setStyleSheet(f"background-color: #00ff00; color: black; padding: {int(8*scale)}px; font-weight: bold; border: 1px solid black; font-size: {font_norm}px;")
                else:
                    if p.czujnik_temp:
                         widgets["status"].setText("BŁĄD TEMP")
                         widgets["status"].setStyleSheet(f"background-color: orange; color: black; padding: {int(8*scale)}px; font-weight: bold; border: 1px solid black; font-size: {font_norm}px;")
                    else:
                        widgets["status"].setText("STOP")
                        widgets["status"].setStyleSheet(f"background-color: #aa0000; color: white; padding: {int(8*scale)}px; font-weight: bold; border: 1px solid black; font-size: {font_norm}px;")
                
                widgets["stats"].setText(f"Praca: {p.czas_pracy:.1f}s\nPrzerwa: {p.czas_odpoczynku:.1f}s")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    controller = PLCController()
    controller.start()
    window = MainWindow(controller)
    window.show()
    sys.exit(app.exec())
