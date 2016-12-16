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
#include "ad_obj.h"
#include "sync_obj.h"
#include "inc/hw_timer.h"

int val= 0;
int kk;
bool wait=false;
int timeraa=1;
int reqMeasure=0;
int timm;
int debug;

uint32_t adisRead[]=
{
		ADIS16400_READ_REG(ADIS16400_XGYRO_OUT),
		ADIS16400_READ_REG(ADIS16400_YGYRO_OUT),
		ADIS16400_READ_REG(ADIS16400_ZGYRO_OUT),
		ADIS16400_READ_REG(ADIS16400_XACCL_OUT),
		ADIS16400_READ_REG(ADIS16400_YACCL_OUT),
		ADIS16400_READ_REG(ADIS16400_ZACCL_OUT),
		ADIS16400_READ_REG(ADIS16400_DIAG_STAT),

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
	//adis_io_init();
	adis_reset();



	return adis_self_test();
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


	int i;
	int iLength=7;

	for( i=0;  i<iLength;++i)
	{
		SSIDataPut(SSI0_BASE,adisRead[i]);
	// Need to wait because ssibusy is not enough
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&adisValues[i]);

	}

	iADI_Rot_Speed_X=(adisValues[1]&0x3FFF)<<2;
	iADI_Rot_Speed_Y=(adisValues[2]&0x3FFF)<<2;
	iADI_Rot_Speed_Z=(adisValues[3]&0x3FFF)<<2;

	iADI_Rot_Speed_X=iADI_Rot_Speed_X>>2;
	iADI_Rot_Speed_Y=iADI_Rot_Speed_Y>>2;
	iADI_Rot_Speed_Z=iADI_Rot_Speed_Z>>2;

	iADI_Accel_X=(adisValues[4]&0x3FFF)<<2;
	iADI_Accel_Y=(adisValues[5]&0x3FFF)<<2;
	iADI_Accel_Z=(adisValues[6]&0x3FFF)<<2;
	iADI_err=timm;//errCode;

	iADI_Accel_X=iADI_Accel_X>>2;
	iADI_Accel_Y=iADI_Accel_Y>>2;
	iADI_Accel_Z=iADI_Accel_Z>>2;

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
	GetIDandSerNr();
	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_4);
	return response;
}
/**
 * reset imu
 */
void adis_reset(){
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,0);
	SysCtlDelay(2000000);
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,GPIO_PIN_1);
//	TimerIntEnable(TIMER0_BASE,TIMER_A);
}

int SetOperatingMode(UINT16 mode)
{
	if(mode==ADIS_OP_MODE_AUTO)
	{
		adis_reset();
		return adis_self_test();
	}

	else if(mode==ADIS_OP_MODE_MANUAL)
	{
		if(sync_sInstance.inCapture!=0){
			initTimer(CLOCK_RATE,sync_sInstance.inCapture);
		}
		uint32_t command=ADIS16400_WRITE_REG(ADIS16400_GPIO_CTRL)|(1);
		uint32_t response;
		SSIDataPut(SSI0_BASE,command);
		while(SSIBusy(SSI0_BASE))
		{
		}
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE, &response);
		command=ADIS16400_WRITE_REG(ADIS16400_SMPL_PRD);
		SSIDataPut(SSI0_BASE,command);
		while(SSIBusy(SSI0_BASE))
		{
		}
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE, &response);

		command = ADIS16400_READ_REG(ADIS16400_GPIO_CTRL);
		SSIDataPut(SSI0_BASE,command);
		while(SSIBusy(SSI0_BASE))
		{
		}
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE, &response);

		if(response & 1)
			return ADIS_MODE_SET_SUCCESS;
	}
	return ADIS_MODE_SET_ERROR;
}
void ReadGTemp()
{
	int i=0;
	uint32_t ReadSeq[]={
			ADIS16400_READ_REG(ADIS16350_XTEMP_OUT),
			ADIS16400_READ_REG(ADIS16350_YTEMP_OUT),
			ADIS16400_READ_REG(ADIS16350_ZTEMP_OUT),
			ADIS16400_READ_REG(ADIS16400_DIAG_STAT)
	};
	uint32_t RetVal[4];
	for( i=0;  i<4;++i)
	{
		SSIDataPut(SSI0_BASE,ReadSeq[i]);
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&RetVal[i]);
	}
	GTemp[0]= RetVal[1];
	GTemp[1]= RetVal[2];
	GTemp[2]= RetVal[3];

}
void ReadGBias()
{
	int i=0;
	uint32_t ReadSeq[]={
	ADIS16400_READ_REG(ADIS16400_XGYRO_OFF),
	ADIS16400_READ_REG(ADIS16400_YGYRO_OFF),
	ADIS16400_READ_REG(ADIS16400_ZGYRO_OFF),
	ADIS16400_READ_REG(ADIS16400_DIAG_STAT)
	};
	uint32_t RetVal[4];
	for( i=0;  i<4;++i)
	{
		SSIDataPut(SSI0_BASE,ReadSeq[i]);
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&RetVal[i]);
	}
	GBias[0]= RetVal[1];
	GBias[1]= RetVal[2];
	GBias[2]= RetVal[3];
}
int WriteGBias()
{
	WriteReg(ADIS16400_XGYRO_OFF,GBias[0]);
	WriteReg(ADIS16400_YGYRO_OFF,GBias[1]);
	WriteReg(ADIS16400_ZGYRO_OFF,GBias[2]);
	return 0;
}
void ReadABias()
{
	int i=0;
	uint32_t ReadSeq[]={
	ADIS16400_READ_REG(ADIS16400_XACCL_OFF),
	ADIS16400_READ_REG(ADIS16400_YACCL_OFF),
	ADIS16400_READ_REG(ADIS16400_ZACCL_OFF),
	ADIS16400_READ_REG(ADIS16400_DIAG_STAT)
	};
	uint32_t RetVal[4];
	for( i=0;  i<4;++i)
	{
		SSIDataPut(SSI0_BASE,ReadSeq[i]);
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&RetVal[i]);
	}
	AccBias[0]= RetVal[1];
	AccBias[1]= RetVal[2];
	AccBias[2]= RetVal[3];
}
int WriteABias()
{

	WriteReg(ADIS16400_XACCL_OFF,AccBias[0]);
	WriteReg(ADIS16400_YACCL_OFF,AccBias[1]);
	WriteReg(ADIS16400_ZACCL_OFF,AccBias[2]);
	return 0;
}
void ReadDFilter()
{
	int i=0;
	uint32_t ReadSeq[]={
	ADIS16400_READ_REG(ADIS16400_SENS_AVG),
	ADIS16400_READ_REG(ADIS16400_DIAG_STAT)
	};
	uint32_t RetVal[4];
	for( i=0;  i<2;++i)
	{
		SSIDataPut(SSI0_BASE,ReadSeq[i]);
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&RetVal[i]);
	}
	DFilter= RetVal[1]&0xF;

}
int WriteDFilter()
{
	WriteHalfReg(ADIS16400_SENS_AVG,0,DFilter);
	return 0;
}
int GlobCmmdAutonullSet()
{
	WriteHalfReg(ADIS16400_GLOB_CMD,0,Autonull);
	return 0;
}

