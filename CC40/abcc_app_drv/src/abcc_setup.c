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
** Implementation of abcc setup state machine
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abcc_drv_if.h"
#include "abp.h"
#include "abcc.h"
#include "abcc_handler.h"
#include "abcc_link.h"
#include "abcc_sys_adapt.h"
#include "abcc_debug_err.h"
#include "abcc_timer.h"



/*******************************************************************************
** Public Globals
********************************************************************************
*/


/*******************************************************************************
** Private Globals
********************************************************************************
*/

typedef enum CmdStateType
{
   /*
   ** Order dependent states.
   */
   MSG_GET_DATA_FORMAT,
   MSG_GET_NW_PARAM_SUPPORT,
   MSG_GET_MODULE_TYPE,
   MSG_GET_NETWORK_TYPE,
   MSG_MAP_PD_READ_WRITE,
   MSG_USER_INIT_START,
   MSG_USER_INIT,
   MSG_READ_RDPD_SIZE,
   MSG_READ_WRPD_SIZE,
   MSG_SETUP_COMPLETE,
   MSG_DONE
}
CmdStateType;


/*------------------------------------------------------------------------------
** iModuleType       - ABCC module type (read out during SETUP state)
** iNetworkType      - ABCC network type (read out during SETUP state)
** abcc_iPdReadSize  - Read process data size
** abcc_iPdWriteSize - Write process data size
** eParameterSupport - Parameter support (read out during SETUP state)
**------------------------------------------------------------------------------
*/
static UINT16   iModuleType;
static UINT16   iNetworkType;
static NetFormatType     eNetFormat;
static ParameterSupportType eParameterSupport;
       
static CmdStateType      eCmdState  = MSG_GET_DATA_FORMAT;
/*
** Help varibales for ADI mapping servcie
*/
static AD_AdiEntryType*    psAdiEntry     = NULL;
static AD_DefaultMapType*  psDefaultMap   = NULL;
static UINT16              iMappingIndex  = 0;


/*
** Currently used processdata sizes
*/
static UINT16   abcc_iPdReadSize    = 0;
static UINT16   abcc_iPdWriteSize   = 0;



/*------------------------------------------------------------------------------
** SetupCommands()
**------------------------------------------------------------------------------
*/


void ABCC_SetupInit( void )
{
   eCmdState  = MSG_GET_DATA_FORMAT;

   iModuleType = 0xFFFF;
   iNetworkType = 0xFFFF;
   eNetFormat = NET_UNKNOWN;
   eParameterSupport = PARAMETER_UNKNOWN;

   psAdiEntry     = NULL;
   psDefaultMap   = NULL;
   iMappingIndex  = 0;
   abcc_iPdReadSize    = 0;
   abcc_iPdWriteSize   = 0;
}


