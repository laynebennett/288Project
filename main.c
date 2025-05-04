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

            //move back a bit and stop the bot. needed so the bump doesn't re-trigger
            set_wheels(-100,-100);
            timer_waitMillis(500);
            set_wheels(0,0);

            supplyCustomers();

            curbFound = false;
        }

        if (sensor_data -> cliffLeft != 0 || sensor_data -> cliffRight != 0 || sensor_data -> cliffFrontLeft != 0 || sensor_data -> cliffFrontRight != 0)
        {

            //move back a bit and stop the bot. needed so the cliff doesn't re-trigger
            set_wheels(-100,-100);
            timer_waitMillis(500);
            set_wheels(0,0);

            if(sensor_data -> cliffLeftSignal < 100 || sensor_data -> cliffRightSignal < 100 || sensor_data -> cliffFrontLeftSignal < 100 || sensor_data -> cliffFrontRightSignal < 100){//CHANGE FOR DIFFERENTIATING BETWEEN HOME AND BORDER
                supplyBot();
            }else{
                if(sensor_data -> cliffLeft != 0){
                    uart_sendStr("Border Left!\n");

                }else if(sensor_data -> cliffRight != 0){
                    uart_sendStr("Border Right!\n");

                }else if(sensor_data -> cliffFrontLeft != 0){
                    uart_sendStr("Border Front Left!\n");

                }else if(sensor_data -> cliffFrontRight != 0){
                    uart_sendStr("Border Front Right!\n");

                }else{
                    uart_sendStr("Border!\n");
                }

            }


            sprintf(puttyString, "%u %u %u %u\n", sensor_data -> cliffLeftSignal, sensor_data -> cliffRightSignal, sensor_data -> cliffFrontLeftSignal, sensor_data -> cliffFrontRightSignal);
            printWholeString(puttyString);

        }


        if(command_flag == 1){
            uart_sendStr("STOPPED\n");
            break;
        }else if(command_flag == 2){ //long scan (use for object measurement)
            command_flag = 0;
            
            //float distAvg;

            fullScan(1, TOTAL_DEG_TURN, DEGREE_INCREMENT);

            //break;

        }else if(command_flag == 3){ //fast scan (use when no objects visible)
            command_flag = 0;

            //float distAvg;

            fullScan(5, TOTAL_DEG_TURN, DEGREE_INCREMENT);

            //break;
        }

    }

    uart_sendStr("FULL EXIT\n");
    oi_free(sensor_data);
    return 0;
}


