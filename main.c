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

        if(command_flag == 1){
            uart_sendStr("STOPPED\n");
            break;
        }else if(command_flag == 2){ //long scan (use for object measurement)
            command_flag = 0;
            
            //float distAvg;

            fullScan(1, TOTAL_DEG_TURN, DEGREE_INCREMENT, 100);

            //break;

        }else if(command_flag == 3){ //fast scan (use when no objects visible)
            command_flag = 0;

            //float distAvg;

            fullScan(1, TOTAL_DEG_TURN, DEGREE_INCREMENT, 20);

            //break;
        }else if(command_flag == 4){ //print hole values
            command_flag = 0;

            //sprintf(puttyString, "%u %u %u %u\n", sensor_data -> cliffLeftSignal, sensor_data -> cliffFrontLeftSignal, sensor_data -> cliffFrontRightSignal, sensor_data -> cliffRightSignal);
            //printWholeString(puttyString);

        }else if(command_flag == 5){ //forward
                    command_flag = 0;

                    set_wheels(150, 150);

        }else if(command_flag == 6){ //turn left
                    command_flag = 0;

                    set_wheels(50, -50);

        }else if(command_flag == 7){ //backward
                    command_flag = 0;

                    set_wheels(-150, -150);

        }else if(command_flag == 8){ //turn right
                    command_flag = 0;

                    set_wheels(-50, 50);

        }else if(command_flag == 9){ //stop
                    command_flag = 0;

                    set_wheels(0, 0);


        }else{

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

                    if(sensor_data -> cliffLeftSignal < 100 || sensor_data -> cliffRightSignal < 100 || sensor_data -> cliffFrontLeftSignal < 100 || sensor_data -> cliffFrontRightSignal < 100)
                    {

                        //move back a bit and stop the bot. needed so the cliff doesn't re-trigger
                        set_wheels(-100,-100);
                        timer_waitMillis(500);
                        set_wheels(0,0);

                        supplyBot();

                        uart_sendStr("Hole!\n");

                        sprintf(puttyString, "%u %u %u %u\n", sensor_data -> cliffLeftSignal, sensor_data -> cliffRightSignal, sensor_data -> cliffFrontLeftSignal, sensor_data -> cliffFrontRightSignal);
                        printWholeString(puttyString);

                    }

                    if(sensor_data -> cliffLeftSignal > 2500){
                        uart_sendStr("l\n");
                        set_wheels(-100,-100);
                        timer_waitMillis(500);
                        set_wheels(0,0);

                    }if(sensor_data -> cliffRightSignal > 2500){
                        uart_sendStr("r\n");
                        set_wheels(-100,-100);
                        timer_waitMillis(500);
                        set_wheels(0,0);

                    }if(sensor_data -> cliffFrontLeftSignal > 2500){
                        uart_sendStr("fl\n");
                        set_wheels(-100,-100);
                        timer_waitMillis(500);
                        set_wheels(0,0);

                    }if(sensor_data -> cliffFrontRightSignal > 2500){
                        uart_sendStr("fr\n");
                        set_wheels(-100,-100);
                        timer_waitMillis(500);
                        set_wheels(0,0);

                    }


        }

    }

    uart_sendStr("FULL EXIT\n");
    oi_free(sensor_data);
    return 0;
}


