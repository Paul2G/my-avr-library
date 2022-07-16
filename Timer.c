#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"

#define TCCRnA	*(timerDir)
#define TCCRnB	*(timerDir+1)
#define OCRnA	*(timerDir+3)
#define TIMSKn	*( timerMsk )

static volatile uint8_t flag;
static volatile uint64_t mSecCnt;

static uint16_t period;

static uint8_t *timerDir, *timerMsk;

void Timer_init ( uint8_t timer, uint16_t msLapse)
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
	
	period = msLapse;
	
	//CTC, OC2A toggle on compare
	TCCRnA = (1<<COM0A0) | (2<<WGM00);
	TCCRnB = (3<<CS00); // clk/64

	//Interrupcion activada para A
	TIMSKn = (1<<OCIE0A);

	//Cada 250 tiempos sucede un millisegundo
	OCRnA = 250 - 1;
	
	sei();
}

uint8_t Timer_flag ( void )
{
    if( flag ) {
        flag=0;
        return 1;
    }
    else {
        return 0;
    }
}

uint64_t getMillis(){
    return mSecCnt; 
}

void setMillis(uint64_t millis){
	mSecCnt = millis;
}

ISR (TIMER0_COMPA_vect)
{
	mSecCnt++;
	if( mSecCnt%period == 0)
		flag=1;
}

ISR (TIMER2_COMPA_vect)
{
    mSecCnt++; 
    if( mSecCnt%period == 0)
		flag=1;
}
