#include "msp430.h"            

;------------------------------------------------------------------------------
;  
;------------------------------------------------------------------------------
    ORG    01100h              ; poczatkowy adres pamieci programu

INIT:   
    mov.w   #0A00h,SP        
    mov.w   #WDTPW+WDTHOLD,&WDTCTL ; watchdoga off
 
    bis.b   #0FFh,&P2DIR         ; ustawienie wszystkich pinow P2 jako wyjscia
    bis.b   #02h,&P1IE           ; wlaczenie przerwania dla pinu P1.2
    mov.b   #02h,&P1IES          ; ustawienie przerwania na zbocze opadajace dla P1.2
    bis.w   #GIE,SR              ; wlaczenie globalnych przerwan
    ;mov.b   #00h,  P2IFG
        
main:  
    push    SR                
    mov.b   #00h, P2OUT          ; inicjalizacja P2OUT
loop:   
    jmp     loop                 ; nieskonczona petla glowna
   
PORT_P: 
    bic.b   #06h,  P1IFG         
    bit.b   #04h,  P1IN          
    jz  increase                
    jnz decrease                
    RETI                         

increase:
    setc                         
    rrc.b  P2OUT                
    RETI                         

decrease:               
    clrc                         ; wyczyszczenie bitu carry
    rlc.b  P2OUT                 ; rotacja w lewo w carry
    RETI


;------------------------------------------------------------------------------
; Wektory przerwan
;------------------------------------------------------------------------------
    ORG    0FFFEh              ; wektor resetu
    DW    INIT                 
    ORG    0FFE8h              ; wektor przerwania
    DW    PORT_P               
    
    END