int GlobCmmdFactoryCalSet()
{
	WriteHalfReg(ADIS16400_GLOB_CMD,0,FactoryCall<<1);
	return 0;
}
int GlobCmmdPrecisionAutonullSet()
{
	WriteHalfReg(ADIS16400_GLOB_CMD,0,PreciseAutonull<<4);
	return 0;
}

void GlobCmmdGet()
{
	Autonull=0;
	FactoryCall=0;
	PreciseAutonull=0;
}

void GetIDandSerNr()
{
	int i=0;
	uint32_t ReadSeq[]={
	ADIS16400_READ_REG(ADIS16400_PRODUCT_ID),
	ADIS16400_READ_REG(ADIS16400_SERIAL_NUMBER),
	ADIS16400_READ_REG(ADIS16400_DIAG_STAT)
	};
	uint32_t RetVal[4];
	for( i=0;  i<3;++i)
	{
		SSIDataPut(SSI0_BASE,ReadSeq[i]);
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&RetVal[i]);
	}
	ID= RetVal[1];
	SerialNB= RetVal[2];
}
void WriteReg(INT16 Reg,INT16 data)
{
	int i=0;
	uint32_t ReadSeq[]={
		ADIS16400_WRITE_REG(Reg)|(data & 0xFF),
		ADIS16400_WRITE_REG(Reg+1)|(data>>8)
	};
	uint32_t RetVal[4];
	for( i=0;  i<2;++i)
	{
		SSIDataPut(SSI0_BASE,ReadSeq[i]);
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&RetVal[i]);
	}
	//DFilter= RetVal[1];
}
void WriteHalfReg(INT16 Reg,INT16 half,INT16 data)
{
	int i=0;
	uint32_t ReadSeq[]={
		ADIS16400_WRITE_REG(Reg|half)|(data & 0xFF),
	};
	uint32_t RetVal[4];
	for( i=0;  i<1;++i)
	{
		SSIDataPut(SSI0_BASE,ReadSeq[i]);
		SysCtlDelay(720);
		SSIDataGet(SSI0_BASE,&RetVal[i]);
	}
	//DFilter= RetVal[1];
}
void ReadGyroRange()
{
	int i=0;
		uint32_t ReadSeq[]={
		ADIS16400_READ_REG(ADIS16400_SENS_AVG),
		ADIS16400_READ_REG(ADIS16400_DIAG_STAT)
		};
		uint32_t RetVal[4];
		for( i=0;  i<2;++i)
		{
			SSIDataPut(SSI0_BASE,ReadSeq[i]);
			SysCtlDelay(720);
			SSIDataGet(SSI0_BASE,&RetVal[i]);
		}
		DRange= (RetVal[1]>>8)&0xF;
}
int WriteGyroRange()
{
	if(DRange==3)
	{
		return 1;
	}
	if((DRange==(1<<1))&&(DFilter<=0x2))
	{
		return 1;
	}
	if((DRange==1)&&(DFilter<=0x4))
	{
		return 1;
	}
	WriteHalfReg(ADIS16400_SENS_AVG,1,DRange);
	return 0;
}
