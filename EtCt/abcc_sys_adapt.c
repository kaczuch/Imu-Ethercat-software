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
#include "abcc_hardware_implementation.h"
/**
 * anybus include files
 */
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_par.h"
#include "abp.h"


BOOL ABCC_SYS_Init( void )
{
	/**
	 * enable ports
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

//static UINT8 sys_bOpmode = ABCC_USER_DRV_PARALLEL;
/*
static ABCC_SYS_SpiDataReceivedCbfType pnDataReadyCbf;



void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
	pnDataReadyCbf = pnDataReceived;
}
void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
	ABCC_SYS_UseCritical();
	ABCC_SYS_EnterCritical();
	anybus_SPI_SEND_Receive(pxSendDataBuffer,pxReceiveDataBuffer,iLength);
	ABCC_SYS_ExitCritical();
	pnDataReadyCbf();
}

*/
void ABCC_SYS_ParallelRead( UINT16 iMemOffset, UINT8* pbData, UINT16 iLength )
{
}

UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
}

UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
}


void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, UINT8* pbData, UINT16 iLength )
{
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
	Hw_Reset();
}


void ABCC_SYS_HWReleaseReset( void )
{
	Hw_Release_Reset();
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
}


UINT8* ABCC_SYS_ParallelGetWrPdBuffer( void )
{
}


/*
void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
}


void ABCC_SYS_SerSendReceive( UINT8* pbTxDataBuffer, UINT8* pbRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{
}
*/

void ABCC_SYS_SerRestart( void )
{
}


void ABCC_SYS_AbccInterruptEnable( void )
{
#ifdef ABCC_USER_INT_ENABLED
	Hw_Int_enable();
#endif
}


void ABCC_SYS_AbccInterruptDisable( void )
{
#ifdef ABCC_USER_INT_ENABLED
	Hw_Int_disable();
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
	Hw_Int_disable();
}
void ABCC_SYS_ExitCriticalImpl( void )
{
	Hw_Int_enable();
}
