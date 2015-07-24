/*
 * main_backup.c
 *
 *  Created on: Mar 19, 2015
 *      Author: teja
 */

#include "utilities/axi_gpio.h"
#include "utilities/fifo_queue.h"
#include "backup_controller.h"

int main( void ){
	init_fifo_queues();
	init_axi_gpio();
	startBackupControl();

	while(1);

	return 0;
}
