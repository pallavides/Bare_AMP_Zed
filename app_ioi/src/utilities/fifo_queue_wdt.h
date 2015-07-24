/*
 * fifo_queue.h
 *
 *  Created on: Mar 17, 2015
 *      Author: teja
 */

#ifndef FIFO_QUEUE_WDT_H_
#define FIFO_QUEUE_WDT_H_

#include "xparameters.h"
#include "axi_gpio.h"

#define QUEUE_DEV_ID		XPAR_AXI_FIFO_IOI_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define FIFO_INTR_ID		XPAR_INTC_0_LLFIFO_0_VEC_ID
#define WDT_DEVICE_ID		XPAR_WDTTB_0_DEVICE_ID
#define WDT_INTR_ID			XPAR_AXI_INTC_IO_INTERMEDIARY_AXI_WDT_IOI_WDT_INTERRUPT_INTR

#define WORD_SIZE 		4 // Size of words in bytes

int init_fifo_queues();

int init_wdt();

void start_wdt();

void reset_wdt();

bool check_wdt();

int init_interrupt_system();

int enqueue(unsigned int* data, int size);

#endif /* FIFO_QUEUE_WDT_H_ */
