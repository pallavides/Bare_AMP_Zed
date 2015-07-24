/*
 * axi_uart.c
 *
 *  Created on: Apr 8, 2015
 *      Author: teja
 */

#include "axi_uart.h"

static XUartLite uart_device;		 // Instance of the UartLite device

int init_axi_uart(){
	int Status;

	// Initialize the UartLite driver so that it is ready to use.
	Status = XUartLite_Initialize(&uart_device, UART_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Perform a self-test to ensure that the hardware was built correctly.
	Status = XUartLite_SelfTest(&uart_device);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int uart_send(u8* sendBuffer, u8 size){
	if(XUartLite_IsSending(&uart_device))
		return 0;

	int SentCount = XUartLite_Send(&uart_device, sendBuffer, size);

	return SentCount;
}

int uart_receive(u8* recBuffer){
	int ReceivedCount = XUartLite_Recv(&uart_device, recBuffer, 3);
	return ReceivedCount;
}
