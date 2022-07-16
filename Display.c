#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "Display.h"

#define DDRx	*(port)
#define PORTx	*(port+1)

uint8_t *port;

const uint8_t segmentConfig[][2] = {
	{0x03, 0x01},
	{0x0C, 0x04},
	{0x30, 0x20},
	{0x60, 0x40},
	{0x03, 0x02},
	{0x14, 0x10},
	{0x28, 0x20},
	{0x50, 0x40},
	{0x06, 0x04},
	{0x0C, 0x08},
	{0x24, 0x04},
	{0x48, 0x08},
	{0x0A, 0x02},
	{0x42, 0x02},
	{0x22, 0x20},
	{0x44, 0x04},
	{0x0A, 0x08},
	{0x12, 0x02},
	{0x24, 0x20},
	{0x48, 0x40},
	{0x06, 0x02},
	{0x18, 0x08},
	{0x30, 0x10},
	{0x60, 0x20},
	{0x05, 0x04},
	{0x14, 0x04},
	{0x28, 0x08},
	{0x50, 0x10},
	{0x18, 0x10},
	{0x05, 0x01},
	{0x42, 0x40},
	{0x22, 0x02},
	{0x12, 0x10},
	{0x44, 0x40},
	{0x11, 0x01},
	{0x41, 0x40},
	{0x21, 0x20},
	{0x09, 0x08},
	{0x11, 0x10},
	{0x41, 0x01},
	{0x21, 0x01},
	{0x09, 0x01}
};

const uint8_t charSegments[][8] = {
	{6, 0, 4, 8, 12, 16, 20},
	{2, 4, 8},
	{5, 0, 4, 12, 16, 24},
	{5, 0, 4, 8, 12, 24},
	{4, 4, 8, 20, 24},
	{5, 0, 8, 12, 20, 24},
	{6, 0, 8, 12, 16, 20, 24},
	{3, 0, 4, 8},
	{7, 0, 4, 8, 12, 16, 20, 24},
	{6, 0, 4, 8, 12, 20, 24},
	{6, 0, 4, 8, 16, 20, 24},
	{5, 8, 12, 16, 20, 24},
	{4, 0, 12, 16, 20},
	{5, 4, 8, 12, 16, 24},
	{5, 0, 12, 16, 20, 24},
	{4, 0, 16, 20, 24}
};

const uint8_t messageSegments[][22] = {
	{22, 0, 2, 4, 5, 6, 7, 8, 9, 10, 11, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24, 27},
	{10, 3, 15, 17, 18, 19, 23, 25, 26, 27, 29}
};

void Display_init(uint16_t ptodisp){
	port =(uint8_t*) ptodisp;
	DDRx = 0x00;
	PORTx = 0x00;
}

void Display_number(uint16_t value, uint8_t base){
    uint8_t i, j, num;

    if(base < 2 || base > 16){
        return;
    }

    for( i=0 ; i<4 ; i++){
        num = value%base;
        for ( j = 1 ; j <= charSegments[num][0] ; j++){
            DDRx = segmentConfig[ charSegments[num][j] + i][0];
            PORTx = segmentConfig[ charSegments[num][j] + i][1];
            _delay_ms(1);
        }
        value = value/base;
    }
}

void Display_char(uint8_t place, uint8_t value){
	uint8_t num, j;
	
	if(place > 3){
		return;
	} 
	
	num = value%16;
	for ( j = 1 ; j <= charSegments[num][0] ; j++){
		DDRx = segmentConfig[ charSegments[num][j] + place][0];
		PORTx = segmentConfig[ charSegments[num][j] + place][1];
		_delay_ms(1);
	}
}

void Display_pair(uint8_t left, uint8_t right, uint8_t baseL, uint8_t baseR){
	uint8_t aux, base, num, i, j;
	
	if(baseR < 2 || baseR > 16 || baseL < 2 || baseL > 16){
		return;
	}
	
	aux = right;
	base = baseR;
	for( i=0 ; i<4 ; i++){
		if(i==2){
			aux = left;
			base = baseL;
		}

			
		num = aux%base;
		for ( j = 1 ; j <= charSegments[num][0] ; j++){
			DDRx = segmentConfig[ charSegments[num][j] + i][0];
			PORTx = segmentConfig[ charSegments[num][j] + i][1];
			_delay_ms(1);
		}
		aux = aux/base;
	}
	
	DDRx =	segmentConfig[28][0];
	PORTx =segmentConfig[28][1];
	
	_delay_ms(1);
}

void Display_decimal(uint16_t integer, uint8_t decimal){
	uint8_t aux, num, i, j;
	
	aux = decimal;
	for( i=0 ; i<4 ; i++){
		if(i==1)
			aux = integer;
		
		num = aux%10;
		for ( j = 1 ; j <= charSegments[num][0] ; j++){
			DDRx = segmentConfig[ charSegments[num][j] + i][0];
			PORTx = segmentConfig[ charSegments[num][j] + i][1];
			_delay_ms(1);
		}
		aux = aux/10;
	}
	
	DDRx =	segmentConfig[29][0];
	PORTx =segmentConfig[29][1];
	
	_delay_ms(1);
}

void Display_battery(uint8_t level){
	if(level==0 || level > 3)
		return;
		
	while(level--){
		DDRx =	segmentConfig[37-level][0];
		PORTx =segmentConfig[37-level][1];
		_delay_ms(1);
	}
}

void Display_message(uint8_t message, uint8_t numErr){
	uint8_t i;
	
	if(message > 1)
		return;
	
	for(i=1 ; i<=messageSegments[message][0] ; i++){
		DDRx =	segmentConfig[ messageSegments[message][i] ][0];
		PORTx =segmentConfig[ messageSegments[message][i] ][1];
		_delay_ms(1);
	}
	
	if(message == ERROR){
		Display_char(0, numErr);
	}
}