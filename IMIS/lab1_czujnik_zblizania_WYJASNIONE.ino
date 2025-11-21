// ========================================================================================
// DEFINICJE PINÓW I ZMIENNYCH GLOBALNYCH
// ========================================================================================

const int led_1 = 11; // Pin cyfrowy 11 (PWM) - Dioda LED 1 (najbliższa przeszkodzie)
const int led_2 = 10; // Pin cyfrowy 10 (PWM) - Dioda LED 2
const int led_3 = 9;  // Pin cyfrowy 9 (PWM) - Dioda LED 3 (najdalsza przeszkodzie)
const int trigPin = 6;  // Pin cyfrowy 6 - TRIG czujnika ultradźwiękowego HC-SR04
const int echoPin = 5;  // Pin cyfrowy 5 - ECHO czujnika ultradźwiękowego HC-SR04
float distance;         // Zmienna do przechowywania aktualnie zmierzonej odległości w cm
const int buzzer = 3;   // Pin cyfrowy 3 (PWM) - Głośnik piezoceramiczny
const int button = 2;   // Pin cyfrowy 2 - Przycisk zmiany trybu (pin obsługujący przerwanie)
int mode = 0;           // Zmienna przechowująca aktualny tryb pracy (0, 1, 2, 3)

// ========================================================================================
// FUNKCJA SETUP - KONFIGURACJA POCZĄTKOWA
// ========================================================================================

void setup()
{
  Serial.begin(9600);           // Inicjalizacja komunikacji szeregowej (do debugowania i wyświetlania)
  pinMode(led_1,OUTPUT);        // Ustawienie pinu LED 1 jako wyjście
  pinMode(led_2,OUTPUT);        // Ustawienie pinu LED 2 jako wyjście
  pinMode(led_3,OUTPUT);        // Ustawienie pinu LED 3 jako wyjście
  pinMode(trigPin, OUTPUT);     // Ustawienie pinu TRIG jako wyjście (wysyłanie impulsu)
  pinMode(echoPin, INPUT);      // Ustawienie pinu ECHO jako wejście (odbiór impulsu)
  pinMode(button, INPUT_PULLUP); // Ustawienie pinu przycisku jako wejście z wewnętrznym rezystorem pull-up
  // Aktywacja przerwania zewnętrznego na pinie button.
  // interrupt_button() zostanie wywołana, gdy stan pinu zmieni się z HIGH na LOW (wciśnięcie przycisku)
  attachInterrupt(digitalPinToInterrupt(button), interrupt_button, FALLING);
}

// ========================================================================================
// FUNKCJA POMIARU ODLEGŁOŚCI
// ========================================================================================

float distance_a(){ //pomiar odległości podany w centymetrach
  // Wysłanie impulsu aktywującego pomiar (krótki impuls LOW, a następnie HIGH)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Pomiar czasu trwania impulsu na pinie ECHO (mikrosekundy)
  // Prędkość dźwięku w cm/µs: 0.0343 (343 m/s)
  // Odległość = (czas * prędkość) / 2 (dzielone przez 2, bo impuls idzie tam i z powrotem)
  distance = (pulseIn(echoPin, HIGH) * 0.0343) / 2;
  
  delay(10); // Krótkie opóźnienie przed następnym pomiarem (można usunąć/skrócić, ale zapobiega to niestabilnym odczytom)
  return distance;
}

// ========================================================================================
// FUNKCJA OBSŁUGI PRZERWANIA (ZMIANA TRYBÓW)
// ========================================================================================

void interrupt_button(){
  // UWAGA: Funkcje przerwań powinny być jak najkrótsze!
  
  Serial.print("Interrupt"); // Komunikat o wywołaniu przerwania
  
  // Cykliczna zmiana trybu: 0 -> 1 -> 2 -> 3 -> 0
  if(mode == 3){
    mode = 0;
  }
  else{
    mode += 1;
  }
  
  // ZAKOMENTOWANY KOD (DOBRZE, ŻE JEST ZAKOMENTOWANY W PRZERWANIU):
  // Serial.println(mode); // Wywołania Serial.print/println są niezalecane w ISR
  // delay(250);          // Funkcja delay NIE MOŻE być używana w ISR, ponieważ blokuje inne operacje!
}

// ========================================================================================
// FUNKCJE TRYBÓW PRACY
// ========================================================================================

