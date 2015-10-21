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
** Source file for the BacNet ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "bac.h"
#include "abp_bac.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const bac_ObjectType bac_sObject = 
{
   "BACnet",                                          /* Name.                            */
   1,                                                 /* Revision.                        */
   1,                                                 /* Number of instances.             */
   1                                                  /* Highest instance number.         */
};

bac_InstanceType bac_sInstance = 
{
#ifdef ENABLE_BAC_IA_OBJECT_NAME  
   "Abc",                                             /* 1 Object Name.                   */
#endif
#ifdef ENABLE_BAC_IA_VENDOR_NAME
   "Abc",                                             /* 2 Vendor Name.                   */
#endif
#ifdef ENABLE_BAC_IA_VENDOR_IDENTIFIER
   0x0000,                                            /* 3 Vendor Identifier.             */
#endif
#ifdef ENABLE_BAC_IA_MODEL_NAME
   "Abc",                                             /* 4 Model Name.                    */
#endif
#ifdef ENABLE_BAC_IA_FIRMWARE_REVISION
   "Abc",                                             /* 5 Firmware Revision.             */
#endif
#ifdef ENABLE_BAC_IA_APP_SOFTWARE_VERSION
   "Abc",                                             /* 6 Application Software Version.  */
#endif
#ifdef ENABLE_BAC_IA_SUPPORT_ADV_MAPPING
   FALSE,                                             /* 7 Support Advanced Mapping.      */      
#endif
#ifdef ENABLE_BAC_IA_CURRENT_DATE_AND_TIME
   { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },      /* 8 Current Date and Time.         */
#endif
#ifdef ENABLE_BAC_IA_PASSWORD
   "Abc"                                              /* 9 Password.                      */   
#endif
};


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** BAC_ProcessCommand()
**------------------------------------------------------------------------------
*/
void BAC_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the BacNet Object and its Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /* 
      ** BacNet object Command. 
      */
      bac_ObjectCommand( psNewMessage );
   }
   else
   {
      /* 
      ** BacNet instance Command.
      */
      bac_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
   
} /* End of BAC_ProcessCommand() */


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** bac_InstanceCommand()
**------------------------------------------------------------------------------
*/
void bac_InstanceCommand( ABP_MsgType* psNewMessage )
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
#ifdef ENABLE_BAC_IA_OBJECT_NAME  
      case ABP_BAC_IA_OBJECT_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( bac_sInstance.abObjectName );

         /* 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */         
         if( iStrLength > ABP_BAC_IA_OBJECT_NAME_MAX_DS )
         {
            iStrLength = ABP_BAC_IA_OBJECT_NAME_MAX_DS;
         }

         /* 
         ** Copy the 1st Instance 1 attribute (Object Name) to the message. 
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.abObjectName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#ifdef ENABLE_BAC_IA_VENDOR_NAME
      case ABP_BAC_IA_VENDOR_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( bac_sInstance.pcVendorName );

         /* 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */         
         if( iStrLength > ABP_BAC_IA_VENDOR_NAME_MAX_DS )
         {
            iStrLength = ABP_BAC_IA_VENDOR_NAME_MAX_DS;
         }

         /* 
         ** Copy the 1 Instance 2 attribute (Vendor Name) to the message. 
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.pcVendorName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#ifdef ENABLE_BAC_IA_VENDOR_IDENTIFIER
      case ABP_BAC_IA_VENDOR_IDENTIFIER:

         /*
		   ** Copy the 1 Instance 3 attribute (Vendor Identifier) to the message.
		   */
		   *(UINT16*)psNewMessage->abData = iTOiLe( bac_sInstance.iVendorIdentifier );		 
		   ABP_SetMsgResponse( psNewMessage, ABP_BAC_IA_VENDOR_IDENTIFIER_DS );		  
		   break;
