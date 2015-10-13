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
** SPI driver implementation
********************************************************************************
********************************************************************************
*/
#include "abcc.h"
#include "abcc_user_def.h"
#ifdef ABCC_USER_DRV_SPI
#include "abcc_td.h"
#include "../abcc_debug_err.h"
#include "abcc_sys_adapt.h"
#include "../abcc_timer.h"
#include "../abcc_drv_if.h"
#include "abcc_crc32.h"
#include "abcc_sys_adapt_spi.h"



/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Help defines.
** Note: They are in words and not bytes. 
**------------------------------------------------------------------------------
*/

#define NUM_BYTES_2_WORDS(x) ( ( (x) + 1 ) >> 1 )


#define SPI_DEFAULT_PD_LEN 0
#define CRC_WORD_LEN_IN_WORDS 2
#define SPI_FRAME_SIZE_EXCLUDING_DATA (7)

#if ABCC_USER_SPI_MSG_FRAG_LEN > ABCC_USER_MAX_MSG_SIZE
#error  spi fragmentation length cannot exceed max msg size
#endif
#define MAX_PAYLOAD_WORD_LEN ( ( NUM_BYTES_2_WORDS( ABCC_USER_SPI_MSG_FRAG_LEN ) ) + ( NUM_BYTES_2_WORDS( ABCC_USER_MAX_PROCESS_DATA_SIZE ) ) + ( CRC_WORD_LEN_IN_WORDS ) )



#define INSERT_SPI_CTRL_CMDCNT( ctrl, cmdcnt)  ctrl = ( ( ctrl ) & ~ABP_SPI_CTRL_CMDCNT) | ( ( cmdcnt ) << 1 )
#define EXTRACT_SPI_STATUS_CMDCNT( status ) ( ( ( status ) & ABP_SPI_STATUS_CMDCNT ) >> 1 )
#define SPI_BASE_FRAME_WORD_LEN  5 /* Frame length excluding MSG and PD data */


/*******************************************************************************
** Typedefs
********************************************************************************
*/



/*------------------------------------------------------------------------------
** SPI MOSI structure (7044 - ABCC40).
**------------------------------------------------------------------------------
*/

typedef struct drv_SpiMosiFrameType
{
   UINT8    bSpiCtrl;
   UINT8    bReserved1;
   UINT16   bMsgLen;
   UINT16   iPdLen;
   UINT8    bAppStatus;
   UINT8    bIntMask;
   UINT16   iData[ MAX_PAYLOAD_WORD_LEN ];
   UINT16   dummy;
} drv_SpiMosiFrameType;


/*------------------------------------------------------------------------------
** SPI MISO structure
**------------------------------------------------------------------------------
*/
typedef struct
{
   UINT16   iReserved1;
   UINT16   iLedStat;
   UINT8    bAnbStat;
   UINT8    bSpiStat;
   UINT16   iNetTime[2];
   UINT16   iData[ MAX_PAYLOAD_WORD_LEN ];
} drv_SpiMisoFrameType;


/*------------------------------------------------------------------------------
** Message fragmentation info types.
**------------------------------------------------------------------------------
*/
typedef struct
{
  ABP_MsgType* psWriteMsg;                /* Pointer to the write message. */
  UINT16*            puCurrPtr;           /* Pointer to the current fragment. */
  INT16              iNumWordsLeft;       /* Number of words left to send. */
  UINT16             iCurrFragLength;     /* Current fragmention block length. */
} drv_SpiWriteMsgFragInfoType;


typedef struct
{
  ABP_MsgType* psReadMsg;                 /* Pointer to the receive message buffer. */
  UINT16*            puCurrPtr;           /* Pointer to the current position in receive buffer. */
  UINT16             iNumWordsReceived;   /* Number of words received. */
} drv_SpiReadMsgFragInfoType;


