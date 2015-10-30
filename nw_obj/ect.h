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
** This is the public header file for the EtherCat object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    ECT_ProcessCmdMsg()     - Processes commands sent to the ECT object.
**
** Private Services:
**    ect_InstanceCommand()   - Processes commands to ECT Instances.
**    ect_ObjectCommand()     - Processes commands to the ECT Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef ECT_H
#define ECT_H


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ect_ObjectType.
** Structure describing an EtherCat Object.
**------------------------------------------------------------------------------
*/
typedef struct ect_ObjectType 
{
  const UINT8* pcName;
  UINT8 bRevision;
  UINT16 iNumberOfInstances;
  UINT16 iHighestInstanceNo;
} 
ect_ObjectType;


/*------------------------------------------------------------------------------
** ect_InstanceType.
** Structure describing an EtherCat Instance.
**------------------------------------------------------------------------------
*/
typedef struct ect_InstanceType 
{
   UINT32 lVendorId;  
 UINT32 lProductCode;
 UINT16 iMajorRevision;
 UINT16 iMinorRevision;
 UINT32 lSerialNumber;
 const UINT8* pcDeviceName;
 const UINT8* pcHWVersion;
} 
ect_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ect_sObject.
** Structure representing the ECT object (instance 0).
**------------------------------------------------------------------------------
*/
extern const ect_ObjectType ect_sObject;


/*------------------------------------------------------------------------------
** ect_sInstance.
** Structure representing the ECT instance.
**------------------------------------------------------------------------------
*/
extern const ect_InstanceType ect_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void ECT_ProcessCmdMsg()
** Processes commands sent to the ECT object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       ECT_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void ECT_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void ect_InstanceCommand()
** Processes commands to the ECT Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       ect_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void ect_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void ect_ObjectCommand()
** Processes commands to the ECT Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       ect_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void ect_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of ect.h
********************************************************************************
*/
