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
** Parallel (PARI) driver implementation.
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#ifdef ABCC_USER_DRV_PARALLEL
#include "abcc_td.h"
#include "../abcc_debug_err.h"
#include "abcc_sys_adapt.h"
#include "../abcc_timer.h"
#include "../abcc_drv_if.h"
#include "abp.h"
#include "abcc_sys_adapt_par.h"



/*******************************************************************************
** Constants
********************************************************************************
*/


/*******************************************************************************
** Typedefs
********************************************************************************
*/


/*******************************************************************************
** Private Globals
********************************************************************************
*/
static ABP_MsgType* par_drv_pbReadMessageData;
static  UINT8* par_drv_pbRdPdBuffer;

static UINT16   par_drv_iSizeOfReadPd;
static UINT16   par_drv_iSizeOfWritePd;

static   UINT8    par_drv_bNbrOfCmds;          /* Number of commands supported by the application. */

static const    UINT16   iWRPDFlag     = 0x01;
static const    UINT16   iRDPDFlag     = 0x02;
static const    UINT16   iWRMSGFlag    = 0x04;
static const    UINT16   iRDMSGFlag    = 0x08;
static const    UINT16   iANBRFlag     = 0x10;
static const    UINT16   iAPPRFlag     = 0x20;
static const    UINT16   iAPPRCLRFlag  = 0x40;

static const    UINT16   iRDPDIFlag    = 0x01;
static const    UINT16   iRDMSGIFlag   = 0x02;
static const    UINT16   iWRMSGIFlag   = 0x04;
static const    UINT16   iANBRIFlag    = 0x08;
static const    UINT16   iSTATUSIFlag  = 0x10;
static const    UINT16   iPWRIFlag     = 0x20;
static const    UINT16   iSYNCIFlag    = 0x40;


/*******************************************************************************
** Private forward declarations
********************************************************************************
*/

/*******************************************************************************
** Private Services
********************************************************************************
*/


/*******************************************************************************
** Public Services
********************************************************************************
*/
void ABCC_DrvParInit( UINT8 bOpmode )
{
   /*
   ** Initialize privates and states.
   */

   ABCC_ASSERT_ERR( ( bOpmode == 7) || ( bOpmode == 8) , ABCC_SEV_FATAL, ABCC_EC_INCORRECT_OPERATING_MODE, (UINT32)bOpmode );

   par_drv_iSizeOfReadPd  = 0;
   par_drv_iSizeOfWritePd = 0;
   par_drv_bNbrOfCmds     = 0;
   par_drv_pbRdPdBuffer = ABCC_SYS_ParallelGetRdPdBuffer();
}


void ABCC_DrvParSetIntMask( const UINT16 iIntMask )
{
   ABCC_SYS_WRITE16( ABP_INTMASK_ADR_OFFSET, iTOiLe( iIntMask ) ) ;
}

#ifdef ABCC_USER_INT_ENABLED
UINT16 ABCC_DrvParISR( void )
{
   UINT16 iIntStatus;
   UINT16 iIntToHandle;

   /*---------------------------------------------------------------------------
   ** Read the interrupt status register and acknowledge all interrupts.
   ** Read interrupt status until all enabled interrupts are acknowledged.
   ** This will make sure that edge triggered interrupt always will trigger
   ** even if a new event has occurred between the int status read the
   ** acknowledge.
   **---------------------------------------------------------------------------
   */
   iIntStatus = ABCC_SYS_READ16( ABP_INTSTATUS_ADR_OFFSET );
   ABCC_SYS_WRITE16( ABP_INTSTATUS_ADR_OFFSET, iIntStatus );
   iIntStatus = ( iLeTOi( iIntStatus ) ) & ABCC_USER_INT_ENABLE_MASK;
   iIntToHandle = iIntStatus;

   while( iIntStatus != 0 )
   {
    iIntStatus = ABCC_SYS_READ16( ABP_INTSTATUS_ADR_OFFSET );
    ABCC_SYS_WRITE16( ABP_INTSTATUS_ADR_OFFSET, iIntStatus );
    iIntStatus = ( iLeTOi( iIntStatus ) ) & ABCC_USER_INT_ENABLE_MASK;
    iIntToHandle |= iIntStatus;
   }

   return iIntToHandle;
}
#else
UINT16 ABCC_DrvParISR( void )
{
   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_INTERNAL_ERROR, 0);
   return 0;
}
#endif



ABP_MsgType* ABCC_DrvParRunDriverRx( void )
{
   /*
   ** Always NULL for the parallel interface.
   */
   return NULL;
}