/*------------------------------------------------------------------------------
** Internal SPI states.
**------------------------------------------------------------------------------
*/
typedef enum
{
   SM_SPI_INIT = 0,
   SM_SPI_RDY_TO_SEND_MOSI,
   SM_SPI_WAITING_FOR_MISO
} drv_SpiStateType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** MISO privates.
**------------------------------------------------------------------------------
*/
static drv_SpiMisoFrameType         spi_drv_sMisoFrame;           /* Place holder for the MISO frame. */
static drv_SpiReadMsgFragInfoType   spi_drv_sReadFragInfo;        /* Read message info. */
static BOOL                         spi_drv_fNewMisoReceived;     /* MISO received flag. */
static UINT8                        spi_drv_bAnbStatus;           /* Latest received anb status. */
static UINT16                       spi_drv_iLedStatus;            /* Latest received anb status. */
static UINT8                        spi_drv_bAnbCmdCnt;            /* Latest correct received cmd count */
static UINT8*                       spi_drv_bpRdPd;                /* Pointer to latest correctly received RdPd */             

static ABP_MsgType*                 spi_drv_psReadMessage;        /* Pointer to the read message. */



/*------------------------------------------------------------------------------
** MOSI privates.
**------------------------------------------------------------------------------
*/
static drv_SpiMosiFrameType         spi_drv_sMosiFrame;           /* Place holder for the MISO frame. */
static drv_SpiWriteMsgFragInfoType  spi_drv_sWriteFragInfo;       /* Write message info. */
static UINT8                        spi_drv_bNbrOfCmds;           /* Number of commands support by the application. */
static UINT8                        spi_drv_bNextAppStatus;       /* Appstatus to be sent in next MOSI frame */
static UINT8                        spi_drv_bNextIntMask;         /* Intmask to be sent in next MOSI frame */


/*------------------------------------------------------------------------------
** General privates.
**------------------------------------------------------------------------------
*/
static UINT16                       spi_drv_iPdOffset;            /* Offset in payload where the PD is located. */
static UINT16                       spi_drv_iCrcOffset;           /* Offset in payload where the CRC is located. */

static UINT16                       spi_drv_iPdSize;              /* PD size in spiFrame. */
static UINT16                       spi_drv_iWritePdSize;         /* Current write PD size. */
static UINT16                       spi_drv_iReadPdSize;          /* Current read PD size. */

static UINT16                       spi_drv_iSpiFrameSize;        /* Current Spi frame size. */
static BOOL                         spi_drv_fRetransmit;          /* Indicate retransmission. */

static drv_SpiStateType             spi_drv_eState;               /* SPI driver state. */
static ABCC_TimerHandle                 xWdTmoHandle;
static BOOL                         fWdTmo;                       /* Current wd timeout status */







/*******************************************************************************
** Private forward declarations.
********************************************************************************
*/
static void				   spi_drv_InsertAppReadyForCmd( void );
static void				   spi_drv_DataReceived( void );
static void				   spi_drv_ResetReadFragInfo( void );
static void				   spi_drv_ResetWriteFragInfo( void );



/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Insert 'ready for command' information into the MOSI frame.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
static void spi_drv_InsertAppReadyForCmd( void )
{
   UINT8 bRdyForCmd = spi_drv_bNbrOfCmds;

   if ( bRdyForCmd > 3 )
   {
      bRdyForCmd = 3;
   }
   INSERT_SPI_CTRL_CMDCNT( spi_drv_sMosiFrame.bSpiCtrl, bRdyForCmd );
}


