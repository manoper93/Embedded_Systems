#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void inic();
void stop();
void reativar();
void inv();
void display();
void usart(char *buffer);
void send(char *buffer);


typedef struct USARTRX{
	char rx_buffer;
	unsigned char rx_status;
	unsigned char buffer_receiver: 1;
	unsigned char error: 1;
}USARTRX_st;


volatile USARTRX_st rxusart = {0, 0, 0, 0};
char instrucao[30];

const unsigned char digitos[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0xFF};

volatile unsigned char flag5ms = 0, flag500ms = 0;
unsigned char set_display = 1, sw = 0, swpress = 0, actflag500ms = 0, sentido = 0, st = 0, comando = 0, stmotor = 1, swdisplay = 1;
int numd0 = 0, numd1 = 0, percentagem = 0, bkp = 0;
float valor = 0;


int main(void){

	inic();
	valor = 0;
	while(1){

		usart(instrucao);

		if(rxusart.buffer_receiver != 1){
			sw = PINA & 0b00111111;
			} else {
			sw = comando;
			rxusart.buffer_receiver = 0;
		}


		switch(sw){
			
			case 0b00111110: 	//sw1
			if(swpress == 0){
				if(comando == 0){
					swdisplay = 2;
				}
				
				if(valor < 255){
					valor+=12.8;
					if(valor > 255){
						valor = 255;
					}
					OCR2 = valor;
				}
			}
			swpress = 1;
			break;
			
			case 0b00111101:	 //sw2
			if(swpress == 0){
				if(comando == 0){
					swdisplay = 2;
				}
				
				if(valor > 0){
					if(percentagem >= 99) valor = 256;
					valor-=12.8;
					if(valor < 0){
						valor = 0;
					}
					OCR2 = valor;
				}
			}
			swpress = 1;
			break;
			
			case 0b00101111: 	 //sw5
			if(swpress == 0){
				if(comando == 0){
					swdisplay = 2;
				}
				
				actflag500ms = 1;
				if(PORTB == 0b01011110){
					bkp = valor;
					sentido = 1;
				} else if(PORTB == 0b00111110) {
					bkp = valor;
					sentido = 2;
				}
			}
			swpress = 1;
			break;
			
			case 0b00011111: 	 //sw6
			if(swpress == 0){
				if(comando == 0){
					swdisplay = 2;
				}
				
				st = 1;
				valor = 0;
				OCR2 = valor;
				stop();
			}
			swpress = 1;
			break;
			
			case 0b00111111: 	 // nenhum pressionado
			swpress = 0;
			break;
			
			
		}
		
		if(comando != 0){
			comando=0;
		}
		
		if(actflag500ms > 0){
			inv();
		}

		if(st == 1){
			reativar();
		}
		
		if(flag5ms == 1){
			flag5ms = 0;
			display();
		}
		
	}
}


void usart(char *buffer){
	if(rxusart.buffer_receiver == 1){
		if(rxusart.error == 1){
			rxusart.error = 0;
			} else {
			*buffer = rxusart.rx_buffer;

			switch(*buffer){
				
				case 'P':
				case 'p':
					swdisplay = 1;
					comando = 0b00011111;
					break;
				
				case '+':
					swdisplay = 1;
					comando = 0b00111110;
					break;

				case '-':
					swdisplay = 1;
					comando = 0b00111101;
					break;
				
				case 'I':
				case 'i':
					swdisplay = 1;
					comando = 0b00101111;
					break;

				case 'B':
				case 'b':
					swdisplay = 1;
					if(stmotor > 1){
						sprintf(instrucao, "Velocidade: %i	Sentido: Invertido\r\n", percentagem);
						} else if(stmotor == 1){
						sprintf(instrucao, "Velocidade: %i	Sentido: Normal\r\n", percentagem);
					}
					
					send(instrucao);
					break;
				
				default:
					break;
			}
			
		}
	}
	
}


void send(char *buffer){
	unsigned char i = 0;
	while(buffer[i] != '\0'){
		while((UCSR1A & 1<<UDRE1) == 0);
		UDR1 = buffer[i];
		i++;
	}
}


void inv(){
	PORTB = 0b01111110;
	valor = 0;

	if(flag500ms == 1){
		
		actflag500ms = 0;
		flag500ms = 0;
		
		
		if(sentido == 1){
			sentido = 0;
			stmotor=2;
			PORTB = 0b00111110;
		} else if(sentido == 2){
			sentido = 0;
			stmotor=1;
			PORTB = 0b01011110;
		}
		valor = bkp;
	}

}

void stop(){
	swdisplay = 1;
	if(PORTB == 0b01011110){
		sentido = 1;
	} else {
		sentido = 2;
	}
	PORTB = 0b01111110;
}

void reativar(){
	if(sentido == 1 && valor > 0){
		st = 0;
		PORTB = 0b01011110;
		} else if(sentido == 2 && valor > 0){
		st = 0;
		PORTB = 0b00111110;
	}
}


void display(){
	
	percentagem = (int)((valor / 255.0) * 100);
	
	if(percentagem >= 100){
		percentagem = 99;
	}
	
	if(percentagem > 9){
		numd0 = percentagem % 10;
		numd1 = percentagem / 10;
		} else {
		numd0 = percentagem;
		numd1 = 0;
	}
	
	if(set_display == 1){
		set_display = 2;
		PORTA = 0b11000000;
		PORTC = digitos[numd0];

	} else if(set_display == 2){
		set_display = 3;
		PORTA = 0b10000000;
		PORTC = digitos[numd1];

	} else if(set_display == 3) {
		set_display = 4;
		PORTC = 0xff;
		if(PORTB == 0b00111110 && valor > 0){
			PORTA = 0b01000000;
			PORTC = 0b10111111;
		}
	} else if(set_display == 4) {
		set_display = 1;
		PORTC = 0xff;
		if(swdisplay > 1){
			PORTA = 0b00000000;
			PORTC = digitos[5];
		} else {
			PORTA = 0b00000000;
			PORTC = 0b10100001;
		}
	}
}


ISR(TIMER0_COMP_vect){
	flag5ms = 1;
	
	if(actflag500ms > 0){
		actflag500ms++;
		if(actflag500ms == 100){
			flag500ms = 1;
		}
	}
}


ISR(USART1_RX_vect){
	rxusart.rx_status = UCSR1A;

	if( rxusart.rx_status & ((1<<FE1)|(1<<DOR1)|(1>>UPE1))){
		rxusart.error = 1;
	}
	
	rxusart.rx_buffer = UDR1;
	rxusart.buffer_receiver = 1;
}


void inic(){
	
	DDRA = 0b11000000;
	PORTA = 0b11111111;
	
	DDRB = 0b11100000;
	PORTB = 0b01011110;
	
	DDRC = 0b11111111;
	PORTC = 0b11111111;
	
	OCR2 = 0;
	TCCR2 = 0b01100011;
	
	OCR0  = 77;
	TCCR0  = 0b00001111;
	TIMSK |= 0b00000010;

	UBRR1H = 0;
	UBRR1L = 103;
	UCSR1A = (1<<U2X1);
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
	
	sei();
}