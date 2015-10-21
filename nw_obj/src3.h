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
** This is the public header file for the SERCOS III object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    SRC3_ProcessCmdMsg()    - Processes commands sent to the SERCOS III object.
**
** Private Services:
**    src3_InstanceCommand()  - Processes commands to SERCOS III Instances.
**    src3_ObjectCommand()    - Processes commands to the SERCOS III Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef SRC3_H
#define SRC3_H


/*******************************************************************************
** Typedefs
********************************************************************************
*/


/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** SRC3 Error Codes.
** Definition of Error Codes translated between SERCOS III and Anybus Compact Com.
**------------------------------------------------------------------------------
*/
#define INVALID_MESSAGE_FORMAT         "X001"
#define UNSUPPORTED_OBJECT             "X001"
#define UNSUPPORTED_iNSTANCE           "X001"
#define UNSUPPORTED_COMMAND            "X001"
#define INVALID_CMDEXT_0               "X001"
#define INVALID_CMDEXT_1               "X001"
#define ATTRIBUTE_NOT_SETTABLE         "X004"
#define ATTRIBUTE_NOT_GETTABLE         "X001"
#define TOO_MUCH_DATA                  "X003"
#define NOT_ENOUGH_DATA                "X002"
#define OUT_OF_RANGE                   "X008"
#define INVALID_STATE                  "X005"
#define OUT_OF_RESOURCES               "X001"
#define SEGMENTATION_FAILURE           "X001"
#define SEGMENTATION_BUFFER_OVERFLOW   "X001"
#define OBJECT_SPECIFIC_ERROR          "X001"


/*------------------------------------------------------------------------------
** src_ObjectType.
** Structure describing an SERCOS III Object.
**------------------------------------------------------------------------------
*/
typedef struct src3_ObjectType 
{
   const UINT8* pcName;
   UINT8 bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
} 
src3_ObjectType;


/*------------------------------------------------------------------------------
** src_InstanceType.
** Structure describing an SERCOS III Instance.
**------------------------------------------------------------------------------
*/
typedef struct src3_InstanceType 
{
   const UINT8* pcComponentName;
/* UINT16 iVendorCode;              */
/* const UINT8* pcDeviceName;       */
/* const UINT8* pcVendorDeviceID;   */
/* const UINT8* pcSWRevision;       */
/* const UINT8* pcSerialNumber;     */
/* BOOL8 fMajorDiagEventLatching;   */
} 
src3_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** src3_sObject.
** Structure representing the SERCOS III object (instance 0).
**------------------------------------------------------------------------------
*/
extern const src3_ObjectType src3_sObject;


/*------------------------------------------------------------------------------
** src3_sInstance.
** Structure representing the SERCOS III instance.
**------------------------------------------------------------------------------
*/
extern const src3_InstanceType src3_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void SRC3_ProcessCmdMsg()
** Processes commands sent to the SRC3 object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       SRC3_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void SRC3_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void src3_InstanceCommand()
** Processes commands to SERCOS III Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       src3_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void src3_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void src3_ObjectCommand()
** Processes commands to the SRC3 Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       src3_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void src3_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of src3.h
********************************************************************************
*/
