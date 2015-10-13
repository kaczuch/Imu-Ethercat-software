/*
 * user_setup.c
 *
 *  Created on: 5 sie 2015
 *      Author: bkaczor
 */
#include "abcc_td.h"
#include "abcc_sys_adapt.h" // Ta bort.
#include "abp.h"
#include "abcc.h"
#include "user_setup.h"

/*******************************************************************************
** Private Globals
********************************************************************************
*/
UINT8 us_bSwitch1;
UINT8 us_bSwitch2;
BOOL8 us_fSwitch1Set = FALSE;
BOOL8 us_fSwitch2Set = FALSE;

static void US_ProcessRespMsg( ABP_MsgType* psNewMessage );

typedef enum
{
   US_NW_TYPE,
   US_FIRMWARE,
   US_SERIAL_NO,
   US_DIP_SW1,
   US_DIP_SW2,
   US_DONE,
}US_StateType;

static US_StateType us_eState = US_NW_TYPE;

void US_Init( void )
{
   us_eState = US_NW_TYPE;
}

void US_RunUserInitStateMachine( void )
{
	ABP_MsgType* psMsgCmd;


	switch ( us_eState )
	   {
	   case US_NW_TYPE:

	      /*
	      ** Get the 'network type string' request sent.
	      */
	      psMsgCmd = ABCC_MsgAlloc();
	      ABCC_GetAttribute( psMsgCmd, ABP_OBJ_NUM_NW, 1, ABP_NW_IA_NW_TYPE_STR, US_NW_TYPE_SRC_ID );
	      us_eState = US_FIRMWARE;
	      break;

	   case US_FIRMWARE:

	      /*
	      ** Get the 'firmware version' request sent.
	      */
	      psMsgCmd = ABCC_MsgAlloc();
	      ABCC_GetAttribute( psMsgCmd, ABP_OBJ_NUM_ANB, 1, ABP_ANB_IA_FW_VERSION, US_FIRMWARE_SRC_ID );
	      us_eState = US_SERIAL_NO;
	      break;

	   case US_SERIAL_NO:

	      /*
	      ** Get serial number.
	      */
	      psMsgCmd = ABCC_MsgAlloc();
	      ABCC_GetAttribute( psMsgCmd, ABP_OBJ_NUM_ANB, 1, ABP_ANB_IA_SERIAL_NUM, US_SERIAL_NO_SRC_ID );


	      /*
	      ** Get 'serial number' request sent.
	      ** Next we send the configuration DIP switch values, if available.
	      ** Otherwise we need to send 'setup complete'.
	      */
	      if( us_fSwitch1Set )
	      {
	         us_eState = US_DIP_SW1;
	      }
	      else
	      {
	         us_eState = US_DONE;
	      }
	      break;

	   case US_DIP_SW1:

	      /*
	      ** The configuration DIP switches are available, and
	      ** we start by sending the value of the first switch.
	      */
	      psMsgCmd = ABCC_MsgAlloc();
	      ABCC_SetByteAttribute( psMsgCmd, ABP_OBJ_NUM_NC, 1, ABP_NC_VAR_IA_VALUE, us_bSwitch1, US_DIP_SW1_SRC_ID );

	      /*
	      ** DIP SW1 settings sent.
	      */
	      if( us_fSwitch2Set )
	      {
	         us_eState = US_DIP_SW2;
	      }
	      else
	      {
	         us_eState = US_DONE;
	      }
	      break;

	   case US_DIP_SW2:

	      /*
	      ** Time to send DIP SW2 settings.
	      */
	      psMsgCmd = ABCC_MsgAlloc();
	      ABCC_SetByteAttribute( psMsgCmd, ABP_OBJ_NUM_NC, 2, ABP_NC_VAR_IA_VALUE, us_bSwitch2, US_DIP_SW2_SRC_ID );

	      us_eState = US_DONE;
	      break;

	   case US_DONE:

	      /*
	      ** Nothing more to do.
	      */
	      ABCC_UserInitComplete( TRUE );

	      return;


	   default:

	      while ( 1 );


	   }

	   ABCC_SendCmdMsg(psMsgCmd, US_ProcessRespMsg );
}
static void US_ProcessRespMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** A response to a command sent by APPC has arrived.
   ** We can identify the response by its source ID.
   */
   switch ( psNewMessage->sHeader.bSourceId )
   {
   case US_NW_TYPE_SRC_ID:

      if ( ABCC_VerifyMessage( psNewMessage ) )
      {
         psNewMessage->abData[ psNewMessage->sHeader.iDataSize ] = 0;
         //printf( "\nNetwork type:     %s\n", psNewMessage->abData );
      }
      else
      {
         while(1);
      }
      break;

   case US_FIRMWARE_SRC_ID:

      if ( ABCC_VerifyMessage( psNewMessage ) )
      {
    	  /*
         printf( "\nFirmware version: %d.%.2d build %d\n",
                 psNewMessage->abData[ 0 ] ,
                 psNewMessage->abData[ 1 ] ,
                 psNewMessage->abData[ 2 ] );
                 */
      }
      else
      {
         while(1);
      }

      break;

   case US_SERIAL_NO_SRC_ID:

      if ( ABCC_VerifyMessage( psNewMessage ) )
      {
         /*
    	  printf( "\nSerial number:    %X:%X:%X:%X\n\n",
                 psNewMessage->abData[ 3 ],
                 psNewMessage->abData[ 2 ],
                 psNewMessage->abData[ 1 ],
                 psNewMessage->abData[ 0 ] );
                 */
      }
      else
      {
         while(1);
      }
      break;

   case US_DIP_SW1_SRC_ID:

      /*
      ** Response to the 'Set DIP switch1' command.
      ** This response is normally an ack to the command, but it could be an
      ** error response in case the current network type has no use for the
      ** DIP switch1 value.
      ** Either way, the response is not of interest to us in this application.
      */
      break;

   case US_DIP_SW2_SRC_ID:

      /*
      ** Response to the 'Set DIP switch2' command.
      ** This response is normally an ack to the command, but it could be an
      ** error response in case the current network type has no use for the
      ** DIP switch2 value (Profibus DPV1 for example).
      ** Either way, the response is not of interest to us in this application.
      */
      break;

   case US_ANB_EXC_SRC_ID:

      if ( ABCC_VerifyMessage( psNewMessage ) )
      {
         //printf( "Exception Code:    %X:\n\n", psNewMessage->abData[ 0 ] );
      }
      else
      {
         while(1);
      }
      break;

   case US_NW_EXC_SRC_ID:

      if ( ABCC_VerifyMessage( psNewMessage ) )
      {
         //printf( "Exception Info:    %X:\n\n", psNewMessage->abData[ 0 ] );
      }
      else
      {
         while(1);
      }
      break;

   default:

      while(1);

      break;
   }

   /* Check if more should be done. */
   US_RunUserInitStateMachine();
 }
