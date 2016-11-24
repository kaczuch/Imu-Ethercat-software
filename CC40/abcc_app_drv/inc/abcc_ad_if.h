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
** Typedefs provided by ABCC_driver, used to specify ADI mapping tables.
********************************************************************************
********************************************************************************
** Services:
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_AD_IF_H_
#define ABCC_AD_IF_H_
#include "abcc_user_def.h"
#include "abcc_td.h"


/*******************************************************************************
** Constants
********************************************************************************
*/


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*
** Type for defining the direction of the process data map
*/
typedef enum
{
   PD_READ,
   PD_WRITE
} PD_DirType;

/*
** Type for default mapping element  
** Note ADI index ( not instance number )
*/
typedef struct
{
  UINT16     iAdiIndex;
  PD_DirType eDir;
} AD_DefaultMapType;


/*------------------------------------------------------------------------------
** ADI type property defines (min/max/default/(enum strings)).
**
** AD_UINT8Type      - ABP_UINT8    property type
** AD_CHARType       - ABP_CHAR     property type
** AD_BOOL8Type      - ABP_BOOL     property type
** AD_SINT8Type      - ABP_SINT8    property type
** AD_UINT16Type     - ABP_UINT16   property type
** AD_SINT16Type     - ABP_UINT16   property type
** AD_UINT32Type     - ABP_UINT32   property type
** AD_SINT32Type     - ABP_SINT32   property type
** AD_FLOAT32Type    - ABP_FLOAT32  property type
** AD_ENUMType       - ABP_ENUM     property type
** AD_UINT64Type     - ABP_UINT64   property type
** AD_SINT64Type     - ABP_SINT64   property type
**
** AD_ENUMStrType    - Enumeration string type, used with AD_ENUMType
**                     to specify value/string mappings.
**------------------------------------------------------------------------------
*/
typedef struct AD_UINT8Type
{
   UINT8 bMinValue;
   UINT8 bMaxValue;
   UINT8 bDefaultValue;
} AD_UINT8Type, AD_CHARType, AD_BOOL8Type;

typedef struct AD_SINT8Type
{
   INT8 bMinValue;
   INT8 bMaxValue;
   INT8 bDefaultValue;
} AD_SINT8Type;

typedef struct AD_UINT16Type
{
   UINT16 iMinValue;
   UINT16 iMaxValue;
   UINT16 iDefaultValue;
} AD_UINT16Type;

typedef struct AD_SINT16Type
{
   INT16 iMinValue;
   INT16 iMaxValue;
   INT16 iDefaultValue;
} AD_SINT16Type;

typedef struct AD_UINT32Type
{
   UINT32 lMinValue;
   UINT32 lMaxValue;
   UINT32 lDefaultValue;
} AD_UINT32Type;

typedef struct AD_SINT32Type
{
   INT32 lMinValue;
   INT32 lMaxValue;
   INT32 lDefaultValue;
} AD_SINT32Type;

typedef struct AD_FLOAT32Type
{
   FLOAT32 rMinValue;
   FLOAT32 rMaxValue;
   FLOAT32 rDefaultValue;
} AD_FLOAT32Type;

typedef struct AD_ENUMStrType
{
   UINT8  eValue;
   char*  acEnumStr;
} AD_ENUMStrType;

typedef struct AD_ENUMType
{
   UINT8             eMinValue;
   UINT8             eMaxValue;
   UINT8             eDefaultValue;
   UINT8             bNumOfEnumStrings;
   AD_ENUMStrType*   pasEnumStrings;
} AD_ENUMType;

#ifdef ABCC_USER_64BIT_ADI_SUPPORT
typedef struct AD_UINT64Type
{
   UINT64 lMinValue;
   UINT64 lMaxValue;
   UINT64 lDefaultValue;
} AD_UINT64Type;

typedef struct AD_SINT64Type
{
   INT64 lMinValue;
   INT64 lMaxValue;
   INT64 lDefaultValue;
} AD_SINT64Type;
#endif


