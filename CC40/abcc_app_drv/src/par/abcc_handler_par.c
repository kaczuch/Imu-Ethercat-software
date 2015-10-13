/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
**                                                                            **
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, permission is      **
** granted to modify, reproduce and distribute the code in binary form        **
** without any restrictions.                                                  **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
** File Description:
** This file implements the ABCC_DunDriver() and ABCC_ISR() routine
** for parallel operating mode.
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#ifdef ABCC_USER_DRV_PARALLEL
#include "abcc_td.h"
#include "../abcc_drv_if.h"
#include "abp.h"
#include "abcc.h"
#include "../abcc_link.h"
#include "abcc_sys_adapt.h"
#include "../abcc_debug_err.h"
#include "../abcc_handler.h"
#include "../abcc_timer.h"



/*******************************************************************************
** Public Globals
********************************************************************************
*/


/*******************************************************************************
** Private Globals
********************************************************************************
*/

#ifndef ABCC_USER_HANDLE_IN_ABCC_ISR_MASK
#define ABCC_USER_HANDLE_IN_ABCC_ISR_MASK              ( ABCC_USER_INT_ENABLE_MASK )
#endif

/*******************************************************************************
** Public Functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ABCC_RunDriver()
**------------------------------------------------------------------------------
*/
BOOL ABCC_ParRunDriver( void )
{
   /*
   ** Set by ABCC_StartDriver to based on what is interrupt driven or not.
   */
   static BOOL  bPollRdPd = ( ( ABCC_USER_INT_ENABLE_MASK & ABP_INTMASK_RDPDIEN ) == 0 );
   static BOOL  bPollRdMsg = ( ( ABCC_USER_INT_ENABLE_MASK & ABP_INTMASK_RDMSGIEN ) == 0 );
   static BOOL  bPollWrMsg = ( ( ABCC_USER_INT_ENABLE_MASK & ( ABP_INTMASK_WRMSGIEN | ABP_INTMASK_ANBRIEN ) ) == 0 );
   static BOOL  bPollAnbStatus = ( ( ABCC_USER_INT_ENABLE_MASK & ABP_INTMASK_STATUSIEN ) == 0 );

   #ifdef ABCC_USER_POLL_WRPD
   static UINT8 bPollWrPd = TRUE;
#else
   static UINT8 bPollWrPd = FALSE;
#endif
   ABCC_MainStateType eMainState = ABCC_GetMainState();   

   if (  eMainState < ABCC_DRV_SETUP )
   {
      if ( eMainState !=  ABCC_DRV_ERROR )
      {
         ABCC_ERROR(ABCC_SEV_WARNING, ABCC_EC_INCORRECT_STATE, 0);
         ABCC_SetMainStateError();
      }
      return FALSE;
   }

   if ( bPollWrPd )
   {
      ABCC_NewWrPdEvent();
   }

   if ( bPollWrMsg )
   {
      ABCC_LinkCheckSendMessage();
   }
   if ( bPollAnbStatus )
   {
      ABCC_CheckAnbStatus();
   }

   if ( bPollRdPd)
   {
      ABCC_RdPdEvent();
   }

   if ( bPollRdMsg )
   {
      ABCC_RdMsgEvent();
   }

   return TRUE;
   /* end of AnybusMain() */
}


#ifdef ABCC_USER_INT_ENABLED
void ABCC_ParISR( void )
{
   UINT16 iIntStatus;
   UINT16 iIntToHandleInISR;
   UINT16 iIntToHandleInCbf;
   ABCC_MainStateType eMainState = ABCC_GetMainState(); 

   /*
   ** Let the driver handle the interrupt and clear the interrupt register.
   */
   iIntStatus = pnABCC_DrvISR();

   if ( eMainState < ABCC_DRV_WAIT_COMMUNICATION_RDY )
   {
      return;
   }

   if ( eMainState == ABCC_DRV_WAIT_COMMUNICATION_RDY )
   {
      ABCC_SetReadyForCommunication();
      return;
   }

   /*
   ** Only handle event defined  in ABCC_USER_HANDLE_IN_ABCC_ISR_MASK
   */
   iIntToHandleInISR = iIntStatus & ABCC_USER_HANDLE_IN_ABCC_ISR_MASK;

   if( iIntToHandleInISR & ( ABP_INTSTATUS_STATUSI ) )
   {
      ABCC_CheckAnbStatus();
   }

   if( iIntToHandleInISR & ABP_INTSTATUS_RDPDI )
   {
      ABCC_RdPdEvent();
   }

   if( iIntToHandleInISR & ABP_INTSTATUS_RDMSGI )
   {
      ABCC_RdMsgEvent();
   }

   if( iIntToHandleInISR & ( ABP_INTSTATUS_WRMSGI | ABP_INTSTATUS_ANBRI ) )
   {
      /*
      ** Check if there is something in the queue to be sent.
      */
      ABCC_LinkCheckSendMessage();
   }

   iIntToHandleInCbf = iIntStatus & ~ABCC_USER_HANDLE_IN_ABCC_ISR_MASK;

   if( iIntToHandleInCbf != 0 )
   {
      ABCC_CbfEvent( iIntToHandleInCbf );
   }
}
#else
void ABCC_ParISR( void )
{
   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_INTERNAL_ERROR, 0);
}
#endif

#endif /* ABCC_USER_DRV_PARALLEL */

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*******************************************************************************
** Tasks
********************************************************************************
*/
