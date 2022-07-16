#include <avr/io.h>
#include "UART.h"

//Inicialziador de UART 0
void UART_init( void ){
	//Activando doble velocidad
	UCSR0A |= (1<<U2X0);

	//Activando transmision y recepcion
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);

	//Tamano de 8 bits, sin paridad y un bit de paro
	UCSR0C = (3<<UCSZ00);
	UCSR0B &= ~(1<<UCSZ02);

	UBRR0 = UBRRx;
}

void UART_putchar (char data){
	while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = data;
}

char UART_getch(){
	while(!(UCSR0A & (1<<RXC0)));
		return UDR0;
}

void UART_puts(char *str){
	while(*str) UART_putchar(*(str++));
}

void UART_gets(char *str){
	char letra;
    uint8_t tam = 0;

    do{
        letra = UART_getch();
        UART_putchar(letra);
        /*Condiciones para limites de la cadena en linea de comando*/
        if( (letra == BACK_SPACE) && (tam > 0) ){
            str--;
            tam--;
            UART_putchar(SPACE); 
			UART_putchar(BACK_SPACE);
        } else if ( (letra == BACK_SPACE) && (tam == 0) ){
            UART_putchar(SPACE);
        } else if( (letra != BACK_SPACE) && (tam < STRL) ){
            *(str++) = letra;
            tam++;
        } else if ( (letra != BACK_SPACE) && (tam == STRL) ){
            UART_putchar(BACK_SPACE);
            UART_putchar(SPACE);
            UART_putchar(BACK_SPACE);
        }

    }while(letra != ENTER);

    /*Caracter nulo al final de la cadena*/
    *str = '\0';
}

void UART_setText(uint8_t tipo, uint8_t color)
{
	static char str[] = "\e[0;30m";
	str[5] = color + '0'; 
    str[2] = tipo + '0'; 

    UART_puts(str);
}

void UART_gotoxy(uint8_t x, uint8_t y)
{
	static char str[] = "\x1b[00;00H";
	  
    str[3] = '0' + y % 10;
    y/=10;
    str[2] = '0' + y %10; 

    str[6] = '0' + x % 10;
    x/=10;
    str[5] = '0' + x %10; 

  UART_puts(str);
}

void UART_clrscr(){
	UART_puts("\x1b[2J");
    UART_puts("\x1b[H");
}

void itoa(uint32_t number, char *str, uint8_t base, uint8_t size){
	uint8_t i;
	uint8_t digit, tam = 0;
	uint32_t aux = number;

	/*Para no imprimir 0s demas*/
	if(size != 0)
	tam = size;
	else
	while(aux){
		aux /= base;
		tam++;
	}

	/*Caracter nulo al final de la cadena*/
	*(str + tam) = '\0';

	/*Proceso de conversion*/
	for (i = 1 ; i <= tam ; i++)
	{
		digit = number%base;
		if(digit<10){
			*(str +(tam - i)) = digit + '0';
			} else {
			*(str +(tam - i)) = digit + '7';
		}

		number = number/base;
	}
}

uint32_t atoi(char *str){
	uint32_t out = 0;

	while( *str <= '9' && *str >= '0' ){
		out = out*10 + ( *(str++) - '0');
	}

	return out;
}
