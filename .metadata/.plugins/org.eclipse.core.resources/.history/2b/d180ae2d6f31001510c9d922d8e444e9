/*
 * axi_spi.c
 *
 *  Created on: Mar 31, 2015
 *      Author: teja
 */

#include "axi_spi.h"

static XSpi SpiInstancePtr;
static XSpi_Config *ConfigPtr;

u8 init_spi(){
	u8 Status;

	// Initialize the SPI driver so that it's ready to use, specify the device ID that is generated in xparameters.h.
	ConfigPtr = XSpi_LookupConfig(SPI_DEV_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	Status = XSpi_CfgInitialize(&SpiInstancePtr, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	// The SPI device is a slave by default and the clock phase and polarity have to be set according to its master.
	Status = XSpi_SetOptions(&SpiInstancePtr, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Start the SPI driver so that the device is enabled.
	XSpi_Start(&SpiInstancePtr);


	// Disable Global interrupt to use polled mode operation.
	XSpi_IntrGlobalDisable(&SpiInstancePtr);

	return XST_SUCCESS;
}

u8 spi_transfer(slave_select slave, u8* writeBuffer, u8* readBuffer, u8 numBytes){
	u8 retval;
	retval = XSpi_SetSlaveSelect(&SpiInstancePtr, slave);
	retval = XSpi_Transfer(&SpiInstancePtr, writeBuffer, readBuffer, numBytes);

	return retval;
}

u32 u32_to_buffer(u32 data, u8* buffer, u8 size){
	int i;
	for(i = size; i >= 0; --i)
		buffer[size-i] = (u8)(data >> (8*(i-1)));
	return data;
}

u32 buffer_to_u32(u8* buffer, u8 size){
	u32 data = 0;
	int i;
	for(i = size; i >= 0; --i)
		data |= (u32)(buffer[size-i] << (8*(i-1)));
	return data;
}
