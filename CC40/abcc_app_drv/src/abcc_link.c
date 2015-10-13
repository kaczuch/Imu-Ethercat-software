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
** Implements message queuing and write message flow control.
**. and response handler functionality
**
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abcc_debug_err.h"
#include "abcc_link.h"
#include "abcc_drv_if.h"
#include "abcc_mem.h"
#include "abcc_sys_adapt.h"
#include "abcc_timer.h"



/*******************************************************************************
** Constants
********************************************************************************
*/

/*
** Set default value for maximum number of resources 
*/
#ifndef ABCC_USER_MAX_NUM_MSG_RESOURCES
#define ABCC_USER_MAX_NUM_MSG_RESOURCES          ( ABCC_USER_MAX_NUM_APPL_CMDS + ABCC_USER_MAX_NUM_ABCC_CMDS )
#endif


/*
** Max number of messages in each send queue.
*/
#define LINK_MAX_NUM_CMDS_IN_Q            ABCC_USER_MAX_NUM_APPL_CMDS
#define LINK_MAX_NUM_RESP_IN_Q            ABCC_USER_MAX_NUM_ABCC_CMDS

/*
** Total number of message resources.
*/
#define LINK_NUM_MSG_IN_POOL              ABCC_USER_MAX_NUM_MSG_RESOURCES

/*
** Max num message handlers for responses.
** The max number of outstanding commands counter is decremented
** before the handler is invoked therefore we need to handle 1 extra
** handler
*/
#define LINK_MAX_NUM_MSG_HDL ( LINK_MAX_NUM_CMDS_IN_Q + 1 )


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*
** Message queue type for queueing cmds and responses.
*/
typedef struct MsgQueueType
{
   ABP_MsgType** queue;
   INT8 bReadIndex;
   INT8 bQueueSize;
   INT8 bNumInQueue;
} MsgQueueType;


/*******************************************************************************
** Public Globals
********************************************************************************
*/


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*
 * Largest supported message size.
 */
static UINT16 link_iMaxMsgSize;

/*
 ** Allocate space for the message buffers (align with 4 bytes )
 */
#define LINK_MSG_RESOURCE_SIZE ( ABCC_USER_MAX_MSG_SIZE + ( ABCC_USER_MAX_MSG_SIZE % 4 ) + sizeof( ABP_MsgHeaderType ) )

UINT8 link_bPool[ABCC_MEM_REQUIRED_POOLSIZE( LINK_MSG_RESOURCE_SIZE , LINK_NUM_MSG_IN_POOL )];
static ABCC_MemHandlerType* link_psMemHandler;

/*
 ** Command and response queues
 */
static ABP_MsgType* link_psCmds[LINK_MAX_NUM_CMDS_IN_Q ];
static ABP_MsgType* link_psResponses[LINK_MAX_NUM_RESP_IN_Q];

static MsgQueueType link_sCmdQueue;
static MsgQueueType link_sRespQueue;

/*
 ** Response handlers
 */
static ABCC_MsgHandlerFuncType link_pnMsgHandler[ LINK_MAX_NUM_MSG_HDL ];
static UINT8              link_bMsgSrcId[ LINK_MAX_NUM_MSG_HDL ];

static ABCC_LinkNotifyIndType pnMsgSentHandler;
static ABP_MsgType* link_psNotifyMsg;


/*
 ** Max number of outstanding commands ( no received response yet )
 */
static UINT8 link_bNumberOfOutstandingCommands = 0;

/*
 ** True if we failed to allocate a msg recive buffer
 */
static BOOL fNoMsgReceiverBuffer = FALSE;

/*******************************************************************************
** Private Services
********************************************************************************
*/
static ABP_MsgType* link_DeQueue( MsgQueueType* psMsgQueue )
{
   ABP_MsgType* psMsg = NULL;
   if ( psMsgQueue->bNumInQueue != 0 )
   {
      psMsg = psMsgQueue->queue[ psMsgQueue->bReadIndex++];
      psMsgQueue->bNumInQueue--;
      psMsgQueue->bReadIndex %= psMsgQueue->bQueueSize;
   }
   ABCC_ASSERT(psMsgQueue->bNumInQueue >= 0);
   return psMsg;
}


