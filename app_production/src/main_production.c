// Kernel includes.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "xil_printf.h"
#include "production_controller.h"
#include "utilities/axi_gpio.h"
#include "utilities/fifo_queue.h"
#include "adversary.h"

#include "xparameters.h"
#include <stdio.h>
#include "xil_io.h"
#include "xil_mmu.h"
#include "xil_cache.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "sleep.h"

#define COMM_VAL    (*(volatile unsigned long *)(0xFFFF0000))

extern u32 MMUTable;

void        Xil_L1DCacheFlush(void);

int main( void )
{
	Xil_SetTlbAttributes(0xFFFF0000,0x14de2);           // S=b1 TEX=b100 AP=b11, Domain=b1111, C=b0, B=b0


    print("CPU1: init_platform\n\r");

	prvInitializeExceptions();
	init_axi_gpio();
	init_fifo_queues();

	startProductionControl();

	startAdversaryTask();

	// Start the tasks and timer running.
	vTaskStartScheduler();

	// If all is well, the scheduler will now be running
//	for( ;; );
	 while(1){
	     	while(COMM_VAL == 0){};
	  //   	if(irq_count > 0) {
	  //       	print("CPU1: Hello World With Interrupt CPU 1\n\r");
	  //       	irq_count = 0;
	  //       	sleep(2);		//Delay so output can be seen
	  //   	} else {
	     	print("CPU1: Hello World CPU 1\n\r");
	 //    	}
	     	COMM_VAL = 0;

	     }
	 return 0;
}



/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue or
	semaphore is created.  It is also called by various parts of the demo
	application.  If heap_1.c or heap_2.c are used, then the size of the heap
	available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

/*-----------------------------------------------------------*/
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* vApplicationStackOverflowHook() will only be called if
	configCHECK_FOR_STACK_OVERFLOW is set to either 1 or 2.  The handle and name
	of the offending task will be passed into the hook function via its
	parameters.  However, when a stack has overflowed, it is possible that the
	parameters will have been corrupted, in which case the pxCurrentTCB variable
	can be inspected directly. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationSetupHardware( void )
{
	/* Do nothing */
}
