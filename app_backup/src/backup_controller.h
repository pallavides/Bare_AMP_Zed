/*
 * backup_controller.h
 *
 *  Created on: Oct 8, 2014
 *      Author: teja
 */

#ifndef BACKUP_CONTROLLER_H_
#define BACKUP_CONTROLLER_H_

#include "xparameters.h"

#define OPERATIONAL_SET_POINT	0

#define TMRCTR_DEVICE_ID		XPAR_TMRCTR_0_DEVICE_ID
#define TMRCTR_INTERRUPT_ID		XPAR_INTC_0_TMRCTR_0_VEC_ID
#define TIMER_CNTR_0	 		0
#define INTC_DEVICE_ID			XPAR_AXI_INTC_BACKUP_CONTROLLER_DEVICE_ID
#define RESET_VALUE	 			0xFFFDB60F // for 150 MHz, 0xFFFCF2BF for 200 MHz, 0xFFFE795F for 100 MHz

int startBackupControl();


#endif /* BACKUP_CONTROLLER_H_ */
