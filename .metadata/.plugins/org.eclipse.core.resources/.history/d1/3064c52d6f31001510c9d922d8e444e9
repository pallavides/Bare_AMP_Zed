/*
 * pendulum_control.h
 *
 *  Created on: Mar 31, 2015
 *      Author: teja
 */

#include "pendulum_control.h"

static u8 cycle_flag = 0;

static bool start_ioi_flag = false;

static PlantParameters stateVector;

void init_pendulum_plant(){
	int i;
	for(i = 0; i < 4; ++i){
		stateVector.xpre[i] = 0.;
		stateVector.xhat[i] = 0.;
	}
	stateVector.cycle_count = 0;

	u32 spiParams, spiWriteData;
	u8 writeBuffer[4], readBuffer[4];

	// Pendulum Encoder Initialization
	// clear counter
	spiParams = (PLANT << 24) |(WRITE << 16) | (BITS_8 << 8) | SS_ENCODER_P;
	spiWriteData = CLR_CNTR;
	u32_to_buffer(spiWriteData, writeBuffer, (spiParams >> 8) & 0xFF);
	spi_transfer(spiParams & 0xFF, writeBuffer, readBuffer, (spiParams >> 8) & 0xFF);

	// write to MDR0 register the following:
	// QUADRX4: X4 quadrature mode (four counts per quadrature cycle)
	// FREE_RUN: run the counter without any limits
	// DISABLE_INDX: index_disabled
	// FILTER_2: filter clock frequency division factor 2
	spiParams = (PLANT << 24) |(WRITE << 16) | (BITS_16 << 8) | SS_ENCODER_P;
	spiWriteData = (WRITE_MDR0 << 8) | QUADRX4 | FREE_RUN | DISABLE_INDX | FILTER_2;
	u32_to_buffer(spiWriteData, writeBuffer, (spiParams >> 8) & 0xFF);
	spi_transfer(spiParams & 0xFF, writeBuffer, readBuffer, (spiParams >> 8) & 0xFF);

	// write to MDR1 register the following:
	// NO_FLAGS: turn off flags
	// BYTE_2: counter will be in two byte mode
	// EN_CNTR: counting enabled
	spiParams = (PLANT << 24) |(WRITE << 16) | (BITS_16 << 8) | SS_ENCODER_P;
	spiWriteData = (WRITE_MDR1 << 8) | NO_FLAGS | BYTE_2 | EN_CNTR;
	u32_to_buffer(spiWriteData, writeBuffer, (spiParams >> 8) & 0xFF);
	spi_transfer(spiParams & 0xFF, writeBuffer, readBuffer, (spiParams >> 8) & 0xFF);

	// Servo Encoder Initialization
	// clear counter
	spiParams = (PLANT << 24) |(WRITE << 16) | (BITS_8 << 8) | SS_ENCODER_S;
	spiWriteData = CLR_CNTR;
	u32_to_buffer(spiWriteData, writeBuffer, (spiParams >> 8) & 0xFF);
	spi_transfer(spiParams & 0xFF, writeBuffer, readBuffer, (spiParams >> 8) & 0xFF);

	// write to MDR0 register the following:
	// QUADRX4: X4 quadrature mode (four counts per quadrature cycle)
	// FREE_RUN: run the counter without any limits
	// DISABLE_INDX: index_disabled
	// FILTER_2: filter clock frequency division factor 2
	spiParams = (PLANT << 24) |(WRITE << 16) | (BITS_16 << 8) | SS_ENCODER_S;
	spiWriteData = (WRITE_MDR0 << 8) | QUADRX4 | FREE_RUN | DISABLE_INDX | FILTER_2;
	u32_to_buffer(spiWriteData, writeBuffer, (spiParams >> 8) & 0xFF);
	spi_transfer(spiParams & 0xFF, writeBuffer, readBuffer, (spiParams >> 8) & 0xFF);

	// write to MDR1 register the following:
	// NO_FLAGS: turn off flags
	// BYTE_2: counter will be in two byte mode
	// EN_CNTR: counting enabled
	spiParams = (PLANT << 24) |(WRITE << 16) | (BITS_16 << 8) | SS_ENCODER_S;
	spiWriteData = (WRITE_MDR1 << 8) | NO_FLAGS | BYTE_2 | EN_CNTR;
	u32_to_buffer(spiWriteData, writeBuffer, (spiParams >> 8) & 0xFF);
	spi_transfer(spiParams & 0xFF, writeBuffer, readBuffer, (spiParams >> 8) & 0xFF);

	// Write DAC 0
	spiParams = (PLANT << 24) |(WRITE << 16) | (BITS_16 << 8) | SS_DAC;
	spiWriteData = (short)((1.65/3.3) * 4096) | DAC_CONFIG_BITS;
	u32_to_buffer(spiWriteData, writeBuffer, (spiParams >> 8) & 0xFF);
	spi_transfer(spiParams & 0xFF, writeBuffer, readBuffer, (spiParams >> 8) & 0xFF);
}

void write_voltage(u32 voltage_data){
	u8 writeBuffer[4];

	if(voltage_data > (DAC_CONFIG_BITS | 0xF84))	voltage_data = (DAC_CONFIG_BITS | 0xF84); // 0xF84 = 3.2/3.3 * 4096

	u32_to_buffer(voltage_data, writeBuffer, BITS_16);
	set_debug(DEBUG4, true);
	spi_transfer(SS_DAC, writeBuffer, NULL, BITS_16);
	set_debug(DEBUG4, false);
	cycle_flag |= SS_DAC;
}

u32 read_sensor(slave_select sensor, u32 data){
	u8 writeBuffer[4], readBuffer[4];
	u32_to_buffer(data, writeBuffer, BITS_32);
	spi_transfer(sensor, writeBuffer, readBuffer, BITS_32);

	cycle_flag |= sensor;

	unsigned int rawSensor = buffer_to_u32(readBuffer, BITS_32);

	unsigned int value = rawSensor;
	value &= ~(0xFF0000FF);
	value = (value >> 8);

	if(sensor == SS_ENCODER_S)
		stateVector.encoder_theta = -(value % 4096);
	else if (sensor == SS_ENCODER_P)
		stateVector.encoder_alpha = 4096 + (-((int)(value)) % 4096);

	return rawSensor;
}

int get_set_point(){
	stateVector.theta_des = stateVector.set_point*pi/180;
	return stateVector.set_point ;
}

void set_set_point(int setPoint){
	stateVector.set_point = setPoint;
}

int start_ioi(){
	start_ioi_flag = true;
	// TODO: start IOI flag
	return 1;
}

bool check_control_cycle(){
	return start_ioi_flag && (cycle_flag == (SS_DAC | SS_ENCODER_P | SS_ENCODER_S));
}

void reset_control_cycle(){
	stateVector.thetaR = stateVector.encoder_theta*Kenc;
	stateVector.alphaR = stateVector.encoder_alpha*Kenc-pi;

	cycle_flag = 0;

	//xil_printf("et:%d ea:%d t:%d a:%d\n", stateVector.encoder_theta, stateVector.encoder_alpha, (int)(stateVector.thetaR*1800/pi), (int)(stateVector.alphaR*1800/pi));

	// TODO: Method to reset the WDT
}

float get_thetaR(){
	return stateVector.thetaR;
}

float get_alphaR(){
	return stateVector.alphaR;
}

void get_state_vector(u32* buffer){
	int i;
	for(i = 0; i < 4; ++i)
		buffer[i] = (u32)stateVector.xpre[i];
}

PlantParameters* get_plant_state_instance(){
	return &stateVector;
}
