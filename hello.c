#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "adis.h"
#include "abcc_hardware_implementation.h"

#include "abcc_user_def.h"
#include "user_setup.h"
#include "user_adi.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "abcc_td.h"



/*******************************************************************************
** Constants
********************************************************************************
*/
typedef enum ABCCPowerStateType{
   ABCCPOWER_RESET,
   ABCCPOWER_STARTDRIVER,
   ABCCPOWER_RESETRELEASE,
   ABCCPOWER_WAITCOM,
   ABCCPOWER_RUNNING,
   ABCCPOWER_RESETREQ,
}ABCCPowerStateType;

/*******************************************************************************
** Globals
********************************************************************************
*/
BOOL8    fResetABCC = FALSE;
ABCCPowerStateType ABCCPowerState = ABCCPOWER_RESET;
//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 9600, 16000000);
}


/*------------------------------------------------------------------------------
** PrintException()
** Prints out network object specific error codes. Can be used for debugging
** when exceptions occur.
**------------------------------------------------------------------------------
*/
static void PrintException( ABP_MsgType* psNewMessage )
{
   switch ( psNewMessage->sHeader.bSourceId )
   {
   case US_ANB_EXC_SRC_ID:

      if ( !ABCC_VerifyMessage( psNewMessage ) )

      {
         while(1);
      }
      break;

   case US_NW_EXC_SRC_ID:

      if ( !ABCC_VerifyMessage( psNewMessage ) )

      {
         while(1);
      }
      break;

   default:

      while(1);


   }
 }
//*****************************************************************************
//
// Configure SSI0 in master Freescale (SPI) mode.  This example will send out
// 3 bytes of data, then wait for 3 bytes of data to come in.  This will all be
// done using the polling method.
//
//*****************************************************************************
int main(void)
{
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
	defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
	uint32_t ui32SysClock;
#endif


    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
	defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320,
    		CLOCK_RATE);

#else
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
#endif


    UINT32   lSw1;
    UINT32   lSw2;
    BOOL8    fPrintErrorMessage = TRUE;
    BOOL8    fQuit = FALSE;
        /*
     * initializing spi
     */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
	iADI_err=adis_init();
	init_anybus_hardware();
	// test while loop
	//SetOperatingMode(0);
	//initTimer(CLOCK_RATE,10000000);
	initTimer1(CLOCK_RATE,5005);// set pulse to 5us

	//TimerEn();
//	while(1)
//	{
//		if(!TimerSt())
//		{
//			//TimerEn();
//
//		}
//	}

/*
	UINT8 abBitReverseTable16[]={0x00, 0x00, 0x1F, 0x00, 0x00,0x00, 0x00, 0x01};
	UINT32  aaa=CRC_Crc32(&abBitReverseTable16,8);
	UARTprintf("SSI ->%d\n",&aaa);
	*/
	SysCtlDelay(800000);

    while(1)
    {

    	 switch( ABCCPowerState )
    	      {
    	      case ABCCPOWER_RESET:
    	         ABCC_SYS_HWReset();
    	         ABCCPowerState = ABCCPOWER_STARTDRIVER;
    	         break;
    	      case ABCCPOWER_STARTDRIVER:
    	    	  if( ABCC_StartDriver( ABCC_GetOpmode(), FALSE ) == ABCC_OK )
    	    	  {
    	    		  ABCCPowerState = ABCCPOWER_RESETRELEASE;
    	    	  }
    	    	  break;
    	      case ABCCPOWER_RESETRELEASE:
    	    	  ABCC_HWReleaseReset();
    	          ABCCPowerState = ABCCPOWER_WAITCOM;
    	          break;

    	      case ABCCPOWER_WAITCOM:

    	          if( ABCC_isReadyForCommunication() )
    	          {
    	        	  AD_NewWrPd();
    	              ABCCPowerState = ABCCPOWER_RUNNING;
    	          }
    	          break;

    	      case ABCCPOWER_RUNNING:
    	    	  if( fResetABCC )
    	          {
    	    		  fResetABCC = FALSE;
    	    		  ABCCPowerState = ABCCPOWER_RESETREQ;
    	    		  break;
    	          }
    	    	  /*
    	    	   ** Check if an exception has occurred and prints out the exception code and info.
    	    	   */
    	    	  int ali= ABCC_AnbState();
    	    	  if(ali==4){
    	    		  int ali= ABCC_AnbState();
    	    	  }
    	    	  if( (  ali== ABP_ANB_STATE_EXCEPTION ) ) /* An error has occurred. */
    	    	  {
    	    		  if( fPrintErrorMessage )
    	    		  {
    	    			  ABP_MsgType* psANBMsgCmd;
    	    			  ABP_MsgType* psNWMsgCmd;

    	    			  psANBMsgCmd = ABCC_MsgAlloc();
    	    			  ABCC_GetAttribute( psANBMsgCmd, ABP_OBJ_NUM_ANB, 1, ABP_ANB_IA_EXCEPTION, US_ANB_EXC_SRC_ID );
    	    			  ABCC_SendCmdMsg( psANBMsgCmd, PrintException );


    	    			  psNWMsgCmd = ABCC_MsgAlloc();
    	    			  ABCC_GetAttribute( psNWMsgCmd, ABP_OBJ_NUM_NW, 1, ABP_NW_IA_EXCEPTION_INFO, US_NW_EXC_SRC_ID );
    	    			  ABCC_SendCmdMsg( psNWMsgCmd, PrintException );


    	    			  fPrintErrorMessage = FALSE;
    	    		  }
    	    	  }
    	    	  else /* No error. */
    	    	  {
    	    		  fPrintErrorMessage = TRUE;
    	    	  }
    	    	  /*
    	    	   ** This function runs the driver and must be called cyclically.
    	    	   ** Typcally this function is called from the main loop.
    	    	   */
    	    	  ABCC_RunDriver();
    	    	  break;
    	      case ABCCPOWER_RESETREQ:
    	    	  ABCC_ShutdownDriver();
    	    	  ABCCPowerState = ABCCPOWER_RESET;
    	    	  break;

    	      default:
    	    	  break;
    	      }

    	 /*
    	 uint32_t response;
    	 uint32_t data = 128;
    	 		SSIDataPut(SSI2_BASE,data);
    	 		while(SSIBusy(SSI2_BASE))
    	 		{
    	 		}
    	 		SSIDataGet(SSI2_BASE, &response);
    	 		*/
/*
    	if(interr==true)
    	{
    		UARTprintf(" Interrupt! Weeee \n");
    		interr=false;
    	}
    	SysCtlDelay(2000);
*/

    }
    return(0);
}
