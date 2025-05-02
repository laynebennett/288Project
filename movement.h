/*
 * movement.h
 *
 *  Created on: Feb 7, 2025
 *      Author: cmh04
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

#endif /* MOVEMENT_H_ */

/*
 * A function to make the robot move forward
 *
 * oi_t *sensor_data is the data in the robot
 * double distance_mm is the amount of distance the robot needs to go
 *
 * returns distance travelled
 */
double move_forward(oi_t *sensor_data, double distance_mm);

/*
 * A function to make the robot move backward
 *
 * oi_t *sensor_data is the data in the robot
 * double distance_mm is the amount of distance the robot needs to go
 *
 * returns distance travelled
 */
double move_backward(oi_t *sensor_data, double distance_mm);

/*
 * Turn da robot right
 *
 * oi_t *sensor_data is the data in the robot
 * double degrees is how many degrees da robot turn
 */
double turn_right(oi_t *sensor_data, double degrees);

/*
 * Turn da robot left
 *
 * oi_t *sensor_data is the data in the robot
 * double degrees is how many degrees da robot turn
 */
double turn_left(oi_t *sensor_data, double degrees);

/*
* Brings set wheels function to movement for use instead of needing open_interface 
*/
double set_wheels(double right, double left);
