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
** Implementation of operation mode independent parts of the abcc handler.
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abcc_drv_if.h"
#include "abp.h"
#include "abcc.h"
#include "abcc_link.h"
#include "abcc_sys_adapt.h"
#include "abcc_debug_err.h"
#include "abcc_handler.h"
#include "abcc_timer.h"
#include "abcc_setup.h"

#ifdef ABCC_USER_DRV_SPI
#include "spi/abcc_drv_spi_if.h"
#endif
#ifdef ABCC_USER_DRV_PARALLEL
#include "par/abcc_drv_par_if.h"
#endif
#ifdef ABCC_USER_DRV_PARALLEL_30
#include "par30/abcc_drv_par30_if.h"
#endif
#ifdef ABCC_USER_DRV_SERIAL
#include "serial/abcc_drv_ser_if.h"
#endif
/*
** Registerd handler functions
*/
EXTFUNC BOOL ABCC_SpiRunDriver( void );
EXTFUNC void ABCC_SpiISR( void );

EXTFUNC BOOL ABCC_ParRunDriver( void );
EXTFUNC void ABCC_ParISR( void );

EXTFUNC BOOL ABCC_Par30RunDriver( void );
EXTFUNC void ABCC_Par30ISR( void );

EXTFUNC BOOL ABCC_SerRunDriver( void );
EXTFUNC void ABCC_SerISR( void );




/*
 ** Default USER defines
 */
#ifndef ABCC_USER_STARTUP_TIME_MS
#define ABCC_USER_STARTUP_TIME_MS ( 1500 )
#endif

/********************************************************************************
** Public Globals
********************************************************************************
*/

/*
** Registerd handler functions
*/

BOOL ( *ABCC_RunDriver )( void );
void ( *ABCC_ISR )( void );

/*
** Registerd driver functions
*/

void  ( *pnABCC_DrvInit )( UINT8 bOpmode );
UINT16 ( *pnABCC_DrvISR )( void );
void ( *pnABCC_DrvRunDriverTx )( void );
ABP_MsgType* ( *pnABCC_DrvRunDriverRx )( void );
BOOL ( *pnABCC_DrvWriteMessage) ( ABP_MsgType* psWriteMsg );
void ( *pnABCC_DrvWriteProcessData )( UINT8* pbProcessData );
BOOL ( *pnABCC_DrvISReadyForWrPd )( void );
BOOL ( *pnABCC_DrvISReadyForWriteMessage )( void );
BOOL ( *pnABCC_DrvISReadyForCmd )( void );
void ( *pnABCC_DrvSetNbrOfCmds )( UINT8 bNbrOfCmds );
void ( *pnABCC_DrvSetAppStatus )( ABP_AppStatusType eAppStatus );
void ( *pnABCC_DrvSetPdSize )( const UINT16 iReadPdSize, const UINT16 iWritePdSize );
void ( *pnABCC_DrvSetMsgReceiverBuffer )( ABP_MsgType* const psReadMsg );
void ( *pnABCC_DrvSetIntMask )( const UINT16 iIntMask );
UINT8* ( *pnABCC_DrvGetWrPdBuffer )( void );
UINT16 ( *pnABCC_DrvGetModCap )( void );
UINT16 ( *pnABCC_DrvGetLedStatus )( void );
UINT16 ( *pnABCC_DrvGetIntStatus )( void );
UINT8 ( *pnABCC_DrvGetAnybusState )( void );
UINT8* ( *pnABCC_DrvReadProcessData )( void );
ABP_MsgType* ( *pnABCC_DrvReadMessage )( void );
BOOL ( *pnABCC_DrvIsSupervised )( void );
UINT8 ( *pnABCC_DrvGetAnbStatus )( void );

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*
** Current anybus status
*/
static UINT8 abcc_bAnbStatus;

static ABCC_MainStateType abcc_eMainState = ABCC_DRV_INIT;

/*
** Pointer to WRPD buffer.
*/
static UINT8* abcc_pbWrPdBuffer;

/*
 ** Tmo handler for
 */
static ABCC_TimerHandle abcc_TmoHandle;

/*
 ** Indicate ready for communication
 */