/**
 * Tryb 0 (mode_0): Sygnalizacja wizualna za pomocą LED (WŁĄCZ/WYŁĄCZ - cyfrowa).
 * Diody zaświecają się sekwencyjnie, gdy przeszkoda jest blisko.
 * UWAGA: Funkcja distance_a() jest wywoływana wielokrotnie (w każdym 'if'), co spowalnia działanie.
 */
void mode_0(){
  noTone(buzzer); // Wyłączenie dźwięku
  
  // Sprawdzenie odległości i ustawienie stanów diod (analogWrite z wartością 255/0 działa jak digitalWrite HIGH/LOW)
  if (distance_a() < 30 and distance_a() > 20){
    analogWrite(led_3,255); // LED 3 - WŁ.
    analogWrite(led_2,0);   // LED 2 - WYŁ.
    analogWrite(led_1,0);   // LED 1 - WYŁ.
    Serial.println("led1");
    Serial.println(distance_a());
  }
  else if (distance_a() < 20 and distance_a() > 10){
    analogWrite(led_3,255); // LED 3 - WŁ.
    analogWrite(led_2,255); // LED 2 - WŁ.
    analogWrite(led_1,0);   // LED 1 - WYŁ.
    Serial.println("led2");Serial.println(distance_a());
  }
  else if(distance_a() < 10 ){
    analogWrite(led_3,255); // LED 3 - WŁ.
    analogWrite(led_2,255); // LED 2 - WŁ.
    analogWrite(led_1,255); // LED 1 - WŁ.
    Serial.println("led3");Serial.println(distance_a());
  }
  else{ // Odległość > 30 cm
    analogWrite(led_3,0);   // Wszystkie diody WYŁ.
    analogWrite(led_2,0);
    analogWrite(led_1,0);
    Serial.println("led4");Serial.println(distance_a());
  }
}

/**
 * Tryb 1 (mode_1): Sygnalizacja wizualna (PŁYNNA JASNOŚĆ - PWM).
 * Diody rozjaśniają się stopniowo w miarę zbliżania się przeszkody,
 * przechodząc przez poszczególne progi (30-20, 20-10, 10-0).
 * UWAGA: Funkcja distance_a() jest wywoływana wielokrotnie - duży narzut!
 */
void mode_1(){
  noTone(buzzer); // Wyłączenie dźwięku

  // LED 3: Rozjaśnianie w zakresie 30 cm do 20 cm
  if (distance_a() <= 30 and distance_a() >= 20){
    // map(wartość, od_min, od_max, do_min, do_max)
    // 30 cm -> 0 (zgaszona), 20 cm -> 255 (pełna jasność)
    analogWrite(led_3,map(distance_a(),20,30,255,0));
    analogWrite(led_2,0);
    analogWrite(led_1,0);
    Serial.println("2led1");Serial.println(distance_a());
  }
  
  // LED 2: Rozjaśnianie w zakresie 20 cm do 10 cm (LED 3 pozostaje włączona na 255)
  else if (distance_a() <= 20 and distance_a() >= 10){
    analogWrite(led_3,255); // LED 3 - pełna jasność
    // 20 cm -> 0, 10 cm -> 255
    analogWrite(led_2,map(distance_a(),10,20,255,0));
    analogWrite(led_1,0);
    Serial.println("2led2");Serial.println(distance_a());
  }

  // LED 1: Rozjaśnianie w zakresie 10 cm do 0 cm (LED 3 i LED 2 pozostają włączone na 255)
  else if(distance_a() <= 10 ){
    analogWrite(led_3,255); // LED 3 - pełna jasność
    analogWrite(led_2,255); // LED 2 - pełna jasność
    // 10 cm -> 0, 0 cm -> 255
    analogWrite(led_1,map(distance_a(),0,10,255,0));
    Serial.println("2led3");Serial.println(distance_a());
  }

  else{ // Odległość > 30 cm
    analogWrite(led_3,0);
    analogWrite(led_2,0);
    analogWrite(led_1,0);
    Serial.println("2led4");Serial.println(distance_a());
  }
}

/**
 * mode__1 - Funkcja o niezgodnej nazwie i prawdopodobnie błędnej logice.
 * Prawdopodobnie miała to być alternatywna wersja trybu płynnej jasności.
 * UWAGA: Ta funkcja nie jest używana w pętli loop().
 */
