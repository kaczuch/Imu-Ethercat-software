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
** Source file for the ControlNet ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "cnt.h"
#include "abp_cnt.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const cnt_ObjectType cnt_sObject = 
{
   "ControlNet",                    /* Name.                                  */
   1,                               /* Revision.                              */
   1,                               /* Number of instances.                   */
   1                                /* Highest instance number.               */
};

const cnt_InstanceType cnt_sInstance = 
{
#ifdef ENABLE_CNT_IA_VENDOR_ID
   0x0000,                          /* 1 Vendor ID.                           */
#endif
#ifdef ENABLE_CNT_IA_DEVICE_TYPE
   0x0000,                          /* 2 Device type.                         */
#endif
#ifdef ENABLE_CNT_IA_PRODUCT_CODE
   0x0000,                          /* 3 Product code.                        */
#endif
#ifdef ENABLE_CNT_IA_REVISION
   0,                               /* 4a Major revision.                     */
   0,                               /* 4b Minor revision.                     */
#endif
#ifdef ENABLE_CNT_IA_SERIAL_NUMBER
   0,                               /* 5 Serial number.                       */
#endif
#ifdef ENABLE_CNT_IA_PRODUCT_NAME
   "Abc",                           /* 6 Product name.                        */
#endif
#ifdef ENABLE_CNT_IA_PROD_INSTANCE
   0x0000,                          /* 7 Producing instance number.           */
#endif
#ifdef ENABLE_CNT_IA_CONS_INSTANCE
   0x0000,                          /* 8 Consuming instance number.           */
#endif
#ifdef ENABLE_CNT_IA_ENABLE_APP_CIP_OBJECTS
   FALSE,                           /* 11 Enable CIP request forwarding.      */
#endif
#ifdef ENABLE_CNT_IA_ENABLE_PARAM_OBJECT
   FALSE,                           /* 12 Enable parameter object.            */
#endif
#ifdef ENABLE_CNT_IA_CONFIG_INSTANCE
   0x0000                           /* 15 Assembly obj Cfg instance number    */
#endif
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** DEV_ProcessCommand()
**------------------------------------------------------------------------------
*/
void CNT_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the Ethernet/IP Object and its Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /* 
      ** The Devicenet object Command.
      */
      cnt_ObjectCommand( psNewMessage );
   }
   else
   {
      /* 
      ** The Devicenet instance Command.
      */
      cnt_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
   
} /* End of CNT_ProcessCommand() */


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** cnt_InstanceCommand()
**------------------------------------------------------------------------------
*/
void cnt_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the ControlNet Instance.
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
#ifdef ENABLE_CNT_IA_VENDOR_ID
      case ABP_CNT_IA_VENDOR_ID:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sInstance.iVendorId );
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_VENDOR_ID_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_DEVICE_TYPE
      case ABP_CNT_IA_DEVICE_TYPE:

         /*
         ** Copy the 2nd Instance 1 attribute (Device type) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sInstance.iDeviceType );
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_DEVICE_TYPE_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_PRODUCT_CODE
      case ABP_CNT_IA_PRODUCT_CODE:

         /*
         ** Copy the 3rd Instance 1 attribute (Product code) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sInstance.iProductCode );
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_PRODUCT_CODE_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_REVISION
      case ABP_CNT_IA_REVISION:
      
         /* 
         ** Copy the 4th Instance 1 attribute (Revision) to the message.
         */
         psNewMessage->abData[ 0 ] = cnt_sInstance.bMajorRevision;
         psNewMessage->abData[ 1 ] = cnt_sInstance.bMinorRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_REVISION_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_SERIAL_NUMBER
      case ABP_CNT_IA_SERIAL_NUMBER:

         /*
         ** Copy the 5th Instance 1 attribute (Serial number) to the message.
         */
         *(UINT32*)psNewMessage->abData = lTOlLe( cnt_sInstance.lSerialNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_SERIAL_NUMBER_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_PRODUCT_NAME
      case ABP_CNT_IA_PRODUCT_NAME:
      {
         UINT16 iStrLength;
      
         iStrLength = (UINT16)strlen( cnt_sInstance.pcProductName );

         /*
         ** Copy the 6th Instance 1 attribute (Product name) to the message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, cnt_sInstance.pcProductName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#ifdef ENABLE_CNT_IA_PROD_INSTANCE
      case ABP_CNT_IA_PROD_INSTANCE:
         
         /*
         ** Copy the 7th Instance 1 attribute (Producing instance number) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sInstance.iProducingInstanceNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_PROD_INSTANCE_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_CONS_INSTANCE
      case ABP_CNT_IA_CONS_INSTANCE:
                  
         /*
         ** Copy the 8th Instance 1 attribute (Consuming instance number) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sInstance.iConsumingInstanceNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_CONS_INSTANCE_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_ENABLE_APP_CIP_OBJECTS
      case ABP_CNT_IA_ENABLE_APP_CIP_OBJECTS:
               
         /* 
         ** Copy the 11th Instance 1 attribute (Enable CIP request forwarding) to the message.
         */
         psNewMessage->abData[ 0 ] = cnt_sInstance.fEnableCipRequestForwarding;
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_ENABLE_APP_CIP_OBJECTS_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_ENABLE_PARAM_OBJECT
      case ABP_CNT_IA_ENABLE_PARAM_OBJECT:
                        
         /* 
         ** Copy the 12th Instance 1 attribute (Enable Parameter object) to the message.
         */
         psNewMessage->abData[ 0 ] = cnt_sInstance.fEnableParameterObject;
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_ENABLE_PARAM_OBJECT_DS );
         break;
#endif
#ifdef ENABLE_CNT_IA_CONFIG_INSTANCE
      case ABP_CNT_IA_CONFIG_INSTANCE:
                           
         /*
         ** Copy the 15th Instance 1 attribute (Assembly object Configuration instance number) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sInstance.iAssemblyObjCfgInstanceNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_CNT_IA_CONFIG_INSTANCE_DS );
         break;
#endif

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
} /* End of cnt_InstanceCommand() */


/*------------------------------------------------------------------------------
** cnt_ObjectCommand()
**------------------------------------------------------------------------------
*/
void cnt_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the ControlNet Object (Instance 0).
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
         iStrLength = (UINT16)strlen( cnt_sObject.pcName );
         ABCC_SYS_MemCpy( psNewMessage->abData, cnt_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:
      
         /* 
         ** Copy the attribute to a message.
         */
         psNewMessage->abData[ 0 ] = cnt_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );                  
         break;

      case ABP_OA_NUM_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cnt_sObject.iHighestInstanceNo );
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
   
#ifdef ENABLE_CNT_IA_ENABLE_APP_CIP_OBJECTS
   case ABP_CNT_CMD_PROCESS_CIP_OBJ_REQUEST:
#error PORTING ALERT!
      /* 
      ** PORTING ALERT!
      ** Not implemented
      */
#endif
   case ABP_CNT_CMD_SET_CONFIG_DATA:
   case ABP_CNT_CMD_GET_CONFIG_DATA:
   default:
   
      /* 
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
      
   } /* End of switch( Command number ) */
} /* End of cnt_ObjectCommand() */


/*******************************************************************************
** End of cnt.c
********************************************************************************
*/
