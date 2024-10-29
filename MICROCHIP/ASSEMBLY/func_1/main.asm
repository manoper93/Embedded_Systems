;
; 2NB_PL1.asm
;
; Created: 02/10/2023 20:44:16
; Author : DEE
;


; Replace with your application code
.CSEG
.ORG 0
	jmp main
.CSEG
.ORG 0x46

inic:
    ldi r16, 0b00000000
	out DDRA, r16
	ldi r16, 0b11111111
	out DDRC, r16
	out PORTC, r16
	ret

main:
	ldi r16, 0xff
	out spl, r16
	ldi r16, 0x10
	out sph, r16

	call inic

ciclo:
	sbis PINA, 0
	rjmp led1

	sbis PINA, 1
	rjmp led2

	sbis PINA, 2
	rjmp led3

	sbis PINA, 3
	rjmp led4

	sbis PINA, 6
	rjmp off

	rjmp ciclo

led1:
	ldi r16, 0b11100111
	rjmp output

led2:
	ldi r16, 0b11011011
	rjmp output

led3:
	ldi r16, 0b10111101
	rjmp output

led4:
	ldi r16, 0b01111110
	rjmp output

off:
	ldi r16, 0b11111111
	rjmp output

output:
	out PORTC, r16
	rjmp ciclo
