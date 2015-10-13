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
** ** This file implements the ABCC_DunDriver() and ABCC_ISR() routine
** for parallel ping/pong operating mode.
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#ifdef ABCC_USER_DRV_PARALLEL_30
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

/*******************************************************************************
** Public Functions
********************************************************************************
*/


/*------------------------------------------------------------------------------
** ABCC_RunDriver()
**------------------------------------------------------------------------------
*/
BOOL ABCC_Par30RunDriver( void )
{
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

#ifndef ABCC_USER_INT_ENABLED
   (void)pnABCC_DrvRunDriverRx();
    ABCC_CheckAnbStatus();
   ABCC_RdPdEvent();
   ABCC_RdMsgEvent();
#endif

   ABCC_NewWrPdEvent();
   ABCC_LinkCheckSendMessage();
   pnABCC_DrvRunDriverTx();

   return TRUE;
   /* end of AnybusMain() */
}


#ifdef ABCC_USER_INT_ENABLED
void ABCC_Par30ISR()
{
   UINT16 iIntStatus;
   ABCC_MainStateType eMainState = ABCC_GetMainState(); 

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
   ** Let the driver handle the interrupt and clear the interrupt register.
   */
   iIntStatus = pnABCC_DrvISR();

   ABCC_LinkRunDriverRx();
   ABCC_CheckAnbStatus();
   ABCC_RdPdEvent();
   ABCC_RdMsgEvent();
}
#else
void ABCC_Par30ISR()
{
   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_INTERNAL_ERROR, 0);
}
#endif

#endif /* ABCC_USER_DRV_PARALLEL_30 */


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*******************************************************************************
** Tasks
********************************************************************************
*/
