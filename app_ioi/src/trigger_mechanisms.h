/*
 * trigger_mechanisms.h
 *
 *  Created on: Apr 7, 2015
 *      Author: teja
 */

#ifndef TRIGGER_MECHANISMS_H_
#define TRIGGER_MECHANISMS_H_

#include "utilities/axi_gpio.h"
#include "taiga_rip_parameters.h"

#define THETA_GUARD		35

bool trivial_trigger_mechanism(PlantParameters* params);

bool prediction_trigger_mechanism(PlantParameters* params, unsigned int numIter);

bool classifier_trigger_mechanism(PlantParameters* params);

float calculateKalmanControlSignal(PlantParameters *params);

float calculateStateFeedbackControlSignal(PlantParameters *params);

#endif /* TRIGGER_MECHANISMS_H_ */
