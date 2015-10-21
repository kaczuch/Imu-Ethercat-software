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
	0,
	0,
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

		}
		break;
	case ABP_CMD_SET_ATTR:
		break;
	}
}

void SYNC_ObjectCommand(ABP_MsgType* psNewMessage )
{

}