static BOOL fAbccReadyForCommunication;


/*
** Set by either interrupt or timeout
*/
void ABCC_SetReadyForCommunication(void)
{
   fAbccReadyForCommunication = TRUE;
}


void ABCC_SetMainStateError( void )
{
   abcc_eMainState = ABCC_DRV_ERROR;
}

ABCC_MainStateType ABCC_GetMainState( void )
{
   return abcc_eMainState;
}

void ABCC_CheckAnbStatus( void )
{
   UINT8 bAnbStatus;

   bAnbStatus = pnABCC_DrvGetAnbStatus();

   if ( bAnbStatus != abcc_bAnbStatus )
   {
      ABCC_CbfAnbStatusChanged();
      abcc_bAnbStatus = bAnbStatus;
   }
}

void ABCC_CheckSendMessage( void )
{
   ABCC_LinkCheckSendMessage();
}


/*******************************************************************************
** Public Functions
********************************************************************************
*/
ABCC_StatusType ABCC_StartDriver( UINT8 bOpmode, BOOL fPingPong )
{
   SetErrorReporter( ABCC_CbfDriverError );

   if( fPingPong )
   {
      switch( bOpmode )
      {
#ifdef ABCC_USER_DRV_PARALLEL_30
      case ABP_OP_MODE_8_BIT_PARALLEL:
         ABCC_ISR                   = &ABCC_Par30ISR;      
         ABCC_RunDriver             = &ABCC_Par30RunDriver;

         pnABCC_DrvInit               = &ABCC_DrvPar30Init;
         pnABCC_DrvISR                = &ABCC_DrvPar30ISR;
         pnABCC_DrvRunDriverTx        = &ABCC_DrvPar30RunDriverTx;;
         pnABCC_DrvRunDriverRx        = &ABCC_DrvPar30RunDriverRx;
         pnABCC_DrvWriteMessage       = &ABCC_DrvPar30WriteMessage;
         pnABCC_DrvWriteProcessData   = &ABCC_DrvPar30WriteProcessData;
         pnABCC_DrvISReadyForWrPd     = &ABCC_DrvPar30IsReadyForWrPd;
         pnABCC_DrvISReadyForWriteMessage = &ABCC_DrvPar30IsReadyForWriteMessage;
         pnABCC_DrvISReadyForCmd      = &ABCC_DrvPar30IsReadyForCmd;
         pnABCC_DrvSetNbrOfCmds       = &ABCC_DrvPar30SetNbrOfCmds;
         pnABCC_DrvSetAppStatus       = &ABCC_DrvPar30SetAppStatus;
         pnABCC_DrvSetPdSize          = &ABCC_DrvPar30SetPdSize;
         pnABCC_DrvSetMsgReceiverBuffer = &ABCC_DrvPar30SetMsgReceiverBuffer;
         pnABCC_DrvSetIntMask         = &ABCC_DrvPar30SetIntMask;
         pnABCC_DrvGetWrPdBuffer      = &ABCC_DrvPar30GetWrPdBuffer;
         pnABCC_DrvGetModCap          = &ABCC_DrvPar30GetModCap;
         pnABCC_DrvGetLedStatus       = &ABCC_DrvPar30GetLedStatus;
         pnABCC_DrvGetIntStatus       = &ABCC_DrvPar30GetIntStatus;
         pnABCC_DrvGetAnybusState     = &ABCC_DrvPar30GetAnybusState;
         pnABCC_DrvReadProcessData    = &ABCC_DrvPar30ReadProcessData;
         pnABCC_DrvReadMessage        = &ABCC_DrvPar30ReadMessage;
         pnABCC_DrvIsSupervised       = &ABCC_DrvPar30IsSupervised;
         pnABCC_DrvGetAnbStatus       = &ABCC_DrvPar30GetAnbStatus;
         break;
#endif
#ifdef ABCC_USER_DRV_SERIAL
      case ABP_OP_MODE_SERIAL_19_2:
      case ABP_OP_MODE_SERIAL_57_6:
      case ABP_OP_MODE_SERIAL_115_2:
      case ABP_OP_MODE_SERIAL_625:
         ABCC_ISR                   = NULL;
         ABCC_RunDriver             = &ABCC_SerRunDriver;

         pnABCC_DrvInit               = &ABCC_DrvSerInit;
         pnABCC_DrvISR                = &ABCC_DrvSerISR;
         pnABCC_DrvRunDriverTx        = &ABCC_DrvSerRunDriverTx;
         pnABCC_DrvRunDriverRx        = &ABCC_DrvSerRunDriverRx;
         pnABCC_DrvWriteMessage       = &ABCC_DrvSerWriteMessage;
         pnABCC_DrvWriteProcessData   = &ABCC_DrvSerWriteProcessData;
         pnABCC_DrvISReadyForWrPd     = &ABCC_DrvSerIsReadyForWrPd;
         pnABCC_DrvISReadyForWriteMessage = &ABCC_DrvSerIsReadyForWriteMessage;
         pnABCC_DrvISReadyForCmd      = &ABCC_DrvSerIsReadyForCmd;
         pnABCC_DrvSetNbrOfCmds       = &ABCC_DrvSerSetNbrOfCmds;
         pnABCC_DrvSetAppStatus       = &ABCC_DrvSerSetAppStatus;
         pnABCC_DrvSetPdSize          = &ABCC_DrvSerSetPdSize;
         pnABCC_DrvSetMsgReceiverBuffer = &ABCC_DrvSerSetMsgReceiverBuffer;
         pnABCC_DrvSetIntMask         = &ABCC_DrvSerSetIntMask;
         pnABCC_DrvGetWrPdBuffer      = &ABCC_DrvSerGetWrPdBuffer;
         pnABCC_DrvGetModCap          = &ABCC_DrvSerGetModCap;
         pnABCC_DrvGetLedStatus       = &ABCC_DrvSerGetLedStatus;
         pnABCC_DrvGetIntStatus       = &ABCC_DrvSerGetIntStatus;
         pnABCC_DrvGetAnybusState     = &ABCC_DrvSerGetAnybusState;
         pnABCC_DrvReadProcessData    = &ABCC_DrvSerReadProcessData;
         pnABCC_DrvReadMessage        = &ABCC_DrvSerReadMessage;
         pnABCC_DrvIsSupervised       = &ABCC_DrvSerIsSupervised;
         pnABCC_DrvGetAnbStatus       = &ABCC_DrvSerGetAnbStatus;
         break;
#endif
      default:
         ABCC_ERROR(ABCC_SEV_FATAL, ABCC_EC_INCORRECT_OPERATING_MODE, (UINT32)bOpmode );
         return ABCC_NOK;
      }
   }

   else
   {
      switch( bOpmode )
      {
#ifdef ABCC_USER_DRV_SPI
      case ABP_OP_MODE_SPI:
         ABCC_ISR                   = &ABCC_SpiISR;
         ABCC_RunDriver             = &ABCC_SpiRunDriver;

         pnABCC_DrvInit               = &ABCC_DrvSpiInit;
         pnABCC_DrvISR                = NULL;
         pnABCC_DrvRunDriverTx        = &ABCC_DrvSpiRunDriverTx;
         pnABCC_DrvRunDriverRx        = &ABCC_DrvSpiRunDriverRx;
         pnABCC_DrvWriteMessage       = &ABCC_DrvSpiWriteMessage;
         pnABCC_DrvWriteProcessData   = &ABCC_DrvSpiWriteProcessData;
         pnABCC_DrvISReadyForWrPd     = &ABCC_DrvSpiIsReadyForWrPd;
         pnABCC_DrvISReadyForWriteMessage = &ABCC_DrvSpiIsReadyForWriteMessage;
         pnABCC_DrvISReadyForCmd      = &ABCC_DrvSpiIsReadyForCmd;
         pnABCC_DrvSetNbrOfCmds       = &ABCC_DrvSpiSetNbrOfCmds;
         pnABCC_DrvSetAppStatus       = &ABCC_DrvSpiSetAppStatus;
         pnABCC_DrvSetPdSize          = &ABCC_DrvSpiSetPdSize;
         pnABCC_DrvSetMsgReceiverBuffer = &ABCC_DrvSpiSetMsgReceiverBuffer;
         pnABCC_DrvSetIntMask         = &ABCC_DrvSpiSetIntMask;
         pnABCC_DrvGetWrPdBuffer      = &ABCC_DrvSpiGetWrPdBuffer;
         pnABCC_DrvGetModCap          = &ABCC_DrvSpiGetModCap;
         pnABCC_DrvGetLedStatus       = &ABCC_DrvSpiGetLedStatus;
         pnABCC_DrvGetIntStatus       = &ABCC_DrvSpiGetIntStatus;
         pnABCC_DrvGetAnybusState     = &ABCC_DrvSpiGetAnybusState;
         pnABCC_DrvReadProcessData    = &ABCC_DrvSpiReadProcessData;
         pnABCC_DrvReadMessage        = &ABCC_DrvSpiReadMessage;
         pnABCC_DrvIsSupervised       = &ABCC_DrvSpiIsSupervised;
         pnABCC_DrvGetAnbStatus       = &ABCC_DrvSpiGetAnbStatus;
         break;  
#endif
#ifdef ABCC_USER_DRV_PARALLEL
      case ABP_OP_MODE_8_BIT_PARALLEL:
      case ABP_OP_MODE_16_BIT_PARALLEL:
         ABCC_ISR                   = &ABCC_ParISR;/**//**/
         ABCC_RunDriver             = &ABCC_ParRunDriver;/**//**/

         pnABCC_DrvInit               = &ABCC_DrvParInit;
         pnABCC_DrvISR                = &ABCC_DrvParISR;
         pnABCC_DrvRunDriverTx        = NULL;
         pnABCC_DrvRunDriverRx        = &ABCC_DrvParRunDriverRx;
         pnABCC_DrvWriteMessage       = &ABCC_DrvParWriteMessage;
         pnABCC_DrvWriteProcessData   = &ABCC_DrvParWriteProcessData;
         pnABCC_DrvISReadyForWrPd     = &ABCC_DrvParIsReadyForWrPd;
         pnABCC_DrvISReadyForWriteMessage = &ABCC_DrvParIsReadyForWriteMessage;
         pnABCC_DrvISReadyForCmd      = &ABCC_DrvParIsReadyForCmd;
         pnABCC_DrvSetNbrOfCmds       = &ABCC_DrvParSetNbrOfCmds;
         pnABCC_DrvSetAppStatus       = &ABCC_DrvParSetAppStatus;
         pnABCC_DrvSetPdSize          = &ABCC_DrvParSetPdSize;
         pnABCC_DrvSetMsgReceiverBuffer = &ABCC_DrvParSetMsgReceiverBuffer;
         pnABCC_DrvSetIntMask         = &ABCC_DrvParSetIntMask;
         pnABCC_DrvGetWrPdBuffer      = &ABCC_DrvParGetWrPdBuffer;
         pnABCC_DrvGetModCap          = &ABCC_DrvParGetModCap;
         pnABCC_DrvGetLedStatus       = &ABCC_DrvParGetLedStatus;
         pnABCC_DrvGetIntStatus       = &ABCC_DrvParGetIntStatus;
         pnABCC_DrvGetAnybusState     = &ABCC_DrvParGetAnybusState;
         pnABCC_DrvReadProcessData    = &ABCC_DrvParReadProcessData;
         pnABCC_DrvReadMessage        = &ABCC_DrvParReadMessage;
         pnABCC_DrvIsSupervised       = &ABCC_DrvParIsSupervised;
         pnABCC_DrvGetAnbStatus       = &ABCC_DrvParGetAnbStatus;
         break;
#endif
      default:
         ABCC_ERROR(ABCC_SEV_FATAL, ABCC_EC_INCORRECT_OPERATING_MODE, (UINT32)bOpmode );
         return ABCC_NOK;
      }
   } /* End of if( fPingPong )*/   

   if ( !( ( abcc_eMainState == ABCC_DRV_INIT )  ||
           ( abcc_eMainState == ABCC_DRV_SHUTDOWN ) ) )
   {
      ABCC_ERROR(ABCC_SEV_FATAL, ABCC_EC_INCORRECT_STATE, (UINT32)abcc_eMainState );
      abcc_eMainState = ABCC_DRV_ERROR;
      return ABCC_NOK;
   }

   if ( !ABCC_SYS_Init() )
   {
      return ABCC_NOK;
   }

   ABCC_TimerInit();
   pnABCC_DrvInit( bOpmode );

   ABCC_LinkInit();
   ABCC_SetupInit();

   abcc_TmoHandle = ABCC_TimerCreate( ABCC_SetReadyForCommunication );
   
   abcc_pbWrPdBuffer = pnABCC_DrvGetWrPdBuffer();
   if ( !ABCC_SYS_ModuleDetect() )
   {
      ABCC_ERROR(ABCC_SEV_WARNING, ABCC_EC_MODULE_NOT_DECTECTED, 0);
      return ABCC_NOK;
   }

   ABCC_SYS_SetOpmode( bOpmode );

   fAbccReadyForCommunication = FALSE;


      switch( bOpmode )
      {
#ifdef ABCC_USER_INT_ENABLED
      case ABP_OP_MODE_16_BIT_PARALLEL:
      case ABP_OP_MODE_8_BIT_PARALLEL:
      case ABP_OP_MODE_SPI:
         ABCC_SYS_AbccInterruptEnable();
         break;
#endif
      case ABP_OP_MODE_SERIAL_19_2:
      case ABP_OP_MODE_SERIAL_57_6:
      case ABP_OP_MODE_SERIAL_115_2:
      case ABP_OP_MODE_SERIAL_625:
      default:
         ABCC_TimerStart( abcc_TmoHandle, ABCC_USER_STARTUP_TIME_MS );
         break;
      }

   abcc_eMainState = ABCC_DRV_WAIT_COMMUNICATION_RDY;
   abcc_bAnbStatus = 0;
   return ABCC_OK;
}

