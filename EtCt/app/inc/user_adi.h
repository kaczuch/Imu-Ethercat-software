/*
 * user_adi.h
 *
 *  Created on: 11 sie 2015
 *      Author: bkaczor
 */
#include "abcc_user_def.h"
#include "abcc.h"

#ifndef ABCC_USER_MAX_NUM_WRITE_ADI_TO_MAP
#define ABCC_USER_MAX_NUM_WRITE_ADI_TO_MAP           ( 10 )
#endif

#ifndef ABCC_USER_MAX_NUM_READ_ADI_TO_MAP
#define ABCC_USER_MAX_NUM_READ_ADI_TO_MAP            ( 10 )
#endif

#ifndef ETCT_APP_USER_ADI_H_
#define ETCT_APP_USER_ADI_H_

#define ADI_MINIMUM_ACCELERATION -2.731 //g
#define ADI_MAXIMUM_ACCELERATION 2.731 //g
#define	ADI_MINIMUM_ROTATION_SPEED -409.6 //deg/sec
#define	ADI_MAXIMUM_ROTATION_SPEED 409.6 //deg/sec

extern UINT16 iADI_Accel_X;
extern UINT16 iADI_Accel_Y;
extern UINT16 iADI_Accel_Z;

extern UINT16 iADI_Rot_Speed_X;
extern UINT16 iADI_Rot_Speed_Y;
extern UINT16 iADI_Rot_Speed_Z;
extern UINT16 iADI_err;
/*------------------------------------------------------------------------------
** Get Number of available ADI:s
**------------------------------------------------------------------------------
** Arguments:
**
** Returns:
**          Number of ADI:s
**------------------------------------------------------------------------------
*/
const AD_DefaultMapType* ADI_GetDefaultMap( void );


/*------------------------------------------------------------------------------
** Get Number of available ADI:s
**------------------------------------------------------------------------------
** Arguments:
**
** Returns:
**          Number of ADI:s
**------------------------------------------------------------------------------
*/
const AD_AdiEntryType* ADI_GetADIEntryList( void );


/*------------------------------------------------------------------------------
** Get Number of available ADI:s
**------------------------------------------------------------------------------
** Arguments:
**
** Returns:
**          Number of ADI:s
**------------------------------------------------------------------------------
*/
UINT16 AD_GetNumAdi( void  );

#endif /* ETCT_APP_USER_ADI_H_ */
