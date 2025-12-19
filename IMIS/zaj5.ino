/*
 * ===================================================================================
 * LABORATORIUM 5: Sterowanie robotem z synchronizacją napędów
 * * Kod realizuje zadania opisane w instrukcji (sekcja 5.1):
 * 1. Sterowanie prędkością (m/s) z regulatorem PID.
 * 2. Synchronizacja napędów (regulator RS w torze sprzężenia krzyżowego).
 * 3. Jazda po łuku, okręgu i ósemce z wykorzystaniem kinematyki odwrotnej.
 * ===================================================================================
 */

// ====================== KROK A: TRYB STARTU ======================
// 1 = Robot startuje od razu z parametrami z setup() (dla jazdy autonomicznej/bez kabla)
// 0 = Robot czeka na komendy 'v' i 'r' z Serial Monitora (do testów na biurku)
#define START_FROM_SETUP 1

// ====================== KROK B: WYBÓR PUNKTU LABORATORIUM (Odkomentuj TYLKO JEDEN) ======================
// --- Punkt 1: Podstawowa jazda (test PID bez zaawansowanej synchronizacji) ---
//#define LAB5_PUNKT_1_PID_DWA_SILNIKI

// --- Punkt 2: Test regulatora synchronizacji RS (Prosty tor, weryfikacja P vs PI) ---
// Realizacja układu z Rys. 28 instrukcji. Sprawdzamy wpływ członu I w RS.
//#define LAB5_PUNKT_2_RS_P_LUB_PI

// --- Punkt 3: Jazda po łuku z synchronizacją ---
// Robot jedzie po zadanym promieniu. RS pilnuje różnicy prędkości wynikającej z geometrii.
//#define LAB5_PUNKT_3_RS_LUK

// --- Punkt 4: Okrąg - połowa szybko, połowa wolno ---
// Robot wykonuje pełne koło: 1/2 okręgu szybko, 1/2 wolno.
//#define LAB5_PUNKT_4_OKRAG_POL_SZYBK

// --- Punkt 5: Ósemka (dwa okręgi o różnych promieniach) ---
// Robot wykonuje nieskończoną pętlę ósemki (jeden łuk w prawo, jeden w lewo).
#define LAB5_PUNKT_5_OSEMKA_ROZNE_R

// ====================== KROK C: KONFIGURACJA RS (Dla Punktu 2) ======================
// Wybór struktury regulatora synchronizacji.
// Instrukcja sugeruje sprawdzenie samego P, a potem PI dla lepszej stabilności (kasowanie uchybu synchronizacji).
//#define RS_TYLKO_P
#define RS_PI   // Domyślnie PI (zalecane dla Punktów 3-5)

// ====================== PARAMETRY STARTOWE (Dla trybu autonomicznego) ======================
const unsigned long START_DELAY_MS = 2000; // Czas na postawienie robota na podłodze po resecie

// Domyślne parametry (mogą być nadpisane przez logikę punktów 4 i 5)
float setup_v_ms = 0.20;   // Prędkość liniowa [m/s]
float setup_r_m  = 1000.0; // Promień skrętu [m] (wartość duża = jazda prosto)

// Parametry dla Punktu 4 (Okrąg zmienna prędkość)
float setup_p4_radius_m = 0.40;
float setup_p4_v_fast_ms = 0.30;
float setup_p4_v_slow_ms = 0.15;

// Parametry dla Punktu 5 (Ósemka)
float setup_p5_v_ms = 0.25;
float setup_p5_r1_m = 0.50; // Promień pierwszej pętli (np. w prawo)
float setup_p5_r2_m = 0.50; // Promień drugiej pętli (np. w lewo)

// ====================== KONFIGURACJA SPRZĘTOWA (PINY) ======================
// Piny zgodne ze schematem robota (mostek H, enkodery)
const int pinSilnikL = 5;   // PWM Lewy
const int pinSilnikP = 6;   // PWM Prawy
const int pinEnkoderL = 2;  // INT0 Lewy (przerwanie)
const int pinEnkoderP = 3;  // INT1 Prawy (przerwanie)
const int ogrnaicz_PWM = 255; // Maksymalne wypełnienie PWM (8-bit)

// ====================== ZMIENNE GLOBALNE ======================
volatile unsigned long licznikTotal_L = 0;
volatile unsigned long licznikTotal_P = 0;

// PARAMETRY FIZYCZNE ROBOTA (Kluczowe dla kinematyki!)
// Należy zmierzyć rzeczywisty rozstaw i średnicę w swoim robocie.
const float rozstawKol_D = 0.14;      // [m] Rozstaw kół
const float srednicaKola = 0.065;     // [m] Średnica koła
const float IMPULSY_NA_OBROT_KOLA = 270.0; // Ilość impulsów na pełny obrót koła (zależy od przekładni)

