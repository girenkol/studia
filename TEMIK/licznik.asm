#include "msp430.h"			
;------------------------------------------------------------------------------
; 
;------------------------------------------------------------------------------
	ORG	01100h			

INIT:	mov.w	#0A00h,SP		
	mov.w	#WDTPW+WDTHOLD,&WDTCTL	
 
        bis.b	#0FFh,&P2DIR
        
        bic     #TASSEL0,&TACTL
        bis     #TASSEL1,&TACTL
        bis     #ID0, &TACTL
        bis     #ID1, &TACTL
        bic     #MC0,&TACTL
        bis     #MC1,&TACTL
        bis     #TAIE, &TACTL
    
        bis.b	#08h,&P4IE
        mov.b   #08h,&P4IES
        
        bis.b	#04h,&P4IE
        mov.b   #04h,&P4IES
    
        bis.w   #GIE,SR                 
        
main:	push	SR
	mov.b	#10h, P2OUT		

loop:   
        jmp     loop
        
timer: 
        inc.b   P2OUT
        dadc.b  P2OUT
        
        bic     #TAIFG, &TACTL
        RETI
        
przycisk_reset:
        mov.b   #00h, P2OUT
        mov.b   #0h,P4IFG
        RET
  

;------------------------------------------------------------------------------
;       Wektory przerwan
;------------------------------------------------------------------------------
	ORG	0FFFEh			
	DW	INIT			
        ORG     0FFEAh                  
        DW      timer
        ORG     0FFE8h                  
        DW      przycisk_reset

	END
