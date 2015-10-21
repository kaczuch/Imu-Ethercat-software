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
** Source file for the Profinet IO object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"
#include "prt.h"
#include "abp_pnio.h"

#include "string.h"


/*******************************************************************************
** Private Globals
********************************************************************************
*/
const prt_ObjectType prt_sObject =
{
   "PROFINET IO",                                  /* Name.                      */
   1,                                              /* Revision.                  */
   1,                                              /* Number of instances.       */
   1                                               /* Highest instance number.   */
};

prt_InstanceType prt_sInstance =
{
   0x000,                                          /* 1 Device ID.                     */
 /*0x000,                                       */ /* 2 Vendor ID.                     */
 /*"Abc",                                       */ /* 3 Station Type.                  */
/* No example code:
   0x00,                                           ** 4 MaxAr.                         **
   0x00,                                           ** 7 Record Data Transparent Mode.  **
*/
 /*"Abc",                                       */ /* 8 I&M Order ID.                  */
 /*"               0",                          */ /* 9 I&M Serial Number.             */
 /*0x00,                                        */ /* 10 I&M Hardware Revision.        */
 /*{ 'V', { 1, 0, 0 } },                        */ /* 11 I&M Software Revision.        */
/* No example code:
   0x000,                                          ** 12 I&M Revision Counter.         **
   0x0000,                                         ** 13 I&M Profile ID.               **
   0x000,                                          ** 14 I&M Profile Specific Type.    **
   0x000,                                          ** 15 I&M Version.                  **
   0x000,                                          ** 16 I&M Supported.                **
   "FFFFFF",                                       ** 17 Port 1 MAC Address.           **
   "FFFFFF",                                       ** 18 Port 2 MAC Address.           **
*/
 /*"Abc",                                       */ /* 19 System Description.           */
/* No example code:
   "Abc",                                          ** 20 Interface Description.        **
   0x000,                                          ** 21 Module ID Assignment Mode.    **
*/
 /*"Abc",                                       */ /* 22 System Contact.               */
/* No example code:
   0x00                                            ** 23 PROFIenergy Functionality.    **
*/
};



