/**
 * main.c lab 10
 */
#include<stdio.h>
#include <math.h>
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "uart-interrupt.h"
#include "movement.h"
#include "scan.h"
#include "taco.h"
#include "putty.h"

#define TOTAL_DEG_TURN 180
#define DEGREE_INCREMENT 2
//#define MIN_DISTANCE 50


int main(void)
{
    // Initializations and calibrations
    lcd_init();
    timer_init();
    uart_interrupt_init();
    adc_init();
    servo_init();

    oi_uartInit();

    char puttyString[100];
    bool curbFound = false;

    oi_t *sensor_data = oi_alloc();

    oi_init(sensor_data);
    
    supplyBot();

    while(1){

        oi_update(sensor_data);

        if (sensor_data -> bumpLeft != 0 || sensor_data -> bumpRight != 0)
        {
            curbFound = true;
            supplyCustomers();
            curbFound = false;
        }


        if(command_flag == 1){
            //oi_free(sensor_data);
            break;
        }else if(command_flag == 2){ //long scan (use for object measurement)
            command_flag == 1;
            
            //float distAvg;

            fullScan(1, TOTAL_DEG_TURN, DEGREE_INCREMENT);

            break;

        }else if(command_flag == 3){ //fast scan (use when no objects visible)
            command_flag == 1;

            //float distAvg;

            fullScan(5, TOTAL_DEG_TURN, DEGREE_INCREMENT);

            break;
        }

    }

    return 0;
}


