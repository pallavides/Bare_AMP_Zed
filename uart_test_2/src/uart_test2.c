/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xuartps.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define UART_DEVICE_ID                  XPAR_XUARTPS_0_DEVICE_ID

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

int UartPsHelloWorldExample(u16 DeviceId);

/************************** Variable Definitions *****************************/

XUartPs Uart_Ps;		/* The instance of the UART Driver */

/*****************************************************************************/
/**
*
* Main function to call the Hello World example.
*
* @param	None
*
* @return
*		- XST_FAILURE if the Test Failed .
*		- A non-negative number indicating the number of characters
*		  sent.
*
* @note		None
*
******************************************************************************/
int main(void)
{
	int Status;

	/*
	 * Run the Hello World example , specify the the Device ID that is
	 * generated in xparameters.h
	 */
	Status = UartPsHelloWorldExample(UART_DEVICE_ID);

	return Status;
}

/****************************************************************************/
/**
*
* This function sends 'Hello World' to an external terminal in polled mode.
* The purpose of this function is to illustrate how to use the XUartPs driver.
*
*
* @param	DeviceId is the unique ID for the device from hardware build.
*
* @return
*		- XST_FAILURE if the UART driver could not be initialized
*		  successfully.
*		- A non-negative number indicating the number of characters
*		  sent.
*
* @note		None.
*
****************************************************************************/
int UartPsHelloWorldExample(u16 DeviceId)
{
	u8 HelloWorld[] = "Hello World";
	int SentCount = 0;
	int Status;
	XUartPs_Config *Config;

	/*
	 * Initialize the UART driver so that it's ready to use
	 * Look up the configuration in the config table and then initialize it.
	 */
	Config = XUartPs_LookupConfig(DeviceId);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XUartPs_CfgInitialize(&Uart_Ps, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XUartPs_SetBaudRate(&Uart_Ps, 115200);

	while (SentCount < (sizeof(HelloWorld) - 1)) {
		/*
		 * Transmit the data
		 */
		SentCount += XUartPs_Send(&Uart_Ps,
					   &HelloWorld[SentCount], 1);
	}

	return SentCount;
}