/*------------------------------------------------------------------------------
** AD_AdiEntryType.
**------------------------------------------------------------------------------
** 1. iInstance               - ADI instance number (1-65535); 0 is reserved for 
**                              the Class.
** 2. pabName                 - Name of each ADI as a character string 
**                              (ADI instance attribute #1).
**                              If NULL, a 0 length name will be returned.
** 3. bDataType               - ADI data type format, which can be one of the 
**                              following:
**                                  ABP_BOOL:    Boolean.
**                                  ABP_SINT8:   8-bit signed integer.
**                                  ABP_SINT16:  16-bit signed integer.
**                                  ABP_SINT32:  32-bit signed integer.
**                                  ABP_UINT8:   8-bit unsigned integer.
**                                  ABP_UINT16:  16-bit unsigned integer.
**                                  ABP_UINT32:  32-bit unsigned integer.
**                                  ABP_CHAR:    Character.
**                                  ABP_ENUM:    Enumeration.
**                                  ABP_SINT64:  64-bit signed integer.
**                                  ABP_UINT64:  64-bit unsigned integer.
**                                  ABP_FLOAT:   floating point value (32-bits).
** 4. bNumOfElements          - Number of elements of the specified data type in (3).
** 5. bDesc                   - Entry descriptor; bits filled with the following
**                              configurations:
**                                  ABP_APPD_DESCR_GET_ACCESS: Get service is 
**                                  allowed on value attribute.
**                                  ABP_APPD_DESCR_SET_ACCESS: Set service is 
**                                  allowed on value attribute.
**                                  ABP_APPD_DESCR_MAPPABLE_WRITE_PD: Set if
**                                  possible to map.
**                                  ABP_APPD_DESCR_MAPPABLE_READ_PD:Set if
**                                  possible to map.
** 6. pxValuePtr              - Pointer to the local value variable.
** 7. psValueProps            - Pointer to the local value properties struct. 
**                              If NULL, no properties are applied (max/min/default).
**------------------------------------------------------------------------------
** 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 
** 6. { { pxValuePtr, pxValuePropPtr } }
**------------------------------------------------------------------------------
*/
typedef struct AD_AdiEntryType
{
   UINT16   iInstance;
   char*    pacName;
   UINT8    bDataType;
   UINT8    bNumOfElements;
   UINT8    bDesc;
   union
   {
      struct
      {
         void* pxValuePtr;
         void* pxValueProps;
      } sVOID;
      struct
      {
         UINT8*         pbValuePtr;
         AD_UINT8Type*  psValueProps;
      } sUINT8, sCHAR, sBOOL;
      struct
      {
         UINT8*       pbValuePtr;
         AD_ENUMType* psValueProps;
      }sENUM;
      struct
      {
         INT8*         pbValuePtr;
         AD_SINT8Type*  psValueProps;
      } sSINT8;
      struct
      {
         UINT16*        piValuePtr;
         AD_UINT16Type* psValueProps;
      } sUINT16;
      struct
      {
         INT16*        piValuePtr;
         AD_SINT16Type* psValueProps;
      } sSINT16;
      struct
      {
         UINT32*        plValuePtr;
         AD_UINT32Type* psValueProps;
      } sUINT32;
      struct
      {
         INT32*        plValuePtr;
         AD_SINT32Type* psValueProps;
      } sSINT32;
      struct
      {
         FLOAT32*        prValuePtr;
         AD_FLOAT32Type* psValueProps;
      } sFLOAT;
#ifdef ABCC_USER_64BIT_ADI_SUPPORT
      struct
      {
         UINT64*        plValuePtr;
         AD_UINT64Type* psValueProps;
      } sUINT64;
      struct
      {
         INT64*        plValuePtr;
         AD_SINT64Type* psValueProps;
      } sSINT64;
#endif

   } uData;
   void (*PreReadVal)();
   int (*PostWriteVal)();
} AD_AdiEntryType;

