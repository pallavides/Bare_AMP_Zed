/*
 * axi_gpio.h
 *
 *  Created on: Sep 30, 2014
 *      Author: teja
 */

#ifndef AXI_GPIO_H_
#define AXI_GPIO_H_

#include "xparameters.h"
#include "xgpio.h"

#define GUARD_TRIGGER_DEVICE_ID	XPAR_AXI_GPIO_TRIGGER_DEVICE_ID
#define GUARD_TRIGGER_CHANNEL	1
#define LED_DEVICE_ID			XPAR_AXI_GPIO_IOI_OUT_DEVICE_ID
#define LED_CHANNEL				1
#define DEBUG_DEVICE_ID			XPAR_AXI_GPIO_IOI_OUT_DEVICE_ID
#define DEBUG_CHANNEL			2
#define BTN_DEVICE_ID			XPAR_AXI_GPIO_IOI_IN_DEVICE_ID
#define BTN_CHANNEL				1
#define SW_DEVICE_ID			XPAR_AXI_GPIO_IOI_IN_DEVICE_ID
#define SW_CHANNEL				2

#define LED1			0x1

typedef enum {DEBUG1=0x1, DEBUG2=0x2, DEBUG3=0x4, DEBUG4=0x8} debug;

typedef enum {false, true} bool;

typedef enum {PRODUCTION = 0, BACKUP = 1} controller;

typedef enum {BTN0 = 0x1, BTN1 = 0x2, BTN2 = 0x4, BTN3 = 0x8} btn;

int init_axi_gpio();

void set_led(u8 leds, bool state);

void set_debug(debug pin, bool state);

void assert_trigger(controller control);

bool read_btn(u8 btn);

u8 read_sw_raw();

#endif /* AXI_GPIO_H_ */