// ZMIENNE POMIAROWE (Metoda: pomiar czasu między impulsami)
// Pozwala to na szybszą reakcję przy małych prędkościach niż zliczanie impulsów w oknie czasu.
volatile unsigned long pomiar_L = 0;
volatile int numer_L = 0;
volatile int pomiary_L[4] = {0, 0, 0, 0}; // Bufor kołowy (4 magnesy w enkoderze)
unsigned long czasBrakuImpulsu_L = 0;

volatile unsigned long pomiar_P = 0;
volatile int numer_P = 0;
volatile int pomiary_P[4] = {0, 0, 0, 0};
unsigned long czasBrakuImpulsu_P = 0;

// Współczynnik do przeliczania czasu impulsów na RPM/prędkość.
// Wymaga kalibracji eksperymentalnej lub wyliczenia z zegara procesora.
const float wspolczynnik = 15000.0; // Przykładowa stała skalująca

// PRĘDKOŚCI ROBOTA [m/s]
float vZad_L_ms = 0.0; // Zadana prędkość lewego koła
float vZad_P_ms = 0.0; // Zadana prędkość prawego koła
float vAkt_L_ms = 0.0; // Zmierzona prędkość lewego
float vAkt_P_ms = 0.0; // Zmierzona prędkość prawego

float rpmAkt_L = 0.0;
float rpmAkt_P = 0.0;

// KOMENDY STERUJĄCE (Kinematyka robota)
float cmdV_ms = 0.0;    // Prędkość liniowa robota V
float cmdR_m  = 1000.0; // Promień skrętu Rc

// ====================== REGULATORY PID (Prędkość kół) ======================
// Każde koło ma swój niezależny regulator PID.
// Parametry (Kp, Ki, Kd) należy dobrać do konkretnego silnika.
float Kp_L = 150.0, Ki_L = 80.0, Kd_L = 5.0; 
float integral_L = 0.0;
float lastError_L = 0.0;
int pwmWyjscie_L = 0;

float Kp_P = 150.0, Ki_P = 80.0, Kd_P = 5.0;
float integral_P = 0.0;
float lastError_P = 0.0;
int pwmWyjscie_P = 0;

// ====================== REGULATOR RS (Synchronizacja) ======================
// Regulator w torze sprzężenia krzyżowego (zgodnie z Rys. 28 instrukcji).
// Odpowiada za utrzymanie zadanej różnicy prędkości (lub jej brak przy jeździe prosto).
struct RSReg {
  float kp;
  float ki;
  float integral;
  float maxCorr; // Maksymalna korekta prędkości [m/s], jaką może wprowadzić RS
  bool useI;     // Czy używać członu całkującego (Lab 5 pkt 2)
};

// Nastawy RS (do dobrania eksperymentalnie na zajęciach)
RSReg rs = {2.0, 1.0, 0.0, 0.15, true};

// TIMING
const unsigned long okresPID = 20; // 20ms = 50Hz (częstotliwość pętli sterowania)
unsigned long czasOstatniPID = 0;
const unsigned long okresWysylania = 100; // Logowanie co 100ms
unsigned long czasOstatniegoWyslania = 0;

// MASZYNA STANÓW DLA ZADAŃ AUTONOMICZNYCH (Pkt 4 i 5)
#if defined(LAB5_PUNKT_4_OKRAG_POL_SZYBK)
enum P4State { P4_HALF1, P4_HALF2, P4_DONE };
P4State p4State = P4_HALF1;
float p4StartDist = 0.0;
#endif

#if defined(LAB5_PUNKT_5_OSEMKA_ROZNE_R)
enum P5State { P5_LOOP1, P5_LOOP2 };
P5State p5State = P5_LOOP1;
float p5StartDist = 0.0;
#endif

// ====================== FUNKCJE POMOCNICZE ======================

// Obwód koła [m]
float wheelCircumference_m() { return PI * srednicaKola; }

// Konwersja RPM na m/s
float rpmToMs(float rpm) { return (rpm * wheelCircumference_m()) / 60.0; }

// Dystans przypadający na jeden impuls enkodera [m]
float metersPerPulse() { return wheelCircumference_m() / IMPULSY_NA_OBROT_KOLA; }

// Średni dystans przejechany przez robota (średnia arytmetyczna z obu kół)
float sredniDystans_m() {
  unsigned long sum = (licznikTotal_L + licznikTotal_P) / 2;
  return (float)sum * metersPerPulse();
}

