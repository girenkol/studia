#include "msp430.h"                     ; #define controlled include file

ORG	01100h			; poczatkowy adres pamieci programu

INIT:	mov.w	#0A00h,SP		; inicjalizacja wsk. stosu
	mov.w	#WDTPW+WDTHOLD,&WDTCTL	; wylaczenie watchdoga
	bis.b	#0FFh,&P2DIR		
        bis.b	#08h,P1IE
        bis.b	#08h,P1IES
        bis.w   #GIE, SR
        
        mov.w   #TASSEL1 + ID1 + ID0 + MC0 + MC1 + TAIE, TACTL ;; POPRAWIC BO KROK TIMERA Z 2ms na 1ms
        mov.w   #064h, TACCR0
          
        mov.b   #00h, P2OUT
        
        bis.b   #08h, P6SEL         ;ustawienie p6.3 wejscia adc
        bis.w   #MSC + REF2_5V + REFON, ADC12CTL0
        bis.w   #SHP + CONSEQ1, ADC12CTL1
        bis.b   #SREF0 + INCH0 + INCH1, ADC12MCTL0
        bis.w   #ADC12ON + ENC + ADC12SC, ADC12CTL0
        
        bis.w   #DAC12IR +  DAC12AMP0 + DAC12AMP1 + DAC12AMP2, DAC12_1CTL
        bis.w   #DAC12ENC, DAC12_1CTL
        
        mov.w   #00h, R8
        mov.w   #00h, R9
        
        bis.b   #0FFh,&P2DIR         ; ustawienie wszystkich pinow P2 jako wyjscia
        bis.b   #02h,&P1IE           ; wlaczenie przerwania dla pinu P1.2
        mov.b   #02h,&P1IES          ; ustawienie przerwania na zbocze opadajace dla P1.2
        bis.w   #GIE,SR              ; wlaczenie globalnych przerwan
        bis.b   #01h,&P1IE
        bis.b   #01h,&P1IES
        
        bis.b   #010h, P4DIR
        mov.b   #00h,   P4OUT
loop: 
        jmp loop
PORT_P: 
        bic.b   #06h,  P1IFG ;chyba clear flagi
        bit.b   #04h,  P1IN          
        jz  increase                
        jnz decrease 
        mov.b #00h,P1IFG ;albo tutaj
        RETI  
        
increase:                         
        add.w   #100d,  R8
        ;mov.b   R8, P4OUT ;; TO NIEPOTRZEBNE jest blad odnoscnie wartosci przekazywanej do P4.4 (czyli nie zadaje wartosci zwiekszanej o tyle ile ruszam enkoderem), w funkcji decrease to samo
        mov.b   #00h,P1IFG
        RETI                         

decrease:                               
        sub.w  #100d,  R8  
        ;mov.b  R8, P4OUT ;; TEZ NIEPOTZREBNE tutaj jest druga czesc wypisywania
        mov.b #00h,P1IFG
        RETI        
       
Timer:
        bic.w   #01h, TACTL
        ;sub.w  ADC12MEM0,  R8
        
        ;porownanie
        cmp.w   ADC12MEM0,  R8
        jhs      zjedz  
        jlo     dojedz   
        RETI
        
dojedz:
        bic.b   #010h, P4OUT
        RETI
zjedz:
        bis.b   #010h, P4OUT
        RETI

;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			; wektor resetu
	DW	INIT			; adres poczatkowy pamieci programu            
        ORG     0FFEAh                  ;timer
        DW      Timer
        ORG     0FFE8h                   ;przycisk
        DW      PORT_P
        
        END
