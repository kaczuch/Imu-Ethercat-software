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
** This is the public header file for the Modbus object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    MOD_ProcessCmdMsg()     - Processes commands sent to the Modbus object.
**
** Private Services:
**    mod_InstanceCommand()   - Processes commands to Modbus Instances.
**    mod_ObjectCommand()     - Processes commands to the Modbus Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef MOD_H
#define MOD_H


/*******************************************************************************
** Constants
********************************************************************************
*/


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** mod_ObjectType.
** Structure describing a Profibus Object.
**------------------------------------------------------------------------------
*/
typedef struct mod_ObjectType
{
    const UINT8*  pcName;
    UINT8  bRevision;
    UINT16 iNumberOfInstances;
    UINT16 iHighestInstanceNo;
}
mod_ObjectType;


/*------------------------------------------------------------------------------
** mod_InstanceType.
** Structure describing a Modbus Instance.
**------------------------------------------------------------------------------
*/


typedef struct mod_InstanceType
{
   const UINT8* pcVendorName;
 /*const UINT8* pcProductCode;                    */
 /*const UINT8* pcMajorMinorRevision;             */
 /*const UINT8* pcVendorURL;                      */
 /*const UINT8* pcProductName;                    */
 /*const UINT8* pcModelName;                      */
 /*const UINT8* pcUserAppName;                    */
 /*const UINT8* pcDeviceID;                       */
 /*UINT8        bNoADIIndexBits;                  */
 /*BOOL8        fEnModMessForward;                */
}
mod_InstanceType;



/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** mod_sObject.
** Structure representing the Modbus object (instance 0).
**------------------------------------------------------------------------------
*/
extern const mod_ObjectType mod_sObject;


/*------------------------------------------------------------------------------
** mod_sInstance.
** Structure representing the Modbus instance.
**------------------------------------------------------------------------------
*/
extern const mod_InstanceType mod_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void MOD_ProcessCmdMsg()
** Processes commands sent to the Modbus object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       MOD_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void MOD_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void mod_InstanceCommand()
** Processes commands to the Modbus Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       mod_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void mod_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void mod_ObjectCommand()
** Processes commands to the Modbus Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       mod_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void mod_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of mod.h
********************************************************************************
*/
