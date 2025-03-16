#include "msp430.h"            

;------------------------------------------------------------------------------
; 
;------------------------------------------------------------------------------
    ORG    01100h              ; początkowy adres pamieci programu

INIT:   
    mov.w   #0A00h,SP        
    mov.w   #WDTPW+WDTHOLD,&WDTCTL ; watchdoga off
 
    bis.b   #0FFh,&P2DIR         ; ustawienie wszystkich pinow P2 jako wyjscia
    bis.b   #08h,&P1IE           ; wlaczenie przerwania dla pinu P1.3
    mov.b   #08h,&P1IES          ; ustawienie przerwania na zbocze opadajace dla P1.3
    bis.w   #GIE,SR              ; wlaczenie globalnych przerwan (flaga GIE w rejestrze SR)
        
main:  
    push    SR                
    mov.b   #01h, P2OUT          ; inicjalizacja P2OUT
loop:   
    jmp     loop                 ; nieskonczona petla glowna
   
PORT_P: 
    bic.b   #0Ch,  P2IFG         ; wyczyszczenie flag przerwan dla pinow P2.2 i P2.3
    bit.b   #04h,  P1IN          ; sprawdzenie stanu pinu P1.2
    jz  increase                 ; jesli P1.2 jest niski skok do increase
    setc                         ; ustawienie bitu carry na 1
    rrc.b  P2OUT                 
    RETI                         

increase:
    cmp.b   #0FFh, P2OUT         ; porownanie wartosci P2OUT z 0FFh
    jz  reset_to_zero            
    clrc                         
    rlc.b  P2OUT                
    RETI                         

decrease:
    cmp.b   #00h, P2OUT          ; porownanie wartosci P2OUT z 0x00
    jz  reset_to_max            
    clrc                         ; wyczyszczenie bitu carry
    rlc.b  P2OUT                 ; rotacja w lewo w carry
    RETI                         

reset_to_zero:
    mov.b #0FFh, P2OUT           
    RETI                         

reset_to_max:
    mov.b  #00h, P2OUT           
    RETI                         

;------------------------------------------------------------------------------
; Wektory przerwan
;------------------------------------------------------------------------------
    ORG    0FFFEh              ; wektor resetu
    DW    INIT                 
    ORG    0FFE8h              ; wektor przerwania
    DW    PORT_P               

    END
