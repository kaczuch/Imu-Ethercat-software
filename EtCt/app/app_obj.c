/*
 * app_obj.c
 *
 *  Created on: 11 sie 2015
 *      Author: bkaczor
 */
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "app_obj.h"

UINT8* app_aacLanguages[ 1 ] = { "English" };
extern BOOL8    fResetABCC;

const app_ObjectType app_sObject =
{
   "Application",       /* Name.                            */
   0x01,                /* Revision.                        */
   1,                   /* Number of instances.             */
   1                    /* Highest instance number.         */
};

app_InstanceType app_sInstance =
{
   0,                   /* Configured.                      */
   ABP_LANG_ENG,        /* Language.                        */
   { ABP_LANG_ENG },    /* Supported languages.             */
   1                    /* Number of supported languages.   */
};

BOOL8 APP_IsResetRequestAllowed( UINT8 bResetType )
{
   /*
   ** Todo: PORTING ALERT!
   ** Check if the reset request is allowed.
   */
   if( bResetType == ABP_RESET_POWER_ON )
   {
      return( TRUE );
   }
   return FALSE;

}

void APP_ProcResetRequest( UINT8 bResetType )
{
   /*
   ** Todo: PORTING ALERT!
   ** Perform a local reset depending on the bResetType, and setup the
   ** ABCC driver for reinitialization.
   */
   switch( bResetType )
   {
   case ABP_RESET_FACTORY_DEFAULT:
      break;

   case ABP_RESET_POWER_ON_FACTORY_DEFAULT:
      break;

   case ABP_RESET_POWER_ON:
      fResetABCC = TRUE;
      break;

   default:
      break;
   }
}

void APP_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /*
      ** Process the Application data Object Command.
      */
      app_ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Process the Application data Instance Command.
      */
      app_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );

}

void app_InstanceCommand( ABP_MsgType* psNewMessage )
{
   UINT16 iStrLength;


   if( psNewMessage->sHeader.iInstance != 1 )
   {
      /*
      ** The requested instance does not exist.
      ** Respond with a error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

      return;
   }

   switch ( psNewMessage->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:
      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_APP_IA_CONFIGURED:

         /*
         ** The 'configured' attribute is requested.
         ** Copy the attribute to a response message.
         */
         psNewMessage->abData[ 0 ] = app_sInstance.fConfigured;
         ABP_SetMsgResponse( psNewMessage, ABP_BOOL_SIZEOF );
         break;

      case ABP_APP_IA_SUP_LANG:

         /*
         ** The 'supported languages' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData,
                     app_sInstance.bSupportedLanguages,
                     app_sInstance.bNumberOfSupportedLanguages );
         ABP_SetMsgResponse( psNewMessage, app_sInstance.bNumberOfSupportedLanguages );
         break;

      default:
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;

   case ABP_CMD_GET_ENUM_STR:
      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_APP_IA_SUP_LANG:

         if( psNewMessage->sHeader.bCmdExt1 <= ABP_LANG_ENG )
         {
            /*
            ** Copy the ENUM STR to a message.
            */
            iStrLength = (UINT16)strlen( app_aacLanguages[ psNewMessage->sHeader.bCmdExt1 ] );
            ABCC_SYS_MemCpy( psNewMessage->abData,
                        app_aacLanguages[ psNewMessage->sHeader.bCmdExt1 ],
                        iStrLength );
            ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         }
         else
         {
            /*
            ** The ENUM value was out of range.
            */
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_1 );
         }
         break;

      default:

         /*
         ** The attribute does not exist, or the attribute is not an ENUM.
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

   }/* End switch( command number ) */

} /* End of app_InstanceCommand() */


/*------------------------------------------------------------------------------
** app_ObjectCommand()
**------------------------------------------------------------------------------
*/
void app_ObjectCommand( ABP_MsgType* psNewMessage )
{
   UINT16 iStrLength;


   switch ( psNewMessage->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:
      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:

         /*
         ** The 'name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         iStrLength = (UINT16)strlen( app_sObject.acName );
         ABCC_SYS_MemCpy( psNewMessage->abData,
                     app_sObject.acName,
                     iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;

      case ABP_OA_REV:

         /*
         ** The 'revision' attribute is requested.
         ** Copy the attribute to a response message.
         */
         psNewMessage->abData[ 0 ] = app_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** The 'Number of Instances' attribute is requested.
         ** Copy the attribute to a response message.
         */
         *(UINT16*)psNewMessage->abData = iLeTOi( app_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** The 'Highest Instance Number' attribute is requested.
         ** Copy the attribute to a response message.
         */
         *(UINT16*)psNewMessage->abData = iLeTOi( app_sObject.iHighestInstanceNo );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;

   case ABP_APP_CMD_RESET_REQUEST:

      /*
      ** Request a command reset.
      */
      if( !APP_IsResetRequestAllowed( psNewMessage->sHeader.bCmdExt1 ) )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_STATE );
      }
      else
      {
         ABP_SetMsgResponse( psNewMessage, 0 );
      }

      break;

   case ABP_CMD_RESET:

      /*
      ** Perform a reset.
      */
      APP_ProcResetRequest( psNewMessage->sHeader.bCmdExt1 );
      break;

   default:

      /*
      ** Unsupported command.
      ** Respond with an error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   }/* End switch( command number ) */

} /* End of app_ObjectCommand() */


