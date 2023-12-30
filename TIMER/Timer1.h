/*
    Creater: Monna(Dang Hoang Anh Chuong)
    Date: 12/2023

    Contact:
    Email: dangtonny0819@gmail.com
    FB: https://www.facebook.com/chuongkomap123/

    This library is still in progress....
*/

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//PWM GPIO OCnx (Multi-select by using "|")
// Used in Func 00
#define OC1X_DISCONNECT         0x00
#define OC1A_TOGGLE             0x40 // COM1A1:0 = 1
#define OC1A_CLEAR_ON_MATCH     0x80 // COM1A1:0 = 2
#define OC1A_SET_ON_MATCH       0xC0 // COM1A1:0 = 3

#define OC1B_TOGGLE             0x10 // COM1B1:0 = 1
#define OC1B_CLEAR_ON_MATCH     0x20 // COM1B1:0 = 2
#define OC1B_SET_ON_MATCH       0x30 // COM1B1:0 = 3

//MODE  (Choose 1)
// Used in Func 01
#define NORMAL                  0x00 // Mode 0
#define PCPWM_8bit              0x01 // Mode 1 WGM13:10 0x01
#define PCPWM_9bit              0x02 // Mode 2 WGM13:10 0x02
#define PCPWM_10bit             0x03 // Mode 3 WGM13:10 0x03
#define CTC_TOP_OCR1A           0x08 // Mode 4 WGM13:10 0x04
#define FPWM_8bit               0x09 // Mode 5 WGM13:10 0x05
#define FPWM_9bit               0x0A // Mode 6 WGM13:10 0x06
#define FPWM_10bit              0x0B // Mode 7 WGM13:10 0x07
#define FPCPWM_TOP_ICR1         0x10 // Mode 8 WGM13:10 0x08
#define FPCPWM_TOP_OCR1A        0x11 // Mode 9 WGM13:10 0x09
#define PCPWM_TOP_ICR1          0x12 // Mode 10
#define PCPWM_TOP_OCR1A         0x13 // Mode 11
#define CTC_TOP_ICR1            0x18 // Mode 12
#define FPWM_TOP_ICR1           0x1A // Mode 14
#define FPWM_TOP_OCR1A          0x1B // Mode 15

//PRESCALER (Choose 1)
// Used in Func 02
#define T1_OFF                  0x00      // T1 stop
#define T1_DIV_1                0x01      // Clock from crystal
#define T1_DIV_8                0x02
#define T1_DIV_64               0x03
#define T1_DIV_256              0x04
#define T1_DIV_1024             0x05
#define T1_EXT_H_TO_L           0x06      //External clock (used as counter), failing edge
#define T1_EXT_L_TO_H           0x07      //External clock (used as counter), rising edge

//ISR  (Multi-select by using "|")
// Used in Func 05,6,7,8
#define T1_CAPT                 0x20
#define T1_COMPB                0x04
#define T1_COMPA                0x02
#define T1_OVF                  0x01

//MASK BYTE
#define GPIO_MASK               0xF0
#define MODEA_MASK              0x03  
#define MODEB_MASK              0x18
#define PRE_MASK                0x07
#define ISR_MASK                0x27

// Function
// Func 00: Init PWM Pin 
void Init_T1_OC1X(uint8_t mode){
  TCCR1A |= mode&GPIO_MASK;
}

// Func 01: Init mode
void Init_T1_MODE(uint8_t mode){
    TCCR1A |= mode&MODEA_MASK ;
    TCCR1B |= mode&MODEB_MASK ;
}
// Func 02: Select prescaler
void Init_T1_PRESCALER(uint8_t pre){
    TCCR1B |= pre&PRE_MASK ;
}
// Func 03: Set timer/counter value
void Set_T1(uint16_t value){
    TCNT1 = value;
}

// Func 04: Get timer/counter value
uint16_t Get_T1(){
    uint16_t value; //Buffer
    value = TCNT1;
    return value;
}

// Func 05: Enable interrupts
void Ena_ISR(uint8_t Int){
    TIMSK1 |= (Int&ISR_MASK); 
}

// Func 06: Disable interrupts
void Dis_ISR(uint8_t Int){
    TIMSK1 &= (~Int); 
}

// Func 07: Check if interrupt flag is set
bool Is_Int(uint8_t Int){
    if(TIFR1&Int)
        return 1;
    return 0;
}
// Func 08: Clear interrupt flag
void Clear_Int(uint8_t Int){
    TIFR1 |= Int;
}

// Func 09: Set TOP value if used.
void Set_TOP(uint16_t value){
    // Check which mode is used.  
    uint8_t mode = (TCCR1A&MODEA_MASK)|(TCCR1B&MODEB_MASK);

    if((mode == CTC_TOP_ICR1) || (mode == FPCPWM_TOP_ICR1) || (mode == PCPWM_TOP_ICR1) || (mode == FPWM_TOP_ICR1)){
        ICR1 = value;
    }
    if((mode == CTC_TOP_OCR1A) || (mode == FPCPWM_TOP_OCR1A) || (mode == PCPWM_TOP_OCR1A) || (mode == FPWM_TOP_OCR1A)){
        OCR1A = value;
    }

    return;
}

// Func 10,11: Set Compare value A, B
void Set_COMPA(uint16_t value){
    OCR1A = value;
}
void Set_COMPB(uint16_t value){
    OCR1B = value;
}

#endif