#endif
#ifdef ENABLE_BAC_IA_MODEL_NAME
      case ABP_BAC_IA_MODEL_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( bac_sInstance.pcModelName );

         /* 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */         
         if( iStrLength > ABP_BAC_IA_MODEL_NAME_MAX_DS )
         {
            iStrLength = ABP_BAC_IA_MODEL_NAME_MAX_DS;
         }

         /* 
         ** Copy the 1 Instance 4 attribute (Model Name) to the message. 
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.pcModelName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#ifdef ENABLE_BAC_IA_FIRMWARE_REVISION
      case ABP_BAC_IA_FIRMWARE_REVISION:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( bac_sInstance.pcFWRevision );

         /* 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */         
         if( iStrLength > ABP_BAC_IA_FIRMWARE_REVISION_MAX_DS )
         {
            iStrLength = ABP_BAC_IA_FIRMWARE_REVISION_MAX_DS;
         }

         /* 
         ** Copy the 1 Instance 5 attribute (Firmware revision) to the message. 
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.pcFWRevision, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#ifdef ENABLE_BAC_IA_APP_SOFTWARE_VERSION
      case ABP_BAC_IA_APP_SOFTWARE_VERSION:
     {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( bac_sInstance.pcApplSWVersion );

         /* 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */         
         if( iStrLength > ABP_BAC_IA_APP_SOFTWARE_VERSION_MAX_DS )
         {
            iStrLength = ABP_BAC_IA_APP_SOFTWARE_VERSION_MAX_DS;
         }

         /* 
         ** Copy the 1 Instance 6 attribute (Application software revision) to the message. 
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.pcApplSWVersion, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
#endif
#ifdef ENABLE_BAC_IA_SUPPORT_ADV_MAPPING
      case ABP_BAC_IA_SUPPORT_ADV_MAPPING:
      {
         /* 
         ** Copy the 1 Instance 7 attribute (Support Advanced mapping) to the message. 
         */
         psNewMessage->abData[0] = bac_sInstance.fSupportAdvancedMapping;
         ABP_SetMsgResponse( psNewMessage, ABP_BAC_IA_SUPPORT_ADV_MAPPING_DS );
         break;
      }
#endif
#ifdef ENABLE_BAC_IA_CURRENT_DATE_AND_TIME
      case ABP_BAC_IA_CURRENT_DATE_AND_TIME:
     {
         /*
         ** Todo: PORTING ALERT!
         ** If implemented fetch current time from your application! 
         */
#error  PORTING ALERT! 
         /* 
         ** Copy the 1 Instance 8 attribute (Current date and time) to the message. 
         */
        ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.abCurrentDateTime, ABP_BAC_IA_CURRENT_DATE_AND_TIME_DS );
        ABP_SetMsgResponse( psNewMessage, ABP_BAC_IA_CURRENT_DATE_AND_TIME_DS );
        break;
      }
#endif
#ifdef ENABLE_BAC_IA_PASSWORD
      case ABP_BAC_IA_PASSWORD:
     {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( bac_sInstance.pcPassword );

         /* 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         */         
         if( iStrLength > ABP_BAC_IA_PASSWORD_MAX_DS )
         {
            iStrLength = ABP_BAC_IA_PASSWORD_MAX_DS;
         }

         /* 
         ** Copy the 1 Instance 6 attribute (Application software revision) to the message. 
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.pcPassword, iStrLength );
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

	case ABP_CMD_SET_ATTR:
	{
		switch( psNewMessage->sHeader.bCmdExt0 )
		{
#ifdef ENABLE_BAC_IA_OBJECT_NAME 
		case ABP_BAC_IA_OBJECT_NAME:    
        /*
         ** Todo: PORTING ALERT!
         ** Set attribute is optional.
         ** If implemented consider saving the the data in non-volatile memory 
         */
#error  PORTING ALERT! 
#if 0  
		   if( iLeTOi(psNewMessage->sHeader.iDataSize) > ABP_BAC_IA_OBJECT_NAME_MAX_DS )
         {
				ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
         }
			else
         {
			   ABCC_SYS_MemCpy( bac_sInstance.abObjectName, psNewMessage->abData, psNewMessage->sHeader.iDataSize );
            bac_sInstance.abObjectName[iLeTOi( psNewMessage->sHeader.iDataSize )] = NULL;   /* Add null termination*/
            ABP_SetMsgResponse( psNewMessage, 0 );
         }
			break;
#else
         /*
         ** This attribute is not settable.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif 

#endif
#ifdef ENABLE_BAC_IA_CURRENT_DATE_AND_TIME
      case ABP_BAC_IA_CURRENT_DATE_AND_TIME:
         /*
         ** Todo: PORTING ALERT!
         ** Set attribute is optional
         ** If implemented remeber to send this data to your application.
         */
#error  PORTING ALERT! 
#if 0
			if( ( ( (psNewMessage->abData[0]>>7 & 0xFFFF ) & psNewMessage->abData[1]) > 2200 || 
				 ( psNewMessage->abData[3]>12 ) || ( psNewMessage->abData[4]>31 ) || 
				 ( psNewMessage->abData[5]>24 ) || ( psNewMessage->abData[6]>59 ) || 
				 ( psNewMessage->abData[7]>59 ) ) )
			{
			   ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_OUT_OF_RANGE );
			}
			else
         {
			   **
			   ** Copy the current date time data.
			   **			
			   ABCC_SYS_MemCpy( psNewMessage->abData, bac_sInstance.abCurrentDateTime, ABP_BAC_IA_CURRENT_DATE_AND_TIME_DS );
            ABP_SetMsgResponse( psNewMessage, 0 )
         }			
			break;
