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
** Source file for the EtherCat ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "ect.h"
#include "abp_ect.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const ect_ObjectType ect_sObject = 
{
   "EtherCAT",                      /* Name.                                  */
   1,                               /* Revision.                              */
   1,                               /* Number of instances.                   */
   1                                /* Highest instance number.               */
};

const ect_InstanceType ect_sInstance = 
{
   0x0000,                          /* 1 Vendor ID.                           */   
/* 0x0000,                       */ /* 2 Product code.                        */
/* 0,                            */ /* 3a Major revision.                     */
/* 0,                            */ /* 3b Minor revision.                     */
/* 0,                            */ /* 5 Serial number.                       */
/* "Abc",                        */ /* 6 Manufacturer device name.            */
/* "x.yy"                        */ /* 7 Manufacturer hardware version.       */
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ECT_ProcessCommand()
**------------------------------------------------------------------------------
*/
void ECT_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the EtherCat Object and its Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /* 
      ** EtherCat object Command.
      */
      ect_ObjectCommand( psNewMessage );
   }
   else
   {
      /* 
      ** EtherCat instance Command.
      */
      ect_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
   
} /* End of ECT_ProcessCommand() */


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ect_InstanceCommand()
**------------------------------------------------------------------------------
*/
void ect_InstanceCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the EtherCat Instance.
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
      case ABP_ECT_IA_VENDOR_ID:

         /*
		   ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
		   */
		   *(UINT32*)psNewMessage->abData = lTOlLe( ect_sInstance.lVendorId );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_VENDOR_ID_DS );
         break;

    /*case ABP_ECT_IA_PRODUCT_CODE:

         **
         ** Copy the 2nd Instance 1 attribute (Product code) to the message.
		   **
		   *(UINT32*)psNewMessage->abData = lTOlLe( ect_sInstance.lProductCode );
		   ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_PRODUCT_CODE_DS );
         break;*/

    /*case ABP_ECT_IA_MAJOR_REV:	      
		 
         ** 
		   ** Copy the 3rd Instance 1 attribute (Major revision) to the message.
		   **         
		   *(UINT16*)psNewMessage->abData = iTOiLe( ect_sInstance.iMajorRevision );		
		   ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_MAJOR_REV_DS );
		   break;*/

    /*case ABP_ECT_IA_MINOR_REV:	

         ** 
		   ** Copy the 4th Instance 1 attribute (Minor revision) to the message.
		   **         
		   *(UINT16*)psNewMessage->abData = iTOiLe( ect_sInstance.iMinorRevision );
		   ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_MINOR_REV_DS );
		   break;*/

    /*case ABP_ECT_IA_SERIAL_NUMBER:

         **
		   ** Copy the 5th Instance 1 attribute (Serial number) to the message.
		   **
         *(UINT32*)psNewMessage->abData = lTOlLe( ect_sInstance.lSerialNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_SERIAL_NUMBER_DS );
		   break;*/

    /*case ABP_ECT_IA_MANF_DEV_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ect_sInstance.pcDeviceName );

         ** 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         **         
         if( iStrLength > ABP_ECT_IA_MANF_DEV_NAME_MAX_DS )
         {
            iStrLength = ABP_ECT_IA_MANF_DEV_NAME_MAX_DS;
         }

         ** 
         ** Copy the 6th Instance 1 attribute (Manufacturer device name) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, ect_sInstance.pcDeviceName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }*/

    /*case ABP_ECT_IA_MANF_HW_VER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ect_sInstance.pcHWVersion );

         ** 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         **         
         if( iStrLength > ABP_ECT_IA_MANF_HW_VER_MAX_DS )
         {
            iStrLength = ABP_ECT_IA_MANF_HW_VER_MAX_DS;
         }

         ** 
         ** Copy the 7th Instance 1 attribute (Manufacturer hardware version) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, ect_sInstance.pcHWVersion, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;     
      }*/
      
      case ABP_ECT_IA_MANF_SW_VER:
      case ABP_ECT_IA_ENUM_ADIS:
      case ABP_ECT_IA_DEVICE_TYPE:

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
} /* End of ect_InstanceCommand() */


/*------------------------------------------------------------------------------
** ect_ObjectCommand()
**------------------------------------------------------------------------------
*/
void ect_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the EtherCat Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ect_sObject.pcName );

         /* 
         ** Copy the attribute to a message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, ect_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:
      
         /* 
         ** Copy the attribute to a message.
         */
         psNewMessage->abData[ 0 ] = ect_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );                  
         break;

      case ABP_OA_NUM_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( ect_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( ect_sObject.iHighestInstanceNo );
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
} /* End of ect_ObjectCommand() */


/*******************************************************************************
** End of ect.c
********************************************************************************
*/
