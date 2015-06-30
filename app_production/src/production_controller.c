/*
 * production_controller.c
 *
 *  Created on: Oct 8, 2014
 *      Author: teja
 */

#include "production_controller.h"
#include "pendulum_control.h"
#include "utilities/axi_gpio.h"
#include "utilities/fifo_queue.h"


float calculateKalmanControlSignal(PlantParameters *params);
void production_control_timer(xTimerHandle pxTimer);

/****************** Common Global variables for all ******************/
volatile float output_V,theta_R,alpha_R,theta_des;
volatile float thetaDot=0.,alphaDot=0.;

int temp;
float u;

static PlantParameters plantParams;

void startProductionControl(){
	// Initialize xpre and xhat
	int i;
	for(i = 0; i < 4; ++i){
		plantParams.xpre[i] = 0.;
		plantParams.xhat[i] = 0.;
	}
	plantParams.cycle_count = 0;

	ProductionControlTimer = xTimerCreate((const signed char *)"Production Controller Timer",1,pdTRUE,(void *) NULL, production_control_timer);
	xTimerStart(ProductionControlTimer, 0);
	xil_printf("Production Controller Timer started\n");
}

void production_control_timer(xTimerHandle pxTimer){
	set_debug(DEBUG8, true);
	set_led(LED3, true);

	set_debug(DEBUG7, true);
	plantParams.set_point = getSetPoint();
	plantParams.theta_des = plantParams.set_point*pi/180;
	set_debug(DEBUG7, false);
	plantParams.encoder_theta = -readEncoder(SS_ENCODER_S) % 4096;
	plantParams.thetaR = plantParams.encoder_theta*Kenc;
	set_debug(DEBUG7, true);
	plantParams.encoder_alpha= 4096+(-readEncoder(SS_ENCODER_P) % 4096);
	plantParams.alphaR = plantParams.encoder_alpha*Kenc-pi;
	set_debug(DEBUG7, false);
	if((plantParams.alphaR >= 0 ? plantParams.alphaR:-plantParams.alphaR) < (20.*pi/180)){
		plantParams.u = -calculateKalmanControlSignal(&plantParams);
	}
	else {
		plantParams.u = 0;
	}
	set_debug(DEBUG7, true);
	writeDAC(plantParams.u);
	set_debug(DEBUG7, false);
	++plantParams.cycle_count;

	++cycleCounter;

	set_led(LED3, false);
	set_debug(DEBUG8, false);
}


float calculateKalmanControlSignal(PlantParameters *params){
	/***** Kalman Filter Constants Start *****/
	static const float Kf[4][2]={
		{0.9943,0},
		{0,0.9943},
		{0.0023,-0.0071},
		{-0.0077,0.2988}
	};
	static const float Kc[4]={-5.1688,27.7667,-2.7224,3.1787};
	static const float Aup[4][4]={
		{0.0057,0.000,0.0000,0.0000},
		{0.0000,0.0057,0.0000,0.0000},
		{-0.0023,0.1059,0.9443,-0.0011},
		{0.0077,-0.1492,-0.0535,0.9980}
	};
	static const float Bup[4]={0,0,0.1013,0.0975};

	/***** Kalman Filter Constants End *****/

	if (params->thetaR < -pi)	params->thetaR += 2*pi; // correction for encoder zeroing error

	int ind;
	for(ind = 0; ind < 4; ind++){
		params->xhat[ind] = params->xpre[ind] + Kf[ind][0]*params->thetaR + Kf[ind][1]*params->alphaR;
	}

	// compute control
	float u = 0;
	for(ind = 0; ind < 4; ind++){
		u += -Kc[ind]*params->xhat[ind]; // changed the sign for kc to positive
	}
	u += Kc[0]*params->theta_des; // changed the sign of kc to negative

	// Saturate signal at -10 or 10 volts
	if(u > 5.)	 u = 5.;
	else if(u < -5.)	u = -5.;

	//precompute the part of xhat we can
	for(ind = 0; ind < 4; ind++){
		params->xpre[ind] = 0.;
		int col;
		for(col = 0; col < 4; col++){
			params->xpre[ind] += Aup[ind][col]*params->xhat[col];
		}
		params->xpre[ind] += Bup[ind]*u;
	}
	return u;
}