#if 0
/*******************************************************************************
** Example of ADI mapping
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ADI instance allocation.
**------------------------------------------------------------------------------
** 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 
** 6.pxValuePtr | 7. pxValuePropPtr
**------------------------------------------------------------------------------
*/

/*
 ** Example with all access flags set.
 */
#define ADI_DESCR_ALL_ACCESS  ( ABP_APPD_DESCR_GET_ACCESS | ABP_APPD_DESCR_SET_ACCESS | ABP_APPD_DESCR_MAPPABLE_WRITE_PD | ABP_APPD_DESCR_MAPPABLE_READ_PD )

const AD_AdiEntryType AD_asADIEntryList[] =
{
   /* Idx:0 */ { 1,  "RadOnOff_BOOL8",          ABP_BOOL,   1, ADI_DESCR_ALL_ACCESS, &VAPP_fRadOnOff,           &VAPP_BOOL8Props_fRadOnOff },
   /* Idx:1 */ { 2,  "RadSetpointTemp_FLOAT",   ABP_FLOAT,  1, ADI_DESCR_ALL_ACCESS, &VAPP_rRadSetpointTemp,    &VAPP_FLOAT32Props_rRadSetpointTemp },
   /* Idx:2 */ { 3,  "RadErrCode_ENUM",         ABP_ENUM,   1, ADI_DESCR_ALL_ACCESS, &VAPP_eRadErrCode,         &VAPP_ENUMProps_RadErrCode },
   /* Idx:3 */ { 4,  "Fan_OnOff_BOOL8",         ABP_BOOL,   1, ADI_DESCR_ALL_ACCESS, &VAPP_fFanOnOff,           &VAPP_BOOL8Props_fRadOnOff },
   /* Idx:4 */ { 5,  "FanOpErrCode_ENUM",       ABP_ENUM,   1, ADI_DESCR_ALL_ACCESS, &VAPP_eFanErrCode,         &VAPP_ENUMProps_FanErrCode },
   /* Idx:5 */ { 6,  "ActTemp_FLOAT",           ABP_FLOAT,  1, ADI_DESCR_ALL_ACCESS, &VAPP_rActTemp,            &VAPP_FLOAT32Props_rRadSetpointTemp },
   /* Idx:6 */ { 500,"ActTempErrCode_ENUM",     ABP_ENUM,   1, ADI_DESCR_ALL_ACCESS, &VAPP_eActTempErrCode,     &VAPP_ENUMProps_ActTempErrCode },
   /* Idx:7 */ { 501,"InputTemp_FLOAT",         ABP_FLOAT,  1, ADI_DESCR_ALL_ACCESS, &VAPP_rInputTemp,          NULL },
   /* Idx:8 */ { 502,"HeatEffect_deg/sec_FLOAT",ABP_FLOAT,  1, ADI_DESCR_ALL_ACCESS, &VAPP_rRadHeatPowerPerSec, NULL },
   /* Idx:9 */ { 503,"SystemTripBits_UINT8",    ABP_UINT8,  1, ADI_DESCR_ALL_ACCESS, &VAPP_bTripBits,           NULL },
};


/*------------------------------------------------------------------------------
** ADI read/write process data mappings.
**------------------------------------------------------------------------------
** Array of mapping elements containing indexes entries in AD_asADIEntryList (Idx:xx)
** in native mapping order and direction of the map.
** Note: Mapping sequence is terminated by index value 0xFFFF and MUST be present 
**       at end of LIST.
**------------------------------------------------------------------------------
*/
AD_DefaultMapType sDefaultMap[{ 2, PD_WRITE },
                              { 4, PD_WRITE },
                              { 5, PD_WRITE },
                              { 0, PD_READ },
                              { 1, PD_READ },
                              { 6, PD_WRITE },
                              { 7, PD_WRITE },
                              { 8, PD_WRITE },
                              { 3, PD_READ },
                              { 9, PD_READ },
                              { 0xFFFF } ];
#endif


#endif  /* inclusion lock */

/*******************************************************************************
** End of abcc_ad_if.h
********************************************************************************
*/