BOOL ABCC_isReadyForCommunication( void )
{
   ABP_MsgType* psReadMessage;

   if ( abcc_eMainState != ABCC_DRV_WAIT_COMMUNICATION_RDY )
   {
      if ( abcc_eMainState !=  ABCC_DRV_ERROR )
      {
         ABCC_ERROR(ABCC_SEV_WARNING, ABCC_EC_INCORRECT_STATE, 0);
         abcc_eMainState = ABCC_DRV_ERROR;
      }
      return FALSE;
   }

   if( fAbccReadyForCommunication == TRUE )
   {
      pnABCC_DrvSetIntMask( ABCC_USER_INT_ENABLE_MASK );
      abcc_eMainState = ABCC_DRV_SETUP;
      fAbccReadyForCommunication = FALSE;
      psReadMessage = ABCC_LinkAlloc();
      ABCC_ASSERT(psReadMessage != 0);
      pnABCC_DrvSetMsgReceiverBuffer( psReadMessage );
      pnABCC_DrvSetNbrOfCmds( ABCC_USER_MAX_NUM_APPL_CMDS );

      ABCC_SetupCommands();

      return TRUE;
   }
   return FALSE;
}


void ABCC_NewWrPdEvent( void )
{
   if( ABCC_GetMainState() == ABCC_DRV_RUNNING )
   {
      /*
      ** Send new "write process data" to the Anybus-CC.
      ** The data format of the process data is network specific.
      ** Application convert the data accordingly.
      */
      if ( pnABCC_DrvISReadyForWrPd() )
      {
         if( ABCC_CbfUpdateWriteProcessData( abcc_pbWrPdBuffer ) )
         {
            pnABCC_DrvWriteProcessData( abcc_pbWrPdBuffer );
         } 
      }
   }
}


