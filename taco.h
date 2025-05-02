/**
 * taco.h
 * Header for taco.c functionality
 */

#include "movement.h"
#include "open_interface.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "ping.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"
#include<stdio.h>
#include <stdbool.h>
#include "adc.h"
#include "servo.h"

/**
 * Calls when the bot reaches home to restock
 */
int void supplyBot();

/**
 * Logic to supply customers with a taco when available.
 * Scans after each customer is served.
 */
int void supplyCustomers();

/**
 * Music to be played
 */
void music(int play_despacito, int play_home_jingle);
