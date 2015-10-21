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
** This is the public header file for the CANopen object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    COP_ProcessCmdMsg()     - Processes commands sent to the COP object.
**
** Private Services:
**    cop_InstanceCommand()   - Processes commands to COP Instances.
**    cop_ObjectCommand()     - Processes commands to the COP Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef COP_H
#define COP_H

/*******************************************************************************
** Defines
********************************************************************************
*/
#define ENABLE_COP_IA_VENDOR_ID              
/*#define ENABLE_COP_IA_PRODUCT_CODE            */
/*#define ENABLE_COP_IA_REVISON                 */       
/*#define ENABLE_COP_IA_SERIAL_NUMBER           */
/*#define ENABLE_COP_IA_MANF_DEV_NAME           */
/*#define ENABLE_COP_IA_MANF_HW_VER             */
/*#define ENABLE_COP_IA_MANF_SW_VER             */

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** cop_ObjectType.
** Structure describing an CANopen Object.
**------------------------------------------------------------------------------
*/
typedef struct cop_ObjectType 
{
   const UINT8* pcName;
   UINT8 bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
} 
cop_ObjectType;


/*------------------------------------------------------------------------------
** cop_InstanceType.
** Structure describing an CANopen Instance.
**------------------------------------------------------------------------------
*/
typedef struct cop_InstanceType 
{
#ifdef ENABLE_COP_IA_VENDOR_ID
   UINT32 iVendorId;  
#endif
#ifdef ENABLE_COP_IA_PRODUCT_CODE
   UINT32 iProductCode;
#endif
#ifdef ENABLE_COP_IA_REVISON
   UINT16 bMajorRevision;
   UINT16 bMinorRevision;
#endif
#ifdef ENABLE_COP_IA_SERIAL_NUMBER
   UINT32 lSerialNumber;
#endif
#ifdef ENABLE_COP_IA_MANF_DEV_NAME
   const UINT8* pcManDeviceName;
#endif
#ifdef ENABLE_COP_IA_MANF_HW_VER
   const UINT8* pcManHWVersion;
#endif
#ifdef ENABLE_COP_IA_MANF_SW_VER
   const UINT8* pcManSWVersion;
#endif
} 
cop_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** cop_sObject.
** Structure representing the COP object (instance 0).
**------------------------------------------------------------------------------
*/
extern const cop_ObjectType cop_sObject;


/*------------------------------------------------------------------------------
** cop_sInstance.
** Structure representing the COP instance.
**------------------------------------------------------------------------------
*/
extern const cop_InstanceType cop_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void COP_ProcessCmdMsg()
** Processes commands sent to the COP object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       COP_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void COP_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void cop_InstanceCommand()
** Processes commands to the COP Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       cop_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void cop_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void cop_ObjectCommand()
** Processes commands to the COP Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       cop_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void cop_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of cop.h
********************************************************************************
*/
