#include <Wire.h> // Biblioteka do obsługi I2C
#define MCP4725_ADDR 0x60 // Adres przetwornika MOD-53

volatile float a = 0;

// Funkcja obsługi przetwornika cyfrowo-analogowego
void PA_analog_out(float wart) {
    uint16_t l;
    wart = -wart;
    if (wart > 10) wart = 10;
    if (wart < -10) wart = -10;
    wart = (wart / 20) * 3.3;
    wart += 1.65;
    l = wart * 4095 / 3.3;
    Wire.beginTransmission(MCP4725_ADDR);
    Wire.write((0x0f00 & l) >> 8);
    Wire.write((0x00ff & l));
    Wire.endTransmission();
}

// Funkcja obsługi przetwornika analogowo-cyfrowego
float PA_analog_in(int kanal) {
    return -((analogRead(kanal) / 4095.0) * 16.0 - 8) * 1.55;
}

// Funkcja obsługi przerwania timera TC1 kanał 0
void TC3_Handler() {
    a = PA_analog_in(A0);
    PA_analog_out(a);
    Serial.println(a); // Wysłanie odczytanej wartości na port rs232
    TC_GetStatus(TC1, 0);
}

// Funkcja inicjalizacji timera
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
    uint32_t rc = VARIANT_MCK / 128 / frequency;
    pmc_set_writeprotect(false);
    pmc_enable_periph_clk((uint32_t)irq);
    TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
    TC_SetRA(tc, channel, rc / 2);
    TC_SetRC(tc, channel, rc);
    TC_Start(tc, channel);
    tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
    tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
    NVIC_EnableIRQ(irq);
}

void setup() {
    Wire.begin(); // Inicjalizacja portu szeregowego I2C
    Serial.begin(9600); // Inicjalizacja portu szeregowego rs232
    pinMode(13, OUTPUT); // Ustawienie pinu 13 jako wyjściowego
    startTimer(TC1, 0, TC3_IRQn, 40); // Inicjalizacja timera dla 40Hz
}

void loop() {
}
