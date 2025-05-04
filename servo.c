/*
 * servo.c
 *
 *  Created on: Apr 17, 2025
 *      Author: cmh04
 */

#include <inc/tm4c123gh6pm.h>
#include "servo.h"
#include "Timer.h"
#include <stdint.h>
#include "button.h"
#include "lcd.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

int leftVal = 311150;
int rightVal = 284361;

int newPos = 90;
int direction = 1;
bool moved = false;
int button = 0;

void servo_init(void){
    SYSCTL_RCGCGPIO_R |= 0x2; // Enable clock for port B
    SYSCTL_RCGCTIMER_R |= 0x2; // Enable clock for timer 1
    while ((SYSCTL_PRGPIO_R & 0x2) == 0)  {}; // Wait for port B

    GPIO_PORTB_DIR_R |= 0x20; // Set PB5 to an input
    GPIO_PORTB_DEN_R |= 0x20; // Set PB5 to an input

    GPIO_PORTB_AFSEL_R |= 0x20; // Enable alternate function select for PB5
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF; // Mux configuration for PB3
    GPIO_PORTB_PCTL_R |= 0x700000; // Mux configuration for PB3

    TIMER1_CTL_R &= ~0x100; // Disable timer 5B --
    TIMER1_CFG_R |= 0x4; // 16 bit timer config
    TIMER1_TBMR_R |= 0xA; // Periodic mode, PWM mode
    TIMER1_TBMR_R &= ~0x5; // Periodic mode, PWM mode
    TIMER1_CTL_R &= ~0x4000;

    TIMER1_TBILR_R = 0xE200; // Loads initial value for timer (320000)

    TIMER1_TBPR_R = 0x4; // Loads starting value into prescale timer

    TIMER1_TBMATCHR_R = 0xC000; // 20ms long low pulse width
    TIMER1_TBPMR_R = 0x4;

    TIMER1_CTL_R |= 0x100; // Enable timer 5B
}

void servo_move(uint16_t degrees){
    unsigned int lowCycles = leftVal - ((leftVal-rightVal)/180)*degrees;

    //lcd_printf("%d", lowCycles);

    TIMER1_TBMATCHR_R = (lowCycles & 0xFFFF);
    TIMER1_TBPMR_R = lowCycles>>16;
}