/*------------------------------------------------------------------------------
**  Handles preparation and transmission of the MOSI frame.
**  Depending on the physical implementation of the SPI transaction this method
**  could be blocking until the MISO is received.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Driver status.
**------------------------------------------------------------------------------
*/
void ABCC_DrvSpiRunDriverTx( void )
{
   UINT32 lCrc;
   BOOL   fHandleWriteMsg = FALSE;
   ABCC_SYS_UseCritical();

   if (spi_drv_eState ==  SM_SPI_RDY_TO_SEND_MOSI )
   {
      spi_drv_eState = SM_SPI_WAITING_FOR_MISO;

      if ( !spi_drv_fRetransmit )
      {
         /*
         ** Everything is OK. Reset retransmission and toggle the T bit.
         */
         spi_drv_sMosiFrame.bSpiCtrl ^= ABP_SPI_CTRL_T;
      }

      spi_drv_fRetransmit = FALSE;

      /*---------------------------------------------------------------------------
      ** Write message handling.
      **---------------------------------------------------------------------------
      */
      ABCC_SYS_EnterCritical();
      if (spi_drv_sWriteFragInfo.psWriteMsg != NULL )
      {
         fHandleWriteMsg = TRUE;
      }
      ABCC_SYS_ExitCritical();

      if ( fHandleWriteMsg )
      {
         ABCC_ASSERT( spi_drv_sWriteFragInfo.puCurrPtr );
         /*
         ** Write the message to be sent.
         */
         spi_drv_sMosiFrame.bSpiCtrl |= ABP_SPI_CTRL_M;

         if( spi_drv_sWriteFragInfo.iNumWordsLeft <= spi_drv_sMosiFrame.bMsgLen )
         {
            spi_drv_sMosiFrame.bSpiCtrl |= ABP_SPI_CTRL_LAST_FRAG;
            spi_drv_sWriteFragInfo.iCurrFragLength = spi_drv_sWriteFragInfo.iNumWordsLeft;
         }
         else
         {
            /*
            ** This is not the last fragment.
            */
            spi_drv_sMosiFrame.bSpiCtrl &= ~ABP_SPI_CTRL_LAST_FRAG;
            spi_drv_sWriteFragInfo.iCurrFragLength = spi_drv_sMosiFrame.bMsgLen;
         }

         /*
         ** Copy the message into the MOSI frame buffer.
         */
         ABCC_SYS_MemCpy( (UINT8*)spi_drv_sMosiFrame.iData,
                     (UINT8*)spi_drv_sWriteFragInfo.puCurrPtr,
                             spi_drv_sWriteFragInfo.iCurrFragLength << 1 );
      }
      else
      {
         /*
         ** There is no message fragment to be sent.
         */
         spi_drv_sMosiFrame.bSpiCtrl &= ~ABP_SPI_CTRL_M;
         spi_drv_sMosiFrame.bSpiCtrl &= ~ABP_SPI_CTRL_LAST_FRAG;
      }

      spi_drv_InsertAppReadyForCmd();

      spi_drv_sMosiFrame.bAppStatus = spi_drv_bNextAppStatus;
      spi_drv_sMosiFrame.bIntMask = spi_drv_bNextIntMask;
      spi_drv_bpRdPd = NULL;


      /*
      ** Apply the CRC checksum.
      */
      lCrc = CRC_Crc32( (UINT8*)&spi_drv_sMosiFrame, spi_drv_iSpiFrameSize*2 - 6 );
      lCrc = lTOlLe( lCrc );
      *(UINT32*)&spi_drv_sMosiFrame.iData[ spi_drv_iCrcOffset ] = lCrc;

      /*
      ** Send the MOSI frame.
      */

      ABCC_SYS_SpiSendReceive( &spi_drv_sMosiFrame, &spi_drv_sMisoFrame, spi_drv_iSpiFrameSize << 1 );
   }
   else if ( spi_drv_eState == SM_SPI_INIT )
   {
      ABCC_TimerStart( xWdTmoHandle, ABCC_USER_WD_TIMEOUT_MS );
      spi_drv_eState = SM_SPI_RDY_TO_SEND_MOSI;
   }
}


