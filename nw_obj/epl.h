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
** This is the public header file for the Ethernet POWERLINK object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    EPL_ProcessCmdMsg()     - Processes commands sent to the EPL object.
**
** Private Services:
**    epl_InstanceCommand()   - Processes commands to EPL Instances.
**    epl_ObjectCommand()     - Processes commands to the EPL Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef EPL_H
#define EPL_H


/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** EPL_MAXIMUM_MANF_DEV_NAME_LENGTH
** Constant for the maximum number of elements allowed in the array (attribute 6).
**------------------------------------------------------------------------------
*/
#define EPL_MAXIMUM_MANF_DEV_NAME_LENGTH  24


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** epl_ObjectType.
** Structure describing an Ethernet POWERLINK Object.
**------------------------------------------------------------------------------
*/
typedef struct epl_ObjectType 
{
  const UINT8* pcName;
  UINT8 bRevision;
  UINT16 iNumberOfInstances;
  UINT16 iHighestInstanceNo;
} 
epl_ObjectType;


/*------------------------------------------------------------------------------
** epl_InstanceType.
** Structure describing an Ethernet POWERLINK Instance.
**------------------------------------------------------------------------------
*/
typedef struct epl_InstanceType 
{
   UINT32 lVendorId;  
   UINT32 lProductCode;
   UINT32 lSerialNumber;
   const UINT8* pcDeviceName;
} 
epl_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** epl_sObject.
** Structure representing the EPL object (instance 0).
**------------------------------------------------------------------------------
*/
extern const epl_ObjectType epl_sObject;


/*------------------------------------------------------------------------------
** epl_sInstance.
** Structure representing the EPL instance.
**------------------------------------------------------------------------------
*/
extern const epl_InstanceType epl_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void EPL_ProcessCmdMsg()
** Processes commands sent to the EPL object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       EPL_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void EPL_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void epl_InstanceCommand()
** Processes commands to the EPL Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       epl_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void epl_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void epl_ObjectCommand()
** Processes commands to the EPL Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       epl_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void epl_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of epl.h
********************************************************************************
*/
