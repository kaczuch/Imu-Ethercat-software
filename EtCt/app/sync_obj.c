/*
 * sync_obj.c
 *
 *  Created on: 29 wrz 2015
 *      Author: bkaczor
 */
#include "sync_obj.h"
#include "abp_sync.h"

const sync_ObjectType sync_sObject=
{
	"Sync",
	1,
	1,
	1

};



sync_InstanceType sync_sInstance=
{
	1000,
	100,
	100,
	100,
	111,
	1000,
	1,
	3

};
/**
 * SYNC_processCmdMsg function processing messages to sync object
 * @param psNewMessage pointer to received message
 */
void SYNC_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
	   /*
	   ** This function processes commands to the sync Object and its Instance.
	   */
	   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
	   {
	      /*
	      ** sync object Command.
	      */
	      SYNC_ObjectCommand( psNewMessage );
	   }
	   else
	   {
	      /*
	      ** sync instance Command.
	      */
	      SYNC_InstanceCommand( psNewMessage );
	   }

	   ABCC_SendRespMsg( psNewMessage );
}

void SYNC_InstanceCommand(ABP_MsgType* psNewMessage )
{
	if(psNewMessage->sHeader.iInstance!=1)
	{
	      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

	      return;
	}
	switch (psNewMessage->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS)
	{
	case ABP_CMD_GET_ATTR:
		switch(psNewMessage->sHeader.bCmdExt0)
		{
		case ABP_SYNC_IA_CYCLE_TIME:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.cyclTime );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_CYCLE_TIME_DS );
			break;

		case ABP_SYNC_IA_OUTPUT_VALID:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.outValid );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_OUTPUT_VALID_DS );
			break;

		case ABP_SYNC_IA_INPUT_CAPTURE:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.inCapture );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_INPUT_CAPTURE_DS );
			break;

		case ABP_SYNC_IA_OUTPUT_PROCESSING:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.outProces );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_OUTPUT_PROCESSING_DS );
			break;

		case ABP_SYNC_IA_INPUT_PROCESSING:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.inProces );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_INPUT_PROCESSING_DS );
			break;

		case ABP_SYNC_IA_MIN_CYCLE_TIME:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.minCyclTime );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_MIN_CYCLE_TIME_DS );
			break;

		case ABP_SYNC_IA_SYNC_MODE:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.syncMode );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_SYNC_MODE_DS );
			break;

		case ABP_SYNC_IA_SUPPORTED_SYNC_MODES:
			*(UINT32*)psNewMessage->abData = lTOlLe( sync_sInstance.supSyncMode );
			ABP_SetMsgResponse( psNewMessage, ABP_SYNC_IA_SUPPORTED_SYNC_MODES_DS );
			break;
		default:
			 ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD);
			break;
		}
		break;
	case ABP_CMD_SET_ATTR:
		switch(psNewMessage->sHeader.bCmdExt0)
		{
		case ABP_SYNC_IA_CYCLE_TIME:

			ABCC_SYS_MemCpy( sync_sInstance.cyclTime, psNewMessage->abData,ABP_SYNC_IA_CYCLE_TIME_DS );
			ABP_SetMsgResponse( psNewMessage, 0 );
			break;

		case ABP_SYNC_IA_OUTPUT_VALID:
			ABCC_SYS_MemCpy( sync_sInstance.outValid, psNewMessage->abData,ABP_SYNC_IA_OUTPUT_VALID_DS );
			ABP_SetMsgResponse( psNewMessage, 0 );
			break;

		case ABP_SYNC_IA_INPUT_CAPTURE:
			ABCC_SYS_MemCpy( sync_sInstance.inCapture, psNewMessage->abData,ABP_SYNC_IA_INPUT_CAPTURE_DS );
			ABP_SetMsgResponse( psNewMessage, 0 );
			break;

		case ABP_SYNC_IA_OUTPUT_PROCESSING:
			ABCC_SYS_MemCpy( sync_sInstance.outProces, psNewMessage->abData,ABP_SYNC_IA_OUTPUT_PROCESSING_DS );
			ABP_SetMsgResponse( psNewMessage, 0 );
			break;

		case ABP_SYNC_IA_INPUT_PROCESSING:
			ABCC_SYS_MemCpy( sync_sInstance.inProces, psNewMessage->abData,ABP_SYNC_IA_INPUT_PROCESSING_DS );
			ABP_SetMsgResponse( psNewMessage, 0 );
			break;

		case ABP_SYNC_IA_MIN_CYCLE_TIME:
			ABCC_SYS_MemCpy( sync_sInstance.minCyclTime, psNewMessage->abData,ABP_SYNC_IA_MIN_CYCLE_TIME_DS );
			ABP_SetMsgResponse( psNewMessage, 0 );
			break;

		case ABP_SYNC_IA_SYNC_MODE:
			ABCC_SYS_MemCpy( sync_sInstance.syncMode, psNewMessage->abData,ABP_SYNC_IA_SYNC_MODE_DS );
			ABP_SetMsgResponse( psNewMessage, 0 );
			break;

		case ABP_SYNC_IA_SUPPORTED_SYNC_MODES:
			ABCC_SYS_MemCpy( sync_sInstance.supSyncMode, psNewMessage->abData,ABP_SYNC_IA_SUPPORTED_SYNC_MODES_DS);
			ABP_SetMsgResponse( psNewMessage, 0 );

			break;
			default:
				 ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD);
				break;

		}
		break;
		default:
			 ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD);
			break;
	}
}

void SYNC_ObjectCommand(ABP_MsgType* psNewMessage )
{
	switch(( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
	{
	case ABP_CMD_GET_ATTR:
		switch( psNewMessage->sHeader.bCmdExt0 )
	    {
		case ABP_OA_NAME:
		{
	         UINT16 iStrLength;

	         iStrLength = (UINT16)strlen( sync_sObject.acName );

	         /*
	         ** Copy the attribute to a message.
	         */
	         ABCC_SYS_MemCpy( psNewMessage->abData, sync_sObject.acName, iStrLength );
	         ABP_SetMsgResponse( psNewMessage, iStrLength );
	         break;
		}
		case ABP_OA_REV:

	         /*
	         ** Copy the attribute to a message.
	         */
	         psNewMessage->abData[ 0 ] = sync_sObject.bRevision;
	         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
	         break;
		case ABP_OA_NUM_INST:

	         /*
	         ** Copy the attribute to a message.
	         */
	         *(UINT16*)psNewMessage->abData = iTOiLe( sync_sObject.iNumberOfInstances );
	         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
	         break;
		case ABP_OA_HIGHEST_INST:

	         /*
	         ** Copy the attribute to a message.
	         */
	         *(UINT16*)psNewMessage->abData = iTOiLe( sync_sObject.iHighestInstanceNo );
	         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
	         break;
		default:

	         /*
	         ** Unsupported attribute.
	         */
	         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
	         break;
	    }
	}
}