void ABCC_RdPdEvent( void )
{
   UINT8* bpRdPd;

   bpRdPd = pnABCC_DrvReadProcessData();

    if ( bpRdPd )
   {
      if( pnABCC_DrvGetAnybusState() == ABP_ANB_STATE_PROCESS_ACTIVE  )
      {
         /*
         ** The "read process data" is only valid in the PROCESS_ACTIVE state.
         ** Retrieve the new "read process data" from the Anybus-CC.
         ** The data format of the process data is network specific.
         ** Convert it to our native format.
         */
         ABCC_CbfNewReadPd( bpRdPd );
      }
   }
}

void ABCC_RdMsgEvent ( void )
{
   ABP_MsgType* psRdMsg = ABCC_LinkReadMessage();

   if( psRdMsg == NULL )
   {
      return;
   }

   /*
   ** A new message is available.
   */
   if( psRdMsg->sHeader.bCmd & ABP_MSG_HEADER_C_BIT )
   {
      /*
      ** The message is a command, let the application respond.
      */
      ABCC_CbfReceiveMsg( psRdMsg );
   }
   else
   {
      if( abcc_eMainState == ABCC_DRV_SETUP )
      {
         /*
         ** The message is a reponse to a setup command we have sent.
         */
         ABCC_SetupResponses( psRdMsg );
         if ( ABCC_SetupCommands() )
         {
            abcc_eMainState = ABCC_DRV_RUNNING;
         }
      }
      else
      {
         ABCC_MsgHandlerFuncType pnMsgHandler = 0;
         pnMsgHandler = ABCC_LinkGetMsgHandler( psRdMsg->sHeader.bSourceId);
         if( pnMsgHandler )
         {
            DEBUG_EVENT(("Rsp Src Id: %d", psRdMsg->sHeader.bSourceId));
            pnMsgHandler( psRdMsg );
         }
         else
         {
            ABCC_CbfReceiveMsg( psRdMsg );
         }
      } /* end if( message is a command ) */
   }
}

