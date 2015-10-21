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
** Source file for the Profibus DPV1 ABCC Object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "dpv1.h"
#include "abp_dpv1.h"
#include "abp.h"
#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const dpv1_ObjectType dpv1_sObject =
{
   "PROFIBUS DP-V1",                      /* Name.                            */
   4,                                     /* Revision.                        */
   1,                                     /* Number of instances.             */
   1                                      /* Highest instance number.         */
};


dpv1_InstanceType dpv_sInstance =
{
   0x0000,                                /* 1 PNO identification number.              */
/* { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   */ /* 2 Parameter data.                         */
/* { 0, 0, 0, 0, 0, 0, 0, 0 },         */ /* 3 Expected configuration.                 */
/* FALSE,                              */ /* 4 SSA enabled.                            */
/* 0,                                  */ /* 5 Size of identifier related diagnostics. */
/* 0,                                  */ /* 6 Buffer mode.                            */
/* { 0x00, 0x00, FALSE },              */ /* 7 Alarm settings.                         */
/* 0x0000,                             */ /* 8 Manufacturer ID.                        */
/* "Abc",                              */ /* 9 Order ID.                               */
/* "123",                              */ /* 10 Serial number.                         */
/* 0x0000,                             */ /* 11 Hardware revision.                     */
/* { 'V', { 1, 0, 0 } },               */ /* 12 Software revision.                     */
/* 0,                                  */ /* 13 Revision counter.                      */
/* 0x0000,                             */ /* 14 Profile ID.                            */
/* 0x0000,                             */ /* 15 Profile specific type.                 */
/* { 1, 1 },                           */ /* 16 IM version.                            */
/* ABP_DPV1_CMD_GET_IM_RECORD,         */ /* 17 IM supported.                          */
/* { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   */ /* 18 IM header.                             */
/* ABP_DPV1_CHK_CFG_BEHAVIOR_0         */ /* 19 Check config behaviour.                */
};



/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** DPV1_ProcessCommand()
**------------------------------------------------------------------------------
*/
void DPV1_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Profibus Object and it's Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /*
      ** Profibus object Command.
      */
      dpv1_ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Profibus instance Command.
      */
      dpv1_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void dpv1_InstanceCommand()
**------------------------------------------------------------------------------
*/

