/*
 * adc.c
 *
 *  Created on: Mar 28, 2025
 *      Author: cmh04
 */
#include<stdio.h>
#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "open_interface.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

void adc_init (void){
    // Register setup

    SYSCTL_RCGCGPIO_R |= 0x2; // Enable clock for port B
    while ((SYSCTL_PRGPIO_R & 0x2) == 0)  {}; // Wait for port B

    SYSCTL_RCGCADC_R |= 0x0001; // Activate ADC 0
    while ((SYSCTL_PRADC_R & 0x01) == 0)  {};
    GPIO_PORTB_DIR_R &= ~0x0010; // Make port 4 input
    GPIO_PORTB_AFSEL_R |= 0x0010; // Enable alternate function on PB4
    GPIO_PORTB_DEN_R &= ~0x0010;  // Disable digital I/O on PB4
    GPIO_PORTB_AMSEL_R |= 0x0010; // Enable analog functionality on PB4

    // Sequencer setup
    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1; // Configure for 125k samples/sec
    ADC0_SSPRI_R = 0x0123; // Set sequencer 0 to highest priority
    ADC0_ACTSS_R &= ADC_ACTSS_ASEN0; // Disable sample sequencer 0
    ADC0_EMUX_R &= ~ADC_EMUX_EM0_PROCESSOR; // seq0 is the software trigger
    ADC0_SSMUX0_R &= ~0x000F;
    ADC0_SSMUX0_R += 10; // Set channel
    ADC0_SSCTL0_R = 0x0006; // No TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0001; // Disable SS0 interrupts
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0; // Enable sample sequencer 0
}

uint32_t adc_read (void){
    ADC0_PSSI_R = ADC_PSSI_SS0; // Initiate

    while ((ADC0_RIS_R & ADC_RIS_INR0) == 0) {}; // Wait

    uint32_t result = ADC0_SSFIFO0_R & 0x00000FFF; // Read

    ADC0_ISC_R |= ADC_ISC_IN0; // Acknowledge completion

    return result;
}
