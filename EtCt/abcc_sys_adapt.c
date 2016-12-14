/**
 * Adis imu headers
 */
#include "adis.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include <driverlib/gpio.h>
#include "driverlib/pin_map.h"
#include "adis_regs.h"

/*
 * Hardware implementation
 */
#include "hardware_utils.h"
/**
 * anybus include files
 */
//#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_par.h"
#include "abp.h"

static UINT8    sys_abReadProcessData[ ABCC_USER_MAX_PROCESS_DATA_SIZE ];  /* Process data byte array. */
static UINT8    sys_abWriteProcessData[ ABCC_USER_MAX_PROCESS_DATA_SIZE ]; /* Process data byte array. */

BOOL ABCC_SYS_Init( void )
{
	/**
	 * Done somwere else 
	 */

   return TRUE;
}
void ABCC_SYS_Close( void )
{
}

#ifdef ABCC_USER_INT_ENABLED
/*
unsigned __stdcall ISR( void *pMyID )
{
	ABCC_SYS_EnterCritical();
	ABCC_ISR();
	ABCC_SYS_ExitCritical();
	return 0;
}
*/
#endif

void ABCC_SYS_ParallelRead( UINT16 iMemOffset, UINT8* pbData, UINT16 iLength )
{
	memcpy(pbData, (void*)(ABCC_USER_PARALLEL_BASE_ADR +iMemOffset), iLength);
}

UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
}

UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
}


void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, UINT8* pbData, UINT16 iLength )
{
	memcpy((void*)(ABCC_USER_PARALLEL_BASE_ADR +iMemOffset),pbData, iLength);
}

void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 pbData )
{
}

void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 pbData )
{
}

void ABCC_SYS_SetOpmode( UINT8 bOpmode )
{
   /*
   ** This is done already in ABCC_SYS_init. Otherwise we cannot read the MD or MI
   ** values from the USB2 board.
   */
   (void)bOpmode;
}


UINT8 ABCC_SYS_GetOpmode( void )
{
	return ABP_OP_MODE_8_BIT_PARALLEL;
}



void ABCC_SYS_HWReset( void )
{
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_0,0);

	Hw_Reset();
}


void ABCC_SYS_HWReleaseReset( void )
{
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_0,GPIO_PIN_0);
}

UINT8 ABCC_SYS_ReadModuleId( void )
{
	return 2;
}


BOOL ABCC_SYS_ModuleDetect( void )
{
	return true;
}

/**
 * serial and parallel functions unimplemented
*/
UINT8* ABCC_SYS_ParallelGetRdPdBuffer( void )
{
   return sys_abReadProcessData;
}


UINT8* ABCC_SYS_ParallelGetWrPdBuffer( void )
{
   return sys_abWriteProcessData;
}

void ABCC_SYS_SerRestart( void )
{
}

void ABCC_SYS_AbccInterruptEnable( void )
{
#ifdef ABCC_USER_INT_ENABLED
	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_PIN_0);

#endif
}

void ABCC_SYS_AbccInterruptDisable( void )
{
#ifdef ABCC_USER_INT_ENABLED
	GPIOIntDisable(GPIO_PORTB_BASE,GPIO_PIN_0);
	GPIOIntClear(GPIO_PORTB_BASE,GPIO_PIN_0);
#endif
}
/**
 * interrupts
 */
void ABCC_SYS_UseCriticalImpl( void )
{
}

void ABCC_SYS_EnterCriticalImpl( void )
{
	ABCC_SYS_AbccInterruptDisable();
}
void ABCC_SYS_ExitCriticalImpl( void )
{
	ABCC_SYS_AbccInterruptEnable();
}