// Obliczanie RPM na podstawie tablicy czasów (średnia z 4 pomiarów)
// Instrukcja zaleca uśrednianie z pełnego obrotu wałka enkodera (4 magnesy),
// aby zniwelować błędy wynikające z nierównego rozmieszczenia magnesów.
float obliczRPM(volatile int* tablicaPomiary) {
  float suma = 0;
  for (int i = 0; i < 4; i++) suma += tablicaPomiary[i];
  // Zabezpieczenie przed dzieleniem przez zero (gdy silnik stoi)
  if ((suma == 0) || (suma > 8000)) return 0.0; 
  return wspolczynnik / suma; 
}

// Wykrywanie zatrzymania (timeout)
// Jeśli brak przerwania przez >200ms, uznajemy prędkość za 0.
void obslugaBrakuImpulsu() {
  unsigned long teraz = millis();
  if (teraz - czasBrakuImpulsu_L > 200) { 
    pomiary_L[numer_L] = 9999; // Wpisz dużą wartość, żeby RPM spadło do 0
    numer_L++; if (numer_L > 3) numer_L = 0;
    czasBrakuImpulsu_L = teraz;
  }
  if (teraz - czasBrakuImpulsu_P > 200) {
    pomiary_P[numer_P] = 9999;
    numer_P++; if (numer_P > 3) numer_P = 0;
    czasBrakuImpulsu_P = teraz;
  }
}

// ====================== KINEMATYKA ODWROTNA ======================
// Przelicza zadaną prędkość liniową V i promień skrętu Rc na prędkości kół vL i vP.
// Wzory (2) i (3) z instrukcji (sekcja 4.2).
void bazowePredkosciKol(float v, float Rc, float &vL, float &vP) {
  if (v < 0) v = 0; // W tym labie robot jeździ tylko do przodu

  // Jazda prosto (dla bardzo dużego promienia)
  if (abs(Rc) > 500.0) {
    vL = v;
    vP = v;
    return;
  }

  // Jazda po łuku:
  // omega = v / Rc
  // vP = omega * (Rc - D/2)
  // vL = omega * (Rc + D/2)
  float omega = v / Rc;
  
  vP = omega * (Rc - rozstawKol_D / 2.0);
  vL = omega * (Rc + rozstawKol_D / 2.0);

  // Zabezpieczenie przed ujemnymi prędkościami (brak wstecznego w tym sterowniku)
  if (vP < 0) vP = 0; 
  if (vL < 0) vL = 0;
}

// ====================== REGULATOR SYNCHRONIZACJI (RS) ======================
// Realizuje sprzężenie krzyżowe.
// Cel: utrzymanie zadanej różnicy prędkości (desiredDelta).
// Jeśli desiredDelta = 0 (jazda prosto), RS dąży do vL = vP.
// Jeśli desiredDelta != 0 (jazda po łuku), RS dąży do vL - vP = desiredDelta.
void applyRS(float desiredDelta, float dt) {
  float actualDelta = vAkt_L_ms - vAkt_P_ms; // Rzeczywista różnica
  float e = desiredDelta - actualDelta;      // Uchyb synchronizacji

  // Człon P regulatora RS
  float u = rs.kp * e;

  // Człon I regulatora RS
  // Całkowanie błędu synchronizacji pozwala skompensować stały znos robota (drift).
  if (rs.useI && dt > 0) {
    rs.integral += rs.ki * e * dt;
    // Anti-windup dla RS
    if (rs.integral > rs.maxCorr) rs.integral = rs.maxCorr;
    if (rs.integral < -rs.maxCorr) rs.integral = -rs.maxCorr;
    u += rs.integral;
  }

  // Ograniczenie wpływu RS (nie może on przejąć pełnej kontroli nad prędkością)
  if (u > rs.maxCorr) u = rs.maxCorr;
  if (u < -rs.maxCorr) u = -rs.maxCorr;

  // Aplikacja korekty: 
  // Przyspieszamy jedno koło, zwalniamy drugie o połowę wartości sterowania.
  vZad_L_ms += 0.5 * u;
  vZad_P_ms -= 0.5 * u;
}

