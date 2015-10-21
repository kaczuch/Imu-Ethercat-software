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
** This is the public header file for the EIP (Ethernet/IP) object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    EIP_ProcessCmdMsg()     - Processes commands sent to the EIP object.
**
** Private Services:
**    eip_InstanceCommand()   - Processes commands to EIP Instances.
**    eip_ObjectCommand()     - Processes commands to the EIP Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef EIP_H
#define EIP_H


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** eip_ObjectType.
** Structure describing an Ethernet/IP Object.
**------------------------------------------------------------------------------
*/
typedef struct eip_ObjectType 
{
   const UINT8* pcName;
   UINT8 bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
} 
eip_ObjectType;


/*------------------------------------------------------------------------------
** eip_InstanceMapType.
** Structure describing an EtherNet/IP Instance Map used for attribute 27 and 28.
**------------------------------------------------------------------------------
*/
typedef struct eip_InstanceMapType
{
   UINT16 iInstanceNumber;
   UINT16 iInstanceSize;
}
eip_InstanceMapType;


/*------------------------------------------------------------------------------
** eip_InstanceType.
** Structure describing the EtherNet/IP Instance 1 attributes.
**------------------------------------------------------------------------------
*/
typedef struct eip_InstanceType 
{
   UINT16 iVendorId;                                     /* 1 Vendor ID.                                                   */
/* UINT16 iDeviceType;                                */ /* 2 Device type.                                                 */
/* UINT16 iProductCode;                               */ /* 3 Product code.                                                */
/* UINT8  bMajorRevision;                             */ /* 4a Revision: Major.                                            */
/* UINT8  bMinorRevision;                             */ /* 4b Revision: Minor.                                            */
/* UINT32 lSerialNumber;                              */ /* 5 Serial number.                                               */
/* const  UINT8* pcProductName;                       */ /* 6 Product name.                                                */
/* UINT16 aiProducingInstanceNumber[ 1 ];             */ /* 7 Producing instance number. Array of UINT16 with one element. */
/* UINT16 aiConsumingInstanceNumber[ 1 ];             */ /* 8 Consuming instance number. Array of UINT16 with one element. */
/* BOOL8  fCommSettingsFromNet;                       */ /* 9 Enable communication settings from net.                      */
/* Reserved.                                          */ /* 10 Reserved.                                                   */
/* BOOL8  fEnableCIPRequestForwarding;                */ /* 11 Enable CIP request forwarding.                              */
/* BOOL8  fEnableParameterObject;                     */ /* 12 Enable Parameter object.                                    */
/* UINT16 iInpOnlyHeartBeatInstNumber;                */ /* 13 Input only heartbeat instance number.                       */
/* UINT16 iListOnlyHeartBeatInstNumber;               */ /* 14 Listen only heartbeat instance number.                      */
/* UINT16 iAssObjectConfInstNumber;                   */ /* 15 Assembly object Configuration instance number.              */
/* BOOL8  fDisableStrictIOMatch;                      */ /* 16 Disable Strict IO match.                                    */
/* BOOL8  fEnableUnconnectedRouting;                  */ /* 17 Enable unconnected routing.                                 */
/* UINT16 iInpOnlyExtHeartBeatInstNumber;             */ /* 18 Input only extended heartbeat instance number.              */
/* UINT16 iListOnlyExtHeartBeatInstNumber;            */ /* 19 Listen only extended heartbeat instance number.             */
/* const  UINT8* pcInterfaceLabelPort1;               */ /* 20 Interface label port 1.                                     */
/* const  UINT8* pcInterfaceLabelPort2;               */ /* 21 Interface label port 2.                                     */
/* const  UINT8* pcInterfaceLabelIntPort;             */ /* 22 Interface label internal port.                              */
/* BOOL8  fEnableCIPRequestForwardingExt;             */ /* 23 Enable CIP request forwarding extended.                     */
/* UINT16 iPrependProducingProfileInstance;           */ /* 24 Prepend producing profile instance.    (Not used in ABCC40) */
/* UINT16 iPrependConsumingProfileInstance;           */ /* 25 Prepend consuming profile instance.    (Not used in ABCC40) */
/* BOOL8  fEnableEIPQuickConnect;                     */ /* 26 Enable EtherNet/IP QuickConnect.                            */
/* eip_InstanceMapType asProducingInstanceMap[ 6 ];   */ /* 27 Producing Instance Map.                (Not used in ABCC40) */
/* eip_InstanceMapType asConsumingInstanceMap[ 6 ];   */ /* 28 Consuming Instance Map.                (Not used in ABCC40) */
/* BOOL8  fIgnoreSequenceCountCheck;                  */ /* 29 Ignore Sequence Count Check.                                */
} 
eip_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** eip_sObject.
** Structure representing the EIP object (instance 0).
**------------------------------------------------------------------------------
*/
extern const eip_ObjectType eip_sObject;


/*------------------------------------------------------------------------------
** eip_sInstance.
** Structure representing the EIP instance.
**------------------------------------------------------------------------------
*/

extern const eip_InstanceType eip_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void EIP_ProcessCmdMsg()
** Processes commands sent to the EIP object
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       EIP_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/

void EIP_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void eip_InstanceCommand()
** Processes commands to EIP Instances
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       eip_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void eip_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void eip_ObjectCommand()
** Processes commands to the EIP Object (Instance 0)
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       eip_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void eip_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of eip.h
********************************************************************************
*/
