#include "abcc_td.h"

#include "abcc.h"
#include "user_setup.h"

#include "ad_obj.h"        /* Application data object:   254                 */
#include "app_obj.h"       /* Application object:        255                 */

void ABCC_CbfUserInitReq( void )
{

   /*
   ** Triggers the user initiation sequence.
   **
   ** The function will indicate to ABCC_Driver that the initiation sequence has
   ** ended by calling ABCC_UserInitComplete(), which in turn will trigger
   ** SETUP_COMPLETE to be sent to the ABCC.
   */
	US_Init();
	US_RunUserInitStateMachine();
}

void ABCC_CbfAdiMappingReq( const AD_AdiEntryType**  const ppsAdiEntry,
                            const AD_DefaultMapType** const ppsDefaultMap )
{
   AD_AdiMappingReq( ppsAdiEntry, ppsDefaultMap );
}

BOOL ABCC_CbfUpdateWriteProcessData( UINT8* pxWritePd )
{
   return AD_UpdatePdWriteData( pxWritePd );
}

#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED
void ABCC_CbfRemapDone( void )
{
   AD_RemapDone();
}
#endif

void ABCC_CbfNewReadPd( UINT8* pxReadPd )
{
      AD_UpdatePdReadData( pxReadPd );
}

void ABCC_CbfDriverError( ABCC_SeverityType eSeverity, UINT16 iErrorCode, UINT32 lAddInfo )
{
/***
 * @todo reset maybe?
 */

   while ( 1 );
}

void ABCC_CbfReceiveMsg( ABP_MsgType* psReceivedMsg )
{
   switch( psReceivedMsg->sHeader.bDestObj )
   {
/*   case ABP_OBJ_NUM_ASM:      *//* Assembly mapping object          */
/*   case ABP_OBJ_NUM_MDD:      *//* Modular device                   */
/*   case ABP_OBJ_NUM_CIPID:    *//* CIP Identity                     */
/*   case ABP_OBJ_NUM_SYNC:     *//* Sync                             */



   case ABP_OBJ_NUM_APPD:

      /*
      ** We have received a command to the Application Data object.
      */
      AD_ProcObjectRequest( psReceivedMsg );
      break;


   case ABP_OBJ_NUM_APP:
      /*
      ** We have received a command to the Application
      */
      APP_ProcessCmdMsg( psReceivedMsg );
      break;

   default:
      /*
      ** We have received a command to an unsupported object.
      */
      ABP_SetMsgErrorResponse( psReceivedMsg, 1, ABP_ERR_UNSUP_OBJ );
      ABCC_SendRespMsg( psReceivedMsg );
      break;
   }
}

/**
 * @todo
 */
void ABCC_CbfWdTimeout( void )
{

}
/**
 * @todo
 */
void ABCC_CbfWdTimeoutRecovered( void )
{

}

void ABCC_CbfEvent( UINT16 iEvents)
{

   if( iEvents & ABP_INTSTATUS_STATUSI  )
   {
      ABCC_CheckAnbStatus();
   }

   if( iEvents & ABP_INTSTATUS_RDPDI )
   {
      ABCC_RdPdEvent();
   }

   if( iEvents & ABP_INTSTATUS_RDMSGI )
   {
      ABCC_RdMsgEvent();
   }

   if( iEvents & ( ABP_INTSTATUS_WRMSGI | ABP_INTSTATUS_ANBRI ) )
   {
      /*
      ** Check if there is something in the queue to be sent.
      */
      ABCC_CheckSendMessage();
   }

   /* Handle events. */
}
/**
 * @todo
 */
void ABCC_CbfAnbStatusChanged( void )
{

}

