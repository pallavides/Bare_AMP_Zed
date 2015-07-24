/*
 * axi_gpio.c
 *
 *  Created on: Sep 30, 2014
 *      Author: teja
 */

#include "axi_gpio.h"
#include "xgpio.h"

static XGpio XGpio_LED;
static XGpio XGpio_debug;

int init_axi_gpio(){
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

	return 0;
}

void set_led(uint8_t leds, bool state){
	static u8 currentState = 0;
	currentState = (state) ? (currentState|leds) : ((currentState&((~leds)&0x03))|(leds&((~leds)&0x03)));
	XGpio_DiscreteWrite(&XGpio_LED, LED_CHANNEL, currentState);
}

void set_debug(debug pin, bool state){
	static u8 currentState = 0;
	currentState = (state) ? (currentState|pin) : (currentState & (~pin));
	XGpio_DiscreteWrite(&XGpio_debug, DEBUG_CHANNEL, currentState);
}