void dpv1_InstanceCommand( ABP_MsgType* psNewMessage )
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
      case ABP_DPV1_IA_IDENT_NUMBER:

         /*
         ** Copy the 1st Instance 1 attribute (Ident number) to the message.
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( dpv_sInstance.iIdentNumber );
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_IDENT_NUMBER_DS );
         break;

    /*case ABP_DPV1_IA_PRM_DATA:

         **
         ** The Parameter data is not getable. It's only settable.
         **
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_GETABLE );
         break;*/

    /*case ABP_DPV1_IA_EXPECTED_CFG_DATA:
      {
         UINT16  iIndex;
         UINT16  iNumberOfElements;

         iNumberOfElements = sizeof( dpv_sInstance.abExpectedCfgData ) / sizeof( dpv_sInstance.abExpectedCfgData[ 0 ] );

         *
         ** Copy the 3rd Instance 1 attribute (Expected configuration) to the message.
         *
         for( iIndex = 0; iIndex < iNumberOfElements; ++iIndex )
         {
            psNewMessage->abData[ iIndex ] = dpv_sInstance.abExpectedCfgData[ iIndex ];
         }
         ABP_SetMsgResponse( psNewMessage, iNumberOfElements * ABP_UINT8_SIZEOF ); * ABP_UINT8_SIZEOF is counted per element. *
         break;
      }*/

    /*case ABP_DPV1_IA_SSA_ENABLED:

         **
         ** Copy the 4th Instance 1 attribute (SSA enabled) to the message.
         **
         psNewMessage->abData[ 0 ] = dpv_sInstance.fSSAEnabled;
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_SSA_ENABLED_DS );
         break;*/

    /*case ABP_DPV1_IA_SIZEOF_ID_REL_DIAG:
        
         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 5th Instance 1 attribute (Size of identifier related diagnostics) to the message.
         **
         psNewMessage->abData[ 0 ] = dpv_sInstance.bSizeOfIdRelDiag;
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_SIZEOF_ID_REL_DIAG_DS );
         break;
      */
    /*case ABP_DPV1_IA_BUFFER_MODE:
        
         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 6th Instance 1 attribute (Buffer mode) to the message.
         **
         psNewMessage->abData[ 0 ] = dpv_sInstance.bBufferMode;
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_BUFFER_MODE_DS );
         break;
      */
    /*case ABP_DPV1_IA_ALARM_SETTINGS:
        
         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 7th Instance 1 attribute (Alarm settings) to the message.
         **
         psNewMessage->abData[ 0 ] = dpv_sInstance.sAlarmSettings.bSupportedAlarmTypes;
         psNewMessage->abData[ 1 ] = dpv_sInstance.sAlarmSettings.bRequiredAlarmTypes;
         psNewMessage->abData[ 2 ] = dpv_sInstance.sAlarmSettings.fSequenceModeEnabled;
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_ALARM_SETTINGS_DS );
         break;
      */

    /*case ABP_DPV1_IA_MANUFACTURER_ID:

         **
         ** Copy the 8th Instance 1 attribute (Manufacturer ID) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( dpv_sInstance.iManufacturerID );
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_MANUFACTURER_ID_DS );
         break;*/

    /*case ABP_DPV1_IA_ORDER_ID:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( dpv_sInstance.pcOrderID );

         ** 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         **         
         if( iStrLength > ABP_DPV1_IA_ORDER_ID_DS )
         {
            iStrLength = ABP_DPV1_IA_ORDER_ID_DS;
         }

         ** 
         ** Copy the 9th Instance 1 attribute (Order ID) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, dpv_sInstance.pcOrderID, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );

         break; 
      }*/

    /*case ABP_DPV1_IA_SERIAL_NO:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( dpv_sInstance.pcSerialNumber );

         ** 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         **         
         if( iStrLength > ABP_DPV1_IA_SERIAL_NO_DS )
         {
            iStrLength = ABP_DPV1_IA_SERIAL_NO_DS;
         }

         ** 
         ** Copy the 10th Instance 1 attribute (Serial number) to the message.
         **         
         ABCC_SYS_MemCpy( psNewMessage->abData, dpv_sInstance.pcSerialNumber, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break; 
      }*/

    /*case ABP_DPV1_IA_HW_REV:

         **
         ** Copy the 11th Instance 1 attribute (Hardware revision) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( dpv_sInstance.iHardwareRevision );
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_HW_REV_DS );
         break;*/

    /*case ABP_DPV1_IA_SW_REV:

         **
         ** Copy the 12th Instance 1 attribute (Software revision) to the message.
         **
         psNewMessage->abData[ 0 ] = dpv_sInstance.sSoftwareRevision.bSymbol;
         psNewMessage->abData[ 1 ] = dpv_sInstance.sSoftwareRevision.baRevision[ 0 ];
         psNewMessage->abData[ 2 ] = dpv_sInstance.sSoftwareRevision.baRevision[ 1 ];
         psNewMessage->abData[ 3 ] = dpv_sInstance.sSoftwareRevision.baRevision[ 2 ];
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_SW_REV_DS );
         break;*/

    /*case ABP_DPV1_IA_REV_COUNTER:
        
         **
         ** Copy the 13th Instance 1 attribute (Revision counter) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( dpv_sInstance.iRevisionCounter );
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_REV_COUNTER_DS );
         break;
      */
    /*case ABP_DPV1_IA_PROFILE_ID:
        
         **
         ** Copy the 14th Instance 1 attribute (Profile ID) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( dpv_sInstance.iProfileID );
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_PROFILE_ID_DS );
         break;
      */
    /*case ABP_DPV1_IA_PROFILE_SPEC_TYPE:
        
         **
         ** Copy the 15th Instance 1 attribute (Profile specific type) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( dpv_sInstance.iProfileSpecType );
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_PROFILE_SPEC_TYPE_DS );
         break;
      */
    /*case ABP_DPV1_IA_IM_VERSION:
        
         **
         ** Copy the 16th Instance 1 attribute (IM version) to the message.
         **
         psNewMessage->abData[ 0 ] = dpv_sInstance.sIMVersion.bMajorVersion;
         psNewMessage->abData[ 1 ] = dpv_sInstance.sIMVersion.bMinorVersion;
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_IM_VERSION_DS );
         break;
      */
    /*case ABP_DPV1_IA_IM_SUPPORTED:
        
         **
         ** Copy the 17th Instance 1 attribute (IM supported) to the message.
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( dpv_sInstance.iIMSupported );
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_IM_SUPPORTED_DS );
         break;
      */
    /*case ABP_DPV1_IA_IM_HEADER:        
      {
         UINT16  iIndex;
         UINT16  iNumberOfElements;

         iNumberOfElements = sizeof( dpv_sInstance.aiIMheader ) / sizeof( dpv_sInstance.aiIMheader[ 0 ] );

         **
         ** Copy the 18th Instance 1 attribute (IM header) to the message.
         **
         for( iIndex = 0; iIndex < iNumberOfElements; ++iIndex )
         {
            psNewMessage->abData[ iIndex ] = dpv_sInstance.aiIMheader[ iIndex ];
         }

         ** 
         ** The value of ABP_DPV1_IA_IM_HEADER_DS is counted per element, and 
         ** should therefore be divided by 10 as long as ABP_DPV1_IA_IM_HEADER_DS 
         ** is defined to be multiplied by 10 in abp_dpv1.h as ( 10 * ABP_UINT8_SIZEOF ).
         **
         ABP_SetMsgResponse( psNewMessage, iNumberOfElements * ( ABP_DPV1_IA_IM_HEADER_DS / 10 ) );
         break;
      }
      */
    /*case ABP_DPV1_IA_CHK_CFG_BEHAVIOR:
        
         **
         ** This attribute was deprecated and removed from Anybus CompactCom 40.
         ** Copy the 19th Instance 1 attribute (Check config behaviour) to the message.
         **
         psNewMessage->abData[ 0 ] = dpv_sInstance.bCheckCfgBehaviour;
         ABP_SetMsgResponse( psNewMessage, ABP_DPV1_IA_CHK_CFG_BEHAVIOR_DS );
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

   case ABP_CMD_SET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_DPV1_IA_IDENT_NUMBER:

         /*
         ** This attribute is not settable.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;

    /*case ABP_DPV1_IA_PRM_DATA:

         **
         ** The data size is verified.
         **
         if( psNewMessage->sHeader.iDataSize > ( DPV1_PRM_STANDARD_BYTES +
               DPV1_PRM_DPV1_STATUS_BYTES + DPV1_PRM_APPL_SPECIFIC_BYTES ) )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
         }
         else if( psNewMessage->sHeader.iDataSize < DPV1_PRM_STANDARD_BYTES )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NOT_ENOUGH_DATA );
         }
         else
         {
            **
            ** If the application has defined user specific parameter data,
            ** this is the place to check the data (from byte 10 and forth)
            ** In this example we only update the attribute.
            **
            ABCC_SYS_MemCpy( dpv_sInstance.abParamData, psNewMessage->abData, 10 );
            ABP_SetMsgResponse( psNewMessage, 0 );
         }
         break;*/

    /*case ABP_DPV1_IA_EXPECTED_CFG_DATA:
      case ABP_DPV1_IA_SSA_ENABLED:

         **
         ** This attribute is not settable.
         **
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;*/

    /*case ABP_DPV1_IA_SIZEOF_ID_REL_DIAG:
      case ABP_DPV1_IA_BUFFER_MODE:
      case ABP_DPV1_IA_ALARM_SETTINGS:

         **
         ** Unsupported attribute.
         **
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;*/

    /*case ABP_DPV1_IA_MANUFACTURER_ID:
      case ABP_DPV1_IA_ORDER_ID:
      case ABP_DPV1_IA_SERIAL_NO:
      case ABP_DPV1_IA_HW_REV:
      case ABP_DPV1_IA_SW_REV:

         **
         ** This attribute is not settable.
         **
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;*/

    /*case ABP_DPV1_IA_REV_COUNTER:
      case ABP_DPV1_IA_PROFILE_ID:
      case ABP_DPV1_IA_PROFILE_SPEC_TYPE:
      case ABP_DPV1_IA_IM_VERSION:
      case ABP_DPV1_IA_IM_SUPPORTED:
      case ABP_DPV1_IA_IM_HEADER:
      case ABP_DPV1_IA_CHK_CFG_BEHAVIOR:
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
}


/*------------------------------------------------------------------------------
** dpv1_ObjectCommand()
**------------------------------------------------------------------------------
*/
void dpv1_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Profibus Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( dpv1_sObject.pcName );
         ABCC_SYS_MemCpy( psNewMessage->abData, dpv1_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:

         psNewMessage->abData[ 0 ] = dpv1_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( dpv1_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( dpv1_sObject.iHighestInstanceNo );
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
** End of dpv1.c
*****************************************************************************
*/
