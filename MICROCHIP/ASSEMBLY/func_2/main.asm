;
; AssemblerApplication1.asm
;
; Created: 09/10/2023 20:50:10
; Author : DEE
;


; Replace with your application code
.CSEG
.ORG 0
	jmp main
.CSEG
.ORG 0x46

.equ ledsoff = 0b11111111

.equ leds1 = 0b11111110
.equ leds2 = 0b11111100
.equ leds3 = 0b11111000
.equ leds4 = 0b11110000
.equ leds5 = 0b11100000
.equ leds6 = 0b11000000
.equ leds7 = 0b10000000
.equ leds8 = 0b00000000

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
	ldi r20, 165
	sbis PINA, 0
	rjmp on

	rjmp ciclo

on:

	ldi r16, leds1
	out PORTC, r16

	call delay2s
	
	ldi r16, leds2
	out PORTC, r16

	call delay2s

	ldi r16, leds3
	out PORTC, r16

	call delay2s

	ldi r16, leds4
	out PORTC, r16

	call delay2s

	ldi r16, leds5
	out PORTC, r16

	call delay2s

	ldi r16, leds6
	out PORTC, r16

	call delay2s

	ldi r16, leds7
	out PORTC, r16

	call delay2s

	ldi r16, leds8
	out PORTC, r16

	rjmp off

off:
	call delay3s

	ldi r16, leds7
	out PORTC, r16

	call delay1s

	ldi r16, leds6
	out PORTC, r16

	call delay1s

	ldi r16, leds5
	out PORTC, r16

	call delay1s

	ldi r16, leds4
	out PORTC, r16
	
	call delay1s

	ldi r16, leds3
	out PORTC, r16

	call delay1s

	ldi r16, leds2
	out PORTC, r16

	call delay1s

	ldi r16, leds1
	out PORTC, r16

	call delay1s

	ldi r16, ledsoff
	out PORTC, r16

	call delay1s

	rjmp ciclo

stop:
	ldi r16, ledsoff
	out PORTC, r16
	rjmp ciclo

// delay 1s
delay1s: 
	push r17 //x
	push r18 //y
	push r19 //z

	ldi r19,255
ciclo1: 
	ldi r18,85
ciclo2: 
	ldi r17,255
ciclo3: 
	dec r17
	sbis PINA, 6
	rjmp stop
	brne ciclo3

	dec r18
	sbis PINA, 6
	rjmp stop
	brne ciclo2

	dec r19
	sbis PINA, 6
	rjmp stop
	brne ciclo1

	pop r19
	pop r18
	pop r17
	ret


// delay 2s
delay2s: 
	push r17 //x
	push r18 //y
	push r19 //z

	mov r19, r20
ciclo4: 
	ldi r18,255
ciclo5: 
	ldi r17,255
ciclo6: 
	dec r17
	sbis PINA, 6
	rjmp stop
	brne ciclo6

	dec r18
	sbis PINA, 6
	rjmp stop
	brne ciclo5

	dec r19
	sbis PINA, 6
	rjmp stop
	brne ciclo4

	subi r20, 26

	pop r19
	pop r18
	pop r17
	ret


// delay 3s
delay3s: 
	push r17 //x
	push r18 //y
	push r19 //z

	ldi r19,255
ciclo7: 
	ldi r18,246
ciclo8: 
	ldi r17,255
ciclo9: 
	dec r17
	sbis PINA, 6
	rjmp stop
	brne ciclo9

	dec r18
	sbis PINA, 6
	rjmp stop
	brne ciclo8

	dec r19
	sbis PINA, 6
	rjmp stop
	brne ciclo7

	pop r19
	pop r18
	pop r17
	ret