static BOOL link_EnQueue( MsgQueueType* psMsgQueue, ABP_MsgType* psMsg )
{
   if ( psMsgQueue->bNumInQueue <  psMsgQueue->bQueueSize )
   {
      psMsgQueue->queue[  ( psMsgQueue->bNumInQueue + psMsgQueue->bReadIndex ) %  psMsgQueue->bQueueSize ] = psMsg;
      psMsgQueue->bNumInQueue++;
      return TRUE;
   }
   return FALSE;
}


static void link_CheckNotification( const ABP_MsgType* const psMsg )
{
   if( ( pnMsgSentHandler != NULL ) &&  ( psMsg == link_psNotifyMsg ) )
   {
      pnMsgSentHandler();
      pnMsgSentHandler = NULL;
   }
}

/*******************************************************************************
** Public Services
********************************************************************************
*/
void ABCC_LinkInit( void )
{
   UINT16 iCount;
   /*
   ** Init Queue structures.
   */
   link_sCmdQueue.bNumInQueue = 0;
   link_sCmdQueue.bQueueSize = LINK_MAX_NUM_CMDS_IN_Q;
   link_sCmdQueue.bReadIndex = 0;
   link_sCmdQueue.queue = link_psCmds;

   link_sRespQueue.bNumInQueue = 0;
   link_sRespQueue.bQueueSize = LINK_MAX_NUM_RESP_IN_Q;
   link_sRespQueue.bReadIndex = 0;
   link_sRespQueue.queue = link_psResponses;

   link_psMemHandler = ABCC_MemCreatePool( link_bPool, LINK_MSG_RESOURCE_SIZE, LINK_NUM_MSG_IN_POOL );

   for( iCount = 0; iCount < LINK_MAX_NUM_CMDS_IN_Q; iCount++  )
   {
      link_pnMsgHandler[ iCount ] = 0;
      link_bMsgSrcId[iCount ] = 0;
   }

   /*
   ** Initialize driver privates and states to default values.
   */   
   link_bNumberOfOutstandingCommands = 0;
   
   pnMsgSentHandler = NULL;
   link_psNotifyMsg = NULL;

   link_iMaxMsgSize = ABCC_USER_MAX_MSG_SIZE;
   /*
    ** Limit ABCC 30 messages to 255 bytes
    */
   if ( ABCC_SYS_ReadModuleId() == ABP_MODULE_ID_ACTIVE_ABCC30 )
   {
      if ( link_iMaxMsgSize > ABP_MAX_MSG_255_DATA_BYTES )
      {
         link_iMaxMsgSize = ABP_MAX_MSG_255_DATA_BYTES;
      }
   }
}


ABP_MsgType* ABCC_LinkReadMessage( void )
{
   ABP_MsgType* psReadMessage;
   ABP_MsgType* link_psReadMessage;
   ABCC_SYS_UseCritical();

   psReadMessage = (ABP_MsgType*)pnABCC_DrvReadMessage();

   if( psReadMessage != NULL )
   {
      ABCC_SYS_EnterCritical();
      if( !fNoMsgReceiverBuffer )
      {
         link_psReadMessage = ABCC_LinkAlloc();
         if( link_psReadMessage == NULL )
         {
            fNoMsgReceiverBuffer = TRUE;
            ABCC_ASSERT_ERR( link_psReadMessage != NULL, ABCC_SEV_WARNING, ABCC_EC_OUT_OF_MSG_BUFFERS, 0 );
         }
         pnABCC_DrvSetMsgReceiverBuffer( link_psReadMessage );
      }
      ABCC_SYS_ExitCritical();

      if( ( psReadMessage->sHeader.bCmd & ABP_MSG_HEADER_C_BIT ) == 0 )
      {
         /*
          ** Decrement number of outstanding commands ifa response is recieved
          */
         ABCC_SYS_EnterCritical();
         link_bNumberOfOutstandingCommands--;
         ABCC_SYS_ExitCritical();
      }
   }
   return psReadMessage;
}


