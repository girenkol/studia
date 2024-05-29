class Tasma(object):
    
    pusty_znak = " "
    
    def __init__(self, ciag_tasmy=""):
        self.__tasma = dict(enumerate(ciag_tasmy))
        
    def __str__(self):
        s = ""
        min_uzywany_indeks = min(self.__tasma.keys()) 
        max_uzywany_indeks = max(self.__tasma.keys())
        for i in range(min_uzywany_indeks, max_uzywany_indeks + 1):
            s += self.__tasma.get(i, Tasma.pusty_znak)
        return s    
    
    def __getitem__(self, indeks):
        return self.__tasma.get(indeks, Tasma.pusty_znak)

    def __setitem__(self, pozycja, znak):
        self.__tasma[pozycja] = znak 


class MaszynaTuringa(object):
    
    def __init__(self, tasma="", pusty_znak=" ", stan_poczatkowy="", stany_koncowe=None, funkcja_przejscia=None):
        self.__tasma = Tasma(tasma)
        self.__pozycja_glowicy = 0
        self.__pusty_znak = pusty_znak
        self.__aktualny_stan = stan_poczatkowy
        if funkcja_przejscia is None:
            self.__funkcja_przejscia = {}
        else:
            self.__funkcja_przejscia = funkcja_przejscia
        if stany_koncowe is None:
            self.__stany_koncowe = set()
        else:
            self.__stany_koncowe = set(stany_koncowe)
        self.__kroki = []

    def pobierz_tasme(self): 
        return str(self.__tasma)
    
    def krok(self):
        znak_pod_glowica = self.__tasma[self.__pozycja_glowicy]
        x = (self.__aktualny_stan, znak_pod_glowica)
        if x in self.__funkcja_przejscia:
            y = self.__funkcja_przejscia[x]
            self.__kroki.append((self.__aktualny_stan, self.__pozycja_glowicy, znak_pod_glowica, y[0], y[1], y[2]))
            self.__tasma[self.__pozycja_glowicy] = y[1]
            if y[2] == "R":
                self.__pozycja_glowicy += 1
            elif y[2] == "L":
                self.__pozycja_glowicy -= 1
            self.__aktualny_stan = y[0]
    
    def koncowy(self):
        return self.__aktualny_stan in self.__stany_koncowe
    
    def pobierz_aktualny_stan(self):
        return self.__aktualny_stan
    
    def zapisz_kroki_do_pliku(self, nazwa_pliku):
        with open(nazwa_pliku, "w", encoding="utf-8") as plik:
            for krok in self.__kroki:
                plik.write(f"Stan: {krok[0]}, Pozycja głowicy: {krok[1]}, Znak pod głowicą: {krok[2]}, Nowy stan: {krok[3]}, Nowy znak: {krok[4]}, Ruch: {krok[5]}\n")


def utworz_maszyne_turinga_pesel(pesel):
    wagi = [1, 3, 7, 9, 1, 3, 7, 9, 1, 3]
    przejscia = {}
    stan = 0
    suma = 0

    for i in range(10):
        przejscia[(str(stan), pesel[i])] = (str(stan + 1), pesel[i], "R")
        suma += int(pesel[i]) * wagi[i]
        stan += 1

    cyfra_kontrolna = (10 - suma % 10) % 10
    przejscia[(str(stan), pesel[10])] = ("koncowy", pesel[10], "R") if int(pesel[10]) == cyfra_kontrolna else ("odrzucony", pesel[10], "R")
    
    stany_koncowe = {"koncowy"}
    return MaszynaTuringa(tasma=pesel, stan_poczatkowy="0", stany_koncowe=stany_koncowe, funkcja_przejscia=przejscia)


def sprawdz_pesel_z_maszyna_turinga(pesel):
    if len(pesel) != 11 or not pesel.isdigit():
        return False
    mt = utworz_maszyne_turinga_pesel(pesel)
    while not mt.koncowy():
        mt.krok()
        if mt.pobierz_aktualny_stan() == "odrzucony":
            mt.zapisz_kroki_do_pliku("kroki_maszyny_turinga.txt")
            return False
    mt.zapisz_kroki_do_pliku("kroki_maszyny_turinga.txt")
    return True


# Przykład użycia
pesel = "02070803628"

if sprawdz_pesel_z_maszyna_turinga(pesel):
    print("Numer PESEL jest poprawny.")
else:
    print("Numer PESEL jest niepoprawny.")
