
#include "hardware_utils.h"
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


#include "utils/uartstdio.h"








void io_init(){
//ADIS IO config
	//interrupt register
	GPIOIntRegister(GPIO_PORTB_BASE,intPortBRoutine);
	//Initializing GPIO
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_4|GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_0,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD_WPU );

	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_1|GPIO_PIN_2,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD );
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_1|GPIO_PIN_2);
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

	// Sync Interrupt configuration
	GPIOIntRegister(GPIO_PORTA_BASE,SyncInt);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_1,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD );
	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_1,GPIO_RISING_EDGE);
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_1);

//Anybus IO config
	//epi config
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
	GPIOPinConfigure(GPIO_PP3_EPI0S30);//CS GPIO_PL4_EPI0S26

	GPIOPinTypeEPI(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7);
	GPIOPinTypeEPI(GPIO_PORTG_BASE,GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinTypeEPI(GPIO_PORTM_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeEPI(GPIO_PORTL_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeEPI(GPIO_PORTQ_BASE,GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinTypeEPI(GPIO_PORTK_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeEPI(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	GPIOPinTypeEPI(GPIO_PORTP_BASE,GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeEPI(GPIO_PORTB_BASE,GPIO_PIN_3);

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
			EPI_HB8_CSCFG_CS |EPI_HB8_WRWAIT_0|EPI_HB8_RDWAIT_0
			), 4);
	//
	// Set the address map.
	//
	EPIAddressMapSet(EPI0_BASE, EPI_ADDR_PER_SIZE_64KB| EPI_ADDR_PER_BASE_A );

	//epi config end
	//
	// Wait for the EPI initialization to complete.
	//
	SysCtlDelay(8000);

	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_0,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_0);
	Hw_Reset();

}
void initTimer(uint32_t ui32SysClock,uint32_t timeNs)
{
	double cc = (double)ui32SysClock*(double)timeNs;
	cc = cc/(double)1000000000;
	uint32_t cycles = (uint32_t)cc;// accounting that time is in ns
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
	TimerLoadSet(TIMER0_BASE, TIMER_A, cycles);
	TimerIntRegister(TIMER0_BASE,TIMER_A,TimerInt);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

}
void initTimer1(uint32_t ui32SysClock,uint32_t timeNs)
{
	double cc = (double)ui32SysClock*(double)timeNs;
	cc = cc/(double)1000000000;
	uint32_t cycles = (uint32_t)cc;// accounting that time is in ns
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
	TimerLoadSet(TIMER1_BASE, TIMER_A, cycles);
	TimerIntRegister(TIMER1_BASE,TIMER_A,TimCLKTimeout);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

}
void initTimer2Enable(uint32_t ui32SysClock)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	SysCtlPeripheralReset(SYSCTL_PERIPH_TIMER3);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_ONE_SHOT_UP);
	TimerLoadSet(TIMER2_BASE, TIMER_A,0xFFFFFFFF);
	HWREG(TIMER2_BASE + TIMER_O_TAV) = 0;
	TimerEnable(TIMER2_BASE, TIMER_A);



}
void TimerInt()
{
	TimerIntDisable(TIMER0_BASE,TIMER_A);
	TimerIntClear(TIMER0_BASE,TIMER_A);
	ReqMeasure();
	TimerIntEnable(TIMER0_BASE,TIMER_A);
}
void TimCLKTimeout()
{
	TimerIntDisable(TIMER1_BASE,TIMER_A);
	TimerIntClear(TIMER1_BASE,TIMER_A);
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0);
	TimerIntEnable(TIMER1_BASE,TIMER_A);
}
void SyncInt()
{
	GPIOIntDisable(GPIO_PORTA_BASE,GPIO_PIN_1);
	// becouse of buffers ned to be cleared on beginning of this function
	GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_1);

	if(sync_sInstance.inCapture>0)
	{
		TimerEnable(TIMER0_BASE, TIMER_A);
		return;
	}
	ReqMeasure();
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_1);
}
void TimerEn()
{
	TimerEnable(TIMER0_BASE, TIMER_A);
	timeraa=1;
}
int TimerSt()
{
	return timeraa;
}
void ReqMeasure()
{
//	initTimer2Enable(CLOCK_RATE); No need to measre execution time anymore
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_PIN_2);
	TimerEnable(TIMER1_BASE, TIMER_A);
	reqMeasure=1;
}

bool isInterruptOnPin(uint32_t port,uint32_t PinNr){
	uint32_t interruptSource;
	bool isOnPinNr=false;
	interruptSource =HWREG(port+0x418);

	if (interruptSource&PinNr){
		isOnPinNr=true;
	}
	return isOnPinNr;
}

void intPortBRoutine(){

	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_0)){
		Hw_Int_disable();
		Hw_Int_enable();
		ABCC_ISR();

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_1)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_2)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_3)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_4)){
		adis_interupt();
	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_5)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_6)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_7)){

	}

}