/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** PRT_ProcessCommand()
**------------------------------------------------------------------------------
*/
void PRT_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Profinet Object and it's Instance.
   */
   if( psNewMessage->sHeader.iInstance == ABP_INST_OBJ )
   {
      /*
      ** Profinet object Command
      */
      prt_ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Profinet instance Command
      */
      prt_InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void prt_InstanceCommand()
**------------------------------------------------------------------------------
*/

void prt_InstanceCommand( ABP_MsgType* psNewMessage )
{
	/*
   ** This function processes commands to the Profinet Instance.
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
      case ABP_PNIO_IA_DEVICE_ID:

         /* 
         ** Copy the 1st Instance 1 attribute (Device ID) to the message. 
         */
         *(UINT16*)psNewMessage->abData = iTOiLe( prt_sInstance.iDeviceID );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_DEVICE_ID_DS );
         break;

     /*case ABP_PNIO_IA_VENDOR_ID:

         ** 
         ** Copy the 2nd Instance 1 attribute (Vendor ID) to the message. 
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( prt_sInstance.iVendorID );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_VENDOR_ID_DS );
         break;*/

    /*case ABP_PNIO_IA_STATION_TYPE:
      {
         UINT16 iStrLength;
         
         iStrLength = (UINT16)strlen( prt_sInstance.pcStationType );

         ** 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         **         
         if( iStrLength > ABP_PNIO_IA_STATION_TYPE_DS )
         {
            iStrLength = ABP_PNIO_IA_STATION_TYPE_DS;
         }

         ** 
         ** Copy the 3rd Instance 1 attribute (Station Type) to the message. 
         **
         ABCC_SYS_MemCpy( psNewMessage->abData, prt_sInstance.pcStationType, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;    
      }*/
	   case ABP_PNIO_IA_MAX_AR:
	   case ABP_PNIO_IA_RTM:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

    /*case ABP_PNIO_IA_IM_ORDER_ID:
      {
         UINT16 iStrLength;
         
         iStrLength = (UINT16)strlen( prt_sInstance.pcIMOrdID );

         ** 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         **         
         if( iStrLength > ABP_PNIO_IA_IM_ORDER_ID_DS )
         {
            iStrLength = ABP_PNIO_IA_IM_ORDER_ID_DS;
         }

         ** 
         ** Copy the 8th Instance 1 attribute (IM Order ID) to the message. 
         **
         ABCC_SYS_MemCpy( psNewMessage->abData, prt_sInstance.pcIMOrdID, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;      
      }*/
	 /*case ABP_PNIO_IA_IM_SERIAL_NBR:
      {
         UINT16 iStrLength;
         
         iStrLength = (UINT16)strlen( prt_sInstance.pcIMSerNr );

         ** 
         ** The maximum number of elements allowed in the array should not be
         ** checked here. It should be checked earlier...
         **         
         if( iStrLength > ABP_PNIO_IA_IM_SERIAL_NBR_DS )
         {
            iStrLength = ABP_PNIO_IA_IM_SERIAL_NBR_DS;
         }

         ** 
         ** Copy the 9th Instance 1 attribute (IM Serial number) to the message. 
         **
         ABCC_SYS_MemCpy( psNewMessage->abData, prt_sInstance.pcIMSerNr, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;      
      }*/
	 /*case ABP_PNIO_IA_IM_HW_REV:

         ** 
         ** Copy the 10th Instance 1 attribute (IM Hardware revision) to the message. 
         **
         *(UINT16*)psNewMessage->abData = iTOiLe( prt_sInstance.iIMHwRev );
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM_HW_REV_DS );
         break;*/

    /*case ABP_PNIO_IA_IM_SW_REV:

         **
         ** Copy the 11th Instance 1 attribute (Software revision) to the message.
         **
         psNewMessage->abData[ 0 ] = prt_sInstance.sSoftwareRevision.bSymbol;
         psNewMessage->abData[ 1 ] = prt_sInstance.sSoftwareRevision.baRevision[ 0 ];
         psNewMessage->abData[ 2 ] = prt_sInstance.sSoftwareRevision.baRevision[ 1 ];
         psNewMessage->abData[ 3 ] = prt_sInstance.sSoftwareRevision.baRevision[ 2 ];
         ABP_SetMsgResponse( psNewMessage, ABP_PNIO_IA_IM_SW_REV_DS );
         break;*/

      case ABP_PNIO_IA_IM_REV_CNT:
	   case ABP_PNIO_IA_IM_PROFILE_ID:
	   case ABP_PNIO_IA_IM_PROFILE_SPEC_TYPE:
	   case ABP_PNIO_IA_IM_VER:
	   case ABP_PNIO_IA_IM_SUPPORTED:
	   case ABP_PNIO_IA_PORT1_MAC_ADDRESS:
	   case ABP_PNIO_IA_PORT2_MAC_ADDRESS:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

    /*case ABP_PNIO_IA_SYSTEM_DESCRIPTION:
      {
         UINT16 iStrLength;
         
         iStrLength = (UINT16)strlen( prt_sInstance.pcSysDescr );

         ** 
         ** Copy the 19th Instance 1 attribute (System description) to the message. 
         **
         ABCC_SYS_MemCpy( psNewMessage->abData, prt_sInstance.pcSysDescr, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;    
      }*/
      case ABP_PNIO_IA_INTERFACE_DESCRIPTION:
	   case ABP_PNIO_IA_MOD_ID_ASSIGN_MODE:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

	 /*case ABP_PNIO_IA_SYSTEM_CONTACT:
      {
         UINT16 iStrLength;
         
         iStrLength = (UINT16)strlen( prt_sInstance.pcSysContact );

         ** 
         ** Copy the 22nd Instance 1 attribute (System contact) to the message. 
         **
         ABCC_SYS_MemCpy( psNewMessage->abData, prt_sInstance.pcSysContact, iStrLength );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;      
      }*/

      case ABP_PNIO_IA_PROFIENERGY_FUNC:
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
** prt_ObjectCommand()
**------------------------------------------------------------------------------
*/
void prt_ObjectCommand( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the Profinet Object (Instance 0).
   */
   switch ( ( psNewMessage->sHeader.bCmd ) & ABP_MSG_HEADER_CMD_BITS )
   {
   case ABP_CMD_GET_ATTR:

      switch( psNewMessage->sHeader.bCmdExt0 )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( prt_sObject.pcName );

         ABCC_SYS_MemCpy( psNewMessage->abData, prt_sObject.pcName, iStrLength );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:

         psNewMessage->abData[ 0 ] = prt_sObject.bRevision;
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( prt_sObject.iNumberOfInstances );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         *(UINT16*)psNewMessage->abData = iTOiLe( prt_sObject.iHighestInstanceNo );
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
** End of prt.c
*****************************************************************************
*/
