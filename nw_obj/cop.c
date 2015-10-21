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
** Source file for the CANopen ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "cop.h"
#include "abp_cop.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const cop_ObjectType cop_sObject = 
{
   "CANopen",                       /* Vendor ID.                             */
   1,                               /* Revision.							         */
   1,                               /* Number of instances.                   */
   1                                /* Highest instance number.               */
};

const cop_InstanceType cop_sInstance = 
{
#ifdef ENABLE_COP_IA_VENDOR_ID
   0x0000,                          /* 1 Vendor ID.                           */   
#endif
#ifdef ENABLE_COP_IA_PRODUCT_CODE
   0x0000,                          /* 2 Product code.                        */
#endif
#ifdef ENABLE_COP_IA_REVISON
   0,                               /* 3 Major revision.                      */
   0,                               /* 4 Minor revision.                      */
#endif
#ifdef ENABLE_COP_IA_SERIAL_NUMBER
   0,                               /* 5 Serial number.                       */
#endif
#ifdef ENABLE_COP_IA_MANF_DEV_NAME
   "Abc",                           /* 6 Manufacturer Device Name.            */
#endif
#ifdef ENABLE_COP_IA_MANF_HW_VER
   "X.yy",                          /* 7 Manufacturer Hardware Version.       */
#endif
#ifdef ENABLE_COP_IA_MANF_SW_VER
   "X.yy"                           /* 8 Manufacturer Software Version.       */  
#endif
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** COP_ProcessCommand()
**------------------------------------------------------------------------------
*/
void COP_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the CANopen Object and it's Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /* 
      ** The CANopen object Command.
      */
      cop_ObjectCommand( psNewMessage );
   }
   else
   {
      /* 
      ** The Devicenet instance Command.
      */
      cop_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
   
} /* End of DEV_ProcessCommand() */


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** cop_InstanceCommand()
**------------------------------------------------------------------------------
*/
void cop_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the DeviceNet Instance.
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
#ifdef ENABLE_COP_IA_VENDOR_ID
      case ABP_COP_IA_VENDOR_ID:
      
         /*
         ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
         */
         *(UINT32*)psNewMessage->abData = lTOlLe( cop_sInstance.iVendorId );
         ABP_SetMsgResponse( psNewMessage, ABP_COP_IA_VENDOR_ID_DS );
         break;
#endif
#ifdef ENABLE_COP_IA_PRODUCT_CODE
      case ABP_COP_IA_PRODUCT_CODE:
      
         /*
         ** Copy the 2nd Instance 1 attribute (Product code) to the message.
         */
         *(UINT32*)psNewMessage->abData = lTOlLe( cop_sInstance.iProductCode );
         ABP_SetMsgResponse( psNewMessage, ABP_COP_IA_PRODUCT_CODE_DS );
         break;
#endif
#ifdef ENABLE_COP_IA_REVISON
      case ABP_COP_IA_MAJOR_REV:
      
         /*
         ** Copy the 3rd Instance 1 attribute (Major revision) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cop_sInstance.bMajorRevision );
         ABP_SetMsgResponse( psNewMessage, ABP_COP_IA_MAJOR_REV_DS );
         break;

      case ABP_COP_IA_MINOR_REV:
      
         /* 
         ** Copy the 4th Instance 1 attribute (Minor revision) to the message.
         */         
         *(UINT16*)psNewMessage->abData = iTOiLe( cop_sInstance.bMinorRevision );
         ABP_SetMsgResponse( psNewMessage, ABP_COP_IA_MINOR_REV_DS );
         break;
#endif
#ifdef ENABLE_COP_IA_SERIAL_NUMBER
      case ABP_COP_IA_SERIAL_NUMBER:
      
         /*
         ** Copy the 5th Instance 1 attribute (Serial number) to the message.
         */
         *(UINT32*)psNewMessage->abData = lTOlLe( cop_sInstance.lSerialNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_COP_IA_SERIAL_NUMBER_DS );
         break;
#endif
#ifdef ENABLE_COP_IA_MANF_DEV_NAME
      case ABP_COP_IA_MANF_DEV_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( cop_sInstance.pcManDeviceName );

         /* 
         ** Copy the 6th Instance 1 attribute (Manufacture device name) to the message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, cop_sInstance.pcManDeviceName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#ifdef ENABLE_COP_IA_MANF_HW_VER
      case ABP_COP_IA_MANF_HW_VER:

      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( cop_sInstance.pcManHWVersion );

         /* 
         ** Copy the 7th Instance 1 attribute (Manufacture hardware version) to the message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, cop_sInstance.pcManHWVersion, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#ifdef ENABLE_COP_IA_MANF_SW_VER
      case ABP_COP_IA_MANF_SW_VER:

      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( cop_sInstance.pcManSWVersion );

         /* 
         ** Copy the 8th Instance 1 attribute (Manufacture software version) to the message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, cop_sInstance.pcManSWVersion, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
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
} /* End of cop_InstanceCommand() */


/*------------------------------------------------------------------------------
** cop_ObjectCommand()
**------------------------------------------------------------------------------
*/
void cop_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the CANopen Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( cop_sObject.pcName );

         /* 
         ** Copy the attribute to a message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, cop_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:
      
         /* 
         ** Copy the attribute to a message.
         */
         psNewMessage->abData[ 0 ] = cop_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );                  
         break;

      case ABP_OA_NUM_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cop_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( cop_sObject.iHighestInstanceNo );
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
} /* End of cop_ObjectCommand() */


/*******************************************************************************
** End of cop.c
********************************************************************************
*/
