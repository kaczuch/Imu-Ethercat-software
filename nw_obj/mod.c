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
** Source file for the Modbus ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "mod.h"
#include "abp_mod.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const mod_ObjectType mod_sObject =
{
   "Modbus",                  /* Name.                                        */
   1,                         /* Revision.                                    */
   1,                         /* Number of instances.                         */
   1                          /* Highest instance number.                     */
};


const mod_InstanceType mod_sInstance =
{
   "Abc",                     /* 1  Vendor Name.                              */
 /*"Abc",                   *//* 2  Product Code.                             */
 /*"X.yy",                  *//* 3  Major Minor Revision.                     */
 /*"Abc",                   *//* 4  Vendor URL.                               */
 /*"Abc",                   *//* 5  Product Name.                             */
 /*"Abc",                   *//* 6  Model Name.                               */
 /*"Abc",                   *//* 7  User Application Name.                    */
 /*"Abc",                   *//* 8  Device ID.                                */
 /*0x00,                    *//* 9  No. of ADI indexing bits.                 */
 /*FALSE,                   *//* 10 Enable Modbus Message forwarding.         */
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** MOD_ProcessCommand()
**------------------------------------------------------------------------------
*/
void MOD_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Modbus Object and its Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /*
      ** Modbus object Command.
      */
      mod_ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Modbus instance Command.
      */
      mod_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void mod_InstanceCommand()
**------------------------------------------------------------------------------
*/
void mod_InstanceCommand( ABP_MsgType* psNewMessage )
{
	UINT16 iStrLength;

   
   /*
   ** This function processes commands to the Modbus Instance.
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
      case ABP_MOD_IA_VENDOR_NAME:

         iStrLength = (UINT16)strlen( mod_sInstance.pcVendorName );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcVendorName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;

    /*case ABP_MOD_IA_PRODUCT_CODE:

         iStrLength = (UINT16)strlen( mod_sInstance.pcProductCode );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcProductCode, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;*/

    /*case ABP_MOD_IA_REVISION:

         iStrLength = (UINT16)strlen( mod_sInstance.pcMajorMinorRevision );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcMajorMinorRevision, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;*/

    /*case ABP_MOD_IA_VENDOR_URL:

         iStrLength = (UINT16)strlen( mod_sInstance.pcVendorURL );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcVendorURL, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;*/

    /*case ABP_MOD_IA_PRODUCT_NAME:

         iStrLength = (UINT16)strlen( mod_sInstance.pcProductName );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcProductName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;*/

    /*case ABP_MOD_IA_MODEL_NAME:

         iStrLength = (UINT16)strlen( mod_sInstance.pcModelName );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcModelName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;*/

	/*case ABP_MOD_IA_USER_APP_NAME:

         iStrLength = (UINT16)strlen( mod_sInstance.pcUserAppName );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcUserAppName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;*/

	/*case ABP_MOD_IA_DEVICE_ID:

         iStrLength = (UINT16)strlen( mod_sInstance.pcDeviceID );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sInstance.pcDeviceID, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;*/

	/*case ABP_MOD_IA_ADI_INDEXING_BITS:

         ** 
         ** Copy the attribute to a message.
         **         
         *(UINT16*)&psNewMessage->abData[ 0 ] = mod_sInstance.bNoADIIndexBits;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;*/

	/*case ABP_MOD_IA_MESSAGE_FORWARDING:

         ** 
         ** Copy the attribute to a message.
         **        
         psNewMessage->abData[ 0 ] = mod_sInstance.fEnModMessForward;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;*/

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
** mod_ObjectCommand()
**------------------------------------------------------------------------------
*/
void mod_ObjectCommand( ABP_MsgType* psNewMessage )
{
   UINT16 iStrLength;


   /*
   ** This function processes commands to the Modbus Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:

         iStrLength = (UINT16)strlen( mod_sObject.pcName );
         ABCC_SYS_MemCpy( psNewMessage->abData, mod_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;

      case ABP_OA_REV:

         psNewMessage->abData[ 0 ] = mod_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( mod_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( mod_sObject.iHighestInstanceNo );
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
** End of mod.c
*****************************************************************************
*/