// ====================== LOGIKA ZADAŃ AUTONOMICZNYCH ======================
void lab5_updateAutonomous() {
#if defined(LAB5_PUNKT_4_OKRAG_POL_SZYBK)
  // Zadanie: Jazda po okręgu, zmiana prędkości w połowie dystansu.
  float R = abs(setup_p4_radius_m);
  float s = sredniDystans_m() - p4StartDist;
  float halfLen = PI * R;       // Długość połówki
  float fullLen = 2.0 * PI * R; // Długość całości

  if (p4State == P4_HALF1) {
    cmdV_ms = setup_p4_v_fast_ms; // Szybko
    cmdR_m  = setup_p4_radius_m;
    if (s >= halfLen) p4State = P4_HALF2;
  } else if (p4State == P4_HALF2) {
    cmdV_ms = setup_p4_v_slow_ms; // Wolno
    cmdR_m  = setup_p4_radius_m;
    if (s >= fullLen) { p4State = P4_DONE; cmdV_ms = 0; }
  }
#endif

#if defined(LAB5_PUNKT_5_OSEMKA_ROZNE_R)
  // Zadanie: Ósemka (dwa łuki o przeciwnych zwrotach).
  float s = sredniDystans_m() - p5StartDist;
  
  if (p5State == P5_LOOP1) {
    float len = 2.0 * PI * abs(setup_p5_r1_m); // Obwód pętli 1
    cmdV_ms = setup_p5_v_ms;
    cmdR_m  = setup_p5_r1_m; // Skręt w prawo (dodatni promień)
    if (s >= len) {
      p5State = P5_LOOP2;
      p5StartDist = sredniDystans_m(); // Reset licznika dystansu dla nowej pętli
    }
  } else { // LOOP2
    float len = 2.0 * PI * abs(setup_p5_r2_m); // Obwód pętli 2
    cmdV_ms = setup_p5_v_ms;
    cmdR_m  = -setup_p5_r2_m; // Skręt w lewo (ujemny promień)
    if (s >= len) {
      p5State = P5_LOOP1;
      p5StartDist = sredniDystans_m();
    }
  }
#endif
}

// ====================== WYLICZANIE WARTOŚCI ZADANYCH ======================
void lab5_computeSetpoints(float dt) {
  float baseL = 0, baseP = 0;
  
  // 1. Kinematyka: Wyliczamy bazowe prędkości vL i vP dla idealnego robota
  bazowePredkosciKol(cmdV_ms, cmdR_m, baseL, baseP);

  // Przypisanie do zmiennych globalnych
  vZad_L_ms = baseL;
  vZad_P_ms = baseP;

  // 2. Synchronizacja RS (Działa dla punktów 2, 3, 4, 5)
  // Jeśli zdefiniowano użycie RS, modyfikujemy vZad_L/P w oparciu o błędy synchronizacji.
  #if defined(LAB5_PUNKT_2_RS_P_LUB_PI) || defined(LAB5_PUNKT_3_RS_LUK) || defined(LAB5_PUNKT_4_OKRAG_POL_SZYBK) || defined(LAB5_PUNKT_5_OSEMKA_ROZNE_R)
    // Oczekiwana różnica prędkości kół (wynikająca z jazdy po łuku)
    float desiredDelta = baseL - baseP;
    
    // Regulator RS koryguje vZad, aby rzeczywista różnica (vAkt_L - vAkt_P) była równa desiredDelta
    applyRS(desiredDelta, dt);
  #endif
}

// ====================== REGULATOR PID (Implementacja) ======================
// Oblicza sygnał sterujący PWM na podstawie uchybu prędkości.
int liczPID(float zadana_ms, float aktualna_ms, float &calka, float &lastErr,
            float kp, float ki, float kd, float dt) {
  float error = zadana_ms - aktualna_ms;

  // Człon P
  float pTerm = kp * error;

  // Człon I z filtrem przeciwnasyceniowym (Anti-windup)
  // Zapobiega "wind-up", czyli narastaniu całki, gdy PWM jest już na max (255).
  float integralCandidate = calka + ki * error * dt;
  if (integralCandidate < 0) integralCandidate = 0; 
  if (integralCandidate > ogrnaicz_PWM) integralCandidate = ogrnaicz_PWM;
  calka = integralCandidate;
  float iTerm = calka;

  // Człon D
  float dTerm = 0.0;
  if (dt > 0) dTerm = kd * (error - lastErr) / dt;
  lastErr = error;

  // Wyjście regulatora
  float w = pTerm + iTerm + dTerm;

  // Ograniczenie do zakresu PWM
  if (w > ogrnaicz_PWM) w = ogrnaicz_PWM;
  if (w < 0) w = 0;
  
  return int(w);
}