#else
         /*
         ** This attribute is not settable.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
#endif

#endif
#ifdef ENABLE_BAC_IA_VENDOR_NAME
      case ABP_BAC_IA_VENDOR_NAME:
#endif
#ifdef ENABLE_BAC_IA_VENDOR_IDENTIFIER
      case ABP_BAC_IA_VENDOR_IDENTIFIER:
#endif
#ifdef ENABLE_BAC_IA_MODEL_NAME
      case ABP_BAC_IA_MODEL_NAME:
#endif
#ifdef ENABLE_BAC_IA_FIRMWARE_REVISION
      case ABP_BAC_IA_FIRMWARE_REVISION:
#endif
#ifdef ENABLE_BAC_IA_APP_SOFTWARE_VERSION
      case ABP_BAC_IA_APP_SOFTWARE_VERSION:
#endif
#ifdef ENABLE_BAC_IA_SUPPORT_ADV_MAPPING
      case ABP_BAC_IA_SUPPORT_ADV_MAPPING:
#endif
#ifdef ENABLE_BAC_IA_PASSWORD
      case ABP_BAC_IA_PASSWORD:
#endif
         /*
         ** This attribute is not settable.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;

		default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
   }
	break;

   default:
   
      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   }
} /* End of bac_InstanceCommand() */


/*------------------------------------------------------------------------------
** bac_ObjectCommand()
**------------------------------------------------------------------------------
*/
void bac_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /* 
   ** This function processes commands to the BacNet Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:
      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( bac_sObject.pcName );

         /* 
         ** Copy the attribute to a message.
         */
         ABCC_SYS_MemCpy( psNewMessage->abData, bac_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
      }
      case ABP_OA_REV:
      
         /* 
         ** Copy the attribute to a message.
         */
         psNewMessage->abData[ 0 ] = bac_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );                  
         break;

      case ABP_OA_NUM_INST:
      
         /* 
         ** Copy the attribute to a message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( bac_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:
      
         /* 
         ** Copy the attribute to a message. 
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( bac_sObject.iHighestInstanceNo );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

#ifdef ENABLE_BAC_IA_SUPPORT_ADV_MAPPING
         /*
         ** Todo: PORTING ALERT!
         ** If Support for advanced mapping is enabeld this services needs to be implemented.
         */
#error  PORTING ALERT!
      case ABP_BAC_CMD_GET_ADI_BY_BACNET_OBJ_INST:
      case ABP_BAC_CMD_GET_ADI_BY_BACNET_OBJ_INST_NAME:
      case ABP_BAC_CMD_GET_ALL_BACNET_OBJ_INSTANCES:
      case ABP_BAC_CMD_GET_BACNET_OBJ_INST_BY_ADI:

#endif

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
} /* End of bac_ObjectCommand() */


/*******************************************************************************
** End of bac.c
********************************************************************************
*/
