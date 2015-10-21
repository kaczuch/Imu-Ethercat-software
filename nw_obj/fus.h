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
** This is the public header file for the Functional Safety Object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    FUS_ProcessCmdMsg()    - Processes commands sent to the Functional Safety Object.
**
** Private Services:
**    fus_InstanceCommand()  - Processes commands to Functional Safety Instances.
**    fus_ObjectCommand()    - Processes commands to the Functional Safety Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef FUS_H
#define FUS_H

#include "abp.h"


/*******************************************************************************
** Defines
********************************************************************************
*/




/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** fus_ObjectType.
** Structure describing an Profibus Object.
**------------------------------------------------------------------------------
*/
typedef struct fus_ObjectType
{
   const  UINT8*  pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
fus_ObjectType;



/*------------------------------------------------------------------------------
** fus_InstanceType.
** Structure describing an Profibus Instance.
**------------------------------------------------------------------------------
*/
typedef struct fus_InstanceType
{
   BOOL8 bSafetyEnabled;                 /* 1 Safety Enabled.       */                      
}
fus_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** fus_sObject.
** Structure representing the Functional Safety Object (instance 0).
**------------------------------------------------------------------------------
*/
extern const fus_ObjectType fus_sObject;


/*------------------------------------------------------------------------------
** fus_sInstance.
** Structure representing the Functional Safety instance.
**------------------------------------------------------------------------------
*/
extern fus_InstanceType fus_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void FUS_ProcessCmdMsg()
** Processes commands sent to the Functional Safety Object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       FUS_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void FUS_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void fus_InstanceCommand()
** Processes commands to FUS Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       fus_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void fus_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void fus_ObjectCommand()
** Processes commands to the Functional Safety Object(Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       fus_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void fus_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of fus.h
********************************************************************************
*/
