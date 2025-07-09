;
; AssemblerApplication2.asm
;
; Created: 2/7/2022 9:30:50 AM
; Author : Artem Dudko + Nikolas Tapanainen
;


; Replace with your application code

;Z = 31/30, Y = 29/28, X = 27/26, W = 25/24
;14 68 54 59 1c 78 53 53 51 4f 58 5d 45 1c 71 5d 5f 54 55 52 59

.include "m328pbdef.inc"
.list
.org 0x0000

start:


	LDI		WH, HIGH(0X320)	;INITIALIZE EEPROM ADDRESS
	LDI		WL, LOW(0X320)	
	LDI		YL,LOW(0X200)		;INITIALIZE SRAM ADDRESS
	LDI		YH,HIGH(0X200)    
	LDI		R18, 20				;INITIALIZE PROGRAM COUNTER
HERE:
	DEC		R18
	CALL	READROM
	BRNE	HERE

READROM:

	SBIC  EECR,EEPE		;Set control of eeprom to read
	RJMP  READROM
	OUT   EEARH,R25
	OUT   EEARL,R24
	SBI   EECR,EERE		;set control of eeprom to write
	IN    R21,EEDR		;load eeprom data into z
	
	;LDI   R17,0X3C
	;EOR   R25,R17   ; DECRYPT
	;EOR   R24,R17

	ST Y+,R21
RET



