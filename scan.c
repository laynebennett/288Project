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
                //float distances[];
            };

float get_IR_Dist(int i){

    servo_move(i);


    float IR_Distval = 0.8 * 2440000 * pow( adc_read() , -1.56);
    //y = 39.46278 + (4084.141 - 39.46278)/(1 + (x/8.054943))

    return IR_Distval;

}



float get_PING_Dist(int i){
    servo_move(i);

    return ping_getDistance();
}



void fullScan(int interval, int totalDeg, int increment, int time_ms){ //NEEDS IMPLEMENTATION FOR OBBJECT WIDTH, DISTANCE, ETC

    float scanDist[4] = {MIN_DISTANCE_SCAN, MIN_DISTANCE_SCAN, MIN_DISTANCE_SCAN, MIN_DISTANCE_SCAN};

    float distAvg;

    bool objDetect = false;

    int objCount = 0;

    float objDist;

    float distances[100] = {0};

    struct Object objects[10] = {0}; //creates 10 objects

    printWholeString("\n");
    char printString[100];

    servo_move(0);

    timer_waitMillis(750);

        int i;
        for( i = 0; i <= totalDeg; i+= increment*interval){

            distAvg = get_IR_Dist(i);

            distAvg = get_avg(scanDist, distAvg);

            if(distAvg < MIN_DISTANCE_SCAN){ //SOMETHING CLOSE

                if(objDetect == false){ //MAKE A NEW OBJECT
                sprintf(printString, "OBJECT DETECTED\n");
                printWholeString(printString);
                objCount++;

                memset(distances, 0, sizeof(distances));//reset array to 0

                objects[objCount].width = 0;
                objects[objCount].angle = i;

                }

                sprintf(printString, "%f dist %i ang\n", distAvg, i);
                printWholeString(printString);

                objDetect = true;
                objects[objCount].width += increment;
                //objects[objCount].distances[(int)(i/increment)] = distAvg;
                distances[i/increment] = distAvg;



            }else{
                        if((objects[objCount].width <= 10) && (objDetect == true)){//REMOVE PREVIOUS OBJECT, TOO SMALL
                            sprintf(printString, "OBJECT TOO SMALL\n");
                            printWholeString(printString);
                            objCount--;
                        }
                        else if(objDetect == true){//finish later

                            objects[objCount].midAngle = (int)(objects[objCount].angle + (objects[objCount].width/2));

                            objects[objCount].distance = distances[(int)(ceil(objects[objCount].midAngle/increment)-(((int)(objects[objCount].midAngle/increment))%2))];

                        }
                            objDetect = false;
                            sprintf(printString, "NO OBJECT\n");
                            printWholeString(printString);
            }
//IF LOOP ENDS ON OBJECT IT WILL NOT UPDATE THE OBJECT INFORMATION CORRECTLY. ADDING IMPLEMENTATION TOMORROW. MAY MAKE SOME CROPPED OBJECT SCAN FUNCTINOALITY
            timer_waitMillis(time_ms);

        }

        objects[0].width = 0;


        sprintf(printString, "\nobjCount: %i\n\n", objCount);
        printWholeString(printString);

        float minWidthCM = 999;
        float minAngle = objects[1].angle+(((double)objects[1].width)/2.0);



        int q;

        for(q=1;q<=objCount;q++){//FIND SMALLEST OBJECT

            //objects[q].midAngle = (int)(objects[q].angle + (objects[q].width/2));

            //objects[q].distance = objects[q].distances[(int)(ceil(objects[q].midAngle/increment)-(((int)(objects[q].midAngle/increment))%2))];

            sprintf(printString, "%i", (int)(ceil(objects[q].midAngle/increment)-(((int)(objects[q].midAngle/increment))%2)));
            printWholeString(printString);

            objects[q].widthCM = 2*objects[q].distance*tan(objects[q].width*(3.14159)/360.0);

            sprintf(printString, "Object %i: Angle = %f, Distance = %f, Width = %fdegrees, WidthCM = %f\n", q, objects[q].midAngle, objects[q].distance, objects[q].width, objects[q].widthCM);
            printWholeString(printString);

            if(objects[q].widthCM<minWidthCM){
                minWidthCM = objects[q].widthCM;
                minAngle = objects[q].angle+(((double)objects[q].width)/2.0);
                objDist = objects[q].distance;
            }

        }

        sprintf(printString, "minAngle: %f\n", minAngle);
        printWholeString(printString);




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
