/*
 * sync_obj.h
 *
 *  Created on: 29 wrz 2015
 *      Author: bkaczor
 */

#ifndef ETCT_APP_INC_SYNC_OBJ_H_
#define ETCT_APP_INC_SYNC_OBJ_H_
#include "abcc_sys_adapt.h"

typedef struct sync_ObjectType
{
  UINT8* acName;
  UINT8 bRevision;
  UINT16 iNumberOfInstances;
  UINT16 iHighestInstanceNo;
}sync_ObjectType;

typedef struct sync_InstanceType
{
  UINT32 cyclTime;
  UINT32 outValid;
  UINT32 inCapture;
  UINT32 outProces;
  UINT32 inProces;
  UINT32 minCyclTime;
  UINT16 syncMode;
  UINT16 supSyncMode;
}sync_InstanceType;

/*------------------------------------------------------------------------------
** app_sObject.
** Structure representing the APP object (instance 0).
**------------------------------------------------------------------------------
*/
extern const sync_ObjectType sync_sObject;


/*------------------------------------------------------------------------------
** app_sInstance.
** Structure representing the APP instance.
**------------------------------------------------------------------------------
*/
extern sync_InstanceType sync_sInstance;

#endif /* ETCT_APP_INC_SYNC_OBJ_H_ */
