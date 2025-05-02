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

#define TOTAL_DEG_TURN 180
#define DEGREE_INCREMENT 2
//#define MIN_DISTANCE 50

void printWholeString(char puttyString[]);

int main(void)
{
    // Initializations and calibrations
    lcd_init();
    timer_init();
    uart_interrupt_init();
    adc_init();
    servo_init();
   // oi_t *sensor_data = oi_alloc();
    //oi_init(sensor_data);
    oi_uartInit();
    //servo_init();
    char puttyString[100];
    
    while(1){

        if(command_flag == 1){
            //oi_free(sensor_data);
            break;
        }else if(command_flag == 2){ //long scan (use for object measurement)
            command_flag == 1;
            
            float distAvg;


            sprintf(puttyString, "\n");
            printWholeString(puttyString);

            int i;
            for( i = 0; i <= TOTAL_DEG_TURN; i+= DEGREE_INCREMENT ){

                distAvg = get_IR_Dist(i);

                sprintf(puttyString, "%i %f\n", i, distAvg);
                printWholeString(puttyString);

            }

            break;

        }else if(command_flag == 3){ //fast scan (use when no objects visible)
            command_flag == 1;

            float distAvg;


            sprintf(puttyString, "\n");
            printWholeString(puttyString);

            int i;
            for( i = 0; i <= TOTAL_DEG_TURN; i+= DEGREE_INCREMENT*5){

                distAvg = get_IR_Dist(i);

                sprintf(puttyString, "%i %f\n", i, distAvg);
                printWholeString(puttyString);

            }

            break;
        }

    }

    return 0;
}


void printWholeString(char puttyString[]){
    int i;
    for(i=0;i<strlen(puttyString);i++){
        uart_sendChar(puttyString[i]);
    }

}
