#include "msp430.h"			; dolaczenie biblioteki do MSP430
;------------------------------------------------------------------------------
; Rotacja "1" na porcie P2, w lewo
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
   
port_A: 
        bit.b   #01h,P1IN
        jz      port_B
        mov.b   #0h,P1IFG
        RETI

port_B:
        bit.b   #02h,P1IN
        jc      incr
        RETI

incr:   
        RRC.b   P2OUT
        
        mov.b   #0h,P1IFG               ; zerowanie flag przerwan IFG
        RETI                            ; zdejmuje ze stosu to co robil w przerwaniu
        
decr: 	
	RLC.b   P2OUT
        
        mov.b   #0h,P1IFG               ; zerowanie flag przerwan IFG
        RETI

;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			; wektor resetu
	DW	INIT			; adres poczatkowy pamieci programu
        ORG     0FFE8h                  ; wektor przerwania 
        DW      port_A

	END
