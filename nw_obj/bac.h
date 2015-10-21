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
** This is the public header file for the BacNet object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    BAC_ProcessCmdMsg()     - Processes commands sent to the BacNet object.
**
** Private Services:
**    bac_InstanceCommand()   - Processes commands to BacNet Instances.
**    bac_ObjectCommand()     - Processes commands to the BacNet Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef BAC_H
#define BAC_H
#include "abp_bac.h"

/*******************************************************************************
** Defines
********************************************************************************
*/
/*#define ENABLE_BAC_IA_OBJECT_NAME             */                   
#define ENABLE_BAC_IA_VENDOR_NAME                         
/*#define ENABLE_BAC_IA_VENDOR_IDENTIFIER       */            
/*#define ENABLE_BAC_IA_MODEL_NAME              */            
/*#define ENABLE_BAC_IA_FIRMWARE_REVISION       */            
/*#define ENABLE_BAC_IA_APP_SOFTWARE_VERSION    */            
/*#define ENABLE_BAC_IA_SUPPORT_ADV_MAPPING     */           
/*#define ENABLE_BAC_IA_CURRENT_DATE_AND_TIME   */            
/*#define ENABLE_BAC_IA_PASSWORD                */            

/*******************************************************************************
** Typedefs
********************************************************************************
*/


/*------------------------------------------------------------------------------
** bac_ObjectType.
** Structure describing an BacNet Object.
**------------------------------------------------------------------------------
*/
typedef struct bac_ObjectType 
{
   const UINT8* pcName;
   UINT8 bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
} 
bac_ObjectType;


/*------------------------------------------------------------------------------
** bac_InstanceType.
** Structure describing an BacNet Instance.
**------------------------------------------------------------------------------
*/
typedef struct bac_InstanceType 
{
#ifdef ENABLE_BAC_IA_OBJECT_NAME  
   UINT8 abObjectName[ABP_BAC_IA_OBJECT_NAME_MAX_DS + 1];   /* +1 for null termination. */
#endif
#ifdef ENABLE_BAC_IA_VENDOR_NAME
   const UINT8* pcVendorName;
#endif
#ifdef ENABLE_BAC_IA_VENDOR_IDENTIFIER
   UINT16 iVendorIdentifier;
#endif
#ifdef ENABLE_BAC_IA_MODEL_NAME
   const UINT8* pcModelName;
#endif
#ifdef ENABLE_BAC_IA_FIRMWARE_REVISION
   const UINT8* pcFWRevision;
#endif
#ifdef ENABLE_BAC_IA_APP_SOFTWARE_VERSION
   const UINT8* pcApplSWVersion;
#endif
#ifdef ENABLE_BAC_IA_SUPPORT_ADV_MAPPING
   BOOL8 fSupportAdvancedMapping;
#endif
#ifdef ENABLE_BAC_IA_CURRENT_DATE_AND_TIME
   UINT8 abCurrentDateTime[ABP_BAC_IA_CURRENT_DATE_AND_TIME_DS];
#endif
#ifdef ENABLE_BAC_IA_PASSWORD
   const UINT8* pcPassword;
#endif
} 
bac_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** bac_sObject.
** Structure representing the BAC object (instance 0).
**------------------------------------------------------------------------------
*/
extern const bac_ObjectType bac_sObject;


/*------------------------------------------------------------------------------
** bac_sInstance.
** Structure representing the BacNet instance.
**------------------------------------------------------------------------------
*/
extern bac_InstanceType bac_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void BAC_ProcessCmdMsg()
** Processes commands sent to the BAC object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       BAC_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void BAC_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void bac_InstanceCommand()
** Processes commands to the BAC Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       bac_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void bac_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void bac_ObjectCommand()
** Processes commands to the BAC Object (Instance 0).
**------------------------------------------------------------------------------
** Inputs:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Outputs:
**       None.
**
** Usage:
**       bac_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void bac_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of bac.h
********************************************************************************
*/
