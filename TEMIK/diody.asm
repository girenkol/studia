#include "msp430.h"			; dolaczenie biblioteki do MSP430
;------------------------------------------------------------------------------
; 
;------------------------------------------------------------------------------
	ORG	01100h			; poczatkowy adres pamieci programu

INIT:	mov.w	#0A00h,SP		; inicjalizacja wsk. stosu
	mov.w	#WDTPW+WDTHOLD,&WDTCTL	; wylaczenie watchdoga
 
        bis.b	#0FFh,&P2DIR   
        bis.b	#08h,&P1IE
        mov.b   #08h,&P1IES
        bis.w   #GIE,SR                 ;wlacza flage globalnego przerwania znajdujaca sie w rejestrze SR
        
main:	push	SR
	mov.b	#01h, P2OUT		;inicjalizacja 
loop:   
        jmp     loop
   
PORT_P: 
        bic.b  #0Ch,  P2IFG  ;czysci flagi przerwania pinu 2.2 i 2.3
        bit.b  #04h,  P1IN
        jz  zwiekszanie
        setc ;set carry na 1
        rrc.b  P2OUT
        RETI
zwiekszanie:
        cmp.b  #0FFh, P2OUT
        jz  reset_zero
        clrc
        rlc.b  P2OUT
        RETI
zmniejszanie:
        cmp.b  #00h, P2OUT
        jz  reset_dziewiec
        clrc
        rlc.b  P2OUT
        RETI
reset_zero:
        mov.b #0FFh, P2OUT
        RETI
reset_dziewiec:
        mov.b  #00h, P2OUT
        RETI

;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			; wektor resetu
	DW	INIT			; adres poczatkowy pamieci programu
  ORG     0FFE8h                  ; wektor przerwania 
  DW      PORT_P

	END