ABCC_MsgStatusType ABCC_SendCmdMsg( ABP_MsgType*  psMsgCmd, ABCC_MsgHandlerFuncType pnMsgHandler )
{
   ABCC_MsgStatusType eMsgStatus;

   eMsgStatus = ABCC_LinkWriteMessage( psMsgCmd );
   if ( ( eMsgStatus == ABCC_MSG_IN_Q ) ||
        ( eMsgStatus == ABCC_MSG_SENT ) )
   {
      /*
      ** Register function to handle response.
      */
      if ( ABCC_LinkMapMsgHandler(psMsgCmd->sHeader.bSourceId, pnMsgHandler ) == ABCC_NOK )
      {
         ABCC_ASSERT( FALSE );
      }
   }
   return eMsgStatus;
}

UINT16 ABCC_GetCmdQueueSize( void )
{
   return ABCC_LinkGetNumCmdQueueEntries();
}


ABCC_MsgStatusType ABCC_SendRespMsg( ABP_MsgType* psMsgResp )
{
   return ABCC_LinkWriteMessage( psMsgResp );
}

ABP_MsgType* ABCC_MsgAlloc( void )
{
   return ABCC_LinkAlloc();
}

ABCC_StatusType ABCC_MsgFree( ABP_MsgType** ppsBuffer )
{
   ABCC_LinkFree( ppsBuffer );
   return ABCC_OK;
}