BOOL ABCC_SetupCommands( void )
{
   UINT16 iLocalMapIndex;
   UINT16 iLocalSize;
   BOOL fRunSM = TRUE;

   ABP_MsgType* pMsgSendBuffer;


   while( fRunSM )
   {
      fRunSM = FALSE;
      switch( eCmdState )
      {
         case MSG_GET_DATA_FORMAT:
            pMsgSendBuffer = ABCC_LinkAlloc( );
            ABCC_GetAttribute( pMsgSendBuffer, ABP_OBJ_NUM_NW, 1 ,
                               ABP_NW_IA_DATA_FORMAT, ABCC_GetNewSourceId() );
            ABCC_LinkWriteMessage( pMsgSendBuffer);
            break;

         case MSG_GET_NW_PARAM_SUPPORT:
            pMsgSendBuffer = ABCC_LinkAlloc( );
            ABCC_GetAttribute( pMsgSendBuffer, ABP_OBJ_NUM_NW, 1 ,
                               ABP_NW_IA_PARAM_SUPPORT, ABCC_GetNewSourceId() );
            ABCC_LinkWriteMessage( pMsgSendBuffer);
            break;

         case MSG_GET_MODULE_TYPE:
            pMsgSendBuffer = ABCC_LinkAlloc( );
            ABCC_GetAttribute( pMsgSendBuffer, ABP_OBJ_NUM_ANB, 1 ,
                               ABP_ANB_IA_MODULE_TYPE, ABCC_GetNewSourceId() );
            ABCC_LinkWriteMessage( pMsgSendBuffer);
            break;

         case MSG_GET_NETWORK_TYPE:
            pMsgSendBuffer = ABCC_LinkAlloc( );
            ABCC_GetAttribute( pMsgSendBuffer, ABP_OBJ_NUM_NW, 1 ,
                               ABP_NW_IA_NW_TYPE, ABCC_GetNewSourceId() );
            ABCC_LinkWriteMessage( pMsgSendBuffer);
            break;

         case MSG_MAP_PD_READ_WRITE:

            if ( ABCC_SYS_ReadModuleId() == ABP_MODULE_ID_ACTIVE_ABCC40 ) /* If an ABCC40 is attached. */
            {
               /* Implement mapping according to the new Extended Command for ABCC40. */
               if( psAdiEntry && psDefaultMap && ( psDefaultMap[ iMappingIndex ].iAdiIndex != 0xFFFF ) )
               {
                  iLocalMapIndex = psDefaultMap[ iMappingIndex ].iAdiIndex;
                  iLocalSize = ABCC_GetDataTypeSize( psAdiEntry[ iLocalMapIndex ].bDataType ) * psAdiEntry[ iLocalMapIndex ].bNumOfElements;


                  pMsgSendBuffer = ABCC_LinkAlloc( );
                  pMsgSendBuffer->sHeader.bDestObj             = ABP_OBJ_NUM_NW;
                  pMsgSendBuffer->sHeader.iInstance            = iTOiLe( 1 );
                  pMsgSendBuffer->sHeader.iDataSize            = 7; /* The number of used bytes in abData. (The bytes written below). */
                  pMsgSendBuffer->sHeader.bCmdExt0             = 1; /* Number of mapping items to add. */
                  pMsgSendBuffer->sHeader.bCmdExt1             = 0; /* Reserved. */


                  *(UINT16*)&pMsgSendBuffer->abData[ 0 ]       = iTOiLe( psAdiEntry[ iLocalMapIndex ].iInstance ); /* ADI Instance number. */
                  pMsgSendBuffer->abData[ 2 ]                  = psAdiEntry[ iLocalMapIndex ].bNumOfElements; /* Total number of elements in ADI. */
                  pMsgSendBuffer->abData[ 3 ]                  = 0; /* Index to the first element to map. 0 if the entire ADI is mapped. */
                  pMsgSendBuffer->abData[ 4 ]                  = psAdiEntry[ iLocalMapIndex ].bNumOfElements; /* Number of consecutive elements to map. */
                  pMsgSendBuffer->abData[ 5 ]                  = 1; /* Number of type descriptors. */
                  pMsgSendBuffer->abData[ 6 ]                  = psAdiEntry[ iLocalMapIndex ].bDataType; /* ADI element data type. */

                  if( psDefaultMap[ iMappingIndex ].eDir == PD_READ )
                  {
                     pMsgSendBuffer->sHeader.bCmd =  ABP_MSG_HEADER_C_BIT | ABP_NW_CMD_MAP_ADI_READ_EXT_AREA;
                     abcc_iPdReadSize             += iLocalSize;
                  } 
                  else
                  {
                     pMsgSendBuffer->sHeader.bCmd =  ABP_MSG_HEADER_C_BIT | ABP_NW_CMD_MAP_ADI_WRITE_EXT_AREA;
                     abcc_iPdWriteSize            += iLocalSize;
                  }
                  iMappingIndex++;

                  ABCC_LinkWriteMessage( pMsgSendBuffer);
               }
               else
               {
                  eCmdState = MSG_USER_INIT_START;
                  fRunSM = TRUE;
               }
               break;
            }
            else /* If an ABCC30 is attached. */
            {
               if( psAdiEntry && psDefaultMap && ( psDefaultMap[ iMappingIndex ].iAdiIndex != 0xFFFF ) )
               {
                  iLocalMapIndex = psDefaultMap[ iMappingIndex ].iAdiIndex;
                  iLocalSize = ABCC_GetDataTypeSize( psAdiEntry[ iLocalMapIndex ].bDataType ) * psAdiEntry[ iLocalMapIndex ].bNumOfElements;


                  pMsgSendBuffer = ABCC_LinkAlloc( );
                  pMsgSendBuffer->sHeader.bDestObj             = ABP_OBJ_NUM_NW;
                  pMsgSendBuffer->sHeader.iInstance            = iTOiLe( 1 );
                  pMsgSendBuffer->sHeader.iDataSize            = 4;
                  *(UINT16*)&pMsgSendBuffer->sHeader.bCmdExt0  = iTOiLe( psAdiEntry[ iLocalMapIndex ].iInstance );  /* ADI Instance number. */


                  pMsgSendBuffer->abData[ 0 ]                  = psAdiEntry[ iLocalMapIndex ].bDataType;            /* ADI data type. */
                  pMsgSendBuffer->abData[ 1 ]                  = psAdiEntry[ iLocalMapIndex ].bNumOfElements;       /* Number of elements in ADI. */
                  *(UINT16*)&pMsgSendBuffer->abData[ 2 ]       = iTOiLe( iLocalMapIndex + 1 );                      /* ADI order number. */

                  if( psDefaultMap[ iMappingIndex ].eDir == PD_READ )
                  {
                     pMsgSendBuffer->sHeader.bCmd =  ABP_MSG_HEADER_C_BIT | ABP_NW_CMD_MAP_ADI_READ_AREA;
                     abcc_iPdReadSize             += iLocalSize;
                  } 
                  else
                  {
                     pMsgSendBuffer->sHeader.bCmd =  ABP_MSG_HEADER_C_BIT | ABP_NW_CMD_MAP_ADI_WRITE_AREA;
                     abcc_iPdWriteSize            += iLocalSize;
                  }
                  iMappingIndex++;

                  ABCC_LinkWriteMessage( pMsgSendBuffer);
               }
               else
               {
                  eCmdState = MSG_USER_INIT_START;
                  fRunSM = TRUE;
               }
               break;
            }

         case MSG_USER_INIT_START:
            eCmdState = MSG_USER_INIT;
            ABCC_CbfUserInitReq();

            break;
         case MSG_USER_INIT:
            /* Do nothing */
            break;

         case MSG_READ_RDPD_SIZE:
            /* Do nothing */
            break;

         case MSG_READ_WRPD_SIZE:
            pMsgSendBuffer = ABCC_LinkAlloc( );
            ABCC_GetAttribute( pMsgSendBuffer, ABP_OBJ_NUM_NW, 1 ,
                               ABP_NW_IA_WRITE_PD_SIZE, ABCC_GetNewSourceId() );
            ABCC_LinkWriteMessage( pMsgSendBuffer);
             break;

         case MSG_SETUP_COMPLETE:

            pMsgSendBuffer = ABCC_LinkAlloc( );

            ABCC_SetByteAttribute(  pMsgSendBuffer, ABP_OBJ_NUM_ANB, 1,
                                    ABP_ANB_IA_SETUP_COMPLETE, TRUE,
                                    ABCC_GetNewSourceId() );

            ABCC_LinkWriteMessage( pMsgSendBuffer);

            break;

         case MSG_DONE:
            return TRUE;
      } /* end switch( command state ) */
   }

   return FALSE;
}


