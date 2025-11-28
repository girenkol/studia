// -------------------- KONFIGURACJA SPRZĘTOWA --------------------
const int pinSterowanieSilnikiem = 5;     // pin PWM (Pin 5) do tranzystora MOSFET/sterownika silnika.
const int pinEnkoder             = 2;     // wejscie z enkodera. Musi być to pin wspierający przerwania zewnętrzne (Pin 2 lub 3 w Arduino Uno/Nano).
int wypelnieniePWM = 150;                 // Początkowa wartość wypełnienia PWM (0-255).

// -------------------- ZMIENNE WSPÓLNE DLA RÓŻNYCH METOD --------------------

// Zmienna dla metody zliczania impulsów (nieużywana w aktywnym pomiarze prędkości poniżej, ale zostawiona)
unsigned long liczbaImpulsow = 0;        

// Zmienne VOLATILE – kluczowe dla wymiany danych między przerwaniem a programem głównym
volatile unsigned long czas    = 0;       // Aktualny czas (millis()) zapisany w przerwaniu.
volatile unsigned long pomiar = 0;       // Czas ostatniego impulsu (millis()) – używany jako punkt odniesienia.
volatile int numer            = 0;       // Indeks do tablicy pomiarów.
volatile int pomiary[4]       = {0, 0, 0, 0};  // Tablica do przechowywania czasów pomiędzy 4 kolejnymi impulsami (okresy).

// -------------------- ZMIENNE DO WYSYŁANIA DANYCH --------------------
const unsigned long okresWysylania = 100;  // Okres (ms) co jaki wysyłamy dane na Serial Plotter (co 100 ms).
unsigned long czasOstatniegoWyslania = 0;  // Czas ostatniej wysyłki.

// -------------------- OBSŁUGA TERMINALA --------------------
void obslugaPWMzTerminala() {
  // Sprawdza, czy dostępne są dane do odczytu w buforze Serial
  if (Serial.available() > 0) {
    int nowaWartosc = Serial.parseInt(); // Odczytuje liczbę całkowitą

    // Sprawdzenie, czy wartość jest w poprawnym zakresie dla PWM (0-255)
    if (nowaWartosc >= 0 && nowaWartosc <= 255) {
      wypelnieniePWM = nowaWartosc;
      analogWrite(pinSterowanieSilnikiem, wypelnieniePWM); // Ustawia nowe wypełnienie PWM

      // Wypisuje potwierdzenie na terminal
      Serial.print("PWM = ");
      Serial.println(wypelnieniePWM);
    } else {
      Serial.println("Blad: podaj liczbe z zakresu 0–255.");
    }
  }
}

// -------------------- FUNKCJA PRZERWANIA (AKTYWNA W SETUP) --------------------
// UWAGA: Ta wersja jest nieaktywna w setup, ale stanowi podstawę ćwiczenia 2.

/*
void przerwanie() {
  czas = millis();                      // Pobranie aktualnego czasu
  pomiary[numer] = int(czas - pomiar); // Obliczenie okresu (różnicy czasu) od poprzedniego impulsu
  pomiar = czas;                       // Zapamiętanie aktualnego czasu jako punktu odniesienia
  numer++;                              // Przejście do kolejnego elementu tablicy
  if (numer > 3) numer = 0;             // Zawinięcie indeksu (użycie tablicy cyklicznej)
}
*/

// -------------------- POMIAR PRĘDKOŚCI --------------------
// Oblicza prędkość na podstawie uśrednionego czasu 4 ostatnich impulsów.
float predkoscMierzona() {
  float suma = 0;
  // Sumuje 4 ostatnio zmierzone okresy
  for (int i = 0; i < 4; i++) suma += pomiary[i];
  
  // Stała konwersyjna (zależna od rozdzielczości enkodera, przełożenia i jednostek)
  // 1371.42857143 jest wynikiem 60 * 1000 / (i * N_imp), gdzie 60*1000 to konwersja ms na min
  // i/lub odzwierciedla rozdzielczość enkodera dla jednego obrotu wału wyjściowego.
  const float wspolczynnik = 1371.42857143;

  // Jeśli silnik stoi (suma jest 0) lub okres jest bardzo duży (>2000 ms), zwraca 0.
  // W przeciwnym razie oblicza: prędkość ~ 1 / okres (prędkość = wspolczynnik / suma)
  return ((suma == 0) || (suma > 2000)) ? 0 : wspolczynnik / suma;
}

void pomiarPredkosciISerialPlotter() {
  czas = millis(); // Pobranie aktualnego czasu
  
  // Detekcja zatrzymania: jeśli od ostatniego impulsu minęło ponad 300 ms,
  // sztucznie wprowadza dużą wartość do tablicy pomiarów. W ten sposób funkcja predkoscMierzona()
  // zwróci 0, co oznacza zatrzymanie silnika.
  if (czas - pomiar > 300) {
    pomiary[numer] = 999;
    numer++;
    if (numer > 3) numer = 0;
    pomiar = czas;
  }

  // Wysyłanie danych do Serial Plotter co 100 ms
  if (czas - czasOstatniegoWyslania >= okresWysylania) {
    float predkosc = predkoscMierzona(); // Obliczenie prędkości (obr/min)
    
    // Wypisanie wartości (zwykle format Serial Plotter wymaga spacji, np. "0 [predkosc]").
    Serial.print("0 ");
    Serial.println(predkosc);

    czasOstatniegoWyslania = czas;
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(9600); // Inicjalizacja komunikacji szeregowej

  // KONFIGURACJA STEROWANIA SILNIKIEM
  pinMode(pinSterowanieSilnikiem, OUTPUT); // Pin PWM jako wyjście
  analogWrite(pinSterowanieSilnikiem, wypelnieniePWM); // Ustawienie początkowego wypełnienia PWM

  // KONFIGURACJA ENKODERA
  // Ustawienie wejścia enkodera z wewnętrznym rezystorem podciągającym (pull-up).
  // Jest to konieczne, jeśli enkoder ma wyjście typu otwarty kolektor.
  pinMode(pinEnkoder, INPUT_PULLUP); 

  // Inicjalizacja Przerwania:
  // Uruchomienie funkcji 'przerwanie' za każdym razem, gdy na Pinie 2
  // nastąpi zmiana stanu (zbocze narastające lub opadające - CHANGE).
  // Pamiętaj, aby odkomentować funkcję przerwania na górze kodu, jeśli jej używasz.
  // attachInterrupt(digitalPinToInterrupt(pinEnkoder), przerwanie, CHANGE);
  
  // Komunikat startowy
  Serial.println("Start. Podaj wartosc PWM 0–255 (NO LINE ENDING).");
}

// -------------------- LOOP --------------------
void loop() {
  // Funkcja do sterowania silnikiem przez wpisanie wartości PWM w terminalu
  obslugaPWMzTerminala();

  // Funkcja do pomiaru prędkości, detekcji zatrzymania i wysyłania danych na Serial Plotter
  pomiarPredkosciISerialPlotter(); 
}
