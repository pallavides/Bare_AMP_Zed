/*
 * ioi_handler.c
 *
 *  Created on: Mar 31, 2015
 *      Author: teja
 */

#include "ioi_handler.h"
#include "utilities/fifo_queue_wdt.h"
#include "utilities/axi_gpio.h"
#include "pendulum_control.h"
#include "supervisory_communication.h"
#include "utilities/axi_uart.h"

bool plant_filter(QueuePacket* packet);

void ioi_handler(QueuePacket fifo_packet){
	set_debug(DEBUG1, true);

	unsigned int returnData[4]; // Data to send back to controller on FIFO

	switch (fifo_packet.command){
		case (PLANT): // SPI
			// SPI Filter
			if(!plant_filter(&fifo_packet)) return; // ERROR HANDLING HERE FOR FILTERING SPI
			if(!fifo_packet.bytes)	return;	// ERROR HANDLING IF TRANSFER BYTES IS 0

			// only accept voltage writes or sensor reads
			if( (fifo_packet.slave == SS_DAC) && (fifo_packet.operation == WRITE) && (fifo_packet.bytes == BITS_16)
					&& ((fifo_packet.data[0] & DAC_CONFIG_BITS) == DAC_CONFIG_BITS) ) // write voltage
				write_voltage(fifo_packet.data[0]);
			else if( (fifo_packet.slave == SS_ENCODER_S || fifo_packet.slave == SS_ENCODER_P) && (fifo_packet.operation == READ)
					&& (fifo_packet.bytes ==  BITS_32) && (fifo_packet.data[0]  == (READ_CNTR << 24))) // read encoder
				returnData[0] = read_sensor(fifo_packet.slave, fifo_packet.data[0]);
			break;
		case (SET_POINT): // SET_POINT
			returnData[0] = (unsigned int)get_set_point();
			break;
		case (STATE_VECTOR): // STATE_INFORMATION
			get_state_vector(returnData);
			break;
		default:
			break;
	}

	// Send the return data back to controller; maximum of 4 bytes returned
	enqueue(returnData, fifo_packet.operation > 4 ? 4 : fifo_packet.operation);

	//assert_trigger(check_wdt()); // backup was not working with this; initially had it to detect livelock attack

	set_debug(DEBUG1, false);
}

bool plant_filter(QueuePacket* packet){
	if( (packet->slave == SS_ENCODER_S || packet->slave == SS_ENCODER_P) && (packet->operation == READ) && (packet->bytes ==  BITS_32) ) // read encoder
		return true;
	if( (packet->slave == SS_DAC) && (packet->operation == WRITE) && (packet->bytes = BITS_16) && ((packet->data[0] & DAC_CONFIG_BITS) == DAC_CONFIG_BITS) )
		return true;
	return false;
}
