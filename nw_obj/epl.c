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
** Source file for the Ethernet POWERLINK ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "epl.h"
#include "abp_epl.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const epl_ObjectType epl_sObject = 
{
   "POWERLINK",                     /* Name.                                  */
   1,                               /* Revision.                              */
   1,                               /* Number of instances.                   */
   1                                /* Highest instance number.               */
};

const epl_InstanceType epl_sInstance = 
{
   0x0000,                          /* 1 Vendor ID.                           */   
   0x0000,                          /* 2 Product code.                        */
   0,                               /* 5 Serial number.                       */
   "Abc"                            /* 6 Manufacturer device name.            */
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** EPL_ProcessCommand()
**------------------------------------------------------------------------------
*/
void EPL_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the Ethernet POWERLINK Object and its Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /* 
      ** Ethernet POWERLINK object Command.
      */
      epl_ObjectCommand( psNewMessage );
   }
   else
   {
      /* 
      ** Ethernet POWERLINK instance Command.
      */
      epl_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
   
} /* End of EPL_ProcessCommand() */


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** epl_InstanceCommand()
**------------------------------------------------------------------------------
*/
void epl_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the Ethernet POWERLINK Instance.
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
      case ABP_EPL_IA_VENDOR_ID:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
         */
         *(UINT32*)psNewMessage->abData = lTOlLe( epl_sInstance.lVendorId );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_VENDOR_ID_DS );
         break;

      case ABP_EPL_IA_PRODUCT_CODE:

         /*
         ** Copy the 2nd Instance 1 attribute (Product code) to the message.
         */
         *(UINT32*)psNewMessage->abData = lTOlLe( epl_sInstance.lProductCode );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_PRODUCT_CODE_DS );
         break;

      case ABP_EPL_IA_MAJOR_REV:	      
      case ABP_EPL_IA_MINOR_REV:	
      
         /* 
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

      case ABP_EPL_IA_SERIAL_NUMBER:

         /*
		   ** Copy the 5th Instance 1 attribute (Serial number) to the message.
		   */
         *(UINT32*)psNewMessage->abData = lTOlLe( epl_sInstance.lSerialNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_EPL_IA_SERIAL_NUMBER_DS );
		   break;

      case ABP_EPL_IA_MANF_DEV_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( epl_sInstance.pcDeviceName );

         /* 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */         
         if( iStrLength > EPL_MAXIMUM_MANF_DEV_NAME_LENGTH )
         {
            iStrLength = EPL_MAXIMUM_MANF_DEV_NAME_LENGTH;
         }

         /* 
         ** Copy the 6th Instance 1 attribute (Manufacturer device name) to the message.
         */         
         ABCC_SYS_MemCpy( psNewMessage->abData, epl_sInstance.pcDeviceName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break; 
      }
      case ABP_EPL_IA_MANF_HW_VER:
      case ABP_EPL_IA_MANF_SW_VER:
      case ABP_EPL_IA_DEVICE_TYPE:
      case ABP_EPL_IA_MANF_NAME:

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
} /* End of epl_InstanceCommand() */


/*------------------------------------------------------------------------------
** epl_ObjectCommand()
**------------------------------------------------------------------------------
*/
void epl_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the Ethernet POWERLINK Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( epl_sObject.pcName );

         /* 
         ** Copy the attribute to a message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, epl_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:
      
         /* 
         ** Copy the attribute to a message.
         */
         psNewMessage->abData[ 0 ] = epl_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );                  
         break;

      case ABP_OA_NUM_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( epl_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( epl_sObject.iHighestInstanceNo );
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
} /* End of epl_ObjectCommand() */


/*******************************************************************************
** End of epl.c
********************************************************************************
*/