BOOL ABCC_DrvParWriteMessage( ABP_MsgType* psWriteMsg )
{
   UINT16 iBufControlWriteFlags;

   iBufControlWriteFlags = 0;

   ABCC_ASSERT( psWriteMsg );

   /*
   ** Write message data.
   **
   ** Note: The application is only allowed to write to the write message 
   ** area when the WRMSG bit it set to 0.
   ** However, the !( iBufControl & iWRMSGFlag ) check should have
   ** already been performed inside link.c, and is thus not required here.
   */
   ABCC_SYS_ParallelWrite( ABP_WRMSG_ADR_OFFSET, (UINT8*)psWriteMsg, sizeof( ABP_MsgHeaderType ) + psWriteMsg->sHeader.iDataSize );

   iBufControlWriteFlags |= iWRMSGFlag;

   /*
   ** Determine if command messages (instead of response messages) can be sent.
   */
   if( !( psWriteMsg->sHeader.bCmd & ABP_MSG_HEADER_C_BIT ) )
   {
      /*
      ** A command message has been received by the host application and a
      ** response message (not a command message) will be sent back to the
      ** Anybus. The number of commands the host application can receive
      ** shall be increased by one, as a previous received command is now
      ** handled.
      */
      par_drv_bNbrOfCmds++;

      /*
      ** When a change of the number of commands which the host application 
      ** can receive is made from 0 to 1, it means that we can set the APPRF 
      ** flag again to indicate for the Anybus that the host is now ready to 
      ** receive a new command.
      */
      if( par_drv_bNbrOfCmds == 1 )
      {
         iBufControlWriteFlags |= iAPPRFlag;
      }
   }
   /*
   ** Update the buffer control register.
   */

   ABCC_SYS_WRITE16( ABP_BUFCTRL_ADR_OFFSET, iTOiLe( iBufControlWriteFlags ) );
   return( TRUE );
}


void ABCC_DrvParWriteProcessData( UINT8* pbProcessData )
{

   if( par_drv_iSizeOfWritePd )
   {
      /*
      ** Write process data.
      */
      ABCC_SYS_WRITE_WRPD( pbProcessData, par_drv_iSizeOfWritePd );

      /*
      ** Update the buffer control register.
      */
      ABCC_SYS_WRITE16( ABP_BUFCTRL_ADR_OFFSET, iTOiLe( iWRPDFlag ) );
   }
}


BOOL ABCC_DrvParIsReadyForWriteMessage( void )
{
   UINT16 iBufControl;
   iBufControl = ABCC_SYS_READ16( ABP_BUFCTRL_ADR_OFFSET );

   return( !( iLeTOi( iBufControl ) & iWRMSGFlag ) );
}


BOOL ABCC_DrvParIsReadyForCmd( void )
{
   UINT16 iBufControl;
   iBufControl = ABCC_SYS_READ16( ABP_BUFCTRL_ADR_OFFSET );
   iBufControl = iLeTOi( iBufControl );
   return( !( iBufControl & iWRMSGFlag ) && ( iBufControl & iANBRFlag ) ); 
}


void ABCC_DrvParSetNbrOfCmds( UINT8 bNbrOfCmds )
{
   par_drv_bNbrOfCmds = bNbrOfCmds;

   /*
   ** Acknowledge that we are ready to accept the first command message.
   */
   ABCC_SYS_WRITE16( ABP_BUFCTRL_ADR_OFFSET, iTOiLe( iAPPRFlag ) );
}


void ABCC_DrvParSetAppStatus( ABP_AppStatusType eAppStatus )
{
   ABCC_SYS_WRITE16( ABP_APPSTATUS_ADR_OFFSET, iTOiLe( (UINT16)eAppStatus ) );
}


void ABCC_DrvParSetPdSize( const UINT16 iReadPdSize, const UINT16 iWritePdSize )
{
   par_drv_iSizeOfReadPd = iReadPdSize;
   par_drv_iSizeOfWritePd = iWritePdSize;
}


void ABCC_DrvParSetMsgReceiverBuffer( ABP_MsgType* const psReadMsg )
{
   par_drv_pbReadMessageData = psReadMsg;

   if( psReadMsg )
   {
      /*
      ** Now we are ready to receive a new message.
      ** Update the buffer control register.
      */
      ABCC_SYS_WRITE16( ABP_BUFCTRL_ADR_OFFSET, iTOiLe( iRDMSGFlag )  );
   }
}


UINT16 ABCC_DrvParGetIntStatus( void )
{
   UINT16 iIntStatus;

   iIntStatus = ABCC_SYS_READ16( ABP_INTSTATUS_ADR_OFFSET );

   return  iLeTOi( iIntStatus );
}

UINT8 ABCC_DrvParGetAnybusState( void )
{
   UINT16 iAnbStatus;

   /*
   ** Reading out the Anybus status.
   */
   iAnbStatus = ABCC_SYS_READ16( ABP_ANBSTATUS_ADR_OFFSET );

   /*
   ** The Anybus state is stored in bits 0-2 of the read register.
   */
   return( (UINT8)( iLeTOi( iAnbStatus ) & 0x07 ) );
}


