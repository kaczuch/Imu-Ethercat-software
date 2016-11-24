/*
 * user_adi.c
 *
 *  Created on: 11 sie 2015
 *      Author: bkaczor
 */
#include "user_adi.h"
#include "adis.h"

INT16 iADI_Accel_X=1;
INT16 iADI_Accel_Y=2;
INT16 iADI_Accel_Z=3;

INT16 iADI_Rot_Speed_X=4;
INT16 iADI_Rot_Speed_Y=5;
INT16 iADI_Rot_Speed_Z=6;

UINT32 iADI_err=7;
UINT32 sdo1;
UINT32 GTemp[3];
UINT32 GBias[3];
UINT32 AccBias[3];
UINT32 DFilter;
UINT32 DRange;
INT16 Autonull;
INT16 FactoryCall;
INT16 PreciseAutonull;
UINT32 ID;
UINT32 SerialNB;

static AD_UINT16Type sADI_AccelProps =
	{0,0xFFFF,0};
static AD_UINT16Type sADI_Rot_SpeedProps =
	{ 0,0xFFFF,0};
static AD_UINT16Type sADI_RegProps =
	{ 0,0xFFFF,0};
static AD_UINT16Type sADI_RegFilter =
	{ 0,0x07,0};
static AD_UINT16Type sADI_RegRange =
	{ 0,0x04,0};
static AD_BOOL8Type sADI_RegCMD =
	{ 0,0x01,0};

static const AD_AdiEntryType AD_asADIEntryList[] =
{
		{101, "Acceleration X", 	ABP_SINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Accel_X, 		&sADI_AccelProps}},NULL,NULL},
		{102, "Acceleration Y", 	ABP_SINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Accel_Y, 		&sADI_AccelProps}},NULL,NULL},
		{103, "Acceleration Z", 	ABP_SINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Accel_Z, 		&sADI_AccelProps}},NULL,NULL},
		{104, "Rotation Speed X", 	ABP_SINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Rot_Speed_X, 	&sADI_Rot_SpeedProps}},NULL,NULL},
		{105, "Rotation Speed Y", 	ABP_SINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Rot_Speed_Y, 	&sADI_Rot_SpeedProps}},NULL,NULL},
		{106, "Rotation Speed Z", 	ABP_SINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_Rot_Speed_Z, 	&sADI_Rot_SpeedProps}},NULL,NULL},
		{107, "Error code",		 	ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD,	{{&iADI_err, 			&sADI_Rot_SpeedProps}},NULL,NULL},
		// gyro temp
		{508, "Gyro Temperature X",		 					ABP_UINT16, 1,  ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD
																		,	{{&GTemp[0], 			&sADI_RegProps}},&ReadGTemp,NULL},
		{509, "Gyro Temperature Y",		 					ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD
																		,	{{&GTemp[1], 			&sADI_RegProps}},&ReadGTemp,NULL},
		{510, "Gyro Temperature Z",		 					ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD
																		,	{{&GTemp[2], 			&sADI_RegProps}},&ReadGTemp,NULL},
		//gyro bias offset
		{511, "Gyro bias offset X",		 					ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&GBias[0], 			&sADI_RegProps}},&ReadGBias,&WriteGBias},
		{512, "Gyro bias offset Y",		 					ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&GBias[1], 			&sADI_RegProps}},&ReadGBias,&WriteGBias},
		{513, "Gyro bias offset Z",		 					ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&GBias[2], 			&sADI_RegProps}},&ReadGBias,&WriteGBias},
		// acc off factor
		{514, "Accelerometer bias offset X",		 		ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&AccBias[0], 			&sADI_RegProps}},&ReadABias,&WriteABias},
		{515, "Accelerometer bias offset Y",		 		ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&AccBias[1], 			&sADI_RegProps}},&ReadABias,&WriteABias},
		{516, "Accelerometer bias offset Z",		 		ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&AccBias[2], 			&sADI_RegProps}},&ReadABias,&WriteABias},

		//dynamic range and digital filter
		{517, "Digital Filter Settings",		 				ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&DFilter, 			&sADI_RegFilter}},&ReadDFilter,&WriteDFilter},
		{518, "Gyro Range Settings",		 					ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&DRange, 			&sADI_RegRange}},&ReadGyroRange,&WriteGyroRange},
		// global cmd

		{519, "Autonull Gyro Bias",		 						ABP_BOOL, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&Autonull, 			&sADI_RegCMD}},&GlobCmmdGet,&GlobCmmdAutonullSet},
		{520, "Restore Factory Calibration",		 			ABP_BOOL, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&FactoryCall,			&sADI_RegCMD}},&GlobCmmdGet,&GlobCmmdFactoryCalSet},
		{521, "Precision Autonull Bias",		 				ABP_BOOL, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD|
				ABP_APPD_DESCR_SET_ACCESS|ABP_APPD_DESCR_MAPPABLE_WRITE_PD,	{{&PreciseAutonull,	&sADI_RegCMD}},&GlobCmmdGet,&GlobCmmdPrecisionAutonullSet},

		//prod ID
		{522, "Product ID",		 							ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD
																		,	{{&ID, 			&sADI_RegProps}},NULL,NULL},
		//ser Num
		{523, "Serial Number",		 						ABP_UINT16, 1, ABP_APPD_DESCR_GET_ACCESS| ABP_APPD_DESCR_MAPPABLE_READ_PD
																		,	{{&SerialNB, 			&sADI_RegProps}},NULL,NULL},
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
