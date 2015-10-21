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
** This is the public header file for the Profinet IO object.
********************************************************************************
********************************************************************************
** Services:
**
** Public Services:
**    PRT_ProcessCmdMsg()     - Processes commands sent to the PRT object.
**
** Private Services:
**    prt_InstanceCommand()   - Processes commands to PRT Instances.
**    prt_ObjectCommand()     - Processes commands to the PRT Object (Instance 0).
********************************************************************************
********************************************************************************
*/
#ifndef PRT_H
#define PRT_H


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** prt_SoftwareVersion.
** Structure describing the Profibus software version.
**------------------------------------------------------------------------------
*/
typedef struct prt_SoftwareRevisionType
{
   UINT8 bSymbol;
   UINT8 baRevision[ 3 ];
}
prt_SoftwareRevisionType;


/*------------------------------------------------------------------------------
** prt_ObjectType.
** Structure describing an ProfiNet Object.
**------------------------------------------------------------------------------
*/
typedef struct prt_ObjectType
{
    const UINT8*  pcName;
    UINT8  bRevision;
    UINT16 iNumberOfInstances;
    UINT16 iHighestInstanceNo;
}
prt_ObjectType;


/*------------------------------------------------------------------------------
** prt_InstanceType.
** Structure describing an ProfiNet Instance.
**------------------------------------------------------------------------------
*/

typedef struct prt_InstanceType
{
   UINT16 iDeviceID;
 /*UINT16 iVendorID;                               */
 /*UINT8* pcStationType;                           */

/* No example code:
   UINT32 lMaxAr;
   UINT8  bRecDataMode;
*/

 /*const  UINT8* pcIMOrdID;                        */
 /*const  UINT8* pcIMSerNr;                        */
 /*UINT16 iIMHwRev;                                */
 /*prt_SoftwareRevisionType sSoftwareRevision;     */

/* No example code:
   UINT16 iIMRevCntr;
   UINT16 iIMProID;
   UINT16 iIMProSpecType;
   UINT16 iIMVer;
   UINT16 iIMSup;
   UINT8  abPort1MACAdd[6];
   UINT8  abPort2MACAdd[6];  
*/

 /*const UINT8* pcSysDescr;

/* No example code:
   const UINT8* pcIntfaceDesc;
   UINT8 bModIDAssMode;
*/

 /*const UINT8* pcSysContact;                      */

/* No example code:
   UINT8  bProfiEnergy;
*/
}
prt_InstanceType;



/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** prt_sObject.
** Structure representing the Profinet object (instance 0).
**------------------------------------------------------------------------------
*/
extern const prt_ObjectType prt_sObject;


/*------------------------------------------------------------------------------
** prt_sInstance
** Structure representing the Profinet instance.
**------------------------------------------------------------------------------
*/
extern prt_InstanceType prt_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void PRT_ProcessCmdMsg()
** Processes commands sent to the Profinet object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       PRT_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
void PRT_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void prt_InstanceCommand()
** Processes commands to PRT Instances.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       prt_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void prt_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** void prt_ObjectCommand()
** Processes commands to the PRT Object (Instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Outputs:
**       None.
**
** Usage:
**       prt_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void prt_ObjectCommand( ABP_MsgType* psNewMessage );


#endif  /* inclusion lock */

/*******************************************************************************
** End of prt.h
********************************************************************************
*/
