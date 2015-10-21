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
** Source file for the SERCOS III Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "src3.h"
#include "abp_src3.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const src3_ObjectType src3_sObject = 
{
   "SERCOS III",                       /* Name.                               */
   1,                                  /* Revision.                           */
   1,                                  /* Number of instances.                */
   1                                   /* Highest instance number.            */
};

const src3_InstanceType src3_sInstance = 
{
   "Abc",                              /* 1 Component name.                         */   
/* 0x0000,                          */ /* 2 Vendor code.                            */
/* "Abc",                           */ /* 3 Device name.                            */
/* "Abc",                           */ /* 4 Vendor device ID.                       */
/* "",                              */ /* 5 Software revision.                      */
/* "",                              */ /* 6 Serial number.                          */
/* FALSE                            */ /* 7 Major diagnostic events are latching.   */
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** SRC3_ProcessCommand()
**------------------------------------------------------------------------------
*/
void SRC3_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the SERCOS III Object and its Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /* 
      ** SERCOS III object Command.
      */
      src3_ObjectCommand( psNewMessage );
   }
   else
   {
      /* 
      ** SERCOS III instance Command.
      */
      src3_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
   
} /* End of SRC3_ProcessCommand() */


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** src3_InstanceCommand()
**------------------------------------------------------------------------------
*/
void src3_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the SERCOS III Instance.
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
      case ABP_SRC3_COMPONENT_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( src3_sInstance.pcComponentName );

         /* 
         ** Copy the 1st Instance 1 attribute (Component name) to the message.
         */         
         ABCC_SYS_MemCpy( psNewMessage->abData, src3_sInstance.pcComponentName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;	  
      }
    /*case ABP_SRC3_IA_VENDOR_CODE:

         **
         ** Copy the 2nd Instance 1 attribute (Vendor code) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( src3_sInstance.iVendorCode );
		   ABP_SetMsgResponse( psNewMessage, ABP_SRC3_IA_VENDOR_CODE_DS );
		   break;*/

    /*case ABP_SRC3_IA_DEVICE_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( src3_sInstance.pcDeviceName );

         ** 
         ** Copy the 3rd Instance 1 attribute (Device name) to the message.
         **
         ABCC_SYS_MemCpy( psNewMessage->abData, src3_sInstance.pcDeviceName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }*/
    /*case ABP_SRC3_IA_VENDOR_ID:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( src3_sInstance.pcVendorDeviceID );

         ** 
         ** Copy the 4th Instance 1 attribute (Vendor ID) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, src3_sInstance.pcVendorDeviceID, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }*/
    /*case ABP_SRC3_IA_SW_VERSION:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( src3_sInstance.pcSWRevision );

         ** 
         ** Copy the 5th Instance 1 attribute (Software version) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, src3_sInstance.pcSWRevision, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }*/
    /*case ABP_SRC3_IA_SERIAL_NUMBER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( src3_sInstance.pcSWRevision );

         ** 
         ** Copy the 6th Instance 1 attribute (Serial number) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, src3_sInstance.pcSWRevision, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }*/
    /*case ABP_SRC3_IA_MAJOR_DIAG_EV_LATCHING:
         **
         ** Copy the 7th Instance 1 attribute (Major diagnostic events are latching) to the message.
         **
         psNewMessage->abData[ 0 ] = src3_sInstance.fMajorDiagEventLatching;
         ABP_SetMsgResponse( psNewMessage, ABP_BOOL_SIZEOF );
         break;		   
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
} /* End of src3_InstanceCommand() */


/*------------------------------------------------------------------------------
** src3_ObjectCommand()
**------------------------------------------------------------------------------
*/
void src3_ObjectCommand( ABP_MsgType* psNewMessage )
{
   UINT16 iStrLength;
   
   
   /* 
   ** This function processes commands to the SERCOS III Object (Instance 0) 
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:
      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      
         /* 
         ** Copy attribute to message 
         */
         iStrLength = (UINT16)strlen( src3_sObject.pcName );
         ABCC_SYS_MemCpy( psNewMessage->abData, src3_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;

      case ABP_OA_REV:
      
         /* 
         ** Copy attribute to message 
         */
         psNewMessage->abData[ 0 ] = src3_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );                  
         break;

      case ABP_OA_NUM_INST:
      
         /* 
         ** Copy attribute to message 
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( src3_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:
      
         /* 
         ** Copy attribute to message 
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( src3_sObject.iHighestInstanceNo );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      default:
      
         /* 
         ** Error: Unsupported attribute 
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
         
      } /* End of switch( Attribute number ) */
      break;
   
   default:
   
      /* 
      ** Unsupported command 
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
} /* End of src3_ObjectCommand() */


/*******************************************************************************
** End of src3.c
********************************************************************************
*/
