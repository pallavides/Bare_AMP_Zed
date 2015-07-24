/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */


/***************************** Include Files *********************************/
#include "xparameters.h"
#include "xil_exception.h"
#include "xstreamer.h"
#include "xil_cache.h"
#include "xllfifo.h"
#include "xstatus.h"
#include "utilities/axi_gpio.h"
#include "utilities/fifo_queue_wdt.h"
#include "utilities/axi_spi.h"
#include "utilities/axi_uart.h"
#include "pendulum_control.h"
#include "trigger_mechanisms.h"
#include "supervisory_communication.h"
#include "ioi_handler.h"


int main()
{
	int Status;

	// Initialize everything. Order is important since some depend on others
	init_axi_uart();
	init_axi_gpio();
	init_spi();
	init_pendulum_plant();
	init_fifo_queues();
	init_wdt();

	set_set_point(10); // Set the starting set-point to 10. Will be updated with supervisory input

	// Start interrupts for FIFO and WDT
	Status = init_interrupt_system();
	if (Status != XST_SUCCESS) {
		xil_printf("Failed intr setup\r\n");
		return XST_FAILURE;
	}

	start_ioi(); // Set the start IOI flag to true so that control cycle can be checked

	set_led(LED1, true);

	static bool startTAIGA = false;		// Flag to execute IOI methods such as trigger mechanism and WDT
	static bool assertTrigger = false;	// Flag for whether the trigger has been asserted or not by trigger mechanism

	while(true){
		// IOI methods started once BTN0 has been pressed
		if(!startTAIGA && read_btn(BTN0)){
			startTAIGA = true;
			start_wdt();
		}

		// Assert the trigger if WDT expires or trigger has been asserted by trigger mechanism
		assert_trigger(startTAIGA & (check_wdt() | assertTrigger));

		supervisor_update_set_point(); // Check for new set-point from supervisory

		// If a control cycle has been completed: 2 encododer reads and voltage write
		if(check_control_cycle()){
			set_led(LED1, true);
			set_debug(DEBUG2, true);
			reset_control_cycle(); // Clear control cycle flag

			// Execute control algorithm to determine process state vector
			if((get_alphaR() >= 0 ? get_alphaR():-get_alphaR()) < (20.*pi/180)){
				// Comment out whatever controller you don't want to use
				calculateKalmanControlSignal(get_plant_state_instance());
				//calculateStateFeedbackControlSignal(get_plant_state_instance());
			}

			// Send the process state vector to UART controller. Packet is split into 2, this is first half
			set_debug(DEBUG2, false);
			supervisor_send_state_vector(get_plant_state_instance()->xhat);
			set_debug(DEBUG2, true);

			// Run a trigger mechanism
			if(startTAIGA){
				reset_wdt();
				set_debug(DEBUG4, true);
				if((get_alphaR() >= 0 ? get_alphaR():-get_alphaR()) < (20.*pi/180)){
					// This can be changed to trivial, prediction, or classifier.
					// For prediction, need to specify iterations (50): prediction_trigger_mechanism(get_plant_state_instance(), 50)
					// Classifier: classifier_trigger_mechanism(get_plant_state_instance())
					if(trivial_trigger_mechanism(get_plant_state_instance())){
						assertTrigger = true;
						set_debug(DEBUG3, true);
					}
					else	set_debug(DEBUG3, false);
				}
				set_debug(DEBUG4, false);
			}

			// Send the remaining process state vector packet. Packet is split into 2, this is the second half
			supervisor_send_tail(get_plant_state_instance()->u, startTAIGA, assertTrigger, check_wdt());
			set_debug(DEBUG2, false);
			set_led(LED1, false);
		}
	}

	return XST_SUCCESS;
}

