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
** Interface for driver internal interface to the abcc_handler
********************************************************************************
********************************************************************************
** Services:
** ABCC_SetReadyForCommunication()  : Indicate anybus ready for communication
** ABCC_SetMainStateError()         : Set driver in ABCC_ERROR state
** ABCC_GetMainState()              : Get current state

********************************************************************************
********************************************************************************
*/

#ifndef ABCC_HANDLER_H_
#define ABCC_HANDLER_H_
#include "abcc_user_def.h"
/*******************************************************************************
** Constants
********************************************************************************
*/

/*
 ** Set default interrupt mask if not defined in abcc_user_def.h
 */
#ifndef ABCC_USER_INT_ENABLE_MASK
#define ABCC_USER_INT_ENABLE_MASK              ( 0 )
#endif

/*******************************************************************************
** Typedefs
********************************************************************************
*/
/*
** Type for ABCC main states
*/
typedef enum ABCC_MainStateType
{
   ABCC_DRV_INIT,
   ABCC_DRV_SHUTDOWN,
   ABCC_DRV_ERROR,
   ABCC_DRV_WAIT_COMMUNICATION_RDY,
   ABCC_DRV_SETUP,
   ABCC_DRV_RUNNING
} ABCC_MainStateType;


/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/


/*------------------------------------------------------------------------------
** ABCC_SetPdSize()
** Sets the new process data sizes.
**------------------------------------------------------------------------------
** Arguments:
**       iReadPdSize       - Size of the read process data (in bytes), used from
**                           this point on.
**       iWritePdSize      - Size of the write process data (in bytes), used from
**                           this point on.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetPdSize( const UINT16 iReadPdSize, const UINT16 iWritePdSize );


/*------------------------------------------------------------------------------
** The anybus is ready for communication. This function shall be called eithjer 
** due to power up interrupt or initial handske timeout
**------------------------------------------------------------------------------
** Arguments: 
**       None.
**
** Returns: 
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetReadyForCommunication( void );

/*------------------------------------------------------------------------------
** Set main state machine into error state. This will stop ABCC_ISR()
** and ABCC_RunDriver to perform any action towards application or anybus
**------------------------------------------------------------------------------
** Arguments: 
**       None.
**
** Returns: 
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetMainStateError( void );

/*------------------------------------------------------------------------------
** Gets currents state
**------------------------------------------------------------------------------
** Arguments: 
**       None.
**
** Returns: 
**       Current state ( ABCC_MainStateType )
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_MainStateType ABCC_GetMainState( void );



#endif  /* inclusion lock */
