#include "msp430.h"			; dolaczenie biblioteki do MSP430
;------------------------------------------------------------------------------
; Rotacja "1" na porcie P2, w lewo
;------------------------------------------------------------------------------
	ORG	01100h			; poczatkowy adres pamieci programu

INIT:	mov.w	#0A00h,SP		; inicjalizacja wsk. stosu
	mov.w	#WDTPW+WDTHOLD,&WDTCTL	; wylaczenie watchdoga
 
        bis.b	#0FFh,&P2DIR
        
        bic     #TASSEL0,&TACTL
        bis     #TASSEL1,&TACTL
        bis     #ID0, &TACTL
        bis     #ID1, &TACTL
        bic     #MC0,&TACTL
        bis     #MC1,&TACTL
        bis     #TAIE, &TACTL
    
        bis.b	#08h,&P1IE
        mov.b   #08h,&P1IES
        
        bis.b	#04h,&P1IE
        mov.b   #04h,&P1IES
    
        bis.w   #GIE,SR                 ;wlacza flage globalnego przerwania znajdujaca sie w rejestrze SR
        
main:	push	SR
	mov.b	#10h, P2OUT		;inicjalizacja 

loop:   
        jmp     loop
        
timer: 
        inc.b   P2OUT
        dadc.b  P2OUT
        
        bic     #TAIFG, &TACTL
        RETI
        
przycisk_reset:
        mov.b   #00h, P2OUT
        mov.b   #0h,P1IFG
        RETI
  

;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			; wektor resetu
	DW	INIT			; adres poczatkowy pamieci programu
        ORG     0FFEAh                  ; wektor przerwania 
        DW      timer
        ORG     0FFE8h                  ; wektor przerwania 
        DW      przycisk_reset

	END
