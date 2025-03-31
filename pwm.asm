#include "msp430.h"			; dolaczenie biblioteki do MSP430
;------------------------------------------------------------------------------
;
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
        
        bis.b   #010h,&P4DIR
        bis.b   #010h, &P4SEL
        mov.w   #032h, &TBCCR0
        mov.w   #019h, &TBCCR4
        mov.b	#00h, P4OUT
        
        
        bic     OUTMOD0, &TBCCTL4
        bis     OUTMOD1, &TBCCTL4
        bic     OUTMOD2, &TBCCTL4
        
        bis.w   #GIE,SR                 ;wlacza flage globalnego przerwania znajdujaca sie w rejestrze SR
        
main:	push	SR

timer: 
        bis.b   #010h,P4OUT
        
        bic     #TAIFG, &TACTL
        RETI



loop:   
      ;  cmp     #019h, &TACTL
        ;jz      up      
        
       ; cmp     #032h, &TACTL
      ; jz      down
        
        
        jmp     loop
        
;up:    
      ;  bis.b   #010h,P4OUT
       ; RET

;down:
       ; bic.b   #010h,P4OUT
       ; RET
  
;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			; wektor resetu
	DW	INIT			; adres poczatkowy pamieci programu
        ORG     0FFEAh                  ; wektor przerwania 
        DW      timer
        ORG    0FFE8h              ; wektor przerwania
        DW    PORT_P 
        
	END
