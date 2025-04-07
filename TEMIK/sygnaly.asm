#include "msp430.h"			; dolaczenie biblioteki do MSP430
;------------------------------------------------------------------------------
; 
;------------------------------------------------------------------------------
	ORG	01100h			; poczatkowy adres pamieci programu
INIT:	mov.w	#0A00h,SP		; inicjalizacja wsk. stosu
	mov.w	#WDTPW+WDTHOLD,&WDTCTL	; wylaczenie watchdoga

	
        bis.w   #GIE, SR
        mov.w   #TASSEL1|ID1|ID0|MC0|MC1, TACTL
        mov.w   #0C350h, TACCR0
        
        bis.b   #010h, P1DIR
        bic.b   #010h, P1OUT
        bis.b   #080h, P5DIR
        bic.b   #080h, P5OUT
        
        mov.b   #00h, P2OUT
        bis.b   #0FFh,&P2DIR 
        bis.w   #TAIE, TACTL
loop:   
	jmp loop
ZEGAR:
        bit.b   #0FFh, P2OUT
        jz      maxuj
        bit.b   #00h, P2OUT
        jz      zeruj
        
        
        bic.w   #TAIFG, TACTL
        RETI
        
zeruj:  
        mov.b   #00h, P2OUT
        RET
maxuj:  
        mov.b   #0FFh, P2OUT
        RET
;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			; wektor resetu
	DW	INIT			; adres poczatkowy pamieci programu
        ORG     0FFEAh
        DW      ZEGAR
	END