/*------------------------------------------------------------------------------
** SetupResponses()
**------------------------------------------------------------------------------
*/
void ABCC_SetupResponses( ABP_MsgType* pMsgReciveBuffer )
{
   ABCC_MsgHandlerFuncType pnMsgHandler = 0;
   switch( eCmdState )
   {
      case MSG_GET_DATA_FORMAT:
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer->abData[0] );
         ABCC_ASSERT(  pMsgReciveBuffer->abData[ 0 ] < 2 );
         eNetFormat = ( CmdStateType )pMsgReciveBuffer->abData[ 0 ];
         DEBUG_EVENT(("RSP MSG_DATA_FORMAT: %d", eNetFormat ));

         eCmdState = MSG_GET_NW_PARAM_SUPPORT;
         break;

      case MSG_GET_NW_PARAM_SUPPORT:
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer->abData[0] );
         eParameterSupport = ( ParameterSupportType )pMsgReciveBuffer->abData[ 0 ];
         DEBUG_EVENT(("RSP MSG_GET_PARAM_SUPPORT: %d ", eParameterSupport));
         eCmdState = MSG_GET_MODULE_TYPE;
         break;

      case MSG_GET_MODULE_TYPE:
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer->abData[0] );
         iModuleType = iLeTOi( *(UINT16*)pMsgReciveBuffer->abData );
         DEBUG_EVENT(("RSP MSG_GET_MODULE_ID: 0x%x ",iModuleType));
         eCmdState = MSG_GET_NETWORK_TYPE;
         break;

      case MSG_GET_NETWORK_TYPE:
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer->abData[0] );
         iNetworkType = iLeTOi( *(UINT16*)pMsgReciveBuffer->abData);
         DEBUG_EVENT(("RSP MSG_GET_NETWORK_ID :0x%x ", iNetworkType));

         /*
         ** We now have all information needed to initialize the ADIs and the mapping.
         */
         eCmdState = MSG_MAP_PD_READ_WRITE;
         ABCC_CbfAdiMappingReq ( (const AD_AdiEntryType** )&psAdiEntry,
                                 (const AD_DefaultMapType** )&psDefaultMap );
         break;

      case MSG_USER_INIT:
         DEBUG_EVENT(("RSP MSG_ABCC_USER_INIT"));
         pnMsgHandler = ABCC_LinkGetMsgHandler( pMsgReciveBuffer->sHeader.bSourceId );
         if( pnMsgHandler )
         {
            pnMsgHandler( pMsgReciveBuffer );
         }
         else
         {
            ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_INVALID_RESP_SOURCE_ID,(UINT32) pMsgReciveBuffer );
         }
         break;

      case MSG_MAP_PD_READ_WRITE:
         DEBUG_EVENT(("RSP MSG_MAP_IO_****"));
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer);
         break;

      case MSG_READ_RDPD_SIZE:
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer);

         if( psDefaultMap == NULL )
         {
            /* Use received read size */
            abcc_iPdReadSize = iLeTOi( *(UINT16*)pMsgReciveBuffer->abData );
         }
         else
         {
            /* Verify that ABCC and driver has the same view */
            ABCC_ASSERT( abcc_iPdReadSize ==  iLeTOi( *(UINT16*)pMsgReciveBuffer->abData ) );
         }
         eCmdState = MSG_READ_WRPD_SIZE;
         break;

      case MSG_READ_WRPD_SIZE:
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer);

         if( psDefaultMap == NULL )
         {
            /* Use received write size */
            abcc_iPdWriteSize = iLeTOi( *(UINT16*)pMsgReciveBuffer->abData );
         }
         else
         {
            /* Verify that ABCC and driver has the same view */
            ABCC_ASSERT(abcc_iPdWriteSize ==  iLeTOi( *(UINT16*)pMsgReciveBuffer->abData ) );
         }

         eCmdState = MSG_SETUP_COMPLETE;
         break;

      case MSG_SETUP_COMPLETE:
         DEBUG_EVENT(("RSP MSG_SETUP_COMPLETE"));
         ABCC_ASSERT_ERR( ABCC_VerifyMessage( pMsgReciveBuffer ), ABCC_SEV_WARNING, ABCC_EC_RESP_MSG_E_BIT_SET, (UINT32) pMsgReciveBuffer );
         
         DEBUG_EVENT(("Mapped PD size, RdPd %d WrPd: %d", abcc_iPdReadSize, abcc_iPdWriteSize));
         pnABCC_DrvSetPdSize(abcc_iPdReadSize, abcc_iPdWriteSize );
         eCmdState = MSG_DONE;
         break;

      default:
         ABCC_ASSERT( FALSE );
         break;
   } /* end switch */

   ABCC_LinkFree( &pMsgReciveBuffer );
}


void ABCC_UserInitComplete( BOOL fSuccess )
{
   ABP_MsgType* pMsgSendBuffer;
   
   if ( fSuccess )
   {
      pMsgSendBuffer = ABCC_LinkAlloc( );
      ABCC_GetAttribute( pMsgSendBuffer, ABP_OBJ_NUM_NW, 1 ,
                         ABP_NW_IA_READ_PD_SIZE, ABCC_GetNewSourceId() );
      ABCC_LinkWriteMessage( pMsgSendBuffer);
      eCmdState         = MSG_READ_RDPD_SIZE;

   }
   else
   {
      ABCC_SetMainStateError();
   }
}

UINT16 ABCC_NetworkType( void )
{
   return iNetworkType;
}

UINT16 ABCC_ModuleType( void )
{
   return iModuleType;
}

NetFormatType ABCC_NetFormatType( void )
{
    return eNetFormat;
}


ParameterSupportType ABCC_ParameterSupport( void )
{
   return eParameterSupport;
}
















