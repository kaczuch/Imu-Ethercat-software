/*
 * abcc_hardware_implementation.c
 *
 *  Created on: 31 lip 2015
 *      Author: bkaczor
 */
#include "abcc_hardware_implementation.h"
/**
 * platform specyfic include files
 */
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include <driverlib/gpio.h>
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
/**
 * anybus include files
 */
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_spi.h"
#include "abcc.h"

#include "utils/uartstdio.h"

#include "../TM4C123GH6PM/support_lib.h"


void init_anybus_hardware()
{
	/**
	 * SSI 2 enabling sequence
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
	GPIOPinConfigure(GPIO_PB4_SSI2CLK);
	GPIOPinConfigure(GPIO_PB5_SSI2FSS);
	GPIOPinConfigure(GPIO_PB6_SSI2RX);
	GPIOPinConfigure(GPIO_PB7_SSI2TX);
	//giving ssi control ove pins
	GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
	                   GPIO_PIN_7);
	//configuration of ssi
	SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3,
	                       SSI_MODE_MASTER, 10000000, 8);
	// Enable the SSI0 module.
	SSIEnable(SSI2_BASE);
	/**
	 * GPIO configuration
	 */

	GPIOIntRegister(GPIO_PORTA_BASE,anybus_interrupt_routine);


	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);

	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,GPIO_PIN_7|GPIO_PIN_6);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_2);

	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_FALLING_EDGE);

}

void anybus_interrupt_routine()
{
	bool onPin6=isInterruptOnPin(GPIO_PORTA_BASE,GPIO_PIN_6);
	bool onPin7=isInterruptOnPin(GPIO_PORTA_BASE,GPIO_PIN_7);
	Hw_Int_disable();

	if(onPin6){

	ABCC_ISR();
	}
	if(onPin7){
		//sync
	}
	Hw_Int_enable();


}
void anybus_SPI_SEND_Receive(void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength)
{


	UINT8* Sendbuf=pxSendDataBuffer;
	UINT8* Receivebuf = pxReceiveDataBuffer;
	uint32_t response;
	int i;
	int j;
	while(SSIDataGetNonBlocking(SSI2_BASE,&response)!=0);
	for( i=0, j=0;i<iLength || j<iLength;)
	{
		uint32_t data = Sendbuf[i];
		if(i<iLength)
		{
			if (SSIDataPutNonBlocking(SSI2_BASE,data)!=0){
				++i;
			}
		}

		if (SSIDataGetNonBlocking(SSI2_BASE,&response)!=0){

			UINT8 recData = (UINT8)response;
			Receivebuf[j]=recData;
			++j;
		}
		//SSIDataGet(SSI2_BASE, &response);

	}

}
void Hw_Reset()
{
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0);
}
void Hw_Release_Reset()
{
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_PIN_2);
}


void Hw_Int_enable()
{
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_6);
	GPIOIntEnable(GPIO_PORTD_BASE,GPIO_PIN_6);
}
void Hw_Int_disable()
{
	GPIOIntDisable(GPIO_PORTA_BASE,GPIO_PIN_6);
	GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_6);
	GPIOIntDisable(GPIO_PORTD_BASE,GPIO_PIN_6);
	GPIOIntClear(GPIO_PORTD_BASE,GPIO_PIN_6);
}
