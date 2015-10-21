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
** Source file for the Functional Safety Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "fus.h"
#include "abp_fusm.h"
#include "abp.h"
#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const fus_ObjectType fus_sObject =
{
   "Functional Safety",                   /* Name.                            */
   1,                                     /* Revision.                        */
   1,                                     /* Number of instances.             */
   1                                      /* Highest instance number.         */
};


fus_InstanceType fus_sInstance =
{
   FALSE,                                /* 1 Safety Enabled                  */
};



/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** FUS_ProcessCommand()
**------------------------------------------------------------------------------
*/
void FUS_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Functional Safety Object and it's Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /*
      ** Functional Safety Object Command.
      */
      fus_ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Functional Safety instance Command.
      */
      fus_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void fus_InstanceCommand()
**------------------------------------------------------------------------------
*/

void fus_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Profibus Instance.
   */
   if( psNewMessage->sHeader.iInstance != 1 )
   {
      /*
      ** The Instance does not exist.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );
      return;
   }

   switch ( psNewMessage->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_SAFE_IA_SAFETY_ENABLE:

         /*
         ** Copy the 1st Instance 1 attribute (Ident number) to the message.
         */
         psNewMessage->abData[0] = fus_sInstance.bSafetyEnabled;
         ABP_SetMsgResponse( psNewMessage, ABP_SAFE_IA_SAFETY_ENABLE_DS );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   }
}


/*------------------------------------------------------------------------------
** fus_ObjectCommand()
**------------------------------------------------------------------------------
*/
void fus_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Functional Safety Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( fus_sObject.pcName );
         ABCC_SYS_MemCpy( psNewMessage->abData, fus_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
      case ABP_OA_REV:

         psNewMessage->abData[ 0 ] = fus_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( fus_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( fus_sObject.iHighestInstanceNo );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      default:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;

   default:

      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   }
}


/****************************************************************************
** End of fus.c
*****************************************************************************
*/