/*------------------------------------------------------------------------------
**  Handle the reception of the MISO frame.
**------------------------------------------------------------------------------
** Arguments:
**       psResp:  Pointer to the response message.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
ABP_MsgType* ABCC_DrvSpiRunDriverRx( void )
{
   UINT32 lCrc;
   ABP_MsgType* psWriteMsg = NULL;



   if (spi_drv_eState == SM_SPI_WAITING_FOR_MISO )
   {
      if( !spi_drv_fNewMisoReceived )
      {
         /*
         ** Nothing has happened. No MISO was received.
         */
         return psWriteMsg;
      }
      else
      {
         spi_drv_fNewMisoReceived = FALSE;
      }

      lCrc = CRC_Crc32( (UINT8*)&spi_drv_sMisoFrame, spi_drv_iSpiFrameSize*2 - 4 );
      lCrc = lLeTOl( lCrc );
      if( ( *(UINT32*)&spi_drv_sMisoFrame.iData[ spi_drv_iCrcOffset ] != lCrc ) || 
          ( spi_drv_sReadFragInfo.puCurrPtr == NULL ) )
      {
         /*
         ** We will request a retransmit if the data is corrupt or if we are unable to
         ** handle a msg because lack of resources.
         */
         spi_drv_fRetransmit = TRUE;
         spi_drv_eState = SM_SPI_RDY_TO_SEND_MOSI;
         return NULL;
      }

      /*
      ** Restart watchdog
      */
      if ( fWdTmo )
      {
        ABCC_CbfWdTimeoutRecovered();
      }

      ABCC_TimerStop( xWdTmoHandle );
      fWdTmo = FALSE;
      ABCC_TimerStart( xWdTmoHandle, ABCC_USER_WD_TIMEOUT_MS );


      /*
      ** Save the current anybus status.
      */
      spi_drv_bAnbStatus =  spi_drv_sMisoFrame.bAnbStat;
      spi_drv_iLedStatus  = spi_drv_sMisoFrame.iLedStat;
      spi_drv_bAnbCmdCnt = EXTRACT_SPI_STATUS_CMDCNT( spi_drv_sMisoFrame.bSpiStat );

      if ( spi_drv_sMisoFrame.bSpiStat & ABP_SPI_STATUS_NEW_PD )
      {
         /*
         ** Report the new process data.
         */
         spi_drv_bpRdPd = (UINT8*)&spi_drv_sMisoFrame.iData[ spi_drv_iPdOffset ];
      }

      /*---------------------------------------------------------------------------
      ** Write message handling.
      **---------------------------------------------------------------------------
      */
      if( spi_drv_sWriteFragInfo.iCurrFragLength != 0 )
      {
         /*
         ** Write the message to be sent.
         */
         if ( !( spi_drv_sMisoFrame.bSpiStat & ABP_SPI_STATUS_WRMSG_FULL ) )
         {
            /*
            ** Write message was received.
            ** Update the write fragmentation information.
            */
            spi_drv_sWriteFragInfo.puCurrPtr         += spi_drv_sWriteFragInfo.iCurrFragLength;
            spi_drv_sWriteFragInfo.iNumWordsLeft     -= spi_drv_sWriteFragInfo.iCurrFragLength;
            spi_drv_sWriteFragInfo.iCurrFragLength    = 0;

            if( spi_drv_sWriteFragInfo.iNumWordsLeft <= 0 )
            {
               psWriteMsg = (ABP_MsgType*)spi_drv_sWriteFragInfo.psWriteMsg;

               spi_drv_ResetWriteFragInfo();

               if ( ( psWriteMsg->sHeader.bCmd & ABP_MSG_HEADER_C_BIT ) == 0)
               {
                  spi_drv_bNbrOfCmds++;
               }
            }
         }
      }

      /*---------------------------------------------------------------------------
      ** Read message handling
      ** --------------------------------------------------------------------------
      */
      if( spi_drv_sMisoFrame.bSpiStat & ABP_SPI_STATUS_M )
      {
         /*
         ** Read message was received.
         ** Update the read fragmentation information.
         */

         ABCC_SYS_MemCpy( spi_drv_sReadFragInfo.puCurrPtr,
                     spi_drv_sMisoFrame.iData,
                     spi_drv_sMosiFrame.bMsgLen << 1 );

         spi_drv_sReadFragInfo.puCurrPtr += spi_drv_sMosiFrame.bMsgLen;
         spi_drv_sReadFragInfo.iNumWordsReceived += spi_drv_sMosiFrame.bMsgLen;
         if ( spi_drv_sMisoFrame.bSpiStat & ABP_SPI_STATUS_LAST_FRAG )
         {
            /*
            ** Last fragment of the read message. Return the message.
            ** Update the application flow control.
            */
            if ( spi_drv_sReadFragInfo.psReadMsg->sHeader.bCmd & ABP_MSG_HEADER_C_BIT )
            {
               spi_drv_bNbrOfCmds--;
            }

            spi_drv_psReadMessage = spi_drv_sReadFragInfo.psReadMsg;
            spi_drv_ResetReadFragInfo();
         }
      }

      /*
      ** Clear the valid pd for the next frame.
      */
      spi_drv_sMosiFrame.bSpiCtrl &= ~ABP_SPI_CTRL_WRPD_VALID;

      spi_drv_eState = SM_SPI_RDY_TO_SEND_MOSI;
   }
   else if ( spi_drv_eState == SM_SPI_INIT )
   {
      spi_drv_eState = SM_SPI_RDY_TO_SEND_MOSI;
   }

   return psWriteMsg;
} /* end of spi_drv_HandleMiso() */


