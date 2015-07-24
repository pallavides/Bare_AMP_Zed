/*
 * trigger_mechanisms.c
 *
 *  Created on: Apr 7, 2015
 *      Author: teja
 */

#include "trigger_mechanisms.h"
#include "math.h"

bool trivial_trigger_mechanism(PlantParameters* params){
	return (params->thetaR >= THETA_GUARD*pi/180 || params->thetaR <= -THETA_GUARD*pi/180);
}

bool prediction_trigger_mechanism(PlantParameters* params, unsigned int numIter){
	/***** Plant Model Constants Start *****/
	static const float A[4][4] = {
		{1,0.0001,0.0012,0},
		{0,1.0001,-0.0000,0.0012},
		{0,0.0988,0.9443,-0.0011},
		{0,0.1497,-0.0536,0.9984}
	};
	static const float B[4] = {0.0001,0.0001,0.1013,0.0975};
	static const float C[2][4] =  {
		{1,0,0,0},
		{0,1,0,0}
	};
	/***** Plant Model Constants End *****/

	static float yplant[2] = {0.,0.};
	static float delThetaR;

	static PlantParameters predictionParams = {.cycle_count = 0};

	unsigned int i = 0, j = 0;
	for(i = 0; i < numIter; ++i){
		// Sync with plant params
		if(predictionParams.cycle_count == 0){
			for(j = 0; j < 4; ++j){
				predictionParams.xpre[j] = 0;
				predictionParams.xhat[j] = params->xhat[j];
			}
			predictionParams.theta_des = params->theta_des;
		}

		// Calculate the virtual sensor output
		yplant[0] = predictionParams.xhat[0];
		yplant[1] = predictionParams.xhat[1];

		// Updating the prediction params with virtual sensor measurements
		predictionParams.thetaR = yplant[0];
		predictionParams.alphaR = yplant[1];

		// calculate the control signal with the virtual sensor measurement
		// Comment out whatever controller you don't want to use
		predictionParams.u = calculateKalmanControlSignal(&predictionParams);
		//predictionParams.u = calculateStateFeedbackControlSignal(&predictionParams);
        for (j = 0; j < 4; j++) {
            predictionParams.xhat[j] =
            		A[j][0] * predictionParams.xhat[0]
                    + A[j][1] * predictionParams.xhat[1]
                    + A[j][2] * predictionParams.xhat[2]
                    + A[j][3] * predictionParams.xhat[3]
                    + B[j] * predictionParams.u;
        }

		if((predictionParams.thetaR >= THETA_GUARD*pi/180)){
			predictionParams.cycle_count = 0;
			return true;
		}

		if(predictionParams.cycle_count >= 1200)	predictionParams.cycle_count = 0;
		else ++predictionParams.cycle_count;
	}

	return false;
}

bool classifier_trigger_mechanism(PlantParameters* params){
	/***** Classifier Constants Start *****/
	// Accepting Input condition from the given data
	// First Hidden Layer Calculation
	static const float H1[5][4]={
		{-2.787277607155044,3.1879217756709948,-1.5791951615800552,1.0961654476296883},
		{-0.044675648209836688,2.0305005948708592,-2.083588705941577,1.657454494577925},
		{2.0546104380250574,-2.1558751720327245,0.89696008899332513,2.8182628898215829},
		{0.25040063570810406,0.27391506650121789,-12.084781701018301,7.9300329456225409},
		{-0.83078219732224268,-0.66755885243429691,-0.58268189399600323,0.25552979769224915}
	};

	static const float B1[5]={3.2012907134520265,-0.42460186372608583,2.1651294814480067,3.3263951023017708,-1.3648819496731714};
	// Second Hidden Layer Calculation
	static const float H2[5]={11.58092258308573,-4.3982970255217744,-0.87874405966423919,12.958800887594947,5.1588060906122486};
	static const float B2 = -8.2856337975078347;
	// Limits on Inputs xmax and xmin define the limit of actual data which is scaled between -1 and 1 specified by matlab
	static const float xmin[4] = {-0.17453292519943295,-0.26179938779914941,-3.0543261909900763,-1.7453292519943295};
	static const float xmax[4] = {0.52359877559829882,0.26179938779914941,2.5307274153917776,1.7453292519943295};
	static const float ymax = 1;
	static const float ymin = -1;
	/****** Classifier Constants End *****/

	static const float out_max = 1, out_min = 0;

	float y[4], z[5], z_1[5], p, p_1, final;

	float *x = params->xhat;

	int i = 0;
	// Solving for a given input
	for (i = 0; i < 4; ++i){
		y[i] = ((ymax - ymin)*(x[i]-xmin[i])/(xmax[i] - xmin[i]) + ymin);
	}

	for (i = 0; i <5; ++i){
		z[i] = H1[i][0]*y[0] + H1[i][1]*y[1] + H1[i][2]*y[2] + H1[i][3]*y[3] + B1[i];
		z_1[i] = 2/(1 + exp(-2*z[i])) - 1;
	}

	p = H2[0]*z_1[0] + H2[1]*z_1[1] + H2[2]*z_1[2] + H2[3]*z_1[3] + H2[4]*z_1[4] + B2;
	p_1 = 2/(1+exp(-2*p)) - 1;

	final = ((out_max - out_min)*(p_1-ymin)/(ymax - ymin) + out_min);

	if(final < 0.8)
		return true;

	return false;
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

	params->u = -u;
	return -u;
}

float calculateStateFeedbackControlSignal(PlantParameters *params){
	if (params->thetaR < -pi)	params->thetaR += 2*pi; // correction for encoder zeroing error

	params->xhat[2] = 0.9391*params->xhat[2] + 60.92*(params->thetaR - params->xhat[0]);
	params->xhat[3] = 0.9391*params->xhat[3] + 60.92*(params->alphaR - params->xhat[1]);

	params->u = -5.38*(params->thetaR - params->theta_des) + 30.14*params->alphaR-2.65*params->xhat[2] + 3.35*params->xhat[3];

	params->xhat[0] = params->thetaR;
	params->xhat[1] = params->alphaR;

	return params->u;
}

