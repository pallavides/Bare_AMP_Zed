/*
 * axi_gpio.h
 *
 *  Created on: Sep 30, 2014
 *      Author: teja
 */

#ifndef AXI_GPIO_H_
#define AXI_GPIO_H_

#include <stdint.h>
#include "xparameters.h"

#define LED_DEVICE_ID	XPAR_AXI_GPIO_BACKUP_CONTROLLER_OUT_DEVICE_ID
#define LED_CHANNEL		1
#define DEBUG_DEVICE_ID			XPAR_AXI_GPIO_BACKUP_CONTROLLER_OUT_DEVICE_ID
#define DEBUG_CHANNEL			2

typedef enum {DEBUG5=0x1, DEBUG6=0x2} debug;

#define LED2			0x1

typedef enum {false, true} bool;

int init_axi_gpio();

void set_led(uint8_t leds, bool state);

void set_debug(debug pin, bool state);

#endif /* AXI_GPIO_H_ */
