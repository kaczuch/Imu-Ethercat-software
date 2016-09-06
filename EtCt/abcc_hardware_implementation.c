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
#include "driverlib/epi.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include <driverlib/gpio.h>
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include <driverlib/timer.h>
/**
 * anybus include files
 */
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_par.h"
#include "abcc.h"
#include "sync_obj.h"

#include "utils/uartstdio.h"

#include "../TM4C123GH6PM/support_lib.h"

double latency=0;
int cycles=1;
void init_anybus_hardware()
{
<<<<<<< Updated upstream
=======
	GPIOPinConfigure(GPIO_PA6_EPI0S8);//A0
	GPIOPinConfigure(GPIO_PA7_EPI0S9);//A1
	GPIOPinConfigure(GPIO_PG1_EPI0S10);//A2
	GPIOPinConfigure(GPIO_PG0_EPI0S11);//A3
	GPIOPinConfigure(GPIO_PM3_EPI0S12);//A4
	GPIOPinConfigure(GPIO_PM2_EPI0S13);//A5
	GPIOPinConfigure(GPIO_PM1_EPI0S14);//A6
	GPIOPinConfigure(GPIO_PM0_EPI0S15);//A7
	GPIOPinConfigure(GPIO_PL0_EPI0S16);//A8
	GPIOPinConfigure(GPIO_PL1_EPI0S17);//A9
	GPIOPinConfigure(GPIO_PL2_EPI0S18);//A10
	GPIOPinConfigure(GPIO_PL3_EPI0S19);//A11
	GPIOPinConfigure(GPIO_PQ0_EPI0S20);//A12
	GPIOPinConfigure(GPIO_PQ1_EPI0S21);//A13

	GPIOPinConfigure(GPIO_PK0_EPI0S0);//D0
	GPIOPinConfigure(GPIO_PK1_EPI0S1);//D1
	GPIOPinConfigure(GPIO_PK2_EPI0S2);//D2
	GPIOPinConfigure(GPIO_PK3_EPI0S3);//D3
	GPIOPinConfigure(GPIO_PC7_EPI0S4);//D4
	GPIOPinConfigure(GPIO_PC6_EPI0S5);//D5
	GPIOPinConfigure(GPIO_PC5_EPI0S6);//D6
	GPIOPinConfigure(GPIO_PC4_EPI0S7);//D7


	GPIOPinConfigure(GPIO_PP2_EPI0S29);//WE
	GPIOPinConfigure(GPIO_PB3_EPI0S28);//OE
	GPIOPinConfigure(GPIO_PL4_EPI0S26);//CS

	GPIOPinTypeEPI(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7);
	GPIOPinTypeEPI(GPIO_PORTG_BASE,GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinTypeEPI(GPIO_PORTM_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeEPI(GPIO_PORTL_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
	GPIOPinTypeEPI(GPIO_PORTQ_BASE,GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinTypeEPI(GPIO_PORTK_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeEPI(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	GPIOPinTypeEPI(GPIO_PORTP_BASE,GPIO_PIN_2);
	GPIOPinTypeEPI(GPIO_PORTB_BASE,GPIO_PIN_3);


>>>>>>> Stashed changes
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EPI0);
	//
	// Set the EPI divider.
	//
	EPIDividerSet(EPI0_BASE, 2);
	//
	// Select SDRAM mode.
	//
	EPIModeSet(EPI0_BASE, EPI_MODE_HB8);
	//
	// Configure SDRAM mode.
	//
	EPIConfigHB8Set(EPI0_BASE, (EPI_HB8_MODE_ADDEMUX |
			EPI_HB8_CSCFG_ALE_DUAL_CS //|EPI_HB8_WRWAIT_3|EPI_HB8_RDWAIT_3
			|EPI_HB8_WORD_ACCESS), 255);
	//
	// Set the address map.
	//
	EPIAddressMapSet(EPI0_BASE, EPI_ADDR_PER_SIZE_64KB| EPI_ADDR_PER_BASE_A | EPI_ADDR_RAM_SIZE_64KB | EPI_ADDR_RAM_BASE_6);
	//
	// Wait for the EPI initialization to complete.
	//
	//while(HWREG(EPI0_BASE + EPI_O_STAT) &  EPI_STAT_INITSEQ)
	//{
	//
	// Wait for SDRAM initialization to complete.
	//
	//}
	//
	// At this point, the SDRAM is accessible and available for use.
	//
	/**
	 * SSI 2 enabling sequence

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

	//GPIOIntRegister(GPIO_PORTB_BASE,anybus_interrupt_routine);


	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_0,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);

	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_0);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_0);

	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_FALLING_EDGE);


}
/*
void anybus_interrupt_routine()
{
	bool onPin6=isInterruptOnPin(GPIO_PORTA_BASE,GPIO_PIN_6);
	bool onPin7=isInterruptOnPin(GPIO_PORTA_BASE,GPIO_PIN_7);
	Hw_Int_disable();

	if(onPin6){

		ABCC_ISR();
	}
	if(onPin7){
		Hw_SyncIntRout();
	}
	Hw_Int_enable();

	initTimer3();
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

}*/
void Hw_Reset()
{
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_0,0);
}
void Hw_Release_Reset()
{
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_0,GPIO_PIN_0);
}


void Hw_Int_enable()
{
	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_0);
	//GPIOIntEnable(GPIO_PORTD_BASE,GPIO_PIN_6);
}
void Hw_Int_disable()
{
	GPIOIntDisable(GPIO_PORTB_BASE,GPIO_PIN_0);
	GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_0);
	//GPIOIntDisable(GPIO_PORTD_BASE,GPIO_PIN_6);
	//GPIOIntClear(GPIO_PORTD_BASE,GPIO_PIN_6);
}

void Hw_SyncIntRout()
{
	//set timer to count time
	startCounting();
	//set high state to pin
	GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_PIN_7);
	//start timer to 5 micro sec

}
void initTimer3() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    SysCtlPeripheralReset(SYSCTL_PERIPH_TIMER3);    //
    TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT_UP);
    TimerControlStall(TIMER3_BASE, TIMER_A, true) ;
}

void startCounting() {
    TimerDisable(TIMER3_BASE, TIMER_A) ;
    TimerLoadSet(TIMER3_BASE, TIMER_A,0xFFFFFFFF) ;
    TimerEnable(TIMER3_BASE, TIMER_A) ;
}

void stopCounting() {
	uint32_t count = TimerValueGet(TIMER3_BASE, TIMER_A);
    TimerIntDisable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    TimerDisable(TIMER3_BASE, TIMER_A);
    if(count>0){
    cycles++;
		if (cycles<100000){
			latency = (double)count/(double)SysCtlClockGet();  // forcing  double just in case there's an issue w compiler
			sync_sInstance.inProces=sync_sInstance.inProces+(latency-sync_sInstance.inProces)/cycles;
		}
    }

}