/*------------------------------------------------------------------------------
** Callback from the physical layer to indicate that a MISO frame was received.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
static void spi_drv_DataReceived( void )
{
   spi_drv_fNewMisoReceived = TRUE;
}

/*------------------------------------------------------------------------------
** Reset the read fragmentation information.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
static void spi_drv_ResetReadFragInfo( void )
{
   spi_drv_sReadFragInfo.iNumWordsReceived = 0;
   spi_drv_sReadFragInfo.psReadMsg = NULL;
   spi_drv_sReadFragInfo.puCurrPtr = NULL;
}


/*------------------------------------------------------------------------------
** Reset the write fragmentation information.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
static void spi_drv_ResetWriteFragInfo( void )
{
   ABCC_SYS_UseCritical();

   spi_drv_sWriteFragInfo.iNumWordsLeft = 0;
   spi_drv_sWriteFragInfo.iCurrFragLength = 0;
   spi_drv_sWriteFragInfo.puCurrPtr = NULL;

   ABCC_SYS_EnterCritical();
   spi_drv_sWriteFragInfo.psWriteMsg = NULL;
   ABCC_SYS_ExitCritical();
}


/*------------------------------------------------------------------------------
** Watchdog timeouthandler
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
static void drv_WdTimeoutHandler( void )
{
   fWdTmo = TRUE;
   ABCC_CbfWdTimeout();
}



/*******************************************************************************
** Public Services
********************************************************************************
*/
void ABCC_DrvSpiInit( UINT8 bOpmode )
{
   /*
   ** Initialize privates and states.
   */
   ABCC_ASSERT_ERR( bOpmode == 1, ABCC_SEV_FATAL, ABCC_EC_INCORRECT_OPERATING_MODE, (UINT32)bOpmode );

   spi_drv_ResetReadFragInfo();
   spi_drv_fNewMisoReceived = FALSE;
   spi_drv_bAnbStatus = 0;
   spi_drv_psReadMessage = 0;
   spi_drv_ResetWriteFragInfo();
   spi_drv_sMosiFrame.bAppStatus = 0;
   spi_drv_sMosiFrame.bIntMask = 0;
   spi_drv_bNbrOfCmds = 0;
   spi_drv_eState = SM_SPI_INIT;
   spi_drv_iPdSize = SPI_DEFAULT_PD_LEN;
   spi_drv_iWritePdSize = SPI_DEFAULT_PD_LEN;
   spi_drv_iReadPdSize = SPI_DEFAULT_PD_LEN;
   spi_drv_iPdOffset = NUM_BYTES_2_WORDS( ABCC_USER_SPI_MSG_FRAG_LEN );
   spi_drv_iCrcOffset = NUM_BYTES_2_WORDS( ABCC_USER_SPI_MSG_FRAG_LEN ) + SPI_DEFAULT_PD_LEN;
   spi_drv_iSpiFrameSize = SPI_FRAME_SIZE_EXCLUDING_DATA + spi_drv_iCrcOffset;
   spi_drv_fRetransmit = FALSE;
   spi_drv_sMosiFrame.bMsgLen = NUM_BYTES_2_WORDS( ABCC_USER_SPI_MSG_FRAG_LEN );
   spi_drv_sMosiFrame.iPdLen = iTOiLe( spi_drv_iPdSize );
   spi_drv_bNextAppStatus = 0;
   spi_drv_bNextIntMask = 0;
   spi_drv_bpRdPd = NULL;
   spi_drv_bAnbCmdCnt = 0;
   xWdTmoHandle = ABCC_TimerCreate( drv_WdTimeoutHandler );
   fWdTmo = FALSE;

   /*
   ** Register the MISO indicator for the physical SPI driver.
   */
   ABCC_SYS_SpiRegDataReceived( spi_drv_DataReceived );
}