UINT8* ABCC_DrvParReadProcessData( void )
{
   UINT16 iBufctrl;

   /*
   ** Check if the Anybus has updated the read process data.
   */
   iBufctrl = ABCC_SYS_READ16( ABP_BUFCTRL_ADR_OFFSET );

   if( iLeTOi( iBufctrl ) & iRDPDFlag  )
   {
      /*
      ** The RDPD flag must be set before we try to read the process data.
      ** Otherwise the buffers won't be switched and we won't have any process
      ** data available.
      */
      ABCC_SYS_WRITE16( ABP_BUFCTRL_ADR_OFFSET, iTOiLe( iRDPDFlag ) );

      /*
      ** We have process data to read.
      */
      ABCC_SYS_READ_RDPD( par_drv_pbRdPdBuffer, par_drv_iSizeOfReadPd );

      return( par_drv_pbRdPdBuffer );
   }
   else
   {
      return NULL;
   }
}


ABP_MsgType* ABCC_DrvParReadMessage( void )
{
   UINT16 iBufctrl;

   if( par_drv_pbReadMessageData == NULL )
   {
      /* 
      ** There is currently no msg buffer allocated to recive a new msg.
      ** This shall resolve itself when the application have handeld a recived
      ** msg or the driver managed to send a msg to the ABCC. 
      ** ABCC_LinkFree() will call pnABCC_DrvSetMsgReceiverBuffer().
      ** NOTE!! We can't trust the iRDMSGFlag now since it have not been cleared
      ** by pnABCC_DrvSetMsgReceiverBuffer() yet.
      */
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_OUT_OF_MSG_BUFFERS, 0 );
      return( NULL );
   }

   iBufctrl = ABCC_SYS_READ16( ABP_BUFCTRL_ADR_OFFSET );

   if( iLeTOi( iBufctrl ) & iRDMSGFlag  )
   {

      /* 
      ** We have message data to read. First read the header and check the size 
      ** of data area.
      */      
      ABCC_SYS_ParallelRead( ABP_RDMSG_ADR_OFFSET, (UINT8*)par_drv_pbReadMessageData, sizeof(ABP_MsgHeaderType) );

      /* 
      ** Continue reading data area if > 0.
      */ 
      if ( par_drv_pbReadMessageData->sHeader.iDataSize > 0 )
      {
         ABCC_SYS_ParallelRead( ABP_RDMSG_ADR_OFFSET + sizeof(ABP_MsgHeaderType) , par_drv_pbReadMessageData->abData, par_drv_pbReadMessageData->sHeader.iDataSize);
      }

      /*
      ** Determine if command messages (instead of response messages) can be read.
      */
      if( par_drv_pbReadMessageData->sHeader.bCmd & ABP_MSG_HEADER_C_BIT )
      {
         /*
         ** A command messages has been sent by the Anybus and it has been read
         ** by the host application. The number of commands allowed by the host
         ** application must be decreased by one.
         */
         par_drv_bNbrOfCmds--;

         /*
         ** Indicates that the host application is not ready to receive a new
         ** command from the Anybus. Writing to this register must only be done 
         ** when the RDMSG bit is set to 1. A check is not required however,
         ** since the RDMSG bit is set to 1 a few lines higher up in the code.
         */
         if( par_drv_bNbrOfCmds == 0 )
         {
            /*
            ** Update the buffer control register.
            */
            ABCC_SYS_WRITE16( ABP_BUFCTRL_ADR_OFFSET, iTOiLe( iAPPRCLRFlag ) );
         }
      }

      return( par_drv_pbReadMessageData );
   }
   else
   {
      return( NULL );
   }
}

UINT8* ABCC_DrvParGetWrPdBuffer( void )
{
   return ABCC_SYS_ParallelGetWrPdBuffer();
}

UINT16 ABCC_DrvParGetModCap( void )
{
   UINT16 iModCap;
   iModCap = ABCC_SYS_READ16( ABP_MODCAP_ADR_OFFSET );
   return iLeTOi( iModCap );
}


UINT16 ABCC_DrvParGetLedStatus( void )
{
   UINT16 iLedStatus;
   iLedStatus = ABCC_SYS_READ16( ABP_LEDSTATUS_ADR_OFFSET );
   return iLeTOi( iLedStatus );
}

BOOL ABCC_DrvParIsReadyForWrPd( void )
{
   return TRUE;
}


BOOL ABCC_DrvParIsSupervised( void )
{
   UINT16 iAnbStatus;
   /*
   ** Reading out the Anybus status.
   */
   iAnbStatus = ABCC_SYS_READ16( ABP_ANBSTATUS_ADR_OFFSET );
   iAnbStatus = iLeTOi( iAnbStatus );

   /*
   ** The Anybus supervision bis is stored in bit 3
   */
   return( ( iAnbStatus >> 3 ) & 1 );

}

UINT8 ABCC_DrvParGetAnbStatus( void )
{
   UINT16 iAnbStatus;
   /*
   ** Reading out the Anybus status.
   */
   iAnbStatus = ABCC_SYS_READ16( ABP_ANBSTATUS_ADR_OFFSET );
   iAnbStatus = iLeTOi( iAnbStatus );
   return (UINT8)iAnbStatus & 0xf ;

}
#endif



/*******************************************************************************
** End of par_drv.c
********************************************************************************
*/
