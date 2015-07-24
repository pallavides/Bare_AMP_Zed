/*
 * supervisory_communication.h
 *
 *  Created on: Apr 8, 2015
 *      Author: teja
 */

#ifndef SUPERVISORY_COMMUNICATION_H_
#define SUPERVISORY_COMMUNICATION_H_

#include "utilities/axi_gpio.h"

void supervisor_send_state_vector(float stateVector[4]);

void supervisor_send_tail(float u, bool startTAIGA, bool trigger, bool wdt);

void supervisor_update_set_point();

#endif /* SUPERVISORY_COMMUNICATION_H_ */
