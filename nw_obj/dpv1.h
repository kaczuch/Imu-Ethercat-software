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
** This is the public header file for the DPV1 (Profibus) object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    DPV1_ProcessCmdMsg()    - Processes commands sent to the DPV1 object.
**
** Private Services:
**    dpv1_InstanceCommand()  - Processes commands to DPV1 Instances.
**    dpv1_ObjectCommand()    - Processes commands to the DPV1 Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef DPV1_H
#define DPV1_H

#include "abp.h"



/*******************************************************************************
** Constants
********************************************************************************
*/


/*------------------------------------------------------------------------------
** DPV1_PRM_STANDARD_BYTES.
** Size of Profibus DPV0 standard Prm data, according to IEC-61158.
**------------------------------------------------------------------------------
*/
#define DPV1_PRM_STANDARD_BYTES        7


/*------------------------------------------------------------------------------
** DPV1_PRM_DPV1_STATUS_BYTES.
** Size of Profibus DPV1 Status bytes in User Prm data, according to IEC-61158.
**------------------------------------------------------------------------------
*/
#define DPV1_PRM_DPV1_STATUS_BYTES     3


/*------------------------------------------------------------------------------
** DPV1_PRM_APPL_SPECIFIC_BYTES.
** Size of application specific user parameter data.
**------------------------------------------------------------------------------
*/
#define DPV1_PRM_APPL_SPECIFIC_BYTES   0


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** dpv1_ObjectType.
** Structure describing an Profibus Object.
**------------------------------------------------------------------------------
*/
typedef struct dpv1_ObjectType
{
   const  UINT8*  pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
dpv1_ObjectType;


/*------------------------------------------------------------------------------
** dpv1_AlarmSettingsType.
** Structure describing the Profibus alarm settings.
**------------------------------------------------------------------------------
*/
typedef struct dpv1_AlarmSettingsType
{
   UINT8 bSupportedAlarmTypes;
   UINT8 bRequiredAlarmTypes;
   BOOL8 fSequenceModeEnabled;
}
dpv1_AlarmSettingsType;


/*------------------------------------------------------------------------------
** dpv1_SoftwareVersion.
** Structure describing the Profibus software version.
**------------------------------------------------------------------------------
*/
typedef struct dpv1_SoftwareRevisionType
{
   UINT8 bSymbol;
   UINT8 baRevision[ 3 ];
}
dpv1_SoftwareRevisionType;


/*------------------------------------------------------------------------------
** dpv1_IMVersionType.
** Structure describing the Profibus IM version.
**------------------------------------------------------------------------------
*/
typedef struct dpv1_IMVersionType
{
   UINT8 bMajorVersion;
   UINT8 bMinorVersion;
}
dpv1_IMVersionType;


/*------------------------------------------------------------------------------
** dpv1_InstanceType.
** Structure describing an Profibus Instance.
**------------------------------------------------------------------------------
*/
typedef struct dpv1_InstanceType
{
   UINT16 iIdentNumber;                                     /* 1 PNO identification number.                                      */
 /*UINT8  abParamData[ DPV1_PRM_STANDARD_BYTES + 
                       DPV1_PRM_DPV1_STATUS_BYTES + 
                       DPV1_PRM_APPL_SPECIFIC_BYTES ];   */ /* 2 Parameter data.                                                 */
/* UINT8  abExpectedCfgData[ 8 ];                        */ /* 3 Expected configuration. Size depends on application.            */
/* BOOL8  fSSAEnabled;                                   */ /* 4 SSA enabled.                                                    */
/* UINT8  bSizeOfIdRelDiag;                              */ /* 5 Size of identifier related diagnostics.    (Not used in ABCC40) */
/* UINT8  bBufferMode;                                   */ /* 6 Buffer mode.                               (Not used in ABCC40) */
/* dpv1_AlarmSettingsType sAlarmSettings;                */ /* 7 Alarm settings.                            (Not used in ABCC40) */
/* UINT16 iManufacturerID;                               */ /* 8 Manufacturer ID.                                                */
/* const  UINT8* pcOrderID;                              */ /* 9 Order ID.                                                       */
/* const  UINT8* pcSerialNumber;                         */ /* 10 Serial number.                                                 */
/* UINT16 iHardwareRevision;                             */ /* 11 Hardware revision.                                             */
/* dpv1_SoftwareRevisionType sSoftwareRevision;          */ /* 12 Software revision.                                             */
/* UINT16 iRevisionCounter;                              */ /* 13 Revision counter.                                              */
/* UINT16 iProfileID;                                    */ /* 14 Profile ID.                                                    */
/* UINT16 iProfileSpecType;                              */ /* 15 Profile specific type.                                         */
/* dpv1_IMVersionType sIMVersion;                        */ /* 16 IM version.                                                    */
/* UINT16 iIMSupported;                                  */ /* 17 IM supported.                                                  */
/* UINT8  aiIMheader[ 10 ];                              */ /* 18 IM header.                                                     */
/* UINT8  bCheckCfgBehaviour;                            */ /* 19 Check config behaviour.                   (Not used in ABCC40) */
}
dpv1_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** dpv1_sObject.
** Structure representing the DPV1 object (instance 0).
**------------------------------------------------------------------------------
*/
extern const dpv1_ObjectType dpv1_sObject;


/*------------------------------------------------------------------------------
** dpv1_sInstance.
** Structure representing the DPV1 instance.
**------------------------------------------------------------------------------
*/
extern dpv1_InstanceType dpv_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void DPV1_ProcessCmdMsg()
** Processes commands sent to the DPV1 object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       DPV1_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void DPV1_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void dpv1_InstanceCommand()
** Processes commands to DPV1 Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       dpv1_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void dpv1_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void dpv1_ObjectCommand()
** Processes commands to the DPV1 Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       dpv1_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void dpv1_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of dpv1.h
********************************************************************************
*/
