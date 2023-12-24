
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>

#define SRCLK(x)		PORTD = ( (x) ? (PORTD|(1<<PD5)) : (PORTD&(~(1<<PD5))) )
#define SER(x)			PORTD = ( (x) ? (PORTD|(1<<PD6)) : (PORTD&(~(1<<PD6))) )
#define RCLK(x)			PORTD = ( (x) ? (PORTD|(1<<PD7)) : (PORTD&(~(1<<PD7))) )

const uint8_t m7d[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,0xFF};
typedef struct{
	uint8_t led[4];	
}D7SEG;

D7SEG d7seg;

void SPI_transfer_1_byte(uint8_t byte){
	for(int ibit = 0x80; ibit > 0; ibit>>=1){
		SER(ibit&byte);
		SRCLK(1); SRCLK(0);
	}
}

void Convert_7Seg(uint16_t number){
    d7seg.led[0] = m7d[(uint16_t)number/1000%10];
    d7seg.led[1] = m7d[(uint16_t)number/100%10];
    d7seg.led[2] = m7d[(uint16_t)number/10%10];
    d7seg.led[3] = m7d[(uint16_t)number/1%10];
}

void D7SEG_Display(uint16_t num){
    Convert_7Seg(num);

    SPI_transfer_1_byte(d7seg.led[3]);
	SPI_transfer_1_byte(d7seg.led[2]);
	SPI_transfer_1_byte(d7seg.led[1]);
	SPI_transfer_1_byte(d7seg.led[0]);
    
    RCLK(1); RCLK(0);
}



#endif