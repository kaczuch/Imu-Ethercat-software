/*
 * app_obj.h
 *
 *  Created on: 11 sie 2015
 *      Author: bkaczor
 */

#ifndef ETCT_INC_APP_OBJ_H_
#define ETCT_INC_APP_OBJ_H_

typedef struct app_ObjectType
{
  UINT8* acName;
  UINT8 bRevision;
  UINT16 iNumberOfInstances;
  UINT16 iHighestInstanceNo;
}
app_ObjectType;


/*------------------------------------------------------------------------------
** app_InstanceType.
** Structure describing an Application Data Instance.
**------------------------------------------------------------------------------
*/
typedef struct app_InstanceType
{
  BOOL8 fConfigured;
  UINT8 bLanguage;
  UINT8 bSupportedLanguages[ 1 ];
  UINT8 bNumberOfSupportedLanguages;
}
app_InstanceType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** app_aacLanguages.
** Array of language strings. One string for each supported language.
**------------------------------------------------------------------------------
*/
extern UINT8* app_aacLanguages[];


/*------------------------------------------------------------------------------
** app_sObject.
** Structure representing the APP object (instance 0).
**------------------------------------------------------------------------------
*/
extern const app_ObjectType app_sObject;


/*------------------------------------------------------------------------------
** app_sInstance.
** Structure representing the APP instance.
**------------------------------------------------------------------------------
*/
extern app_InstanceType app_sInstance;


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** APP_IsResetRequestAllowed()
** Called to check if the requested reset is permitted by the application.
**------------------------------------------------------------------------------
** Arguments:
**       bResetType        - Type of reset, see ABP_RESET_XXX defines.
**
** Returns:
**       BOOL8             - TRUE: Reset request is allowed.
**                           FALSE: Reset request NOT allowed.
**
** Usage:
**       APP_IsResetRequestAllowed();
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL8 APP_IsResetRequestAllowed( UINT8 bResetType );


/*------------------------------------------------------------------------------
** APP_ProcResetRequest()
** Called to perform a reset upon request.
**------------------------------------------------------------------------------
** Arguments:
**       bResetType        - Type of reset, see ABP_RESET_XXX defines.
**
** Returns:
**       None.
**
** Usage:
**       APP_ProcResetRequest();
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_ProcResetRequest( UINT8 bResetType );


/*------------------------------------------------------------------------------
** APP_ProcessCmdMsg()
** The function that processes the commands sent to the APP object.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       APP_ProcessCmdMsg( &sMessage );
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_ProcessCmdMsg( ABP_MsgType* psNewMessage );


/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** void app_InstanceCommand().
** The function that processes the commands to the Application Instance.
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       app_InstanceCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void app_InstanceCommand( ABP_MsgType* psNewMessage );


/*------------------------------------------------------------------------------
** app_ObjectCommand().
** The function that processes the commands to the Application Object (instance 0).
**------------------------------------------------------------------------------
** Arguments:
**       psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**       None.
**
** Usage:
**       app_ObjectCommand( &sMessage );
**------------------------------------------------------------------------------
*/
void app_ObjectCommand( ABP_MsgType* psNewMessage );



#endif /* ETCT_INC_APP_OBJ_H_ */
