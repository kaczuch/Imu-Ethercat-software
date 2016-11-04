/*
 * adis.c
 *
 *  Created on: 16 lip 2015
 *      Author: bkaczor
 */

#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include <driverlib/gpio.h>
#include "driverlib/pin_map.h"
#include "adis.h"
#include "user_adi.h"
#include <inc/hw_types.h>
#include "TM4C123GH6PM\support_lib.h"
#include "ad_obj.h"

int val= 0;
	int kk;
	bool wait=false;

uint32_t adisRead[]=
{
		ADIS16400_READ_REG(ADIS16400_XGYRO_OUT),
		ADIS16400_READ_REG(ADIS16400_YGYRO_OUT),
		ADIS16400_READ_REG(ADIS16400_ZGYRO_OUT),
		ADIS16400_READ_REG(ADIS16400_XACCL_OUT),
		ADIS16400_READ_REG(ADIS16400_YACCL_OUT),
		ADIS16400_READ_REG(ADIS16400_ZACCL_OUT),
		ADIS16400_READ_REG(ADIS16400_DIAG_STAT)
};
/**
 * Global adis value
 */
uint32_t adisValues[7];

/**
 * initialization of imu for work
 */
uint32_t adis_init(){
	interr=false;
	adis_io_init();
	adis_reset();
	//initTimer();

	return adis_self_test();
}
/**
 * function initializing inputs and outputs for imu
 */
void adis_io_init(){
	//enabling port D
	//interrupt register
	GPIOIntRegister(GPIO_PORTB_BASE,intPortBRoutine);
	//Initializing GPIO
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_4|GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_0,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU );
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_1);
	//setting interrupt type
	GPIOIntTypeSet(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_RISING_EDGE);
	GPIOIntTypeSet(GPIO_PORTB_BASE,GPIO_PIN_0,GPIO_LOW_LEVEL);


	//configuring spi connection to imu
	//enabling ssi peripherial
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	//enabling port A

	//configure pins as ssi type
	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
	GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);

	//giving ssi control ove pins
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
	                   GPIO_PIN_2);

	//configuration of ssi

	SSIConfigSetExpClk(SSI0_BASE, CLOCK_RATE, SSI_FRF_MOTO_MODE_3,
	                       SSI_MODE_MASTER, 900000, 16);


	// Enable the SSI0 module.
	SSIEnable(SSI0_BASE);

	//enabling interupt on D port
	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_4|GPIO_PIN_0);


}
/**
 * interupt function gathering data from imu on demand
 * */
void adis_interupt()
{

	GPIOIntDisable(GPIO_PORTB_BASE,GPIO_PIN_4);
	// becouse of buffers ned to be cleared on beginning of this function
	GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_4);
	//read all the data
	uint32_t response;

	int i,j;
	int iLength=7;

	for( i=0;  i<iLength;++i)
	{
		SSIDataPut(SSI0_BASE,adisRead[i]);
		/*while(SSIBusy(SSI0_BASE))
				{
				}*/

		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&adisValues[i]);

	}


	iADI_Rot_Speed_X=adisValues[1]&0x3FFF;
	iADI_Rot_Speed_Y=adisValues[2]&0x3FFF;
	iADI_Rot_Speed_Z=adisValues[3]&0x3FFF;

	iADI_Accel_X=adisValues[4]&0x3FFF;
	iADI_Accel_Y=adisValues[5]&0x3FFF;
	iADI_Accel_Z=adisValues[6]&0x3FFF;
	iADI_err=val;//errCode;
	//stopCounting();
	//@todo dopisz czas do œredniego czasu oczekiwania

	AD_NewWrPd();

	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_4);
}

/**
 * make self test of imu
 */
uint32_t adis_self_test(){
	// disable interupts
	GPIOIntDisable(GPIO_PORTB_BASE,GPIO_PIN_4);
	//setting tenth bit in MSC_CTRL register in upper half shift is equal to number of bit minus 8
	uint32_t command=ADIS16400_WRITE_REG(ADIS16400_MSC_CTRL|ADIS16400_WRITE_UPPER_HALF)|(1<<2);
	uint32_t response;
	SSIDataPut(SSI0_BASE,command);
	while(SSIBusy(SSI0_BASE))
	{
	}
	SysCtlDelay(720);

	SSIDataGet(SSI0_BASE, &response);

	command=ADIS16400_READ_REG(ADIS16400_MSC_CTRL);
	SSIDataPut(SSI0_BASE,command);
	while(SSIBusy(SSI0_BASE))
	{
	}
	SysCtlDelay(720);

	SSIDataGet(SSI0_BASE, &response);
	// again to be able to get response
	SSIDataPut(SSI0_BASE,command);
	while(SSIBusy(SSI0_BASE))
	{
	}
	SysCtlDelay(720);

	SSIDataGet(SSI0_BASE, &response);
	//check if it is end of test
	while(response & ADIS16400_MSC_CTRL_INT_SELF_TEST!=0){
		SSIDataPut(SSI0_BASE,command);
		while(SSIBusy(SSI0_BASE))
		{
		}
		SysCtlDelay(720);

		SSIDataGet(SSI0_BASE, &response);
	}
	//we got response about our test now we can read result of it
	command = ADIS16400_READ_REG(ADIS16400_DIAG_STAT);
	SSIDataPut(SSI0_BASE,command);
	while(SSIBusy(SSI0_BASE))
	{
	}
	SysCtlDelay(720);
	SSIDataGet(SSI0_BASE, &response);
	// now we send something to get out our answer
	SSIDataPut(SSI0_BASE,command);
	while(SSIBusy(SSI0_BASE))
	{
	}
	SysCtlDelay(720);

	SSIDataGet(SSI0_BASE, &response);
	//


	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_4);

	return response;
}
/**
 * reset imu
 */
void adis_reset(){
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_7,0);
	SysCtlDelay(2000000);
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_7,GPIO_PIN_7);
//	TimerIntEnable(TIMER0_BASE,TIMER_A);
}
void initTimer()
{
	TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_ONE_SHOT));
	TimerClockSourceSet(TIMER0_BASE,TIMER_CLOCK_SYSTEM);
	TimerIntRegister(TIMER0_BASE,TIMER_A,TimerInt);

}
void TimerInt()
{
	TimerIntDisable(TIMER0_BASE,TIMER_A);
	TimerIntClear(TIMER0_BASE,TIMER_A);
	wait=false;
	TimerIntEnable(TIMER0_BASE,TIMER_A);
}