void ABCC_LinkCheckSendMessage( void )
{
   BOOL fSendMsg = FALSE;
   ABP_MsgType* psWriteMessage = NULL;
   ABCC_SYS_UseCritical();
   /*
   ** Check if any messages are queued. 
   ** If the queue index > 0 then there are messages in the qeueue.
   ** Response messages are prioritised before command messages.
   */
   ABCC_SYS_EnterCritical();

   if ( link_sRespQueue.bNumInQueue > 0 )
   {
      if( pnABCC_DrvISReadyForWriteMessage() )
      {
         psWriteMessage = link_DeQueue( &link_sRespQueue );
         fSendMsg = TRUE;
         DEBUG_EVENT(("RESP send msg"));
      }
   }
   else if ( link_sCmdQueue.bNumInQueue > 0 )
   {
      if( pnABCC_DrvISReadyForCmd() )
      {
         psWriteMessage = link_DeQueue( &link_sCmdQueue );
         fSendMsg = TRUE;
         DEBUG_EVENT(("CMD send msg"));
      }
      else
      {
         DEBUG_EVENT(("Denied CMD send msg"));
      }
   }

   if ( fSendMsg )
   {
      if ( pnABCC_DrvWriteMessage( psWriteMessage ) )
      {
         link_CheckNotification( psWriteMessage );
         ABCC_LinkFree( &psWriteMessage );
      }
   }

   ABCC_SYS_ExitCritical();
}


void ABCC_LinkRunDriverRx( void )
{
   ABP_MsgType* psSentMsg;

   psSentMsg = pnABCC_DrvRunDriverRx();
   /*
   ** If a write message was sent, free the buffer.
   */
   if( psSentMsg )
   {
      link_CheckNotification( psSentMsg );
      ABCC_LinkFree( &psSentMsg );
   }
}


UINT16 ABCC_LinkGetNumCmdQueueEntries( void )
{
   UINT16 iQEntries;
   iQEntries =  ABCC_USER_MAX_NUM_APPL_CMDS - link_bNumberOfOutstandingCommands;
   return iQEntries;
}


ABCC_MsgStatusType ABCC_LinkWriteMessage( ABP_MsgType* psWriteMsg )
{
   BOOL fSendMsg = FALSE;
   ABCC_MsgStatusType eQStatus = ABCC_MSG_IN_Q;
   
   if ( psWriteMsg->sHeader.iDataSize > link_iMaxMsgSize )
   {
      return ABCC_MSG_SIZE_ERR;
   }


   ABCC_SYS_UseCritical();
   ABCC_SYS_EnterCritical();

   if ( psWriteMsg->sHeader.bCmd & ABP_MSG_HEADER_C_BIT )
   {
      if ( ( ( ABCC_USER_MAX_NUM_APPL_CMDS - link_bNumberOfOutstandingCommands ) <= 0 ) )
      {
         eQStatus = ABCC_MSG_Q_FULL;
      }
      else if( ( link_sCmdQueue.bNumInQueue == 0 ) && pnABCC_DrvISReadyForCmd()  )
      {
         link_bNumberOfOutstandingCommands++;
         fSendMsg = TRUE;
      }
      else if ( link_EnQueue(&link_sCmdQueue,  psWriteMsg ) )
      {
         link_bNumberOfOutstandingCommands++;
      }
      else
      {
         ABCC_ASSERT( FALSE );
      }
   }
   else
   {
      if( ( link_sRespQueue.bNumInQueue == 0 ) && pnABCC_DrvISReadyForWriteMessage() )
      {
         fSendMsg = TRUE;
      }
      else if ( !link_EnQueue(&link_sRespQueue,  psWriteMsg ) )
      {
         eQStatus = ABCC_MSG_ERROR;
         ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_LINK_CMD_QUEUE_FULL, (UINT32)psWriteMsg );
         ABCC_LinkFree( &psWriteMsg );
      }
   }

   if ( fSendMsg )
   {
      if ( pnABCC_DrvWriteMessage( psWriteMsg ) )
      {
         eQStatus = ABCC_MSG_SENT;
         ABCC_LinkFree( &psWriteMsg );
      }
   }
   ABCC_SYS_ExitCritical();
   return eQStatus;
}



