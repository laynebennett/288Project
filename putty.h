/*
 * putty.c
 *
 *  Created on: May 3, 2025
 *      Author: layneben
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

/**
 * print string to putty
 */
void printWholeString(char puttyString[]);

