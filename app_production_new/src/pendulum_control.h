/*
 * pendulum_control.h
 *
 *  Created on: Mar 31, 2015
 *      Author: teja
 */

#ifndef PENDULUM_CONTROL_H_
#define PENDULUM_CONTROL_H_

#include "utilities/fifo_queue.h"

int getSetPoint();

int readEncoder(slave_select encoder);

float readADC();

int writeDAC(float voltage);


#endif /* PENDULUM_CONTROL_H_ */
