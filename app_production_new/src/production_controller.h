/*
 * production_controller.h
 *
 *  Created on: Oct 8, 2014
 *      Author: teja
 */

#ifndef PRODUCTION_CONTROLLER_H_
#define PRODUCTION_CONTROLLER_H_

#include "FreeRTOS.h"
#include "timers.h"

unsigned long cycleCounter;

xTimerHandle ProductionControlTimer;

void startProductionControl();


#endif /* PRODUCTION_CONTROLLER_H_ */
