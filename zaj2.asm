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
	mov.b	#0FFh, P2OUT		;inicjalizacja 
loop:   
        jmp     loop

PORT_P: 
	;bic.b	#08h, P1IFG		
        bit.b	#01h,P4IN
        jz      incr			;zwiekszenie
        
        bit.b	08h,P4IN
        jz      decr			;zmniejszenie

        mov.b   #0h,P1IFG
        RETI
      

incr:   
	cmp.b	#09h, P2OUT
	jz	resetz
        inc.b	P2OUT
        mov.b   #0h,P1IFG               ; zerowanie flag przerwan IFG
        RETI                            ; zdejmuje ze stosu to co robil w przerwaniu
        
decr: 	
	cmp.b	#00h, P2OUT
	jz	resetd
        dec.b	P2OUT
        mov.b   #0h,P1IFG               ; zerowanie flag przerwan IFG
        RETI

resetd:	
	mov.b #0Ah, P2OUT               ; dalem A bo wtedy z 0 dec idzie do 9
	RETI
resetz
	mov.b #00h, P2OUT
	RETI
;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			; wektor resetu
	DW	INIT			; adres poczatkowy pamieci programu
        ORG     0FFE8h                  ; wektor przerwania 
        DW      PORT_P

	END
