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
** This is the public header file for the DEV (DeviceNet) object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    DEV_ProcessCmdMsg()     - Processes commands sent to the DEV object.
**
** Private Services:
**    dev_InstanceCommand()   - Processes commands to DEV Instances.
**    dev_ObjectCommand()     - Processes commands to the DEV Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef DEV_H
#define DEV_H

/*******************************************************************************
** Defines
********************************************************************************
*/

#define ENABLE_DEV_IA_VENDOR_ID                
/*#define ENABLE_DEV_IA_DEVICE_TYPE              */
/*#define ENABLE_DEV_IA_PRODUCT_CODE             */
/*#define ENABLE_DEV_IA_REVISION                 */
/*#define ENABLE_DEV_IA_SERIAL_NUMBER            */
/*#define ENABLE_DEV_IA_PRODUCT_NAME             */
/*#define ENABLE_DEV_IA_PROD_INSTANCE            */
/*#define ENABLE_DEV_IA_CONS_INSTANCE            */
/*#define ENABLE_DEV_IA_ADDRESS_FROM_NET         */
/*#define ENABLE_DEV_IA_BAUD_RATE_FROM_NET       */
/*#define ENABLE_DEV_IA_ENABLE_APP_CIP_OBJECTS   */
/*#define ENABLE_DEV_IA_ENABLE_PARAM_OBJECT      */
/*#define ENABLE_DEV_IA_ENABLE_QUICK_CONNECT     */
/*#define ENABLE_DEV_IA_PREPEND_PRODUCING        */
/*#define ENABLE_DEV_IA_PREPEND_CONSUMING        */

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** dev_ObjectType.
** Structure describing an DeviceNet Object.
**------------------------------------------------------------------------------
*/
typedef struct dev_ObjectType 
{
   const UINT8* pcName;
   UINT8 bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
} 
dev_ObjectType;


/*------------------------------------------------------------------------------
** dev_InstanceType.
** Structure describing an DeviceNet Instance.
**------------------------------------------------------------------------------
*/
typedef struct dev_InstanceType 
{
#ifdef ENABLE_DEV_IA_VENDOR_ID
   UINT16 iVendorId;
#endif
#ifdef ENABLE_DEV_IA_DEVICE_TYPE
   UINT16 iDeviceType;
#endif
#ifdef ENABLE_DEV_IA_PRODUCT_CODE
   UINT16 iProductCode;
#endif
#ifdef ENABLE_DEV_IA_REVISION
   UINT8  bMajorRevision;
   UINT8  bMinorRevision;
#endif
#ifdef ENABLE_DEV_IA_SERIAL_NUMBER
   UINT32 lSerialNumber;
#endif
#ifdef ENABLE_DEV_IA_PRODUCT_NAME
   const UINT8* pcProductName;
#endif
#ifdef ENABLE_DEV_IA_PROD_INSTANCE
   UINT16 iProducingInstanceNumber;
#endif
#ifdef ENABLE_DEV_IA_CONS_INSTANCE
   UINT16 iConsumingInstanceNumber;
#endif
#ifdef ENABLE_DEV_IA_ADDRESS_FROM_NET
   BOOL8  fEnableAddressFromNet;
#endif
#ifdef ENABLE_DEV_IA_BAUD_RATE_FROM_NET
   BOOL8  fEnableBaudRateFromNet;
#endif
#ifdef ENABLE_DEV_IA_ENABLE_APP_CIP_OBJECTS
   BOOL8  fEnableCipRequestForwarding;
#endif
#ifdef ENABLE_DEV_IA_ENABLE_PARAM_OBJECT
   BOOL8  fEnableParameterObject;
#endif
#ifdef ENABLE_DEV_IA_ENABLE_QUICK_CONNECT
   BOOL8  fEnableQuickConnect;
#endif
#ifdef ENABLE_DEV_IA_PREPEND_PRODUCING
   UINT16 iPrependProducingInstance;
#endif
#ifdef ENABLE_DEV_IA_PREPEND_CONSUMING
   UINT16 iPrependConsumingInstance;
#endif
} 
dev_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** dev_sObject.
** Structure representing the DEV object (instance 0).
**------------------------------------------------------------------------------
*/
extern const dev_ObjectType dev_sObject;


/*------------------------------------------------------------------------------
** dev_sInstance.
** Structure representing the DEV instance.
**------------------------------------------------------------------------------
*/
extern const dev_InstanceType dev_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void DEV_ProcessCmdMsg()
** Processes commands sent to the DEV object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       DEV_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void DEV_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void dev_InstanceCommand()
** Processes commands to DEV Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       dev_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void dev_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void dev_ObjectCommand()
** Processes commands to the DEV Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       dev_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void dev_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of dev.h
********************************************************************************
*/
