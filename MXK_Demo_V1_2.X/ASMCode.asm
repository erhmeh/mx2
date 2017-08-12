#include "Config.h"
#ifdef ASMMODE
    #include <xc.inc>

    global _LCD_OUT
    global _MYASM
    global _OUTCHAR
    
    NEWLINE EQU 13
    NEWPAGE EQU 12

    PSECT mytext,local,class=CODE,reloc=2

    _OUTCHAR: 
	;Assume that 
	MOVWF _LCD_OUT
	;Wait for write to LCD to complete
	HERE:
	TSTFSZ  _LCD_OUT
	GOTO    HERE
	NOP
	return

    TOLCD MACRO CHAR
	MOVLW   CHAR
	call    _OUTCHAR
	ENDM

    _MYASM:
	TOLCD '1'
	TOLCD '1'
	TOLCD '2'
	TOLCD '0'
	TOLCD '9'
	TOLCD '0'
	TOLCD '6'
	TOLCD '9'
	TOLCD NEWLINE
	TOLCD 'h'
	TOLCD 'i'
	TOLCD '.'

	;LOOP FOREVER
	HERE1:
	GOTO    HERE1
	return
#endif