/*
    Creater: Monna(Dang Hoang Anh Chuong)
    Date: 12/2023

    Contact:
    Email: dangtonny0819@gmail.com
    FB: https://www.facebook.com/chuongkomap123/

    This library is still in progress....
*/

#ifndef __ADC_H__
#define __ADC_H__

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>


// ADMUX
// Vref
#define ADC_AREF			0x00
#define ADC_AVCC			0x40
#define ADC_INTERNAL		0xC0	

// Adjust result
#define ADC_LEFT            0x20
#define ADC_RIGH            0x00

//Chanel (Single Ended input)
#define ADC0                0x00
#define ADC1                0x01
#define ADC2                0x02
#define ADC3                0x03
#define ADC4                0x04
#define ADC5                0x05
#define ADC6                0x06
#define ADC7                0x07
#define INTERNAL_TEMP       0x08
#define VBG_1_1V            0x0E
#define GND                 0x0F

// Prescaler
/*
	               F_system
	F	=    ------------------      = 1kHz
			 2*Presaler*(1+OCR1A)
*/
//ADCSRA
#define ADC_CLK_DIV_2	    0x01
#define ADC_CLK_DIV_4	    0x02
#define ADC_CLK_DIV_8	    0x03
#define ADC_CLK_DIV_16	    0x04
#define ADC_CLK_DIV_32	    0x05
#define ADC_CLK_DIV_64	    0x06
#define ADC_CLK_DIV_128	    0x07

//ADCSRA
#define ADC_ENA             0x80
#define ADC_START           0x40
#define ADC_AUTO            0x20
#define ADC_INT_FLAG        0x10
#define ADC_INT_ENA         0x08


// Auto trigger mode 
// Has effect when ADATE in ADCSRA is set to 1
#define FREE_RUNNING	    0x00
#define ANA_COMP		    0x01
#define EXT_INT_0		    0x02
#define T0_COMP_A		    0x03
#define T0_OVF			    0x04
#define T1_COMP_B		    0x05
#define T1_OVF			    0x06
#define T1_CAP_E		    0x07


void Init_ADC(uint8_t PRE,uint8_t VREF){
    // ADC Control and Status Register A
	ADCSRA &= ~ADC_ENA;     // disable Adc module
	ADCSRA &= 0xF8;         // clear older prescaler
	ADCSRA |= PRE;          // set prescaler = 111(128) (125kHz)
	
	//ADC Multiplexer Selection Register
	ADMUX	&= 0x3F;        //clear older vref
	ADMUX	|= VREF;        // set reference voltage AVCC

    // ADC Control and Status Register A
	ADCSRA	|= ADC_ENA;     // enable Adc module
}

void Auto_trig_ADC(uint8_t SOURCE){
    ADCSRA |= ADC_AUTO;     // enable auto triger source
	ADCSRA |= ADC_INT_ENA;  // enable ADC complete interrupt
	ADCSRB  &= 0xF8;        // clear older source
	ADCSRB	|= SOURCE; 
}
void Set_channel_ADC(uint8_t CHAN){
	ADMUX &= 0xF0;          // clear older channel
	ADMUX |= CHAN;
}

void Start_conversion_ADC(){
    ADCSRA |= ADC_START;     // Starts a new conversion
}

bool Is_complete_ADC(){
    if(ADCSRA&ADC_INT_FLAG) // if the conversion is done
        return 1;           // return 1
    return 0;
}

uint16_t read_ADC(){
    uint16_t buff;
    buff = ADC;
    return buff;
}

#endif