/*
 * pendulum_control.c
 *
 *  Created on: Mar 31, 2015
 *      Author: teja
 */

#include "pendulum_control.h"

static QueuePacket fifo_packet;
static unsigned int buffer[8];

int getSetPoint(){
	fifo_packet.command = SET_POINT;
	fifo_packet.operation = READ;
	fifo_packet.bytes = BITS_0;
	fifo_packet.slave = NO_SLAVE;
	fifo_packet.length = 0;

	enqueue(&fifo_packet);
	dequeue(buffer);

	return (int)buffer[0];
}

int readEncoder(slave_select encoder){
	fifo_packet.command = PLANT;
	fifo_packet.operation = READ;
	fifo_packet.bytes = BITS_32;
	fifo_packet.slave = encoder;
	fifo_packet.length = 1;
	fifo_packet.data[0] = (READ_CNTR << 24);

	enqueue(&fifo_packet);
	dequeue(buffer);

	unsigned int value = buffer[0];
	value &= ~(0xFF0000FF);
	value = (value >> 8);
	return value;
}

float readADC(){
	float vref = 3.3;
	float voltage;
	unsigned int hex;

	// read the ADC
	fifo_packet.command = PLANT;
	fifo_packet.operation = READ;
	fifo_packet.bytes = BITS_32;
	fifo_packet.slave = SS_ADC;
	fifo_packet.length = 1;
	fifo_packet.data[0] = ADC_CONFIG_BITS;
	enqueue(&fifo_packet);
	dequeue(buffer);

	hex = buffer[0];

	// convert hex to voltage
	hex &= 0x00000FFF;  // clear configByte and other garbage
	voltage  = vref * hex / (4096.0);

	// scale voltage from 0V --- +3.3V to -10V --- +10V
	voltage -= 1.65; // offset voltage;
	voltage *= 6.0; // gain of op amp

	return voltage;
}

int writeDAC(float voltage){
    float vref = 3.3;
    short hex;

    // I uncommented this so I can execute bias-injection voltage sweep attacks outside actuator limits
    // Clip voltage to 10 volts
    //if(voltage > 10)	voltage = 10;
    //else if(voltage < -10)	voltage = -10;

    // scale voltage from -10V --- +10V to 0V --- +3.3V
    voltage /= 6.0; // gain of op amp
    voltage += 1.65; // offset voltage;
    if(voltage > 3.2)	voltage = 3.2;
    else if(voltage <0)	voltage = 0;

    // convert voltage to hex
    hex = (voltage/vref) * 4096;

    hex |= DAC_CONFIG_BITS; // add configuration bits

	fifo_packet.command = PLANT;
	fifo_packet.operation = WRITE;
	fifo_packet.bytes = BITS_16;
	fifo_packet.slave = SS_DAC;
	fifo_packet.length = 1;
	fifo_packet.data[0] = hex;
	enqueue(&fifo_packet);
    return (hex & (~0xF000));
}