BOOL ABCC_DrvSpiWriteMessage( ABP_MsgType* psWriteMsg )
{
   ABCC_ASSERT( psWriteMsg );
   ABCC_SYS_UseCritical();

   ABCC_SYS_EnterCritical();
   ABCC_ASSERT(spi_drv_sWriteFragInfo.psWriteMsg == NULL );
   spi_drv_sWriteFragInfo.puCurrPtr = (UINT16*)psWriteMsg;
   spi_drv_sWriteFragInfo.iNumWordsLeft = NUM_BYTES_2_WORDS( psWriteMsg->sHeader.iDataSize + sizeof( ABP_MsgHeaderType ) );
   spi_drv_sWriteFragInfo.psWriteMsg = psWriteMsg;
   ABCC_SYS_ExitCritical();

   /*
   ** The SPI driver still owns the buffer.
   */
   return( FALSE );
}


void ABCC_DrvSpiWriteProcessData( UINT8* pbProcessData )
{
   if( spi_drv_eState == SM_SPI_RDY_TO_SEND_MOSI )
   {
      spi_drv_sMosiFrame.bSpiCtrl |= ABP_SPI_CTRL_WRPD_VALID;
   }
   else
   {
      ABCC_ERROR(ABCC_SEV_WARNING, ABCC_EC_SPI_OP_NOT_ALLOWED_DURING_SPI_TRANSACTION,0);
   }
}

BOOL ABCC_DrvSpiIsReadyForWriteMessage( void )
{
   BOOL fRdyForWrMsg = FALSE;
   ABCC_SYS_UseCritical();

   ABCC_SYS_EnterCritical();
   if ( spi_drv_sWriteFragInfo.psWriteMsg == NULL )
   {
      fRdyForWrMsg = TRUE;
   }
   ABCC_SYS_ExitCritical();
   return fRdyForWrMsg;
}


BOOL ABCC_DrvSpiIsReadyForCmd( void )
{
   return ( pnABCC_DrvISReadyForWriteMessage() &&  ( spi_drv_bAnbCmdCnt > 0 ) );
}


void ABCC_DrvSpiSetNbrOfCmds( UINT8 bNbrOfCmds )
{
   spi_drv_bNbrOfCmds = bNbrOfCmds;
}


void ABCC_DrvSpiSetAppStatus( ABP_AppStatusType eAppStatus )
{
   spi_drv_bNextAppStatus = eAppStatus;
}


void ABCC_DrvSpiSetPdSize( const UINT16  iReadPdSize, const UINT16  iWritePdSize)
{
   if( spi_drv_eState == SM_SPI_RDY_TO_SEND_MOSI )
   {   
      /*
      ** Use the largest PD data size since the PD cannot be fragmented.
      */
      spi_drv_iWritePdSize = NUM_BYTES_2_WORDS( iWritePdSize );
      spi_drv_iReadPdSize = NUM_BYTES_2_WORDS( iReadPdSize );

      spi_drv_iPdSize = spi_drv_iWritePdSize;
      if( iReadPdSize > spi_drv_iWritePdSize )
      {
         spi_drv_iPdSize = spi_drv_iReadPdSize;
      }

      /*
      ** Update the CRC position and the total frame size since the process data 
      ** size might have changed.
      */
      spi_drv_iCrcOffset = spi_drv_iPdOffset + spi_drv_iPdSize;
      spi_drv_iSpiFrameSize = SPI_FRAME_SIZE_EXCLUDING_DATA + spi_drv_iCrcOffset;
      spi_drv_sMosiFrame.iPdLen = iTOiLe( spi_drv_iPdSize );
   }
   else
   {
      ABCC_ERROR(ABCC_SEV_WARNING, ABCC_EC_SPI_OP_NOT_ALLOWED_DURING_SPI_TRANSACTION,0);
   }
}

