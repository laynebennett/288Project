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

#define MIN_DISTANCE_SCAN 50


struct Object { //struct for each object found

                float width;
                float angle;
                float widthCM;
                float distance;
                float midAngle;
                bool customer;
            };

float get_IR_Dist(int i){

    servo_move(i);


    float IR_Distval = 0.8 * 2440000 * pow( adc_read() , -1.56);


    return IR_Distval;

}



float get_PING_Dist(int i){
    servo_move(i);

    return ping_getDistance();
}



bool fullScan(int interval, int totalDeg, int increment, int time_ms){ //NEEDS IMPLEMENTATION FOR OBBJECT WIDTH, DISTANCE, ETC

    float scanDist[4] = {MIN_DISTANCE_SCAN, MIN_DISTANCE_SCAN, MIN_DISTANCE_SCAN, MIN_DISTANCE_SCAN};

    float distAvg;

    bool objDetect = false;

    bool customerDetect = false;

    int objCount = 0;

    float objDist;

    float distances[100] = {0};

    struct Object objects[10] = {0}; //creates 10 objects

    printWholeString("\n");
    char printString[100];

    servo_move(0);

    timer_waitMillis(750);

    printWholeString("SCAN_START\n");

        int i;
        for( i = 0; i <= totalDeg; i+= increment*interval){

            distAvg = get_IR_Dist(i);

            distAvg = get_avg(scanDist, distAvg);

            if(distAvg < MIN_DISTANCE_SCAN){ //SOMETHING CLOSE

                if(objDetect == false){ //MAKE A NEW OBJECT
                //sprintf(printString, "OBJECT DETECTED\n");
                //printWholeString(printString);
                objCount++;

                memset(distances, 0, sizeof(distances));//reset array to 0

                objects[objCount].width = 0;
                objects[objCount].angle = i;

                }

                sprintf(printString, "%i,%f\n",  i, distAvg);
                printWholeString(printString);

                objDetect = true;
                objects[objCount].width += increment;

                distances[i/increment] = distAvg;



            }else{
                        if((objects[objCount].width <= 10) && (objDetect == true)){//REMOVE PREVIOUS OBJECT, TOO SMALL
                            //sprintf(printString, "OBJECT TOO SMALL\n");
                            //printWholeString(printString);
                            objCount--;
                        }
                        else if(objDetect == true){//finish later

                            objects[objCount].midAngle = (int)(objects[objCount].angle + (objects[objCount].width/2));

                            objects[objCount].distance = distances[(int)(ceil(objects[objCount].midAngle/increment)-(((int)(objects[objCount].midAngle/increment))%2))];

                        }
                            objDetect = false;
                            //sprintf(printString, "NO OBJECT\n");
                            sprintf(printString, "%i,50\n",  i);
                            printWholeString(printString);
            }
//IF LOOP ENDS ON OBJECT IT WILL NOT UPDATE THE OBJECT INFORMATION CORRECTLY. ADDING IMPLEMENTATION TOMORROW. MAY MAKE SOME CROPPED OBJECT SCAN FUNCTINOALITY
            timer_waitMillis(time_ms);

        }

        objects[objCount].midAngle = (int)(objects[objCount].angle + (objects[objCount].width/2));

        objects[objCount].distance = distances[(int)(ceil(objects[objCount].midAngle/increment)-(((int)(objects[objCount].midAngle/increment))%2))];


        //sprintf(printString, "\nobjCount: %i\n\n", objCount);
        //printWholeString(printString);
        printWholeString("END\n");

        int q;
        for(q=1;q<=objCount;q++){//FIND SMALLEST OBJECT

            objDetect = true;

            objects[q].widthCM = 2*objects[q].distance*tan(objects[q].width*(3.14159)/360.0);

            if(objects[q].widthCM < 12){
                objects[q].customer = true;
                customerDetect = true;
                //printWholeString("CUSTOMER FOUND\n");
            }else{
                objects[q].customer = false;
            }

            //sprintf(printString, "Object %i: Angle = %f, Distance = %f, Width = %fdegrees, WidthCM = %f\n", q, objects[q].midAngle, objects[q].distance, objects[q].width, objects[q].widthCM);
            //printWholeString(printString);

            sprintf(printString, "%f,%f,%f\n",objects[q].midAngle, objects[q].distance, objects[q].widthCM);
            printWholeString(printString);

        }

        printWholeString("END2\n");

        return customerDetect;

}



float get_avg(float scanDist[], float distance){

    scanDist[3] = distance;

    float distSum = 0;

    int j;
    for(j=1; j<4; j++){

        distSum+=scanDist[j];
        scanDist[j-1] = scanDist[j];

    }

    return distSum/3.0;
}