void ABCC_SetPdSize( const UINT16 iReadPdSize, const UINT16 iWritePdSize )
{
   DEBUG_EVENT((" New process data sizes RdPd %d WrPd %d ", iReadPdSize, iWritePdSize));
   pnABCC_DrvSetPdSize( iReadPdSize, iWritePdSize );
}


void ABCC_HWReset( void )
{
   DEBUG_EVENT((" HW Reset "));
   ABCC_ShutdownDriver();
   ABCC_SYS_HWReset();
}


void ABCC_ShutdownDriver( void )
{
   DEBUG_EVENT((" Enter Shutdown state "));
#ifdef ABCC_USER_INT_ENABLED
   ABCC_SYS_AbccInterruptDisable();
#endif
   ABCC_SYS_Close();
   abcc_eMainState = ABCC_DRV_SHUTDOWN;
}

BOOL ABCC_ModuleDetect( void )
{
   return ABCC_SYS_ModuleDetect();
}

UINT16 ABCC_ModCap( void )
{
   return pnABCC_DrvGetModCap();
}

UINT16 ABCC_LedStatus()
{
   return pnABCC_DrvGetLedStatus();
}

UINT8 ABCC_AnbState( void )
{
   return pnABCC_DrvGetAnybusState();
}

BOOL ABCC_IsSupervised( void )
{
   return pnABCC_DrvIsSupervised();
}

