/*
 * movement.c
 *
 *  Created on: Feb 7, 2025
 *      Author: cmh04
 */

#include "movement.h"
#include "open_interface.h"

double move_forward (oi_t *sensor_data, double distance_mm){
    double sum = 0;
    double target = distance_mm - distance_mm*.03;

    oi_setWheels(250,250); // right wheel, left wheel

    while (sum < target){
        oi_update(sensor_data);

        if (sensor_data -> bumpLeft != 0){
            move_backward(sensor_data, 150);
            target += (150 - 150*.03);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_left(sensor_data, 90);
            oi_setWheels(250,250);

        }
        else if (sensor_data -> bumpRight != 0){
            move_backward(sensor_data, 150);
            target += (150 - 150*.03);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_right(sensor_data, 90);
            oi_setWheels(250,250);
        }

        sum += sensor_data -> distance;
        lcd_printf("Distance: %0.4lf", sensor_data -> distance);
    }

    oi_setWheels(0,0);

    return distance_mm; // Return distance travelled
}

double move_backward (oi_t *sensor_data, double distance_mm){
    double sum = 0;
    double target = -distance_mm + distance_mm*.03;

    oi_setWheels(-250,-250); // right wheel, left wheel

    while (sum > target){
        oi_update(sensor_data);
        sum += sensor_data -> distance;
        lcd_printf("Distance: %0.4lf", sensor_data -> distance);
        }
    oi_setWheels(0,0);

    return distance_mm; // Return distance travelled
}

double turn_right(oi_t *sensor_data, double degrees){
    double sum = 0;
    double target = -degrees + degrees*.1;

    oi_setWheels(-250, 250); // right wheel, left wheel

    while (sum > target){
        oi_update(sensor_data);
        sum += sensor_data -> angle;
    }

    oi_setWheels(0,0);

    return 0;
}

double turn_left(oi_t *sensor_data, double degrees){
    double sum = 0;
    double target = degrees - degrees*.1;

    oi_setWheels(250, -250); // right wheel, left wheel

    while (sum < target){
        oi_update(sensor_data);
        sum += sensor_data -> angle;
    }

    oi_setWheels(0,0);

    return 0;
}

double set_wheels(double right, double left){

    oi_setWheels(right,left);

    return 0;

}

double move_forward_test (oi_t *sensor_data){
    float sum = 0;

     // right wheel, left wheel

    oi_update(sensor_data);

    oi_setWheels(150,150);

    timer_waitMillis(1000);

    oi_setWheels(0,0);

    oi_update(sensor_data);

    sum = sensor_data -> distance*.97;

    lcd_printf("Step Distance: %0.4lf", sum / 50);


    return sum/50; // Return distance travelled
}
