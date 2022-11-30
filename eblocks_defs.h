/*

EBlocks in SJG/02 Embedded Systems Lab at Huddersfield

STM32F103 pin names to physical presentation

The ports A to F do NOT map to the ports on the STM32

*/

#include "mbed.h"

// PORTA - the analog board
#define RA0 PA_0        // LDR
#define RA1 PA_1        // Potentiometer
#define RA2 PA_4        // Temperature sensor
#define RA3 PB_0
#define RA4 PC_1
#define RA5 PC_0
#define RA6 PC_3
#define RA7 PB_2


// PORTB - the LCD display
#define RB0 PC_8        // PWM 
#define RB1 PC_6        // PWM
#define RB2 PC_5 
#define RB3 PA_12 
#define RB4 PA_11 
#define RB5 PB_12 
#define RB6 PB_10 
#define RB7 PB_2  

//BusOut PORTB(RB0,RB1,RB2,RB3,RB4,RB5,RB7);


// PORTC - LEDs
#define RC0 PC_9
#define RC1 PB_8
#define RC2 PB_9
#define RC3 PA_5
#define RC4 PA_6
#define RC5 PA_7
#define RC6 PB_6
#define RC7 PC_7

//BusOut PORTC(RC0,RC1,RC2,RC3,RC4,RC5,RC6,RC7);


// PORTD - Switches
#define RD0 PB_1        // PWM
#define RD1 PB_15       // PWM
#define RD2 PB_14       // PWM
#define RD3 PB_13       // PWM
#define RD4 PC_4
#define RD5 PA_10
#define RD6 PB_3
#define RD7 PB_5

//BusIn PORTD(RD0,RD1,RD2,RD3,RD4,RD5,RD6,RD7);


// Nothing is connected to "PortF" but it could be ....
// PORTF
#define RF0 PB_7
#define RF1 PA_15       // PWM
#define RF2 PA_13
#define RF3 PC_12
#define RF4 PC_10
#define RF5 PC_11
#define RF6 PD_2
#define RF7 PB_4       // PWM

//BusOut PORTF(RF0,RF1,RF2,RF3,RF4,RF5,RF6,RF7);

