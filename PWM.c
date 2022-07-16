#include <avr/io.h>
#include "PWM.h"

#define TCCRnA	*(timerDir)
#define TCCRnB	*(timerDir+1)
#define OCRnA	*(timerDir+3)
#define OCRnB	*(timerDir+4)

static uint8_t *timerDir;

void PWM_init ( uint8_t timer )
{
	if(timer==0){
		timerDir = (uint8_t*) 0x44;
		DDRG |= (1<<PG5);
	} else if (timer==2){
		timerDir = (uint8_t*) 0xB0;
		DDRH |= (1<<PH6);
	} else {
		return;
	}

	//Fast PWM modo 7, ancho de pulso 50% por defecto
	//Clear OC2B al comparar
	TCCRnA = (3<<WGM00);
	//Preescalador 1024
	TCCRnB |= (1<<WGM02) | (5<<CS00);

	// Inicializacion del PWM sin sentido
	OCRnA = 2;
	OCRnB = OCRnA/2;
}

void PWM_freq_gen(uint8_t ticks, uint8_t pw){
	/* 	Si "ticks" es mayor que 0 entonces, inicializa y habilita el Generador 
		de Frecuencia del Timer2 con el tope dado por "ticks".
		De lo contrario se requiere deshabilitar el Generador, generando de 
		esta forma el silencio (analogico). */

	if(ticks != 0){
		//Activando salida
		TCCRnA |= (2<<COM0B0);
		//Asignando ticks para hacer funcionar la frecuencia
		OCRnA = ticks;
		OCRnB = (OCRnA*pw) / 255;
	} else {
		//Desactivando salida
		TCCRnA &= ~(3<<COM0B0);
	}
}
