/*
 * servo.h
 *
 *  Created on: Apr 17, 2025
 *      Author: cmh04
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include "servo.h"
#include "Timer.h"
#include <stdint.h>
#include "button.h"
#include "lcd.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

void servo_init(void);

void servo_move(uint16_t degrees);

#endif /* SERVO_H_ */
