/*
 * "pendulum_control.h"
 *
 *  Created on: Mar 31, 2015
 *      Author: teja
 */

#ifndef PENDULUM_CONTROL_H_
#define PENDULUM_CONTROL_H_

#include "taiga_rip_parameters.h"
#include "utilities/axi_gpio.h"
#include "utilities/axi_spi.h"

void init_pendulum_plant();

void write_voltage(u32 voltage_data);

u32 read_sensor(slave_select sensor, u32 data);

int get_set_point();

void set_set_point(int setPoint);

int start_ioi();

bool check_control_cycle();

void reset_control_cycle();

float get_thetaR();

float get_alphaR();

void get_state_vector(u32* buffer);

PlantParameters* get_plant_state_instance();

#endif /* PENDULUM_CONTROL_H_ */
