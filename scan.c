/**
scan.c
in control of scanning IR and PING values
 
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
#include "putty.h"


struct Object { //struct for each object found

                float width;
                float angle;
                float widthCM;
                float distance;
                float distances[];
            };

float get_IR_Dist(int i){

    servo_move(i);


    float IR_Distval = -5.745863 + (10206970 + 5.745863)/(1 + pow( adc_read() /0.00007989898, 0.7910138));
    //y = 39.46278 + (4084.141 - 39.46278)/(1 + (x/8.054943))

    return IR_Distval;

}

float get_PING_Dist(int i){
    servo_move(i);

    return ping_getDistance();
}

void fullScan(int interval, int totalDeg, int increment, int time_ms){ //NEEDS IMPLEMENTATION FOR OBBJECT WIDTH, DISTANCE, ETC



    printWholeString("\n");
    char printString[100];

        int i;
        for( i = 0; i <= totalDeg; i+= increment*interval){

            sprintf(printString, "%i %f\n", i, get_IR_Dist(i));
            printWholeString(printString);

            timer_waitMillis(time_ms);

        }
}
