/*
 * adversary.c
 *
 *  Created on: Jun 10, 2015
 *      Author: teja
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "projdefs.h"
#include "semphr.h"
#include "production_controller.h"
#include "utilities/axi_gpio.h"
#include "pendulum_control.h"

static void adversaryTask(void * param);
void voltSweep_timer(xTimerHandle pxTimer);

typedef enum {NONE = 0, DoS=0x1, LIVELOCK_ATTACK=0x2, SET_POINT_ATTACK=0x4, VOLT_SWEEP_ATTACK=0x8, COVERT_ATTACK=0x16} attack_menu;

void startAdversaryTask(){
	xTaskCreate(adversaryTask, (signed char*) "Adversary Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, (xTaskHandle *) NULL);
}

static void adversaryTask(void * param){
	attack_menu attacksExecuted = NONE;

	for(;;){
		/*// Execute DoS attack at time 30s
		if(!(attacksExecuted & DoS) && cycleCounter >= 60000){
			xTimerStop(ProductionControlTimer, 0);
			attacksExecuted |= DoS;
		}*/

		/*// Execute Voltage Sweep at time 30s
		if(!(attacksExecuted & VOLT_SWEEP_ATTACK) && cycleCounter >= 20000){
			xTimerStop(ProductionControlTimer, 0);
			xTimerHandle voltageSweepTimer = xTimerCreate((const signed char *)"Voltage Sweep Timer",1,pdTRUE,(void *) NULL, voltSweep_timer);
			xTimerStart(voltageSweepTimer, 0);
			attacksExecuted |= VOLT_SWEEP_ATTACK;
		}*/

		taskYIELD(); // This task runs infinitely in this loop. Make sure you always yield for other processes and tasks to run
	}
}

void voltSweep_timer(xTimerHandle pxTimer){
	set_debug(DEBUG8, true);
	static float voltage = 5;
	static bool positive = true;

	readEncoder(SS_ENCODER_S);
	readEncoder(SS_ENCODER_P);

	writeDAC(voltage);

	if(positive)	voltage += 0.2;
	else	voltage -= 0.2;
	if(voltage > 15)	positive = false;
	if(voltage < -15)	positive = true;
	set_debug(DEBUG8, false);
}
