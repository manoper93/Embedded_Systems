#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

void inic();
void usart();
void send(char *buffer);
void motor(unsigned char sentido);
void angulo(int angulo_set);
void angulo_invert(int angulo_set);
void calculo_impulso(int angulo_set);


typedef struct USARTRX{
	char rx_buffer;
	unsigned char rx_status;
	unsigned char buffer_receiver: 1;
	unsigned char error: 1;
}USARTRX_st;


volatile USARTRX_st rxusart = {0, 0, 0, 0};
char instrucao[30];

uint8_t fase[] = {0b00000000,0b00001000, 0b00001100, 0b00000100, 0b00000110, 0b00000010, 0b00000011, 0b00000001, 0b00001001};
uint8_t buffer = 0, sentido = 0, posicao_fase = 0;;
int16_t angulo_atual = 0, impulsos = 0, impulsos_bk = 0, impulsos_def = 0;


int main(void){

	inic();
	
	while(1){

		usart();
		
	}
}

void motor(unsigned char sentido){
	if(sentido == 0){
		
		if(posicao_fase < 8 && impulsos < 20){
			posicao_fase++;
			impulsos++;
			} else if(posicao_fase >= 8 && impulsos < 20){
			posicao_fase = 1;
			impulsos++;
		}
		
		if(impulsos > 20) impulsos = 20; 
		
		angulo_atual = 9*impulsos;
		PORTE = fase[posicao_fase];
		_delay_ms(25);
		
	} else {
		
		if(posicao_fase > 1 && impulsos > -20){
			posicao_fase--;
			impulsos--;
			} else if(posicao_fase <= 1 && impulsos > -20){
			posicao_fase = 8;
			impulsos--;
		}
		
		if(impulsos < -20) impulsos = -20;
		
		angulo_atual = 9*impulsos;
		PORTE = fase[posicao_fase];
		_delay_ms(25);
		
	}
	sprintf(instrucao, "Posicao_fase: %i |  Angulo: %i |  Implusos: %i |  Implusos Guardados: %i \r\n", posicao_fase, angulo_atual, impulsos, impulsos_bk);
	send(instrucao);
}

void angulo(int angulo_set){
	
	calculo_impulso(angulo_set);
	
	while(impulsos < impulsos_def){
		motor(0);
	}
	while(impulsos > impulsos_def){
		motor(1);
	}
	
	sprintf(instrucao, "Posicao_fase: %i |  Angulo: %i |  Implusos: %i |  Implusos Guardados: %i \r\n", posicao_fase, angulo_atual, impulsos, impulsos_bk);
	send(instrucao);
}

void angulo_invert(int angulo_set){
	
	calculo_impulso(-angulo_set);
	
	while(impulsos < impulsos_def){
		motor(0);
	}
	while(impulsos > impulsos_def){
		if(posicao_fase == 0) posicao_fase = 8;
		motor(1);
	}
	
	sprintf(instrucao, "Posicao_fase: %i |  Angulo: %i |  Implusos: %i |  Implusos Guardados: %i \r\n", posicao_fase, angulo_atual, impulsos, impulsos_bk);
	send(instrucao);
}

void calculo_impulso(int angulo_set){
	if(angulo_set > 0){
		
		impulsos_def = angulo_set/9;
		
	} else if(angulo_set < 0){
		
		impulsos_def = angulo_set/9;
		
	} else {
		
		impulsos_def = 0;
		
	}
}

void usart(){
	if(rxusart.buffer_receiver == 1){
		if(rxusart.error == 1){
			rxusart.error = 0;
		} else {
			buffer = rxusart.rx_buffer;

			switch(buffer){
				
				case 'S':
				case 's':
					impulsos_bk = impulsos;
					sprintf(instrucao, "(Define Referencia) Impulso Saved: %i \r\n", impulsos_bk);
					send(instrucao);
					break;
				
				case 'D':
				case 'd':
					angulo(0);
					impulsos_bk = impulsos;
					sprintf(instrucao, "(Default Angulo 0) Impulso Saved:: %i | Impulso Definido: %i \r\n", impulsos_bk, impulsos_def);
					send(instrucao);
					break;
				
				case 'R':
				case 'r':
				sprintf(instrucao, "(Angulo++)\r\n");
				send(instrucao);
				motor(0);
				break;

				case 'L':
				case 'l':
				sprintf(instrucao, "(Angulo--)\r\n");
				send(instrucao);
				motor(1);
				break;
				
				case '1':
				sprintf(instrucao, "(Angulo 180)\r\n");
				send(instrucao);
				angulo(180);
				break;
				
				case '2':
				sprintf(instrucao, "(Angulo -180)\r\n");
				send(instrucao);
				angulo_invert(180);
				break;
				
				case '9':
				sprintf(instrucao, "(Angulo 90)\r\n");
				send(instrucao);
				angulo(90);
				break;
				
				case '0':
				sprintf(instrucao, "(Angulo 0 referencia)\r\n");
				send(instrucao);
				angulo(9*impulsos_bk);
				break;
				
				default:
				break;
			}
		}
	}
	rxusart.buffer_receiver = 0;
	buffer = 0;
}

void send(char *buffer){
	unsigned char i = 0;
	while(buffer[i] != '\0'){
		while((UCSR1A & 1<<UDRE1) == 0);
		UDR1 = buffer[i];
		i++;
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
	
	DDRE = 0b00001111;
	PORTE = 0b00001111;
	
	UBRR1H = 0;
	UBRR1L = 103;
	UCSR1A = (1<<U2X1);
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
	
	sei();
}