// ====================== PĘTLA REGULACJI (Wywoływana cyklicznie) ======================
void aktualizujPID() {
  unsigned long teraz = millis();
  if (teraz - czasOstatniPID < okresPID) return; // Pilnujemy stałego czasu próbkowania (dt)

  float dt = (float)(teraz - czasOstatniPID) / 1000.0;
  czasOstatniPID = teraz;

  // 1. Pomiar prędkości (konwersja impulsów enkoderów na m/s)
  rpmAkt_L = obliczRPM(pomiary_L);
  rpmAkt_P = obliczRPM(pomiary_P);
  vAkt_L_ms = rpmToMs(rpmAkt_L);
  vAkt_P_ms = rpmToMs(rpmAkt_P);

  // 2. Logika autonomiczna (np. zmiana promienia skrętu w ósemce)
  lab5_updateAutonomous();

  // 3. Obliczenie wartości zadanych (Kinematyka + Korekta RS)
  lab5_computeSetpoints(dt);

  // 4. PID dla silników (obliczenie PWM)
  pwmWyjscie_L = liczPID(vZad_L_ms, vAkt_L_ms, integral_L, lastError_L, Kp_L, Ki_L, Kd_L, dt);
  pwmWyjscie_P = liczPID(vZad_P_ms, vAkt_P_ms, integral_P, lastError_P, Kp_P, Ki_P, Kd_P, dt);

  // 5. Wysterowanie mostka H
  analogWrite(pinSilnikL, pwmWyjscie_L);
  analogWrite(pinSilnikP, pwmWyjscie_P);
}

// ====================== PRZERWANIA (ISR) ======================
void przerwanieL() {
  unsigned long t = millis();
  pomiary_L[numer_L] = int(t - pomiar_L); // Czas od ostatniego impulsu
  pomiar_L = t;
  numer_L++; if (numer_L > 3) numer_L = 0;
  czasBrakuImpulsu_L = t;
  licznikTotal_L++;
}

void przerwanieP() {
  unsigned long t = millis();
  pomiary_P[numer_P] = int(t - pomiar_P);
  pomiar_P = t;
  numer_P++; if (numer_P > 3) numer_P = 0;
  czasBrakuImpulsu_P = t;
  licznikTotal_P++;
}

// ====================== KOMUNIKACJA ======================
void obslugaKomunikacji() {
  if (Serial.available()) {
    char c = Serial.read();
    // Prosty interfejs: 'v' ustawia prędkość, 'r' ustawia promień
    if (c == 'v') cmdV_ms = Serial.parseFloat();
    if (c == 'r') cmdR_m  = Serial.parseFloat();
    // Przełączanie trybu RS (P / PI) w locie (dla Punktu 2)
    if (c == 'R') { 
      rs.useI = !rs.useI;
      Serial.print("RS Mode: "); Serial.println(rs.useI ? "PI" : "P");
    }
  }
}

// ====================== SETUP ======================
void setup() {
  Serial.begin(9600);
  
  // Konfiguracja pinów
  pinMode(pinSilnikL, OUTPUT);
  pinMode(pinSilnikP, OUTPUT);
  pinMode(pinEnkoderL, INPUT_PULLUP);
  pinMode(pinEnkoderP, INPUT_PULLUP);

  // Przerwania zewnętrzne
  attachInterrupt(digitalPinToInterrupt(pinEnkoderL), przerwanieL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinEnkoderP), przerwanieP, CHANGE);

  // Inicjalizacja liczników
  pomiar_L = millis(); pomiar_P = millis();
  czasBrakuImpulsu_L = millis(); czasBrakuImpulsu_P = millis();

  // Konfiguracja flagi członu I w RS
  #if defined(RS_TYLKO_P)
    rs.useI = false;
  #elif defined(RS_PI)
    rs.useI = true;
  #endif
  
  // Reset dystansu dla trybów autonomicznych
  p4StartDist = sredniDystans_m();
  p5StartDist = sredniDystans_m();

  Serial.println("Start Lab 5. Gotowy.");

  // Opcjonalny start od razu (przydatne przy jeździe na podłodze)
  #if START_FROM_SETUP
    delay(START_DELAY_MS); // Czas na odłączenie USB/postawienie robota
    cmdV_ms = setup_v_ms;
    cmdR_m  = setup_r_m;
  #endif
}

// ====================== LOOP ======================
void loop() {
  obslugaKomunikacji();
  obslugaBrakuImpulsu();
  aktualizujPID(); // Główna pętla sterowania (20ms)

  // Logowanie danych dla Serial Plottera (do debugowania wykresów)
  if (millis() - czasOstatniegoWyslania > okresWysylania) {
    czasOstatniegoWyslania = millis();
    Serial.print("vZad:"); Serial.print(cmdV_ms);
    Serial.print(" vL:"); Serial.print(vAkt_L_ms);
    Serial.print(" vP:"); Serial.println(vAkt_P_ms);
  }
}
