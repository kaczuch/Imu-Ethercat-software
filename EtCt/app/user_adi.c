/*
 * user_adi.c
 *
 *  Created on: 11 sie 2015
 *      Author: bkaczor
 */
#include "user_adi.h"


UINT16 iADI_Accel_X=1;
UINT16 iADI_Accel_Y=2;
UINT16 iADI_Accel_Z=3;

UINT16 iADI_Rot_Speed_X=4;
UINT16 iADI_Rot_Speed_Y=5;
UINT16 iADI_Rot_Speed_Z=6;
UINT16 iADI_err=7;

static AD_UINT16Type sADI_AccelProps =
	{0,0xFFFF,0};
static AD_UINT16Type sADI_Rot_SpeedProps =
	{ 0,0xFFFF,0};


static const AD_AdiEntryType AD_asADIEntryList[] =
{
		{101, "Acceleration X", 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Accel_X, 		&sADI_AccelProps}}},
		{102, "Acceleration Y", 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Accel_Y, 		&sADI_AccelProps}}},
		{103, "Acceleration Z", 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Accel_Z, 		&sADI_AccelProps}}},
		{104, "Rotation Speed X", 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Rot_Speed_X, 	&sADI_Rot_SpeedProps}}},
		{105, "Rotation Speed Y", 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Rot_Speed_Y, 	&sADI_Rot_SpeedProps}}},
		{106, "Rotation Speed Z", 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Rot_Speed_Z, 	&sADI_Rot_SpeedProps}}},
		{107, "Error code",		 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_err, 			&sADI_Rot_SpeedProps}}},
};
static const AD_DefaultMapType AD_asDefaultMap[] = {
		{ 0, PD_WRITE },
        { 1, PD_WRITE },
        { 2, PD_WRITE },
		{ 3, PD_WRITE },
		{ 4, PD_WRITE },
		{ 5, PD_WRITE },
		{ 6, PD_WRITE },
        { 0xFFFF} };

UINT16 AD_GetNumAdi(void  )
{
   return sizeof( AD_asADIEntryList ) / sizeof( AD_AdiEntryType );
}

const AD_DefaultMapType* ADI_GetDefaultMap(void  )
{
   return AD_asDefaultMap;
}

const AD_AdiEntryType* ADI_GetADIEntryList(void  )
{
   return AD_asADIEntryList;
}
