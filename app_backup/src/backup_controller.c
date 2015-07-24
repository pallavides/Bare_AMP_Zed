/*
 * production_controller.c
 *
 *  Created on: Oct 8, 2014
 *      Author: teja
 */

#include "backup_controller.h"
#include "pendulum_control.h"
#include "utilities/axi_gpio.h"
#include "utilities/fifo_queue.h"
#include "xtmrctr.h"
#include "xintc.h"


float calculateKalmanControlSignal(PlantParameters *params);
void backup_control_timer(void *CallBackRef, u8 TmrCtrNumber);
void stopBackupControl();

static int TmrCtrSetupIntrSystem(XIntc* IntcInstancePtr, XTmrCtr* InstancePtr, u16 DeviceId, u16 IntrId, u8 TmrCtrNumber);

XIntc InterruptController;
XTmrCtr TimerCounterInst;

/****************** Common Global variables for all ******************/
volatile unsigned sec1000; // This is updated 1000 times per second by interrupt handler
volatile float output_V,theta_R,alpha_R,theta_des;
volatile float thetaDot=0.,alphaDot=0.;


int temp;
float u;

static PlantParameters plantParams;

int startBackupControl(){
	// Initialize xpre and xhat
	getStateInformation(plantParams.xpre);

	int Status;

	// Initialize the timer counter so that it's ready to use
	Status = XTmrCtr_Initialize(&TimerCounterInst, TMRCTR_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Perform a self-test to ensure that the hardware was built correctly, use the 1st timer in the device (0)
	Status = XTmrCtr_SelfTest(&TimerCounterInst, TIMER_CNTR_0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Connect the timer counter to the interrupt subsystem such that interrupts can occur.  This function is application specific.
	Status = TmrCtrSetupIntrSystem(&InterruptController, &TimerCounterInst, TMRCTR_DEVICE_ID, TMRCTR_INTERRUPT_ID, TIMER_CNTR_0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Setup the handler for the timer counter that will be called from the interrupt context when the timer expires, specify a pointer to the
	XTmrCtr_SetHandler(&TimerCounterInst, backup_control_timer, &TimerCounterInst);

	// Use auto reload mode such that the timer counter will reload itself automatically and continue repeatedly, without this option it would expire once only
	XTmrCtr_SetOptions(&TimerCounterInst, TIMER_CNTR_0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);


	// Set a reset value for the timer counter
	XTmrCtr_SetResetValue(&TimerCounterInst, TIMER_CNTR_0, RESET_VALUE);

	// Start the timer counter such that it's incrementing by default
	XTmrCtr_Start(&TimerCounterInst, TIMER_CNTR_0);

	return XST_SUCCESS;
}

void backup_control_timer(void *CallBackRef, u8 TmrCtrNumber){
	set_led(LED2, true);

	plantParams.theta_des = 0*pi/180;
	set_debug(DEBUG5, true);
	plantParams.encoder_theta = -readEncoder(SS_ENCODER_S) % 4096;
	plantParams.thetaR = plantParams.encoder_theta*Kenc;
	set_debug(DEBUG5, false);
	plantParams.encoder_alpha =  4096+(-readEncoder(SS_ENCODER_P) % 4096);
	plantParams.alphaR = plantParams.encoder_alpha*Kenc-pi;
	set_debug(DEBUG5, true);
	if((plantParams.alphaR >= 0 ? plantParams.alphaR:-plantParams.alphaR) < (45.*pi/180)){
		plantParams.u = -calculateKalmanControlSignal(&plantParams);
	}
	else plantParams.u = 0;
	set_debug(DEBUG5, false);
	writeDAC(plantParams.u);

	++plantParams.cycle_count;

	set_led(LED2, false);
}


static int TmrCtrSetupIntrSystem(XIntc* IntcInstancePtr, XTmrCtr* TmrCtrInstancePtr, u16 DeviceId, u16 IntrId, u8 TmrCtrNumber){
	int Status;

	// Initialize the interrupt controller driver so that
	Status = XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Connect a device driver handler that will be called when an interrupt...
	Status = XIntc_Connect(IntcInstancePtr, IntrId,
				(XInterruptHandler)XTmrCtr_InterruptHandler,
				(void *)TmrCtrInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	// Start the interrupt controller
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Enable the interrupt for the timer counter
	XIntc_Enable(IntcInstancePtr, IntrId);

	// Initialize the exception table.
	Xil_ExceptionInit();

	// Register the interrupt controller handler with the exception table.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) XIntc_InterruptHandler, IntcInstancePtr);

	// Enable non-critical exceptions.
	Xil_ExceptionEnable();

	return XST_SUCCESS;
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
