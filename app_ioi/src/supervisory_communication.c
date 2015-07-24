/*
 * supervisory_communication.c
 *
 *  Created on: Apr 8, 2015
 *      Author: teja
 */


#include "supervisory_communication.h"
#include "utilities/axi_spi.h"
#include "utilities/axi_uart.h"
#include "trigger_mechanisms.h"

void supervisor_send_state_vector(float stateVector[4]){
	u8 uartBuffer[20];

	// First send the 4 process states
	u32_to_buffer((u32)(stateVector[0]*10000), &uartBuffer[0], 4);
	u32_to_buffer((u32)(stateVector[1]*10000), &uartBuffer[4], 4);
	u32_to_buffer((u32)(stateVector[2]*1000), &uartBuffer[8], 4);
	u32_to_buffer((u32)(stateVector[3]*1000), &uartBuffer[12], 4);


	while(!uart_send(uartBuffer, 16));
}

void supervisor_send_tail(float u, bool startTAIGA, bool trigger, bool wdt){
	u8 uartBuffer[8];

	// Send the voltage
	u32_to_buffer((u32)(u*10000), &uartBuffer[0], 4);

	// Send a flag representing the state of the system
	if(trigger && wdt)	uartBuffer[4] = 'T';	// WDT and trigger mechanism asserted trigger
	else if (trigger)	uartBuffer[4] = 'G';	// Trigger asserted by trigger mechanism
	else if (wdt)	uartBuffer[4] = 'W';		// Trigger asserted by WDT
	else if (startTAIGA)	uartBuffer[4] = 'S';	// IOI methods started with button press, guards being enforced
	else uartBuffer[4] = 'P';					// Operation under production control, guards not being enforced

	// Packet delimiter tail
	uartBuffer[5] = '-';
	uartBuffer[6] = '-';
	uartBuffer[7] = '\n';

	while(!uart_send(uartBuffer, 8));
}

void supervisor_update_set_point(){
	u8 supervisorInput[3];
	int receiveCount = uart_receive(supervisorInput);

	if(receiveCount == 0) return;

	// Set-point command is of syntax SP# where # contains magnitude and sign data of new set-point
	// This syntax is handled and implemented using this FSM
	int setPoint;
	static char stateChar = 'x';
	int i = 0;
	for(i = 0; i < receiveCount; ++i){
		switch (stateChar){
			case 'S':
				if (supervisorInput[i] == 'P')	stateChar = 'P';
				else	stateChar = 'x';
				break;
			case 'P':
				setPoint = (0x80 & supervisorInput[i]) ? -(supervisorInput[i]&0x7F) : (supervisorInput[i]&0x7F);
				set_set_point(setPoint);
				stateChar = 'x';
				break;
			default:
				if (supervisorInput[i] == 'S')	stateChar = 'S';
				else	stateChar = 'x';
				break;
		}
	}
}
