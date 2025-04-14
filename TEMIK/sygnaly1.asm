#include "msp430.h"

;-------------------------------------------------------------------------------
            ORG     01100h
;-------------------------------------------------------------------------------
INIT:       mov.w   #0A00h,SP              ; inicjalizacja stosu
            mov.w   #WDTPW+WDTHOLD,&WDTCTL ; zatrzymanie watchdoga

; konfiguracja enkodera na P1.0 i P1.1
            bic.b   #03h,&P1DIR            ; P1.0 i P1.1 jako wejścia
            bis.b   #03h,&P1REN            ; rezystory podciągające
            bis.b   #03h,&P1OUT

; konfiguracja PWM na P4.4 (TB4)
            bis.b   #010h,&P4DIR
            bis.b   #010h,&P4SEL           ; funkcja alternatywna – PWM

            mov.w   #064h, &TBCCR0         ; okres PWM (np. 100 dzielony zegarem)
            mov.w   #032h, &TBCCR4         ; wypełnienie początkowe 50%
            mov.w   #OUTMOD_7, &TBCCTL4    ; tryb Reset/Set

            mov.w   #TBSSEL_2 + MC_1, &TBCTL ; SMCLK, tryb UP

            mov.w   #0, R5                 ; zmienna: aktualny tryb (0,1,2)
            mov.w   #0, R6                 ; indeks tablicy LUT

; konfiguracja timera do generowania sygnału z LUT
            mov.w   #TASSEL_2 + MC_1 + ID_3, &TACTL ; SMCLK, UP, dzielnik 8
            mov.w   #5000, &TACCR0         ; częstotliwość próbkowania

            bis.w   #CCIE, &TACCTL0        ; włącz przerwanie CCR0
            bis.w   #GIE, SR               ; globalne przerwania

MAIN:       jmp     MAIN                   ; pętla główna

;-------------------------------------------------------------------------------
; Przerwanie od enkodera - P1
;-------------------------------------------------------------------------------
PORT_P:     push.w  SR
            push.w  R10

            mov.b   &P1IN, R10
            bit.b   #01h, R10
            jz      DECREMENT
            inc     R5
            jmp     WRAP

DECREMENT:  dec     R5

WRAP:       cmp     #3, R5
            jl      ENDPORT
            mov.w   #0, R5
            jmp     ENDPORT

ENDPORT:    bic.b   #03h, &P1IFG
            pop.w   R10
            pop.w   SR
            reti

;-------------------------------------------------------------------------------
; Przerwanie CCR0 – generowanie sygnału
;-------------------------------------------------------------------------------
timer:
            push.w  R4
            push.w  R5
            push.w  R6

            cmp     #0, R5
            jeq     PWM_KWADRAT

            cmp     #1, R5
            jeq     PWM_TROJKAT

PWM_SIN:    ; tryb sinusoidalny
            mov.w   LUT_SIN(R6), &TBCCR4
            jmp     NEXT_SAMPLE

PWM_TROJKAT:
            mov.w   LUT_TRI(R6), &TBCCR4
            jmp     NEXT_SAMPLE

PWM_KWADRAT:
            mov.w   #064h, &TBCCR4
            xor.w   #064h, &TBCCR4

NEXT_SAMPLE:
            inc     R6
            cmp     #16, R6
            jl      SKIP_RST
            mov.w   #0, R6

SKIP_RST:
            pop.w   R6
            pop.w   R5
            pop.w   R4
            reti

;-------------------------------------------------------------------------------
; Tablice LUT: 16 kroków (prostych)
;-------------------------------------------------------------------------------
            ORG     01200h

LUT_SIN:    DW 0,13,25,38,50,57,63,65
            DW 63,57,50,38,25,13,0,0

LUT_TRI:    DW 0,8,16,24,32,40,48,56
            DW 64,56,48,40,32,24,16,8

;-------------------------------------------------------------------------------
; WEKTORY PRZERWAŃ
;-------------------------------------------------------------------------------
            ORG     0FFE0h
            DW      timer           ; TACCR0
            ORG     0FFE8h
            DW      PORT_P          ; Port 1
            ORG     0FFFEh
            DW      INIT            ; RESET

            END
