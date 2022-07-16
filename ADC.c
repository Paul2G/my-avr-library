#include <avr/io.h>
#include "ADC.h"

void ADC_init( void ){
	//Alineado a la izquierda  y voltaje de referencia AVcc
	ADMUX |= (1<<ADLAR) | (1<<REFS0);
	//Trabajando con ADC0
	ADMUX &= ~(0x1F<<MUX0);
	ADCSRB &= ~(1<<MUX5);

	//Activando conversion, auto trigger y preescalador de 8
	ADCSRA |= (1<<ADEN)  | (7<<ADPS0);

	//Activando freerunning
	ADCSRB &= ~(7<<ADTS0);

	//Digital input disable ADC0
	DIDR0 = 0x00;
}

uint8_t ADC_read(uint8_t channel){
	if (channel > 7)
		return 0 ;

	ADMUX &= ~(0x1F<<MUX0);
	ADMUX |= (channel << MUX0);

	ADCSRA |= (1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
	
	return ADCH;
}
