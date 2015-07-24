/*
 * fifo_queue.h
 *
 *  Created on: Mar 17, 2015
 *      Author: teja
 */

#ifndef FIFO_QUEUE_H_
#define FIFO_QUEUE_H_

#include "xparameters.h"
#include "taiga_rip_parameters.h"

#define FIFO_DEV_ID		XPAR_AXI_FIFO_BACKUP_DEVICE_ID

#define WORD_SIZE 		4 // Size of words in bytes

int init_fifo_queues();

int enqueue(QueuePacket* enqueue_packet);

int dequeue(int* buffer);

#endif /* FIFO_QUEUE_H_ */
