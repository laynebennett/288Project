/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "ping.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

// Global shared variables
// Use extern declarations in the header file

volatile uint32_t g_start_time = 0;
volatile uint32_t g_end_time = 0;
volatile uint32_t ms_start_time = 0;
volatile uint32_t ms_end_time = 0;
volatile enum{LOW, HIGH, DONE} g_state = LOW; // State of ping echo pulse

void ping_init (void){

    // Port B settings
    SYSCTL_RCGCGPIO_R |= 0x2; // Enable clock for port B
    while ((SYSCTL_PRGPIO_R & 0x2) == 0)  {}; // Wait for port B

    GPIO_PORTB_DIR_R &= ~0x8; // Set PB3 to an input
    GPIO_PORTB_AFSEL_R |= 0x8; // Enable alternate function select for PB3
    GPIO_PORTB_PCTL_R &= ~0xF000; // Mux configuration for PB3
    GPIO_PORTB_PCTL_R |= 0x7000; // Mux configuration for PB3
    GPIO_PORTB_DEN_R |= 0x8; // Digital enable for PB3
    SYSCTL_RCGCTIMER_R |= 0x8; // Enable system clock
    while ((SYSCTL_PRTIMER_R & 0x8) == 0) {} // Wait for timer to turn on

    // Timer 3B settings
    TIMER3_CTL_R &= ~0x100; // Disable timer 3B
    TIMER3_CFG_R |= 0x4; // 16 bit timer config
    TIMER3_TBMR_R |= 0x7; // Enable capture mode and edge time mode
    TIMER3_TBILR_R = 0xFFFF; // Loads initial value for timer
    TIMER3_TBPR_R = 0xFF; // Loads starting value into prescale timer
    TIMER3_TBMR_R &= ~0x10; // Force count down
    TIMER3_IMR_R |= 0x400; // Enables interrupts for timer 3B on capture mode event
    TIMER3_ICR_R |= 0x400; // Clear capture mode event interrupt

    // Interupt handler
    NVIC_EN1_R = 0x10; // Enable interrupt for port B
    NVIC_PRI9_R = 0x8; // Set higher priority for timer 3B

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R |= 0x0D00; // Enable timer 3B, both edges
}

void ping_trigger (void){
    g_state = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    GPIO_PORTB_DIR_R |= 0x08; // Set pin 3 to output

    // Send signal to start sensing
    GPIO_PORTB_DATA_R &= ~0x08; // Send low

    GPIO_PORTB_DATA_R |= 0x08; // Send high
    timer_waitMicros(5); // Wait 5 uS
    GPIO_PORTB_DATA_R &= ~0x08; // Send low

    // Get ready to read inputs
    GPIO_PORTB_DIR_R &= ~0x08; // Set pin 3 to input - come back to this?

    /*
     * The difference in counter values (rising edge to falling edge) represents the
     * pulse width in clock cycles. The time in seconds can be calculated based on the system clock
     * frequency, which determines the amount of time for each counter value.
     *
     * Return pulse width - AKA the time between edges in terms of clock cycles
     */

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.
    if (TIMER3_MIS_R & 0x400) {


        if (g_state == LOW){
            g_start_time = TIMER3_TBR_R;
            ms_start_time = timer_getMillis();
            g_state = HIGH;
        }
        else if (g_state == HIGH) {
            g_end_time = TIMER3_TBR_R;
            ms_end_time = timer_getMillis();
            g_state = DONE;
        }
    }
    TIMER3_ICR_R |= 0x400;

}

float ping_getDistance (void){

    // Call ping trigger
    ping_trigger();
    while (g_state != DONE){}
    float pulseWidthC = 0;
    int pulseWidthMS = 0;
    int overflow = 0;
    int overflowCount = 0;
    float distance = 0;

    // Check overflow, store pulseWidth, display to lcd
    if (g_start_time > g_end_time){ /// Values are proper
        pulseWidthC = g_start_time - g_end_time; // Time in clock cycles
        pulseWidthMS = ms_end_time - ms_start_time; // Time in MS
    }
    else { // Overflow occurred
        overflow++;
        overflowCount += 1; // Increment number of overflows
        pulseWidthC = ((long)overflow << 24) + g_end_time - g_start_time;
        pulseWidthMS = ms_end_time - ms_start_time; // Time in MS
    }

    // Return distance in cm
    distance = ((pulseWidthC / 16000000) / 2) * 343 * 100;

    lcd_printf("Cycles : %.4f\nMS: %d\nDistance: %.4f\nOF: %d", pulseWidthC, pulseWidthMS, distance, overflowCount);

    return distance;
}