ABCC_MsgStatusType ABCC_LinkWrMsgWithNotification( ABP_MsgType* psWriteMsg, ABCC_LinkNotifyIndType pnHandler )
{
   ABCC_MsgStatusType eMsgStatus;
   /*
    * Save function to call when message is successfully sent.
    */

   eMsgStatus = ABCC_LinkWriteMessage( psWriteMsg );

   if ( eMsgStatus  == ABCC_MSG_IN_Q )
   {
      ABCC_ASSERT( pnMsgSentHandler == NULL );
      pnMsgSentHandler = pnHandler;
      link_psNotifyMsg = psWriteMsg;
   }

   return eMsgStatus;
}




ABP_MsgType* ABCC_LinkAlloc( void )
{
   ABP_MsgType* psMsg;

   psMsg = (ABP_MsgType*)ABCC_MemAlloc( link_psMemHandler );
   return psMsg;
}


void ABCC_LinkFree( ABP_MsgType** ppsBuffer )
{
   ABP_MsgType* link_psReadMessage;

   ABCC_ASSERT_ERR( *ppsBuffer != 0, ABCC_SEV_WARNING, ABCC_EC_TRYING_TO_FREE_NULL_POINTER, 0 );
   ABCC_MemFree( link_psMemHandler, (UINT8**)ppsBuffer );

   ABCC_SYS_UseCritical();
   ABCC_SYS_EnterCritical();
   if( fNoMsgReceiverBuffer )
   {
      link_psReadMessage = ABCC_LinkAlloc();
      if( link_psReadMessage )
      {
         fNoMsgReceiverBuffer = FALSE;
         pnABCC_DrvSetMsgReceiverBuffer( link_psReadMessage );
      }
   }
   ABCC_SYS_ExitCritical();
}

ABCC_StatusType ABCC_LinkMapMsgHandler( UINT8 bSrcId, ABCC_MsgHandlerFuncType  pnMSgHandler )
{
   UINT16 iIndex;
   ABCC_StatusType eResult = ABCC_NOK;
   ABCC_SYS_UseCritical();
   
   /*
   ** Find free spot.
   */
   ABCC_SYS_EnterCritical();
   for ( iIndex = 0; iIndex < LINK_MAX_NUM_MSG_HDL ; iIndex++ )
   {
      if (link_pnMsgHandler[ iIndex ] == 0 )
      {
         link_pnMsgHandler[ iIndex ] = pnMSgHandler;
         link_bMsgSrcId[ iIndex ] = bSrcId;
         eResult = ABCC_OK;
         break;
      }
   }
   ABCC_SYS_ExitCritical();
   return eResult;
}

ABCC_MsgHandlerFuncType  ABCC_LinkGetMsgHandler( UINT8 bSrcId )
{
   UINT16 iIndex;
   ABCC_MsgHandlerFuncType pnHandler = NULL;
   ABCC_SYS_UseCritical();

   /*
   ** Find message handler. If not found return NULL.
   */
   ABCC_SYS_EnterCritical();
   for ( iIndex = 0; iIndex < LINK_MAX_NUM_MSG_HDL ; iIndex++ )
   {
      if ( ( link_pnMsgHandler[ iIndex ] != NULL ) && ( link_bMsgSrcId[ iIndex ] == bSrcId ) )
      {
         pnHandler = link_pnMsgHandler[ iIndex ];
         link_pnMsgHandler[ iIndex ] = NULL;
         break;
      }
   }
   ABCC_SYS_ExitCritical();
   return pnHandler;
}

BOOL ABCC_LinkIsSrcIdUsed( UINT8 bSrcId )
{
   BOOL fFound = FALSE;
   UINT16 iIndex;

   for ( iIndex = 0; iIndex < LINK_MAX_NUM_MSG_HDL ; iIndex++ )
   {
      if ( ( link_pnMsgHandler[ iIndex ] != NULL ) && ( link_bMsgSrcId[ iIndex ] == bSrcId ) )
      {
         fFound = TRUE;
         break;
      }
   }
   return fFound;
}