void ABCC_HWReleaseReset( void )
{
   ABCC_SYS_HWReleaseReset();
}

void ABCC_SetAppStatus(  ABP_AppStatusType eAppStatus  )
{
   pnABCC_DrvSetAppStatus( eAppStatus );
}

UINT8 ABCC_ReadModuleId( void )
{
   return ABCC_SYS_ReadModuleId();
}

void ABCC_RunTimerSystem( const INT16 iDeltaTimeMs )
{
   ABCC_TimerTick( iDeltaTimeMs );
}


UINT8 ABCC_GetNewSourceId( void )
{
   static UINT8 bSourceId = 0;
   UINT8 bTempSrcId;
   ABCC_SYS_UseCritical();

   do
   {
      ABCC_SYS_EnterCritical();
      bTempSrcId = ++bSourceId;
      ABCC_SYS_ExitCritical();
   }  while (  ABCC_LinkIsSrcIdUsed( bTempSrcId ) );

   return( bTempSrcId );
}

UINT8 ABCC_GetOpmode( void )
{
   return  ABCC_SYS_GetOpmode();
}


void ABCC_GetAttribute( ABP_MsgType* const psMsg,
                                   UINT8 bObject,
                                   UINT16 iInstance,
                                   UINT8 bAttribute,
                                   UINT8 bSourceId )
{

   psMsg->sHeader.bSourceId = bSourceId;            /* SourceId                */
   psMsg->sHeader.bDestObj = bObject;               /* Object                  */
   psMsg->sHeader.iInstance = iTOiLe ( iInstance );  /* Instance                */
   psMsg->sHeader.bCmd = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;  /* Command, Get attribute. */
   psMsg->sHeader.iDataSize = 0;                    /* Data size               */
   psMsg->sHeader.bCmdExt0 = bAttribute;            /* CmdExt0 (Attribute).    */
   psMsg->sHeader.bCmdExt1 = 0;                     /* CmdExt1 (reserved).     */

}


void ABCC_SetByteAttribute(ABP_MsgType* const psMsg,
                           UINT8 bObject,
                           UINT16 iInstance,
                           UINT8 bAttribute,
                           UINT8 bVal,
                           UINT8 bSourceId )
{
   psMsg->sHeader.bSourceId = bSourceId;            /* SourceId                */
   psMsg->sHeader.bDestObj = bObject;               /* Object                  */
   psMsg->sHeader.iInstance = iTOiLe( iInstance ); /* Instance                */
   psMsg->sHeader.bCmd = ABP_MSG_HEADER_C_BIT | ABP_CMD_SET_ATTR;  /* Command, Set attribute. */
   psMsg->sHeader.iDataSize = 1;                    /* Data size               */
   psMsg->sHeader.bCmdExt0 = bAttribute;            /* CmdExt0 (Attribute)     */
   psMsg->sHeader.bCmdExt1 = 0;                     /* CmdExt1 (reserved)      */
   psMsg->abData[0] = bVal;                         /* Data                    */
}


BOOL ABCC_VerifyMessage( const ABP_MsgType* const psMsg )
{
   return ( ( psMsg->sHeader.bCmd & ABP_MSG_HEADER_E_BIT ) == 0 );
}


void SetErrorReporter( ABCC_ErrorReporter nFunc )
{
   ErrorReporter = nFunc;
}


/*------------------------------------------------------------------------------
** ABCC_GetDataTypeSize()
**------------------------------------------------------------------------------
*/

UINT8 ABCC_GetDataTypeSize( UINT8 bDataType )
{
   UINT8 bSize;

   switch( bDataType )
   {
   case ABP_BOOL:
   case ABP_CHAR:
   case ABP_UINT8:
   case ABP_ENUM:
   case ABP_SINT8:
      bSize = 1;
      break;
   case ABP_UINT16:
   case ABP_SINT16:
      bSize = 2;
      break;
   case ABP_UINT32:
   case ABP_SINT32:
   case ABP_FLOAT:
      bSize = 4;
      break;
   case ABP_UINT64:
   case ABP_SINT64:
      bSize = 8;
      break;
   default:
      while(1); /* Trap */
   }

   return( bSize );
}
