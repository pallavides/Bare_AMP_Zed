/*
 * axi_spi.h
 *
 *  Created on: Mar 31, 2015
 *      Author: teja
 */

#ifndef AXI_SPI_H_
#define AXI_SPI_H_

#include "xspi.h"
#include "xparameters.h"
#include "taiga_rip_parameters.h"

#define SPI_DEV_ID		XPAR_AXI_QUAD_SPI_PLANT_DEVICE_ID

u8 init_spi();

u8 spi_transfer(slave_select slave, u8* writeBuffer, u8* readBuffer, u8 numBytes);

u32 u32_to_buffer(u32 data, u8* buffer, u8 size);

u32 buffer_to_u32(u8* buffer, u8 size);

#endif /* AXI_SPI_H_ */
