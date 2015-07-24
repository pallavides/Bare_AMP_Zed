/*
 * axi_gpio.c
 *
 *  Created on: Sep 30, 2014
 *      Author: teja
 */

#include "axi_gpio.h"

static XGpio XGpio_LED;
static XGpio XGpio_debug;
static XGpio XGpio_Guard_Trigger;
static XGpio XGpio_BTN;
static XGpio XGpio_SW;

int init_axi_gpio(){
	// Initialize the LED
	if (XGpio_Initialize(&XGpio_LED, LED_DEVICE_ID) != XST_SUCCESS)  {
		return XST_FAILURE;
	}

	// Set the direction for all signals to be outputs
	XGpio_SetDataDirection(&XGpio_LED, LED_CHANNEL, 0x0);

	// Set the GPIO outputs to low
	XGpio_DiscreteWrite(&XGpio_LED, LED_CHANNEL, 0x0);

	// Initialize the DEBUG output
	if (XGpio_Initialize(&XGpio_debug, DEBUG_DEVICE_ID) != XST_SUCCESS)  {
		return XST_FAILURE;
	}

	// Set the direction for all signals to be outputs
	XGpio_SetDataDirection(&XGpio_debug, DEBUG_CHANNEL, 0x0);

	// Set the GPIO outputs to low
	XGpio_DiscreteWrite(&XGpio_debug, DEBUG_CHANNEL, 0x0);

	// Initialize the Guard Trigger
	if (XGpio_Initialize(&XGpio_Guard_Trigger, GUARD_TRIGGER_DEVICE_ID) != XST_SUCCESS)  {
		return XST_FAILURE;
	}

	// Set the direction for all signals to be outputs
	XGpio_SetDataDirection(&XGpio_Guard_Trigger, GUARD_TRIGGER_CHANNEL, 0x0);

	// Set the GPIO outputs to low
	XGpio_DiscreteWrite(&XGpio_Guard_Trigger, GUARD_TRIGGER_CHANNEL, 0x0);

	// Initialize the BTN
	if (XGpio_Initialize(&XGpio_BTN, BTN_DEVICE_ID) != XST_SUCCESS)  {
		return XST_FAILURE;
	}

	// Set the direction for all signals to be inputs
	XGpio_SetDataDirection(&XGpio_BTN, BTN_CHANNEL, 0xF);

	// Initialize the SW
	if (XGpio_Initialize(&XGpio_SW, SW_DEVICE_ID) != XST_SUCCESS)  {
		return XST_FAILURE;
	}

	// Set the direction for all signals to be inputs
	XGpio_SetDataDirection(&XGpio_SW, SW_CHANNEL, 0xF);

	return 0;
}

void set_led(u8 leds, bool state){
	static u8 currentState = 0;
	currentState = (state) ? (currentState|leds) : (currentState & (~leds));
	XGpio_DiscreteWrite(&XGpio_LED, LED_CHANNEL, currentState);
}

void set_debug(debug pin, bool state){
	static u8 currentState = 0;
	currentState = (state) ? (currentState|pin) : (currentState & (~pin));
	XGpio_DiscreteWrite(&XGpio_debug, DEBUG_CHANNEL, currentState);
}

void assert_trigger(controller control){
	XGpio_DiscreteWrite(&XGpio_Guard_Trigger, GUARD_TRIGGER_CHANNEL, control);
}

bool read_btn(u8 btn){
	return (bool)(XGpio_DiscreteRead(&XGpio_BTN, BTN_CHANNEL)&btn);
}

u8 read_sw_raw(){
	return XGpio_DiscreteRead(&XGpio_SW, SW_CHANNEL);;
}
