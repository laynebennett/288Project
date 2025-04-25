/**
 * main.c lab 10
 */
#include<stdio.h>
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"



int main(void)
{
    // Initializations and calibrations
    lcd_init();
    timer_init();
    uart_interrupt_init();
    servo_init();

    servo_move(90);
    timer_waitMillis(500);
    servo_move(30);
    timer_waitMillis(500);
    servo_move(150);
    timer_waitMillis(500);
    servo_move(90);
    return 0;
}
