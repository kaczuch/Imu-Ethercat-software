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
** This is the public header file for the CNT (ControlNet) object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    CNT_ProcessCmdMsg()     - Processes commands sent to the CNT object.
**
** Private Services:
**    cnt_InstanceCommand()   - Processes commands to CNT Instances.
**    cnt_ObjectCommand()     - Processes commands to the CNT Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef CNT_H
#define CNT_H

/*******************************************************************************
** Defines
********************************************************************************
*/
#define ENABLE_CNT_IA_VENDOR_ID               
/*#define ENABLE_CNT_IA_DEVICE_TYPE             */
/*#define ENABLE_CNT_IA_PRODUCT_CODE            */
/*#define ENABLE_CNT_IA_REVISION                */
/*#define ENABLE_CNT_IA_SERIAL_NUMBER           */
/*#define ENABLE_CNT_IA_PRODUCT_NAME            */
/*#define ENABLE_CNT_IA_PROD_INSTANCE           */
/*#define ENABLE_CNT_IA_CONS_INSTANCE           */
/*#define ENABLE_CNT_IA_ENABLE_APP_CIP_OBJECTS  */
/*#define ENABLE_CNT_IA_ENABLE_PARAM_OBJECT     */
/*#define ENABLE_CNT_IA_CONFIG_INSTANCE         */

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** cnt_ObjectType.
** Structure describing an ControlNet Object.
**------------------------------------------------------------------------------
*/
typedef struct cnt_ObjectType 
{
   const UINT8* pcName;
   UINT8 bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
} 
cnt_ObjectType;


/*------------------------------------------------------------------------------
** cnt_InstanceType.
** Structure describing an ControlNet Instance.
**------------------------------------------------------------------------------
*/
typedef struct cnt_InstanceType 
{
#ifdef ENABLE_CNT_IA_VENDOR_ID
   UINT16 iVendorId;
#endif
#ifdef ENABLE_CNT_IA_DEVICE_TYPE
   UINT16 iDeviceType;
#endif
#ifdef ENABLE_CNT_IA_PRODUCT_CODE
   UINT16 iProductCode;
#endif
#ifdef ENABLE_CNT_IA_REVISION
   UINT8  bMajorRevision;
   UINT8  bMinorRevision;
#endif
#ifdef ENABLE_CNT_IA_SERIAL_NUMBER
   UINT32 lSerialNumber;
#endif
#ifdef ENABLE_CNT_IA_PRODUCT_NAME
   const UINT8* pcProductName;
#endif
#ifdef ENABLE_CNT_IA_PROD_INSTANCE
   UINT16 iProducingInstanceNumber;
#endif
#ifdef ENABLE_CNT_IA_CONS_INSTANCE
   UINT16 iConsumingInstanceNumber;
#endif
#ifdef ENABLE_CNT_IA_ENABLE_APP_CIP_OBJECTS
   BOOL8  fEnableCipRequestForwarding;
#endif
#ifdef ENABLE_CNT_IA_ENABLE_PARAM_OBJECT
   BOOL8  fEnableParameterObject;
#endif
#ifdef ENABLE_CNT_IA_CONFIG_INSTANCE
   UINT16 iAssemblyObjCfgInstanceNumber; 
#endif
} 
cnt_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** cnt_sObject.
** Structure representing the CNT object (instance 0).
**------------------------------------------------------------------------------
*/
extern const cnt_ObjectType cnt_sObject;


/*------------------------------------------------------------------------------
** cnt_sInstance.
** Structure representing the CNT instance.
**------------------------------------------------------------------------------
*/
extern const cnt_InstanceType cnt_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void CNT_ProcessCmdMsg()
** Processes commands sent to the CNT object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       CNT_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void CNT_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void cnt_InstanceCommand()
** Processes commands to the CNT Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       cnt_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void cnt_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void cnt_ObjectCommand()
** Processes commands to the CNT Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Outputs:
**       None.
**
** Usage:
**       cnt_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void cnt_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of cnt.h
********************************************************************************
*/
