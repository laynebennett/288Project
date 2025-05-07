/**
 * taco.h
 * Header file for taco.c
 * Updates taco and delivery related information
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
 * Logic for supplying the bot at home
 */
void supplyBot();

/**
 * Logic for scanning customers and supplying customers
 */
void supplyCustomers();

/**
 * Music library
 */
void music(int play_despacito, int play_home_jingle, int play_despacito2, int play_despacito3);
