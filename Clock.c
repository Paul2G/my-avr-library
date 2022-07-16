#include <avr/interrupt.h>
#include <inttypes.h>
#include "Clock.h"
#include "UART.h"

#define TCCRnA	*(timerDir)
#define TCCRnB	*(timerDir+1)
#define OCRnA	*(timerDir+3)
#define TIMSKn	*( timerMsk )

static volatile uint8_t SecFlag;
static volatile uint64_t mSecCnt;
static volatile uint8_t secs;
static volatile uint8_t mins;
static volatile uint8_t hour;

static uint8_t *timerDir, *timerMsk;

void Clock_init ( uint8_t timer )
{
	if(timer==0){
		timerDir = (uint8_t*) 0x44;
		timerMsk = (uint8_t*) 0x6E;
	} else if (timer==2){
		timerDir = (uint8_t*) 0xB0;
		timerMsk = (uint8_t*) 0x70;
	} else {
		return;
	}

    DDRB = (1<<PB7);
    PORTB = ^(1<<PB7);

	//CTC
	TCCRnA = (1<<COM0A0) | (3<<WGM00);
	TCCRnB = (3<<CS00); //clk/64

    //Interrupcion activada para A
	TIMSKn = (1<<OCIE0A);

	//Cada 250 tiempos sucede un millisegundo
	OCRnA = 250 - 1;

    sei(); /* habilita interrupciones (global) */
}

uint8_t Clock_secFlag ( void )
{
    if( SecFlag ) {
        SecFlag=0;
        return 1;
    }
    else {
        return 0;
    }
}

void Clock_set (uint8_t hr, uint8_t mn, uint8_t sc){
    secs = sc;
    mins = mn;
    hour = hr;
}

void Clock_puts (){
    char str[6];
    itoa(hour, str, 10, 2);
    UART_puts(str);

    itoa(mins, str, 10, 2);
    UART_putchar(':');
    UART_puts(str);

    itoa(secs, str, 10, 2);
    UART_putchar(':');
    UART_puts(str);
}

uint64_t millis(){
    return mSecCnt; 
}

uint8_t seconds(){
	return secs;
}

uint8_t minutes(){
	return mins;
}

uint8_t hours(){
	return hour;
}

ISR (TIMER0_COMPA_vect)
{
    mSecCnt++; /* Incrementa contador de milisegundos */
    if( mSecCnt%1000 == 0)
    {
        SecFlag=1; /* Bandera de Segundos */

        secs++;
        if(secs>=60){
            secs=0;
            mins++;
            if(mins>=60){
                mins=0;
                hour++;
                if(hour>=24){
                    hour=0;
                }
            }
        }
    }
}

ISR (TIMER2_COMPA_vect)
{
	mSecCnt++; /* Incrementa contador de milisegundos */
	if( mSecCnt%1000 == 0)
	{
		SecFlag=1; /* Bandera de Segundos */

		secs++;
		if(secs>=60){
			secs=0;
			mins++;
			if(mins>=60){
				mins=0;
				hour++;
				if(hour>=24){
					hour=0;
				}
			}
		}
	}
}