void ABCC_DrvSpiSetMsgReceiverBuffer( ABP_MsgType* const psReadMsg )
{
   if ( spi_drv_sReadFragInfo.puCurrPtr == NULL )
   {
      DEBUG_EVENT(( "SetMsgRcveBuff %d", psReadMsg ));
      spi_drv_sReadFragInfo.psReadMsg = psReadMsg;
      spi_drv_sReadFragInfo.puCurrPtr = (UINT16*)psReadMsg;
      spi_drv_sReadFragInfo.iNumWordsReceived = 0;
   }
   else
   {
      ABCC_ERROR(ABCC_SEV_WARNING, ABCC_EC_SPI_OP_NOT_ALLOWED_DURING_SPI_TRANSACTION,0);
   }
}


UINT16 ABCC_DrvSpiGetIntStatus( void )
{
   ABCC_ERROR(ABCC_SEV_FATAL, ABCC_EC_INTSTATUS_NOT_SUPPORTED_BY_DRV_IMPL, 0);

   return 0xFFFF;
}


UINT8 ABCC_DrvSpiGetAnybusState( void )
{
   return spi_drv_bAnbStatus & 0x7;
}


UINT8* ABCC_DrvSpiReadProcessData( void )
{
   UINT8* pxRdPd = NULL;

   if(  spi_drv_eState == SM_SPI_RDY_TO_SEND_MOSI )
   {
      pxRdPd = spi_drv_bpRdPd;
   }
   return pxRdPd;
}


ABP_MsgType* ABCC_DrvSpiReadMessage( void )
{
   ABP_MsgType* psRdMsg = NULL;

   if(  spi_drv_eState == SM_SPI_RDY_TO_SEND_MOSI )
   {
      if ( spi_drv_psReadMessage != NULL )
      {
         psRdMsg = spi_drv_psReadMessage;
         spi_drv_psReadMessage = NULL;
     }
   }
   return psRdMsg;
}

void ABCC_DrvSpiSetIntMask( const UINT16 iIntMask )
{
   spi_drv_bNextIntMask = (UINT8)iIntMask;
}


UINT8* ABCC_DrvSpiGetWrPdBuffer( void )
{
   return (UINT8*)&spi_drv_sMosiFrame.iData[ spi_drv_iPdOffset ];
}


UINT16 ABCC_DrvSpiGetModCap( void )
{
   ABCC_ERROR( ABCC_SEV_WARNING , ABCC_EC_MODCAP_NOT_SUPPORTED_BY_DRV_IMPL, 0);
   return 0;
}

UINT16 ABCC_DrvSpiGetLedStatus( void )
{
   return spi_drv_iLedStatus;
}

BOOL ABCC_DrvSpiIsReadyForWrPd( void )
{
   if ( spi_drv_eState == SM_SPI_RDY_TO_SEND_MOSI )
   {
      return TRUE;
   }
   return FALSE;
}


BOOL ABCC_DrvSpiIsSupervised( void )
{
   /*
   ** The Anybus supervision bis is stored in bit 3
   */
   return  ( spi_drv_bAnbStatus  >> 3 ) & 1;
}

UINT8 ABCC_DrvSpiGetAnbStatus( void )
{
   return (UINT8)spi_drv_bAnbStatus & 0xf;
}


#endif





/*******************************************************************************
** End of spi_drv.c
********************************************************************************
*/
