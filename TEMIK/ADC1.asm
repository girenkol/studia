#include "msp430.h"                     ; #define controlled include file

E       EQU BIT1
RS      EQU BIT0
DB4     EQU BIT2
DB5     EQU BIT3
DB6     EQU BIT4
DB7     EQU BIT5
DATA4   EQU 00111100b
FuncSet EQU 00001000b
DDRAMSet EQU 10000000b


        NAME    main                    ; module name

        PUBLIC  main                    ; make the main label vissible
                                        ; outside this module

        ORG     0FFFEh
        DC16    init                    ; set reset vector to 'init' label             
        ORG    0FFE8h              ; wektor przerwania
        DW    PORT_P  
        RSEG    CSTACK                  ; pre-declaration of segment
        RSEG    CODE                    ; place program in 'CODE' segment

init:   MOV     #SFE(CSTACK), SP        ; set up stack
         
main:   NOP                             ; main program
        MOV.W   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
        BIS.B   #0FFh, P2DIR
        BIS.B   #0FFh, P2OUT
        ;-------------------------
        bis.b   #02h,&P1IE           ; wlaczenie przerwania dla pinu P1.2
        mov.b   #02h,&P1IES          ; ustawienie przerwania na zbocze opadajace dla P1.2
        bis.w   #GIE,SR              ; wlaczenie globalnych przerwan
        bis.b       #01h,&P1IE
        bis.b       #01h,&P1IES
        mov.w #030h, R8
        mov.w #030h, R9
        call #init_lcd
        
        
        bis.b   #02h, &P6SEL
        
        ;mov.w   #0404h, &ADC12CTL1
        ;mov.w   #0F3h, &ADC12CTL0
        mov.w   #MSC + ADC12ON + REFON + REF2_5V , &ADC12CTL0
        mov.W #SHP + CONSEQ1 , &ADC12CTL1
        mov.b   #SREF0  + INCH0 , &ADC12MCTL0
        bis.w #ENC, &ADC12CTL0
        bis.w #ADC12SC, &ADC12CTL0
        
        
    ;;dane  
        bis.b #RS, P2OUT

        mov.b #'V', R12
        call #send4bit
        mov.b #'P', R12
        call #send4bit
         mov.b #'O', R12
        call #send4bit
         mov.b #'T', R12
         call #send4bit
         mov.b #'=', R12
         call #send4bit
       
        
   
        
        JMP $
  

        
        
init_lcd:
        call #delay40ms
        clr.b P2OUT
        mov #3, R14
rep3    bis.b #DB4+DB5+E, P2OUT
        call #delay40ms
        bic.b #E, P2OUT
        call #delay40ms
        dec R14
        jnz rep3
        mov.b #DB5+E, P2OUT
        call #delay40ms
        bic.b #E, P2OUT
        call #delay40ms
        ;;od teraz int 4bit
;function set 4bti + nr line+font
        mov.b #00101100b, R12
        call #send4bit
 ;display off     
        mov.b #00001000b, R12
        call #send4bit

; display clear        
        mov.b #E, P2OUT
        call #delay40ms
        bic.b #E, P2OUT
        call #delay40ms
        mov.b #DB4+E, P2OUT
        call #delay40ms
        bic.b #E, P2OUT
        call #delay40ms
 
   ;entry mode set    
        mov.b #110b, R12
        call #send4bit
                 ;display on+cursor on+blinking on     
        mov.b #00001111b, R12
        call #send4bit
   ;set ddram adres           
        mov.b #DDRAMSet+2, R12  ;ustawienie adresu 5
        call #send4bit 
        
        RET
        
send4bit:
        ;procedra przesylajaca 8 bitowa dana w trybie 4 bitowym
        ;wykorzystuje rejestry R12, R13 i R15
        ;przesylana dana w rejestrze R15
        mov.b R12, R13          ;przechowaj oryginal w R13
        bis.b #E, P2OUT         ;ustawienie E dla zbocza opadajacego
        mov.b P2OUT, R11        ;skopiowanie stanu P2 do R11
        bic.b #00111100b, R11   ;wyzerowanie pozycji na ktorych beda dane
        rrc.b R12               ;rotacje aby uzyskac starsze 4 bity
        rrc.b R12
        bic.b #11000011b, R12   ;pozostawienie starszych 4 bit reszta na zero
        bis.b R12, R11          ;skopiowanie starszych 4 bitow do przygotowane R11
        mov.b R11, P2OUT        ;odtworzenie stanu portu P2 z nowymi 4 bitami
        bic.b #E, P2OUT         ;przeslanie danych do LCD
        call #delay40us          ;opoznienie nota katalogowa
        
        mov.b R13, R12
        bis.b #E, P2OUT
        mov.b P2OUT, R11
        bic.b #00111100b, R11
        rlc.b R12
        rlc.b R12
        bic.b #11000011b, R12
        bis.b R12, R11
        mov.b R11, P2OUT
        bic.b #E, P2OUT
        call #delay40us
        RET
        
delay40ms:
        mov #10000, R15
wait40ms:   
        dec R15
        jnz wait40ms
        RET
        
delay40us:
        mov #5, R15
wait40us:   
        dec R15
        jnz wait40us
        RET
        
PORT_P: 
    bic.b   #06h,  P1IFG
    bit.b #01h, P1IN
    jz    reset
    bit.b   #04h,  P1IN          
    jz  increase                
    jnz decrease 
    mov.b #00h,P1IFG
    RETI                         
    
increase:                        
    INC.b  R8
    
    
    cmp #03Ah, R8
    jz resetzero
    
    bic.b #RS, P2OUT
    mov.b #DDRAMSet+51, R12  ;ustawienie adresu 5
    call #send4bit 
    bis.b #RS, P2OUT
    mov.b  R9, R12
    call #send4bit
    mov.b  R8, R12
    call #send4bit 
    mov.b #00h,P1IFG
    RETI                         

decrease:                                       
    DEC.b  R8
    
    cmp #02Fh, R8
    jz resetdziewiec
    
    bic.b #RS, P2OUT
    mov.b #DDRAMSet+51, R12  ;ustawienie adresu 5
    call #send4bit 
    bis.b #RS, P2OUT
    mov.b  R9, R12
    call #send4bit
    mov.b  R8, R12
    call #send4bit  
    mov.b #00h,P1IFG
    RETI
    
resetzero:
    Inc.b R9
    mov.b       #02Fh,R8
    RETI
    
resetdziewiec:
     DEC.b R9
     mov.b       #03Ah,R8
     RETI
     
reset:
    mov.b       #02Fh,R8
    mov.b #00h,P1IFG
    RETI        
       
         
        END