void mode__1(){
  noTone(buzzer);
  
  // Jeżeli odległość jest w zakresie 30-20 cm, wszystkie trzy diody próbują się mapować,
  // ale piny led_2 i led_1 dostają błędne dane mapowania (mapują od 20 do 10 i 10 do 0,
  // podczas gdy wejściowa distance_a() jest > 20).
  if (distance_a() <= 30 and distance_a() >= 20){
    analogWrite(led_3,map(distance_a(),20,30,255,0));
    analogWrite(led_2,map(distance_a(),10,20,255,0)); // Błąd logiczny
    analogWrite(led_1,map(distance_a(),0,10,255,0));  // Błąd logiczny
    Serial.println("2led1");Serial.println(distance_a());
  }
  else{
    analogWrite(led_3,0);
    analogWrite(led_2,0);
    analogWrite(led_1,0);Serial.println("2led4");Serial.println(distance_a());
  }
}

/**
 * Tryb 2 (mode_2): Sygnalizacja dźwiękowa - Impulsy (z blokującym delay).
 * Częstotliwość impulsów jest odwrotnie proporcjonalna do odległości
 * (im bliżej, tym impulsy są częstsze, bo delay jest krótszy).
 * UWAGA: Użycie funkcji delay() jest ryzykowne, ponieważ blokuje działanie.
 */
void mode_2(){
  // Pomiary w zakresie 0 cm do 30 cm
  if (distance_a() < 30 and distance_a() >= 0){
    analogWrite(led_3,0); // Wygaszenie diod
    analogWrite(led_2,0);
    analogWrite(led_1,0);
      
    tone(buzzer, 1000);   // Włącz ton o stałej częstotliwości 1000 Hz
    delay(50);            // Ton trwa 50 ms (impuls)
    noTone(buzzer);       // Wyłącz ton
    
    // Obliczenie opóźnienia między impulsami (sekcja ciszy)
    // Wzór: 1 / (31 - odległość) * 1000 -> 
    // Dla 30 cm: 1/(1) * 1000 = 1000 ms przerwy (rzadko)
    // Dla 1 cm: 1/(30) * 1000 ≈ 33 ms przerwy (często)
    // UWAGA: Wynik jest typu int, więc jest zaokrąglany, a obliczenia mogą być nieprecyzyjne.
    // Powinno być to (unsigned long)(1.0 / (31.0 - distance_a()) * 1000.0)
    delay((1/(31-distance_a())*1000));
    
    Serial.println("buzzer 1");Serial.println(distance_a());
  }
  else{
    noTone(buzzer); // Wyłączenie dźwięku, gdy odległość jest poza zakresem
    Serial.println("buzzer 2");Serial.println(distance_a());
  }
}

/**
 * Tryb 3 (mode_3): Sygnalizacja dźwiękowa - Ciągła częstotliwość (zmienna).
 * Częstotliwość jest odwrotnie proporcjonalna do odległości.
 * (im bliżej, tym wyższa częstotliwość/wyższy dźwięk).
 */
void mode_3(){
  // Pomiary w zakresie 0 cm do 30 cm
  if (distance_a() < 30 and distance_a() >= 0){
    analogWrite(led_3,0); // Wygaszenie diod
    analogWrite(led_2,0);
    analogWrite(led_1,0);
    
    // Obliczenie częstotliwości: (1 / (31 - odległość)) * 1000
    // Dla 30 cm: 1000 Hz (niski)
    // Dla 1 cm: ≈ 33 Hz (bardzo niski, raczej nie słyszalny) - TO JEST PŁYTKI DŹWIĘK!
    // Prawdopodobnie celem było: tone(buzzer, map(distance_a(), 0, 30, 2500, 200));
    // Dźwięk trwa 50 ms (parametr opcjonalny funkcji tone)
    tone(buzzer,(1/(31-distance_a())*1000),50);
    
    Serial.println("2buzzer 1");Serial.println(distance_a());
  }
  
  else{
    noTone(buzzer); // Wyłączenie dźwięku, gdy odległość jest poza zakresem
    Serial.println("2buzzer 2");Serial.println(distance_a());
  }
}

// ========================================================================================
// FUNKCJA LOOP - GŁÓWNA PĘTLA PROGRAMU
// ========================================================================================

void loop()
{
  Serial.println(mode); // Wyświetlenie aktualnego trybu pracy
  
  // Logika sterująca wywołaniem odpowiedniej funkcji trybu
  if(mode==3){
    mode_3();
  }
  else if(mode==2){
    mode_2();
  }
  else if(mode==1){
    mode_1();
  }
  else if(mode==0){
    mode_0();
  }
  
  // W każdej iteracji loop() wykonywana jest tylko jedna funkcja trybu.
  // Pętla będzie działać w sposób ciągły.
}
