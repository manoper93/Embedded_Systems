#include <avr/io.h>

.extern leituraH
.global analogExtern

analogExtern:
    lds r16, ADCSRA
    ori r16, (1<<ADSC)
    sts ADCSRA, r16

read0:
    lds r16, ADCSRA
    sbrc r16, ADSC
    rjmp read0

	lds r18, ADCH

	sts leituraH, r18

	ret