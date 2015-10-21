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
** Source file for the Ethernet/IP ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "eip.h"
#include "abp.h"
#include "abp_eip.h"
#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const eip_ObjectType eip_sObject = 
{
   "EtherNet/IP",                               /* Name.                                              */
   2,                                           /* Revision.                                          */
   1,                                           /* Number of instances.                               */
   1                                            /* Highest instance number.                           */
};

const eip_InstanceType eip_sInstance = 
{
   0x0000,                                      /* 1  Vendor ID.                                       */
/* 0x0000,                                   */ /* 2  Device type.                                     */
/* 0x0000,                                   */ /* 3  Product code.                                    */
/* 0,                                        */ /* 4a Revision: Major.                                */
/* 0,                                        */ /* 4b Revision: Minor.                                */
/* 0,                                        */ /* 5  Serial number.                                   */
/* "Abc",                                    */ /* 6  Product name.                                    */
/* { 0x0000 },                               */ /* 7  Producing instance number.                       */
/* { 0x0000 },                               */ /* 8  Consuming instance number.                       */
/* FALSE,                                    */ /* 9  Enable communication settings from net.          */
/* Reserved.                                 */ /* 10 Reserved.                                       */
/* FALSE,                                    */ /* 11 Enable CIP request forwarding.                  */
/* FALSE,                                    */ /* 12 Enable Parameter object.                        */
/* 0x0003,                                   */ /* 13 Input only heartbeat instance number.           */
/* 0x0004,                                   */ /* 14 Listen only heartbeat instance number.          */
/* 0x0005,                                   */ /* 15 Assembly object Configuration instance number.  */
/* FALSE,                                    */ /* 16 Disable Strict IO match.                        */
/* FALSE,                                    */ /* 17 Enable unconnected routing.                     */
/* 0x0000,                                    */ /* 18 Input only extended heartbeat instance number.  */
/* 0x0000,                                    */ /* 19 Listen only extended heartbeat instance number. */
/* "Port 1",                                 */ /* 20 Interface label port 1.                         */
/* "Port 2",                                 */ /* 21 Interface label port 2.                         */
/* "Internal",                               */ /* 22 Interface label internal port.                  */
/* FALSE,                                    */ /* 23 Enable CIP request forwarding extended.         */
/* 0,                                        */ /* 24 Prepend producing profile instance.             */
/* 0,                                        */ /* 25 Prepend consuming profile instance.             */
/* FALSE,                                    */ /* 26 Enable EtherNet/IP QuickConnect.                */
/* { {0,1},{0,1},{0,1},{0,1},{0,1},{0,1} },  */ /* 27 Producing Instance Map.                         */
/* { {0,1},{0,1},{0,1},{0,1},{0,1},{0,1} },  */ /* 28 Consuming Instance Map.                         */
/* FALSE                                     */ /* 29 Ignore Sequence Count Check.                    */
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** EIP_ProcessCommand()
**------------------------------------------------------------------------------
*/
void EIP_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the Ethernet/IP Object and it's Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /* 
      ** Ethernet/IP object Command.
      */
      eip_ObjectCommand( psNewMessage );
   }
   else
   {
      /* 
      ** Ethernet/IP instance Command.
      */
      eip_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
} /* End of EIP_ProcessCommand() */


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** eip_InstanceCommand()
**------------------------------------------------------------------------------
*/
void eip_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the Ethernet/IP Instance.
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
	  case ABP_EIP_IA_VENDOR_ID:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iVendorId );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_VENDOR_ID_DS );
         break;

	/*case ABP_EIP_IA_DEVICE_TYPE:

         **
         ** Copy the 2nd Instance 1 attribute (Device type) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iDeviceType );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_DEVICE_TYPE_DS );
         break;
      */
	 /*case ABP_EIP_IA_PRODUCT_CODE:

         **
         ** Copy the 3rd Instance 1 attribute (Product code) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iProductCode );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_PRODUCT_CODE_DS );
         break;
      */
    /*case ABP_EIP_IA_REVISION:

         ** 
         ** Copy the 4th Instance 1 attribute (Revision) to the message.
         **         
         psNewMessage->abData[ 0 ] = eip_sInstance.bMajorRevision;
         psNewMessage->abData[ 1 ] = eip_sInstance.bMinorRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_REVISION_DS );
         break;
      */
    /*case ABP_EIP_IA_SERIAL_NUMBER:

         **
         ** Copy the 5th Instance 1 attribute (Serial number) to the message.
         **
         *(UINT32*)psNewMessage->abData = lTOlLe( eip_sInstance.lSerialNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_SERIAL_NUMBER_DS );
         break;
      */
    /*case ABP_EIP_IA_PRODUCT_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( eip_sInstance.pcProductName );

         ** 
         ** Copy the 6th Instance 1 attribute (Product name) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, eip_sInstance.pcProductName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      */
    /*case ABP_EIP_IA_PROD_INSTANCE:
      
      {
         UINT16* piMessageData;
         UINT16  iIndex;
         UINT16  iNumberOfElements;

         piMessageData     = (UINT16*)psNewMessage->abData;
         iNumberOfElements = sizeof( eip_sInstance.aiProducingInstanceNumber ) / sizeof( eip_sInstance.aiProducingInstanceNumber[ 0 ] );

         **
         ** Copy the 7th Instance 1 attribute (Producing instance number) to the message.
         **
         for( iIndex = 0; iIndex < iNumberOfElements; ++iIndex )
         {
            piMessageData[ iIndex ] = iTOiLe( eip_sInstance.aiProducingInstanceNumber[ iIndex ] );
         }

         ** 
         ** The value of ABP_EIP_IA_PROD_INSTANCE_DS is counted per element, and 
         ** should therefore be divided by 5 as long as ABP_EIP_IA_PROD_INSTANCE_DS 
         ** is defined to be multiplied by 5 in abp_eip.h as ( 5 * ABP_UINT16_SIZEOF ).
         **
         ABP_SetMsgResponse( psNewMessage, iNumberOfElements * ( ABP_EIP_IA_PROD_INSTANCE_DS / 5 ) );
         break;
      }
      */
    /*case ABP_EIP_IA_CONS_INSTANCE:
      
      {
         UINT16* piMessageData;
         UINT16  iIndex;
         UINT16  iNumberOfElements;

         piMessageData     = (UINT16*)psNewMessage->abData;
         iNumberOfElements = sizeof( eip_sInstance.aiConsumingInstanceNumber ) / sizeof( eip_sInstance.aiConsumingInstanceNumber[ 0 ] );

         **
         ** Copy the 8th Instance 1 attribute (Consuming instance number) to the message.
         **
         for( iIndex = 0; iIndex < iNumberOfElements; ++iIndex )
         {
            piMessageData[ iIndex ] = iTOiLe( eip_sInstance.aiConsumingInstanceNumber[ iIndex ] );
         }

         ** 
         ** The value of ABP_EIP_IA_CONS_INSTANCE_DS is counted per element, and 
         ** should therefore be divided by 5 as long as ABP_EIP_IA_CONS_INSTANCE_DS 
         ** is defined to be multiplied by 5 in abp_eip.h as ( 5 * ABP_UINT16_SIZEOF ).
         **
         ABP_SetMsgResponse( psNewMessage, iNumberOfElements * ( ABP_EIP_IA_CONS_INSTANCE_DS / 5 ) );
         break;
      }
      */
    /*case ABP_EIP_IA_COMM_SETTINGS_FROM_NET:
      
         **
         ** Copy the 9th Instance 1 attribute (Enable communication settings from net) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fCommSettingsFromNet;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_COMM_SETTINGS_FROM_NET_DS );
         break;
      */
    /*case ABP_EIP_IA_ENABLE_APP_CIP_OBJECTS:
      
         **
         ** Copy the 11th Instance 1 attribute (Enable CIP request forwarding) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fEnableCIPRequestForwarding;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_ENABLE_APP_CIP_OBJECTS_DS );
         break;
      */
	/*case ABP_EIP_IA_ENABLE_PARAM_OBJECT:
      
         **
         ** Copy the 12th Instance 1 attribute (Enable Parameter object) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fEnableParameterObject;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_ENABLE_PARAM_OBJECT_DS );
         break;
      */
    /*case ABP_EIP_IA_INPUT_INSTANCE_OBJECT:
      
         **
         ** Copy the 13th Instance 1 attribute (Input only heartbeat instance number) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iInpOnlyHeartBeatInstNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_INPUT_INSTANCE_DS );
         break;
      */
    /*case ABP_EIP_IA_LISTEN_INSTANCE_OBJECT:
      
         **
         ** Copy the 14th Instance 1 attribute (Listen only heartbeat instance number) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iListOnlyHeartBeatInstNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_LISTEN_INSTANCE_DS );
         break;
      */
    /*case ABP_EIP_IA_CONFIG_INSTANCE:
      
         **
         ** Copy the 15th Instance 1 attribute (Assembly object Configuration instance number) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iAssObjectConfInstNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_CONFIG_INSTANCE_DS );
         break;
      */
    /*case ABP_EIP_IA_DISABLE_STRICT_IO_MATCH:
      
         **
         ** Copy the 16th Instance 1 attribute (Disable Strict IO match) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fDisableStrictIOMatch;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_DISABLE_STRICT_IO_MATCH_DS );
         break;
      */
	 /*case ABP_EIP_IA_ENABLE_UNCONNECTED_SEND:
      
         **
         ** Copy the 17th Instande 1 attribute (Enable unconnected routing) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fEnableUnconnectedRouting;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_ENABLE_UNCONNECTED_SEND_DS );
         break;
      */
	 /*case ABP_EIP_IA_INPUT_EXT_INSTANCE_OBJECT:
      
         **
         ** Copy the 18th Instance 1 attribute (Input only extended heartbeat instance number) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iInpOnlyExtHeartBeatInstNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_INPUT_EXT_INSTANCE_DS );
         break;
      */
	 /*case ABP_EIP_IA_LISTEN_EXT_INSTANCE_OBJECT:
      
         **
         ** Copy the 19th Instance 1 attribute (Listen only extended heartbeat instance number) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iListOnlyExtHeartBeatInstNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_LISTEN_EXT_INSTANCE_DS );
         break;
      */
	 /*case ABP_EIP_IA_IF_LABEL_PORT_1:
      
      {
         UINT16 iStrLength;

         ** 
         ** Copy the 20th Instance 1 attribute (Interface label port 1) to the message.
         **         
         iStrLength = (UINT16)strlen( eip_sInstance.pcInterfaceLabelPort1 );
         ABCC_SYS_MemCpy( psNewMessage->abData, eip_sInstance.pcInterfaceLabelPort1, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break; 
      }
      */
	 /*case ABP_EIP_IA_IF_LABEL_PORT_2:
      
      {
         UINT16 iStrLength;

         ** 
         ** Copy the 21st Instance 1 attribute (Interface label port 2) to the message.
         **         
         iStrLength = (UINT16)strlen( eip_sInstance.pcInterfaceLabelPort2 );
         ABCC_SYS_MemCpy( psNewMessage->abData, eip_sInstance.pcInterfaceLabelPort2, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break; 
      }
      */
	 /*case ABP_EIP_IA_IF_LABEL_PORT_INT:
      
      {
         UINT16 iStrLength;

         ** 
         ** Copy the 22nd Instance 1 attribute (Interface label internal port) to the message.
         **         
         iStrLength = (UINT16)strlen( eip_sInstance.pcInterfaceLabelIntPort );
         ABCC_SYS_MemCpy( psNewMessage->abData, eip_sInstance.pcInterfaceLabelIntPort, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break; 
      }
      */
	 /*case ABP_EIP_IA_ENABLE_APP_CIP_OBJECTS_EXT:
      
         **
         ** Copy the 23rd Instance 1 attribute (Enable CIP request forwarding extended) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fEnableCIPRequestForwardingExt;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_ENABLE_APP_CIP_OBJECTS_EXT_DS );
         break;
      */
	 /*case ABP_EIP_IA_PREPEND_PRODUCING:
      
         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 24th Instance 1 attribute (Prepend producing profile instance) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iPrependProducingProfileInstance );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_PREPEND_PRODUCING_DS );
         break;
      */
	 /*case ABP_EIP_IA_PREPEND_CONSUMING:
      
         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 25th Instance 1 attribute (Prepend consuming profile instance) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sInstance.iPrependConsumingProfileInstance );
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_PREPEND_CONSUMING_DS );
         break;
      */
	 /*case ABP_EIP_IA_ENABLE_EIP_QC:
      
         **
         ** Copy the 26th Instance 1 attribute (Enable EtherNet/IP QuickConnect) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fEnableEIPQuickConnect;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_ENABLE_EIP_QC_DS );
         break;
      */
	 /*case ABP_EIP_IA_PROD_INSTANCE_MAP:
      
      {
         UINT16* piMessageData;
         UINT16  iIndex;
         UINT16  iNumberOfElements;

         piMessageData     = (UINT16*)psNewMessage->abData;
         iNumberOfElements = sizeof( eip_sInstance.asProducingInstanceMap ) / sizeof( eip_sInstance.asProducingInstanceMap[ 0 ] );

         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 27th Instance 1 attribute (Producing Instance Map) to the message.
         **
         for( iIndex = 0; iIndex < iNumberOfElements; ++iIndex )
         {
            *piMessageData++ = iTOiLe( eip_sInstance.asProducingInstanceMap[ iIndex ].iInstanceNumber );
            *piMessageData++ = iTOiLe( eip_sInstance.asProducingInstanceMap[ iIndex ].iInstanceSize );
         }

         ** 
         ** The value of ABP_EIP_IA_PROD_INSTANCE_MAP_DS is counted per element, and 
         ** should therefore be divided by 6 as long as ABP_EIP_IA_PROD_INSTANCE_MAP_DS 
         ** is defined to be multiplied by 6 in abp_eip.h as ( 6 * 2 * ABP_UINT16_SIZEOF ).
         **
         ABP_SetMsgResponse( psNewMessage, iNumberOfElements * ( ABP_EIP_IA_PROD_INSTANCE_MAP_DS / 6 ) );
         break;
      }
      */
	 /*case ABP_EIP_IA_CONS_INSTANCE_MAP:
      
      {
         UINT16* piMessageData;
         UINT16  iIndex;
         UINT16  iNumberOfElements;

         piMessageData     = (UINT16*)psNewMessage->abData;
         iNumberOfElements = sizeof( eip_sInstance.asConsumingInstanceMap ) / sizeof( eip_sInstance.asConsumingInstanceMap[ 0 ] );

         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 28th Instance 1 attribute (Consuming Instance Map) to the message.
         **
         for( iIndex = 0; iIndex < iNumberOfElements; ++iIndex )
         {
            *piMessageData++ = iTOiLe( eip_sInstance.asConsumingInstanceMap[ iIndex ].iInstanceNumber );
            *piMessageData++ = iTOiLe( eip_sInstance.asConsumingInstanceMap[ iIndex ].iInstanceSize );
         }

         ** 
         ** The value of ABP_EIP_IA_CONS_INSTANCE_MAP_DS is counted per element, and 
         ** should therefore be divided by 6 as long as ABP_EIP_IA_CONS_INSTANCE_MAP_DS 
         ** is defined to be multiplied by 6 in abp_eip.h as ( 6 * 2 * ABP_UINT16_SIZEOF ).
         **
         ABP_SetMsgResponse( psNewMessage, iNumberOfElements * ( ABP_EIP_IA_CONS_INSTANCE_MAP_DS / 6 ) );
         break;
      }
      */
	 /*case ABP_EIP_IA_IGNORE_SEQ_COUNT_CHECK:
      
         **
         ** Copy the 29th Instance 1 attribute (Ignore Sequence Count Check) to the message.
         **
         psNewMessage->abData[ 0 ] = eip_sInstance.fIgnoreSequenceCountCheck;
         ABP_SetMsgResponse( psNewMessage, ABP_EIP_IA_IGNORE_SEQ_COUNT_CHECK_DS );
         break;
      */

      /* 
      ** Advice: If you want to remove a command from the switch-list, you can 
      ** use the following comment...
      **
      ** In this example, the default values of these attributes are desired,
      ** so we can simply respond with an error when they are requested.
      **
      ** ...and you can use the following error respond message:
      ** ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
      **
      ** For more information have a look at the error code enumeration list 
      ** ABP_MsgErrorCodeType found in abp.h.
      */

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
} /* End of eip_InstanceCommand() */


/*------------------------------------------------------------------------------
** eip_ObjectCommand()
**------------------------------------------------------------------------------
*/
void eip_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the Ethernet/IP Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:
      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         /* 
         ** Copy the attribute to a message.
         */
         iStrLength = (UINT16)strlen( eip_sObject.pcName );
         ABCC_SYS_MemCpy( psNewMessage->abData, eip_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:
      
         /* 
         ** Copy the attribute to a message.
         */
         psNewMessage->abData[ 0 ] = eip_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );                  
         break;

      case ABP_OA_NUM_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( eip_sObject.iHighestInstanceNo );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      default:
      
         /* 
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
         
      } /* End of switch( Attribute number ) */
      break;
   
   default:
   
      /* 
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
      
   } /* End of switch( Command number ) */
} /* End of eip_ObjectCommand() */


/*******************************************************************************
** End of eip.c
********************************************************************************
*/
