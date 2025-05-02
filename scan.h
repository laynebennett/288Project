/**
 * Header file for scan.c
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



/**
Returns IR dist given the angle of the servo, i

*/
float get_IR_Dist(int i);

/**
Returns PING dist given the angle of the servo, i
 */
float get_PING_Dist(int i);