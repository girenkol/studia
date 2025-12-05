const int PIN_PWM = 6;      // Wyjście PWM do sterowania silnikiem
const int PIN_ENC = 2;      // Wejście enkodera (zewnętrzne przerwanie 0)

const float IMPULSY_NA_OBROT = 15.38; 
const float WSP_PRZEKLADNI = (1.0 * 60.0 * 1000.0) / IMPULSY_NA_OBROT; 
const int MAX_PWM = 255;    
const int CZAS_STOPU_MS = 300; 

// PID
float Kp = 0.5;   // Wzmocnienie proporcjonalne
float Ki = 0.05;  // Wzmocnienie całkujące
float Kd = 0.01;  // Wzmocnienie różniczkujące

float V_ref = 100.0;    // Prędkość zadana [obr/min]
float V_out = 0.0;      // Prędkość mierzona [obr/min]
float uchyb = 0.0;      // Uchyb (e)

float calka_uchybu = 0.0; // Wartość skumulowanej całki uchybu
float uchyb_poprzedni = 0.0;
float Ts = 0.0;             // Czas próbkowania w sekundach

// Zmienne czasowe i wyjście
unsigned long czas_probkowania = 0;
unsigned long poprzedni_czas_probkowania = 0;
int pwm_d = 0;              // Wyjście regulatora (PWM 0-255)

// Zmienne pomiaru prędkości 
volatile unsigned long czas_impulsu = 0; 
const int BUFOR_IMPULSOW = 4;
volatile int pomiary_interwalow[BUFOR_IMPULSOW]; // Bufor interwałów czasowych [ms]
volatile int numer_interwalu = 0; 

void przerwanie_enkoder() {
  unsigned long aktualny_czas = millis();
  
  int interwal = (int)(aktualny_czas - czas_impulsu);
  pomiary_interwalow[numer_interwalu] = interwal;
  
  czas_impulsu = aktualny_czas;
  
  numer_interwalu++;
  if (numer_interwalu >= BUFOR_IMPULSOW) {
    numer_interwalu = 0;
  }
}

void setup() {
  pinMode(PIN_PWM, OUTPUT);
  pinMode(PIN_ENC, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PIN_ENC), przerwanie_enkoder, CHANGE);
  
  Serial.begin(9600);
  
  for (int i = 0; i < BUFOR_IMPULSOW; i++) {
    pomiary_interwalow[i] = CZAS_STOPU_MS + 10;
  }

  poprzedni_czas_probkowania = millis();
}

float predkoscMierzona() { // Jeśli od ostatniego impulsu minęło zbyt dużo czasu, silnik stoi
  if (millis() - czas_impulsu > CZAS_STOPU_MS) return 0.0;

  unsigned long suma_interwalow = 0;
  noInterrupts();
  for (int i = 0; i < BUFOR_IMPULSOW; i++) {
    suma_interwalow += pomiary_interwalow[i];
  }
  interrupts();

  if (suma_interwalow == 0) return 0.0;

  // poprawne przeliczenie: (60000 * N) / (IMPULSY_NA_OBROT * suma_interwalow)
  return (60000.0 * (float)BUFOR_IMPULSOW) / (IMPULSY_NA_OBROT * (float)suma_interwalow);
}



void obslugaKomunikacji() {
  if (Serial.available()) {
    char komenda = Serial.read();
    
    switch (komenda) {
      case 's': // Ustawienie prędkości zadanej
        V_ref = Serial.parseFloat();
        break;
      case 'p': // Ustawienie Kp
        Kp = Serial.parseFloat();
        break;
      case 'i': // Ustawienie Ki (reset całki po zmianie)
        Ki = Serial.parseFloat();
        calka_uchybu = 0.0; 
        break;
      case 'd': // Ustawienie Kd
        Kd = Serial.parseFloat();
        break;
      case '?': // Zwrócenie parametrów
        Serial.print("V_ref: "); Serial.println(V_ref);
        Serial.print("Kp: "); Serial.println(Kp);
        Serial.print("Ki: "); Serial.println(Ki);
        Serial.print("Kd: "); Serial.println(Kd);
        break;
    }
    // Wyczyść bufor wejściowy
    while (Serial.available()) {
      Serial.read();
    }
  }
}

void regulatorPID() {
  czas_probkowania = millis(); // Obliczenie czasu próbkowania Ts w sekundach
  Ts = (float)(czas_probkowania - poprzedni_czas_probkowania) / 1000.0; 
  poprzedni_czas_probkowania = czas_probkowania;

  // Pomiar i uchyb
  V_out = predkoscMierzona();
  uchyb = V_ref - V_out;

  // Człony regulatora
  float w_p = Kp * uchyb;
  float rozniczka = (uchyb - uchyb_poprzedni) / Ts;
  float w_d = Kd * rozniczka;
  
  // Wstępne wyjście regulatora
  float w_regulatora = w_p + (Ki * calka_uchybu) + w_d; 
  int w_calkowite = (int)w_regulatora;

  // Ograniczenie wyjścia (klampowanie)
  int w_limitowane = w_calkowite;

  if (w_limitowane > MAX_PWM) {
    w_limitowane = MAX_PWM;
  } else if (w_limitowane < 0) {
    w_limitowane = 0;
  }
  
  // Anti-Windup: Całkuj uchyb TYLKO, gdy regulator nie jest nasycony
  if (w_calkowite == w_limitowane) {
      calka_uchybu += uchyb * Ts;
  } 

  // Finalizacja
  pwm_d = w_limitowane;
  uchyb_poprzedni = uchyb;
}


void loop() {
  
  obslugaKomunikacji(); 

  regulatorPID();

  // Wysterowanie silnika
  analogWrite(PIN_PWM, pwm_d);

  // Wysłanie danych do Serial Plottera (V_out, V_ref, PWM)
  Serial.print(V_out);
  Serial.print(" ");
  Serial.print(V_ref);
  Serial.print(" ");
  Serial.println(pwm_d);
}
