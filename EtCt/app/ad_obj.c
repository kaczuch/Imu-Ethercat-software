/*
 * ad_obj.c
 *
 *  Created on: 11 sie 2015
 *      Author: bkaczor
 */



#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abp.h"
#include "abcc_ad_if.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "user_adi.h"





/*******************************************************************************
** Public Globals
********************************************************************************
*/

#define AD_MAX_NUM_WRITE_ADI_TO_MAP  ABCC_USER_MAX_NUM_WRITE_ADI_TO_MAP
#define AD_MAX_NUM_READ_ADI_TO_MAP  ABCC_USER_MAX_NUM_READ_ADI_TO_MAP



/*******************************************************************************
** Private Globals
********************************************************************************
*/

const AD_DefaultMapType* AD_asDefaultMap;    /* Pointer to default map */
const AD_AdiEntryType* AD_asADIEntryList;    /* Pointer to ADI entry list */

/*
** Type describing read and write map
** paiMappedAdiList: List of ADI index that is ampped
** AD_NumMappedAdi:  Number of mapped ADI
** iPdSize:          Current PD size
** asElementInfo     Descripe what part of array is mapped ( if applicable )
*/
typedef struct
{
   UINT16  paiMappedAdiList[ AD_MAX_NUM_READ_ADI_TO_MAP ];
   UINT16   AD_NumMappedAdi;
   UINT16   iPdSize;
   AD_ArrayInfoType asElementInfo[ AD_MAX_NUM_READ_ADI_TO_MAP ];
} AD_PdReadMapType;

typedef struct
{
   UINT16  paiMappedAdiList[ AD_MAX_NUM_WRITE_ADI_TO_MAP ];
   UINT16   AD_NumMappedAdi;
   UINT16   iPdSize;
   AD_ArrayInfoType asElementInfo[ AD_MAX_NUM_WRITE_ADI_TO_MAP ];
} AD_PdWriteMapType;


#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED

static UINT8 bLatestRemapCommand = 0;
static UINT16 AD_RemapProcessDataCommand( ABP_MsgType* psMsg,
                                          const UINT16 iCurrNumMappedAdi,
                                          UINT16* piCurrMap,
                                          AD_ArrayInfoType* piCurrElemInfo,
                                          UINT16* piNewMap,
                                          AD_ArrayInfoType* piNewElemInfo,
                                          UINT16* piNewNumAdi,
                                          UINT16 iMaxMappedAdi);

#endif


static UINT16   AD_iNumOfADIs;
static UINT16   AD_iHighestInstanceNumber;
static BOOL     AD_fNewWrPd   = FALSE;

static NetFormatType NetFormat;

static AD_PdReadMapType AD_ReadMapInfo;
static AD_PdWriteMapType AD_WriteMapInfo;

#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED
static AD_PdReadMapType AD_RemapReadMapInfo;
static AD_PdWriteMapType AD_RemapWriteMapInfo;
#endif

extern UINT32 Data8[];
/*******************************************************************************
** Public Functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** AD_Init()
**------------------------------------------------------------------------------
*/
void AD_Init( void )
{
   UINT16 iMapIndex = 0;
   UINT16 iAdiIndex = 0;
   UINT16 iSize = 0;
   UINT16 i;

   /*
   ** In this context we should initialize the AD object to be prepared for
   ** startup.
   **
   */
   NetFormat = ABCC_NetFormatType();
   AD_asADIEntryList = ADI_GetADIEntryList();
   AD_asDefaultMap = ADI_GetDefaultMap();

   AD_ReadMapInfo.iPdSize = 0;
   AD_ReadMapInfo.AD_NumMappedAdi = 0;
   AD_WriteMapInfo.iPdSize = 0;
   AD_WriteMapInfo.AD_NumMappedAdi = 0;

   if ( AD_asDefaultMap != NULL )
   {
      while ( AD_asDefaultMap[ iMapIndex ].iAdiIndex != 0xffff )
      {
         iSize = ABCC_GetDataTypeSize( AD_asADIEntryList[ AD_asDefaultMap[ iMapIndex ].iAdiIndex].bDataType ) *
         AD_asADIEntryList[ AD_asDefaultMap[ iMapIndex ].iAdiIndex].bNumOfElements;

         if ( AD_asDefaultMap[ iMapIndex ].eDir == PD_READ )
         {
            AD_ReadMapInfo.paiMappedAdiList[ AD_ReadMapInfo.AD_NumMappedAdi++ ] = AD_asDefaultMap[ iMapIndex ].iAdiIndex;
            AD_ReadMapInfo.iPdSize += iSize;
         }
         else
         {
            AD_WriteMapInfo.paiMappedAdiList[ AD_WriteMapInfo.AD_NumMappedAdi++ ] = AD_asDefaultMap[ iMapIndex ].iAdiIndex;
            AD_WriteMapInfo.iPdSize += iSize;
         }
         iMapIndex++;
      }
   }

   AD_iNumOfADIs =  AD_GetNumAdi();

   AD_iHighestInstanceNumber = 0;

   for( iAdiIndex = 0; iAdiIndex < AD_iNumOfADIs; iAdiIndex++ )
   {
      if( AD_asADIEntryList[ iAdiIndex ].iInstance > AD_iHighestInstanceNumber )
      {
         AD_iHighestInstanceNumber = AD_asADIEntryList[ iAdiIndex ].iInstance;
      }
   }


   for ( i = 0 ; i < AD_ReadMapInfo.AD_NumMappedAdi ; i++)
   {
      AD_ReadMapInfo.asElementInfo[ i ].bStartIndex = 0;
      AD_ReadMapInfo.asElementInfo[ i ].bNumElements = AD_asADIEntryList[ AD_ReadMapInfo.paiMappedAdiList[ i ] ].bNumOfElements;
   }

   for ( i = 0 ; i < AD_WriteMapInfo.AD_NumMappedAdi ; i++)
   {
      AD_WriteMapInfo.asElementInfo[ i ].bStartIndex = 0;
      AD_WriteMapInfo.asElementInfo[ i ].bNumElements = AD_asADIEntryList[ AD_WriteMapInfo.paiMappedAdiList[ i ]  ].bNumOfElements;
   }

   AD_fNewWrPd   = TRUE;
}


/*------------------------------------------------------------------------------
** AD_GetAdiIndex()
**------------------------------------------------------------------------------
*/
const UINT16 AD_GetAdiIndex( UINT16 iInstance )
{
   UINT16 i;
   UINT16  iIndex = 0xffff;

   for( i = 0; i < AD_iNumOfADIs; i++ )
   {
      if( AD_asADIEntryList[ i ].iInstance == iInstance )
      {
         iIndex = i;
         break;
      }
   }
   return( iIndex );
}

/*------------------------------------------------------------------------------
** AD_GetAdiInstEntry()
**------------------------------------------------------------------------------
*/
const AD_AdiEntryType* AD_GetAdiInstEntry( UINT16 iInstance )
{
   UINT16 i;
   const AD_AdiEntryType* psEntry = NULL;

   i = AD_GetAdiIndex( iInstance );
   if ( i != 0xffff )
   {
      psEntry = &AD_asADIEntryList[ i ];
   }
   return( psEntry );
}


/*------------------------------------------------------------------------------
** AD_ProcObjectRequest()
**------------------------------------------------------------------------------
*/
void AD_ProcObjectRequest( ABP_MsgType* psMsgBuffer )
{
   const AD_AdiEntryType* psAdiEntry;
   UINT16 iItemSize;

   if( iLeTOi( psMsgBuffer->sHeader.iInstance ) == ABP_INST_OBJ )
   {
      /*
      ** A request to the object instance.
      */
      switch( psMsgBuffer->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS )
      {
      case ABP_CMD_GET_ATTR:
         switch( psMsgBuffer->sHeader.bCmdExt0 )
         {
         case ABP_OA_NAME: /* Name. */
            ABCC_SYS_MemCpy( psMsgBuffer->abData, "Application data", 16 );
            psMsgBuffer->sHeader.iDataSize = 16;
            break;

         case ABP_OA_REV: /* Revision. */
            psMsgBuffer->abData[ 0 ] = 1;
            psMsgBuffer->sHeader.iDataSize = ABP_OA_REV_DS;
            break;

         case ABP_OA_NUM_INST: /* Number of instances. */
            *(UINT16*)&psMsgBuffer->abData[ 0 ] = iTOiLe( AD_iNumOfADIs );
            psMsgBuffer->sHeader.iDataSize = ABP_OA_NUM_INST_DS;
            break;

         case ABP_OA_HIGHEST_INST: /* Highest instance number. */
            *(UINT16*)&psMsgBuffer->abData[ 0 ] = iTOiLe( AD_iHighestInstanceNumber );
            psMsgBuffer->sHeader.iDataSize = ABP_OA_HIGHEST_INST_DS;
            break;

         case ABP_APPD_OA_NR_READ_PD_MAPPABLE_INSTANCES: /* Number of mappable read data instances. */
            {
               UINT16 iIndex;
               UINT16 iCnt = 0;

               for( iIndex=0; iIndex<AD_iNumOfADIs; iIndex++)
               {
                  if( AD_asADIEntryList[iIndex ].bDesc & ABP_APPD_DESCR_MAPPABLE_READ_PD )
                  {
                     iCnt++;
                  }
               }
               *(UINT16*)&psMsgBuffer->abData[ 0 ] = iTOiLe( iCnt );
               psMsgBuffer->sHeader.iDataSize = ABP_UINT16_SIZEOF;
            }
            break;

         case ABP_APPD_OA_NR_WRITE_PD_MAPPABLE_INSTANCES: /* Number of mappable write data instances. */
            {
               UINT16 iIndex;
               UINT16 iCnt = 0;

               for( iIndex=0; iIndex<AD_iNumOfADIs; iIndex++)
               {
                  if( AD_asADIEntryList[ iIndex ].bDesc & ABP_APPD_DESCR_MAPPABLE_WRITE_PD )
                  {
                     iCnt++;
                  }
               }
               *(UINT16*)&psMsgBuffer->abData[ 0 ] = iTOiLe( iCnt );
               psMsgBuffer->sHeader.iDataSize = ABP_UINT16_SIZEOF;
            }

            break;

         default: /* Unsupported attribute. */
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
            break;
         }
         break;

      case ABP_APPD_CMD_GET_INST_BY_ORDER:
         if( ( iLeTOi( *(UINT16*)&psMsgBuffer->sHeader.bCmdExt0 ) == 0 ) ||
             ( iLeTOi( *(UINT16*)&psMsgBuffer->sHeader.bCmdExt0 ) > AD_iNumOfADIs ) )
         {
            /*
            ** Requested order number does not exist.
            */
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
         }
         else
         {
            *(UINT16*)&psMsgBuffer->abData[ 0 ] = iTOiLe( AD_asADIEntryList[ iLeTOi( *(UINT16*)&psMsgBuffer->sHeader.bCmdExt0 ) - 1 ].iInstance );
            psMsgBuffer->sHeader.iDataSize = ABP_UINT16_SIZEOF;
         }
         break;


#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED

      case ABP_APPD_REMAP_ADI_WRITE_AREA:
         AD_RemapWriteMapInfo.iPdSize = AD_RemapProcessDataCommand( psMsgBuffer,
                                                    AD_WriteMapInfo.AD_NumMappedAdi,
                                                    AD_WriteMapInfo.paiMappedAdiList,
                                                    AD_WriteMapInfo.asElementInfo,
                                                    AD_RemapWriteMapInfo.paiMappedAdiList,
                                                    AD_RemapWriteMapInfo.asElementInfo,
                                                    &AD_RemapWriteMapInfo.AD_NumMappedAdi,
                                                    AD_MAX_NUM_WRITE_ADI_TO_MAP );

         bLatestRemapCommand = ABP_APPD_REMAP_ADI_WRITE_AREA;
         ABCC_SendRemapRespMsg( psMsgBuffer, AD_ReadMapInfo.iPdSize, AD_RemapWriteMapInfo.iPdSize);
         break;

      case ABP_APPD_REMAP_ADI_READ_AREA:
         AD_RemapReadMapInfo.iPdSize = AD_RemapProcessDataCommand( psMsgBuffer,
                                                    AD_ReadMapInfo.AD_NumMappedAdi,
                                                    AD_ReadMapInfo.paiMappedAdiList,
                                                    AD_ReadMapInfo.asElementInfo,
                                                    AD_RemapReadMapInfo.paiMappedAdiList,
                                                    AD_RemapReadMapInfo.asElementInfo,
                                                    &AD_RemapReadMapInfo.AD_NumMappedAdi,
                                                    AD_MAX_NUM_READ_ADI_TO_MAP );

         bLatestRemapCommand = ABP_APPD_REMAP_ADI_READ_AREA;
         ABCC_SendRemapRespMsg( psMsgBuffer, AD_RemapReadMapInfo.iPdSize, AD_WriteMapInfo.iPdSize);

         break;
#endif
      case ABP_APPD_GET_INSTANCE_NUMBERS:

         if(  iLeTOi( psMsgBuffer->sHeader.bCmdExt0 ) != 0 )
         {
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
         }
         else
         {
            UINT16 iStartingOrder = iLeTOi( *(UINT16*)&psMsgBuffer->abData[ 0 ] );
            UINT16 iReqInstances  = iLeTOi( *(UINT16*)&psMsgBuffer->abData[ 2 ] );
            UINT16 i;
            UINT16 ii;
            UINT16* paiData       = (UINT16*) psMsgBuffer->abData;

            switch ( iLeTOi( psMsgBuffer->sHeader.bCmdExt1 ) )
            {
            case ABP_APPD_LIST_TYPE_ALL:
               psMsgBuffer->sHeader.iDataSize = 0;
               for (i = 0; ( i < iReqInstances ) && ( iStartingOrder + i <= AD_iNumOfADIs ); i++ )
               {
                  *paiData++ = iTOiLe( AD_asADIEntryList[ iStartingOrder + i - 1 ].iInstance );
                  psMsgBuffer->sHeader.iDataSize += ABP_UINT16_SIZEOF;
               }
               break;

            case ABP_APPD_LIST_TYPE_RD_PD_MAPPABLE:
               psMsgBuffer->sHeader.iDataSize = 0;
               for (i = 0, ii = 1; ( ii < (iStartingOrder + iReqInstances ) ) && ( i < AD_iNumOfADIs ); i++ )
               {
                  if( AD_asADIEntryList[ i ].bDesc & ABP_APPD_DESCR_MAPPABLE_READ_PD )
                  {
                     if( ii >= iStartingOrder )
                     {
                        *paiData++ = iTOiLe( AD_asADIEntryList[ i ].iInstance );
                        psMsgBuffer->sHeader.iDataSize += ABP_UINT16_SIZEOF;
                     }
                     ii++;
                  }
               }
               break;
            case ABP_APPD_LIST_TYPE_WR_PD_MAPPABLE:
               psMsgBuffer->sHeader.iDataSize = 0;
               for (i = 0, ii = 1; ( ii < (iStartingOrder + iReqInstances ) ) && ( i < AD_iNumOfADIs ); i++ )
               {
                  if( AD_asADIEntryList[ i ].bDesc & ABP_APPD_DESCR_MAPPABLE_WRITE_PD )
                  {
                     if( ii >= iStartingOrder )
                     {
                        *paiData++ = iTOiLe( AD_asADIEntryList[ i ].iInstance );
                        psMsgBuffer->sHeader.iDataSize += ABP_UINT16_SIZEOF;
                     }
                     ii++;
                  }
               }
               break;
            default:
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_1 );
               break;
            }

         }

         break;

      default: /* Unsupported command */
         ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
         break;
      }
   }
   else if( ( psAdiEntry = AD_GetAdiInstEntry( iLeTOi( psMsgBuffer->sHeader.iInstance ) ) ) != NULL )
   {
      /*
      ** The ADI instance was found. Now switch on command.
      */
      switch( psMsgBuffer->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS )
      {
      case ABP_CMD_GET_ATTR:

         /*
         ** Switch on attribute.
         */
         switch( psMsgBuffer->sHeader.bCmdExt0 )
         {
         case ABP_APPD_IA_NAME: /* Instance name. */
            if( psAdiEntry->pacName )
            {
               psMsgBuffer->sHeader.iDataSize = strlen( psAdiEntry->pacName );
               ABCC_SYS_MemCpy( psMsgBuffer->abData, psAdiEntry->pacName, psMsgBuffer->sHeader.iDataSize );
            }
            else
            {
               psMsgBuffer->sHeader.iDataSize = 0;
            }
            break;

         case ABP_APPD_IA_DATA_TYPE: /* Data type. */
            psMsgBuffer->abData[ 0 ] = psAdiEntry->bDataType;
            psMsgBuffer->sHeader.iDataSize = ABP_APPD_IA_DATA_TYPE_DS;
            break;

         case ABP_APPD_IA_NUM_ELEM: /* Number of elements. */
            psMsgBuffer->abData[ 0 ] = psAdiEntry->bNumOfElements;
            psMsgBuffer->sHeader.iDataSize = ABP_APPD_IA_NUM_ELEM_DS;
            break;

         case ABP_APPD_IA_DESCRIPTOR: /* Descriptor. */
            psMsgBuffer->abData[ 0 ] = psAdiEntry->bDesc;
            psMsgBuffer->sHeader.iDataSize = ABP_APPD_IA_DESCRIPTOR_DS;
            break;

         case ABP_APPD_IA_VALUE: /* Value. */
            if( !( psAdiEntry->bDesc & ABP_APPD_DESCR_GET_ACCESS ) )
            {
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_ATTR_NOT_GETABLE );
               break;
            }
            //TODO: call function from pointer before load the value
            if(psAdiEntry->PreReadVal!=NULL)
            {

            	(*psAdiEntry->PreReadVal)();
            }

            psMsgBuffer->sHeader.iDataSize = (UINT8)AD_GetAdiValue( psAdiEntry, &psMsgBuffer->abData[ 0 ], NULL );
            break;

         case ABP_APPD_IA_MAX_VALUE:
         case ABP_APPD_IA_MIN_VALUE:
         case ABP_APPD_IA_DFLT_VALUE:
            if( psAdiEntry->uData.sUINT8.psValueProps )
            {
               switch( psAdiEntry->bDataType )
               {
               case ABP_BOOL:
               case ABP_CHAR:
               case ABP_UINT8:
               case ABP_SINT8:
               case ABP_ENUM:
                  switch( psMsgBuffer->sHeader.bCmdExt0 )
                  {
                  case ABP_APPD_IA_MAX_VALUE:
                     psMsgBuffer->abData[ 0 ] = psAdiEntry->uData.sUINT8.psValueProps->bMaxValue;
                     break;
                  case ABP_APPD_IA_MIN_VALUE:
                     psMsgBuffer->abData[ 0 ] = psAdiEntry->uData.sUINT8.psValueProps->bMinValue;
                     break;
                  default:
                     psMsgBuffer->abData[ 0 ] = psAdiEntry->uData.sUINT8.psValueProps->bDefaultValue;
                     break;
                  }

                  psMsgBuffer->sHeader.iDataSize = 1;
                  break;
               case ABP_UINT16:
               case ABP_SINT16:
                  switch( psMsgBuffer->sHeader.bCmdExt0 )
                  {
                  case ABP_APPD_IA_MAX_VALUE:
                     *(UINT16*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? iTOiBe( psAdiEntry->uData.sUINT16.psValueProps->iMaxValue ) : iTOiLe( psAdiEntry->uData.sUINT16.psValueProps->iMaxValue );
                     break;
                  case ABP_APPD_IA_MIN_VALUE:
                     *(UINT16*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? iTOiBe( psAdiEntry->uData.sUINT16.psValueProps->iMinValue ) : iTOiLe( psAdiEntry->uData.sUINT16.psValueProps->iMinValue );
                     break;
                  default:
                     *(UINT16*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? iTOiBe( psAdiEntry->uData.sUINT16.psValueProps->iDefaultValue ) : iTOiLe( psAdiEntry->uData.sUINT16.psValueProps->iDefaultValue );
                     break;
                  }

                  psMsgBuffer->sHeader.iDataSize = 2;
                  break;
               case ABP_UINT32:
               case ABP_SINT32:
               case ABP_FLOAT:
                  switch( psMsgBuffer->sHeader.bCmdExt0 )
                  {
                  case ABP_APPD_IA_MAX_VALUE:
                     *(UINT32*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe( psAdiEntry->uData.sUINT32.psValueProps->lMaxValue ) : lTOlLe( psAdiEntry->uData.sUINT32.psValueProps->lMaxValue );
                     break;
                  case ABP_APPD_IA_MIN_VALUE:
                     *(UINT32*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe( psAdiEntry->uData.sUINT32.psValueProps->lMinValue ) : lTOlLe( psAdiEntry->uData.sUINT32.psValueProps->lMinValue );
                     break;
                  default:
                     *(UINT32*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe( psAdiEntry->uData.sUINT32.psValueProps->lDefaultValue ) : lTOlLe( psAdiEntry->uData.sUINT32.psValueProps->lDefaultValue );
                     break;
                  }

                  psMsgBuffer->sHeader.iDataSize = 4;
                  break;
#ifdef ABCC_USER_64BIT_ADI_SUPPORT
               case ABP_UINT64:
               case ABP_SINT64:
                  switch( psMsgBuffer->sHeader.bCmdExt0 )
                  {
                  case ABP_APPD_IA_MAX_VALUE:
                     *(UINT64*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe64( psAdiEntry->uData.sUINT64.psValueProps->lMaxValue ) : lTOlLe64( psAdiEntry->uData.sUINT64.psValueProps->lMaxValue );
                     break;
                  case ABP_APPD_IA_MIN_VALUE:
                     *(UINT64*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe64( psAdiEntry->uData.sUINT64.psValueProps->lMinValue ) : lTOlLe64( psAdiEntry->uData.sUINT64.psValueProps->lMinValue );
                     break;
                  default:
                     *(UINT64*)&psMsgBuffer->abData[ 0 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe64( psAdiEntry->uData.sUINT64.psValueProps->lDefaultValue ) : lTOlLe64( psAdiEntry->uData.sUINT64.psValueProps->lDefaultValue );
                     break;
                  }

                  psMsgBuffer->sHeader.iDataSize = 8;
                  break;
#endif
               default:
                  while( 1 ); /* Trap: The ADI type is not supported. */

               }
            }
            else
            {
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
            }
            break;

         default: /* Unsupported attribute. */
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
            break;
         }
         break;
      case ABP_CMD_SET_ATTR:
         switch( psMsgBuffer->sHeader.bCmdExt0 )
         {
         case ABP_APPD_IA_NAME:
         case ABP_APPD_IA_DATA_TYPE:
         case ABP_APPD_IA_NUM_ELEM:
         case ABP_APPD_IA_DESCRIPTOR: /* Attributes sre not settable. */
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_ATTR_NOT_SETABLE );
            break;
         case ABP_APPD_IA_VALUE:
            if( !( psAdiEntry->bDesc & ABP_APPD_DESCR_SET_ACCESS ) )
            {
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_ATTR_NOT_SETABLE );
               break;
            }

            /*
            ** Check the length of each array.
            */
            iItemSize = ABCC_GetDataTypeSize( psAdiEntry->bDataType );
            if( psMsgBuffer->sHeader.iDataSize > ( iItemSize * psAdiEntry->bNumOfElements ) )
            {
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_TOO_MUCH_DATA );
               break;
            }
            else if( psMsgBuffer->sHeader.iDataSize < ( iItemSize * psAdiEntry->bNumOfElements ) )
            {
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_NOT_ENOUGH_DATA );
               break;
            }

            if( AD_SetAdiValue( psAdiEntry, &psMsgBuffer->abData[ 0 ],NULL, FALSE ) == -1 )
            {
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_OUT_OF_RANGE );
            }
            else
            {
               /*
               ** Success.
               */
            	if(psAdiEntry->PostWriteVal!=NULL){
            		if((*psAdiEntry->PostWriteVal)())
					{
						ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_OUT_OF_RANGE );
					}
            	}
            	//TODO: uruchomiæ funkcjê ze wskaŸnika po sprawdzeniu czy nie NULL
               psMsgBuffer->sHeader.iDataSize = 0;
            }
            break;

         default: /* Unsupported attribute. */
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
            break;
         }
         break;
      case ABP_CMD_GET_ENUM_STR:
         switch( psMsgBuffer->sHeader.bCmdExt0 )
         {
         case ABP_APPD_IA_VALUE:
            if( psAdiEntry->bDataType == ABP_ENUM )
            {
               if( ( psAdiEntry->uData.sENUM.psValueProps == NULL ) ||
                   ( psAdiEntry->uData.sENUM.psValueProps->pasEnumStrings == NULL ) )
               {
                  ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
               }
               else
               {
                  UINT8 b = 0;

                  for( b = 0; b < psAdiEntry->uData.sENUM.psValueProps->bNumOfEnumStrings; b++ )
                  {
                     if( psAdiEntry->uData.sENUM.psValueProps->pasEnumStrings[ b ].eValue == psMsgBuffer->sHeader.bCmdExt1 )
                     {
                        break;
                     }
                  }

                  if( b < psAdiEntry->uData.sENUM.psValueProps->bNumOfEnumStrings )
                  {
                     psMsgBuffer->sHeader.iDataSize = strlen( psAdiEntry->uData.sENUM.psValueProps->pasEnumStrings[ b ].acEnumStr );
                     ABCC_SYS_MemCpy( &psMsgBuffer->abData[ 0 ], psAdiEntry->uData.sENUM.psValueProps->pasEnumStrings[ b ].acEnumStr, psMsgBuffer->sHeader.iDataSize );
                  }
                  else
                  {
                     /*
                     ** The enum value was not found in the string lookup.
                     */
                     ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_OUT_OF_RANGE );
                  }
               }
            }
            else
            {
               ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
            }
            break;
         default:
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
            break;
         }
         break;
      case ABP_CMD_GET_INDEXED_ATTR:
         switch( psMsgBuffer->sHeader.bCmdExt0 )
         {
            case ABP_APPD_IA_VALUE:
               if( !( psAdiEntry->bDesc & ABP_APPD_DESCR_GET_ACCESS ) )
               {
                  ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_ATTR_NOT_GETABLE );
                  break;
               }
               else
               {
                  AD_ArrayInfoType sSelectedElement;
                  sSelectedElement.bStartIndex  = psMsgBuffer->sHeader.bCmdExt1;
                  sSelectedElement.bNumElements = 1;

                  psMsgBuffer->sHeader.iDataSize = (UINT8)AD_GetAdiValue( psAdiEntry, &psMsgBuffer->abData[ 0 ], &sSelectedElement );
                  if( psMsgBuffer->sHeader.iDataSize == 0 )
                  {
                     ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_OUT_OF_RANGE );
                  }
               }
            break;


         default:
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
            break;
         }
         break;
      case ABP_CMD_SET_INDEXED_ATTR:
         switch( psMsgBuffer->sHeader.bCmdExt0 )
         {
            case ABP_APPD_IA_VALUE:
               if( !( psAdiEntry->bDesc & ABP_APPD_DESCR_SET_ACCESS ) )
               {
                  ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_ATTR_NOT_SETABLE );
                  break;
               }
               {
                  AD_ArrayInfoType sSelectedElement;
                  sSelectedElement.bStartIndex  = psMsgBuffer->sHeader.bCmdExt1;
                  sSelectedElement.bNumElements = 1;

                  if( AD_SetAdiValue( psAdiEntry, &psMsgBuffer->abData[ 0 ], &sSelectedElement, FALSE ) == -1 )
                  {
                     ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_OUT_OF_RANGE );
                  }
                  else
                  {
                     /*
                     ** Success.
                     */
                     psMsgBuffer->sHeader.iDataSize = 0;
                  }
               }
               break;

         default:
            ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
            break;
         }
         break;

      default: /* Unsupported command. */
         ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
         break;
      }
   }
   else
   {
      /* The instance was not found. */
      ABP_SetMsgErrorResponse( psMsgBuffer, 1, ABP_ERR_UNSUP_INST );
   }


   ABP_SetMsgResponse( psMsgBuffer, psMsgBuffer->sHeader.iDataSize );

#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED
   /*
   ** Special handling. The remap response is already handled.
   ** If remap support is disabled this check needs to be disabled so the error
   ** response can be sent.
   */
   if ( ( psMsgBuffer->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS ) != ABP_APPD_REMAP_ADI_WRITE_AREA  &&
        ( psMsgBuffer->sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS ) != ABP_APPD_REMAP_ADI_READ_AREA  )
   {
      ABCC_SendRespMsg( psMsgBuffer );
   }
#else
   ABCC_SendRespMsg( psMsgBuffer );
#endif
}

/*------------------------------------------------------------------------------
** AD_GetAdiValue()
**------------------------------------------------------------------------------
*/
UINT16 AD_GetAdiValue( const AD_AdiEntryType* psAdiEntry, UINT8* pabDataPtr, AD_ArrayInfoType* const psArrayInfo )
{
   UINT16 i;
   UINT16 iSize = 0;
   AD_ArrayInfoType sArrayLimits;

   /*
    * Check if only parts of the array should be set.
    * If NULL all elements is set.
    */
   if( psArrayInfo )
   {
      sArrayLimits = *psArrayInfo;
   }
   else
   {
      sArrayLimits.bStartIndex = 0;
      sArrayLimits.bNumElements = psAdiEntry->bNumOfElements;
   }

   switch( psAdiEntry->bDataType )
   {
   case ABP_BOOL:
   case ABP_CHAR:
   case ABP_UINT8:
   case ABP_SINT8:
   case ABP_ENUM:
      ABCC_SYS_MemCpy( pabDataPtr, &psAdiEntry->uData.sUINT8.pbValuePtr[ sArrayLimits.bStartIndex ], sArrayLimits.bNumElements );
      iSize = psAdiEntry->bNumOfElements;
      break;
   case ABP_SINT16:
   case ABP_UINT16:
      for( i = sArrayLimits.bStartIndex; i < sArrayLimits.bNumElements; i++ )
      {
         iSize += 2;
         *(UINT16*)&pabDataPtr[ i*2 ] = ( NetFormat == NET_BIGENDIAN ) ? iTOiBe( psAdiEntry->uData.sUINT16.piValuePtr[ i ] ) : iTOiLe( psAdiEntry->uData.sUINT16.piValuePtr[ i ] );
      }
      break;
   case ABP_UINT32:
   case ABP_SINT32:
   case ABP_FLOAT:
      for( i = sArrayLimits.bStartIndex; i < sArrayLimits.bNumElements; i++ )
      {
         iSize += 4;
         *(UINT32*)&pabDataPtr[ i*4 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe( psAdiEntry->uData.sUINT32.plValuePtr[ i ] ) : lTOlLe( psAdiEntry->uData.sUINT32.plValuePtr[ i ] );
      }
      break;
#ifdef ABCC_USER_64BIT_ADI_SUPPORT
   case ABP_UINT64:
   case ABP_SINT64:
      for( i = sArrayLimits.bStartIndex; i < sArrayLimits.bNumElements; i++ )
      {
         iSize += 8;
         *(UINT64*)&pabDataPtr[ i*8 ] = ( NetFormat == NET_BIGENDIAN ) ? lTOlBe64( psAdiEntry->uData.sUINT64.plValuePtr[ i ] ) : lTOlLe64( psAdiEntry->uData.sUINT64.plValuePtr[ i ] );
      }
      break;
#endif
   default:
      while( 1 ); /* Trap: Unknown data type. */

   }

   return( iSize );
}


/*------------------------------------------------------------------------------
** AD_SetAdiValue()
**------------------------------------------------------------------------------
*/
INT16 AD_SetAdiValue( const AD_AdiEntryType* psAdiEntry, UINT8* pabDataPtr, AD_ArrayInfoType* const psArrayInfo, BOOL8 fPdRead )
{
   BOOL8 fCheckFailed = FALSE;
   INT16 iSetSize = -1;
   UINT16 i;
   AD_ArrayInfoType sArrayLimits;

   union
   {
      UINT16 iValue;
      INT16 iSignedValue;
      UINT32 lValue;
      INT32 lSignedValue;
      float  rValue;
#ifdef ABCC_USER_64BIT_ADI_SUPPORT
      UINT64 l64Value;
      INT64 l64SignedValue;
#endif
   } sValue;

   /*
    * Check if only parts of the array should be set.
    * If NULL all elements is set.
    */

   if( psArrayInfo )
   {
      sArrayLimits = *psArrayInfo;
   }
   else
   {
      sArrayLimits.bStartIndex = 0;
      sArrayLimits.bNumElements = psAdiEntry->bNumOfElements;
   }

   if( psAdiEntry->uData.sUINT8.psValueProps )
   {
      /*
      ** Check for the array's max/min limits.
      */
      for( i = sArrayLimits.bStartIndex; i < sArrayLimits.bNumElements; i++ )
      {
         switch( psAdiEntry->bDataType )
         {
         case ABP_BOOL:
         case ABP_CHAR:
         case ABP_UINT8:
         case ABP_ENUM:
            if( ( *(UINT8*)&pabDataPtr[ i ] > psAdiEntry->uData.sUINT8.psValueProps->bMaxValue ) ||
                ( *(UINT8*)&pabDataPtr[ i ] < psAdiEntry->uData.sUINT8.psValueProps->bMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
         case ABP_SINT8:
            if( ( *(INT8*)&pabDataPtr[ i ] > psAdiEntry->uData.sSINT8.psValueProps->bMaxValue ) ||
                ( *(INT8*)&pabDataPtr[ i ] < psAdiEntry->uData.sSINT8.psValueProps->bMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;

         case ABP_UINT16:
            sValue.iValue = ( NetFormat == NET_BIGENDIAN ) ? iBeTOi( *(UINT16*)&pabDataPtr[ i*2 ] ) : iLeTOi( *(UINT16*)&pabDataPtr[ i*2 ] );

            if( ( sValue.iValue > psAdiEntry->uData.sUINT16.psValueProps->iMaxValue ) ||
                ( sValue.iValue < psAdiEntry->uData.sUINT16.psValueProps->iMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
         case ABP_SINT16:
            sValue.iValue = ( NetFormat == NET_BIGENDIAN ) ? iBeTOi( *(UINT16*)&pabDataPtr[ i*2 ] ) : iLeTOi( *(UINT16*)&pabDataPtr[ i*2 ] );

            if( ( sValue.iSignedValue > psAdiEntry->uData.sSINT16.psValueProps->iMaxValue ) ||
                ( sValue.iSignedValue < psAdiEntry->uData.sSINT16.psValueProps->iMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
         case ABP_UINT32:
            sValue.lValue = ( NetFormat == NET_BIGENDIAN ) ? lBeTOl( *(UINT32*)&pabDataPtr[ i*4 ] ) : lLeTOl( *(UINT32*)&pabDataPtr[ i*4 ] );

            if( ( sValue.lValue > psAdiEntry->uData.sUINT32.psValueProps->lMaxValue ) ||
                ( sValue.lValue < psAdiEntry->uData.sUINT32.psValueProps->lMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
         case ABP_SINT32:
            sValue.lValue = ( NetFormat == NET_BIGENDIAN ) ? lBeTOl( *(UINT32*)&pabDataPtr[ i*4 ] ) : lLeTOl( *(UINT32*)&pabDataPtr[ i*4 ] );

            if( ( sValue.lSignedValue > psAdiEntry->uData.sSINT32.psValueProps->lMaxValue ) ||
                ( sValue.lSignedValue < psAdiEntry->uData.sSINT32.psValueProps->lMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
         case ABP_FLOAT:
            sValue.lValue = ( NetFormat == NET_BIGENDIAN ) ? lBeTOl( *(UINT32*)&pabDataPtr[ i*4 ] ) : lLeTOl( *(UINT32*)&pabDataPtr[ i*4 ] );

            if( ( sValue.rValue > psAdiEntry->uData.sFLOAT.psValueProps->rMaxValue ) ||
                ( sValue.rValue < psAdiEntry->uData.sFLOAT.psValueProps->rMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
#ifdef ABCC_USER_64BIT_ADI_SUPPORT
         case ABP_UINT64:
            sValue.l64Value = ( NetFormat == NET_BIGENDIAN ) ? lBeTOl64( *(UINT64*)&pabDataPtr[ i*8 ] ) : lLeTOl64( *(UINT64*)&pabDataPtr[ i*8 ] );

            if( ( sValue.l64Value > psAdiEntry->uData.sUINT64.psValueProps->lMaxValue ) ||
                ( sValue.l64Value < psAdiEntry->uData.sUINT64.psValueProps->lMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
         case ABP_SINT64:
            sValue.l64Value = ( NetFormat == NET_BIGENDIAN ) ? lBeTOl64( *(UINT64*)&pabDataPtr[ i*8 ] ) : lLeTOl64( *(UINT64*)&pabDataPtr[ i*8 ] );

            if( ( sValue.l64SignedValue > psAdiEntry->uData.sSINT64.psValueProps->lMaxValue ) ||
                ( sValue.l64SignedValue < psAdiEntry->uData.sSINT64.psValueProps->lMinValue ) )
            {
               fCheckFailed = TRUE;
            }
            break;
#endif
         }

         if( fCheckFailed )
         {
            break;
         }
      }
   }

   /*
   ** Update iSetSize.
   */
   iSetSize = ( ABCC_GetDataTypeSize( psAdiEntry->bDataType ) * sArrayLimits.bNumElements );

   if( !fCheckFailed )
   {
      /*
      ** Copy data to the local memory.
      */
      for( i = 0; i < sArrayLimits.bNumElements; i++ )
      {
         switch( psAdiEntry->bDataType )
         {
         case ABP_BOOL:
         case ABP_CHAR:
         case ABP_UINT8:
         case ABP_SINT8:
         case ABP_ENUM:
            psAdiEntry->uData.sUINT8.pbValuePtr[ i + sArrayLimits.bStartIndex ] = pabDataPtr[ i ];
            break;
         case ABP_UINT16:
         case ABP_SINT16:
            psAdiEntry->uData.sUINT16.piValuePtr[ i + sArrayLimits.bStartIndex ] = ( NetFormat == NET_BIGENDIAN ) ? iBeTOi( *(UINT16*)&pabDataPtr[ i*2 ] ) : iLeTOi( *(UINT16*)&pabDataPtr[ i*2 ] );
            break;
         case ABP_UINT32:
         case ABP_SINT32:
         case ABP_FLOAT:
            psAdiEntry->uData.sUINT32.plValuePtr[ i + sArrayLimits.bStartIndex ] = ( NetFormat == NET_BIGENDIAN ) ? lBeTOl( *(UINT32*)&pabDataPtr[ i*4 ] ) : lLeTOl( *(UINT32*)&pabDataPtr[ i*4 ] );
            break;
#ifdef ABCC_USER_64BIT_ADI_SUPPORT
         case ABP_UINT64:
         case ABP_SINT64:
            psAdiEntry->uData.sUINT64.plValuePtr[ i + sArrayLimits.bStartIndex ] = ( NetFormat == NET_BIGENDIAN ) ? lBeTOl64( *(UINT64*)&pabDataPtr[ i*8 ] ) : lLeTOl64( *(UINT64*)&pabDataPtr[ i*8 ] );
            break;
#endif
         default:
            while(1);


         }
      }

      if( fPdRead )
      {
         /*
         ** NOTE! Error check not implemented.
         ** This is a successful RDPD update. Clear the error.
         */
      }
   }
   else if( fPdRead )
   {
      /*
      ** NOTE! Error check not implemented.
      ** This is an unsuccessful RDPD update. Indicate mapping error.
      */
   }
   else
   {
      iSetSize = -1; /* Indicate an error in the response message. */
   }

   return( iSetSize );
}


/*------------------------------------------------------------------------------
** AD_UpdatePdReadData()
**------------------------------------------------------------------------------
*/
void AD_UpdatePdReadData( UINT8* pabPdDataBuf )
{
   UINT16 i;
   const UINT16* AD_paiPdReadMap = AD_ReadMapInfo.paiMappedAdiList;
   const UINT16 iNumMappedAdi = AD_ReadMapInfo.AD_NumMappedAdi;

   if( AD_paiPdReadMap )
   {
      for ( i = 0; i < iNumMappedAdi; i++)
      {
         pabPdDataBuf += AD_SetAdiValue( &AD_asADIEntryList[ AD_paiPdReadMap[ i ] ], pabPdDataBuf, &AD_ReadMapInfo.asElementInfo[i] , TRUE );
         /*
         ** Note! Error check not implemented.
         ** Performing the limit error check.
         */
      }

   }
}


/*------------------------------------------------------------------------------
** AD_UpdatePdWriteData()
**------------------------------------------------------------------------------
*/
BOOL AD_UpdatePdWriteData( UINT8* pabPdDataBuf )
{
   UINT16 i;
   const UINT16* AD_paiPdWriteMap = AD_WriteMapInfo.paiMappedAdiList;
   const UINT16 iNumMappedAdi = AD_WriteMapInfo.AD_NumMappedAdi;

   if( AD_paiPdWriteMap && AD_fNewWrPd )
   {
      AD_fNewWrPd = FALSE;


      for( i = 0; i < iNumMappedAdi ; i++)
      {
#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED
         pabPdDataBuf += AD_GetAdiValue( &AD_asADIEntryList[ AD_paiPdWriteMap[ i ] ], pabPdDataBuf, &AD_WriteMapInfo.asElementInfo[i] );
#else
         pabPdDataBuf += AD_GetAdiValue( &AD_asADIEntryList[ AD_paiPdWriteMap[ i ] ], pabPdDataBuf, NULL );
#endif
      }
      return TRUE;
   }
   return FALSE;
}

void AD_NewWrPd(  void )
{
   AD_fNewWrPd   = TRUE;
}


/*------------------------------------------------------------------------------
** AD_AdiMappingReq()
**------------------------------------------------------------------------------
*/
void AD_AdiMappingReq(   const AD_AdiEntryType** ppsAdiEntry,
                              const AD_DefaultMapType** ppsDefaultMap )
{
   AD_Init();
   *ppsAdiEntry = ADI_GetADIEntryList();
   *ppsDefaultMap = ADI_GetDefaultMap();
}



/*------------------------------------------------------------------------------
** AD_RemapProcessDataCommand()
**------------------------------------------------------------------------------
*/

#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED
static UINT16 AD_RemapProcessDataCommand( ABP_MsgType* psMsg,
                                          const UINT16 iCurrNumMappedAdi,
                                          UINT16* piCurrMap,
                                          AD_ArrayInfoType* piCurrElemInfo,
                                          UINT16* piNewMap,
                                          AD_ArrayInfoType* piNewElemInfo,
                                          UINT16 *piNewNumAdi,
                                          UINT16 iMaxMappedAdi)
{

   UINT16  iNumAdi;
   UINT16  iItemsToRemove;
   UINT16  iItemsToAdd;
   UINT16  iMapIndex;
   UINT16  iAdiNbr;
   UINT16  bStartOfRemap;
   UINT16  iPdSize;


   /*
   ** A lot of sanity checks first since all actions of the command shall
   ** either be carried out or rejected.
   */
   if( psMsg->sHeader.iDataSize < 4 )
   {
      /*
      ** Not enough data provided
      */
      ABP_SetMsgErrorResponse( psMsg, 1, ABP_ERR_NOT_ENOUGH_DATA );
      return 0xFFFF;
   }

   bStartOfRemap = iLeTOi( *(UINT16*)&psMsg->sHeader.bCmdExt0 );

   if( bStartOfRemap > iCurrNumMappedAdi )
   {
      /*
      ** Not an allowed mapping number
      */
      ABP_SetMsgErrorResponse( psMsg, 1, ABP_ERR_INV_CMD_EXT_0 );
      return 0xFFFF;
   }

   iItemsToRemove = iLeTOi( *((UINT16*)psMsg->abData ) );

   if( bStartOfRemap + iItemsToRemove > iCurrNumMappedAdi )
   {
      /*
      ** Cannot remove more than currently is mapped
      */
      ABP_SetMsgErrorResponse( psMsg, 1, ABP_ERR_OUT_OF_RANGE );
      return 0xFFFF;
   }

   iItemsToAdd = iLeTOi( *((UINT16*)&psMsg->abData[ 2 ] ) );

   if( bStartOfRemap + iItemsToAdd - iItemsToRemove  > iMaxMappedAdi )
   {
      /*
      ** This will result in more maps than we can handle
      */
      ABP_SetMsgErrorResponse( psMsg, 1, ABP_ERR_NO_RESOURCES );
      return 0xFFFF;
   }

   if( psMsg->sHeader.iDataSize < 4 + ( iItemsToAdd * 4 ) )
   {
      ABP_SetMsgErrorResponse( psMsg, 1, ABP_ERR_NOT_ENOUGH_DATA );
      return 0xFFFF;
   }

   if( psMsg->sHeader.iDataSize > 4 + ( iItemsToAdd * 4 ) )
   {
      ABP_SetMsgErrorResponse( psMsg, 1, ABP_ERR_TOO_MUCH_DATA );
      return 0xFFFF;
   }

   /*
    * End checking
    */
   iPdSize = 0;
   iNumAdi = 0;
   for( iMapIndex = 0; iMapIndex < bStartOfRemap; iMapIndex++ )
   {

      piNewMap[iNumAdi] = piCurrMap[iNumAdi];
      piNewElemInfo[iNumAdi] = piCurrElemInfo[iNumAdi];

      iPdSize += ABCC_GetDataTypeSize( AD_asADIEntryList[ piNewMap[iNumAdi]].bDataType  * piNewElemInfo[iNumAdi].bNumElements );
      iNumAdi++;
   }

   for( iMapIndex = 0; iMapIndex < iItemsToAdd; iMapIndex++ )
   {
      iAdiNbr = *((UINT16*)&psMsg->abData[ 4 + ( iMapIndex * 4 ) ] );
      piNewMap[iNumAdi] = AD_GetAdiIndex( iAdiNbr );

      if( piNewMap[iNumAdi] == 0xFFFF )
      {
         ABP_SetMsgErrorResponse( psMsg, 1, ABP_ERR_OUT_OF_RANGE );
         return 0xFFFF;
      }

      piNewElemInfo[iNumAdi].bStartIndex = psMsg->abData[ 6 + ( iMapIndex * 4 ) ];
      piNewElemInfo[iNumAdi].bNumElements = psMsg->abData[ 7 + ( iMapIndex * 4 ) ];

      iPdSize += ABCC_GetDataTypeSize( AD_asADIEntryList[ piNewMap[iNumAdi]].bDataType * piNewElemInfo[iNumAdi].bNumElements );

      if( ( piNewElemInfo[iNumAdi].bStartIndex
          + piNewElemInfo[iNumAdi].bNumElements )
         > ( AD_asADIEntryList[ iNumAdi ].bNumOfElements ) ) /* Total amount of elements */
      {
         /*
         ** ADI does not have that many elements
         */
         ABP_SetMsgErrorResponse( psMsg, 2, ABP_ERR_OBJ_SPECIFIC );
         psMsg->abData[ 1 ] = 0x02; /* ABP_APPD_ERR_INVALID_NUM_ELEMENTS */
         return 0xFFFF;

      }
      iNumAdi++;
   }

   for( iMapIndex = bStartOfRemap + iItemsToRemove ; iMapIndex < iCurrNumMappedAdi  ; iMapIndex++ )
   {
      piNewMap[ iNumAdi ] = piCurrMap[ iMapIndex ];
      piNewElemInfo[ iNumAdi ] = piCurrElemInfo[ iMapIndex ];
      iPdSize += ABCC_GetDataTypeSize( AD_asADIEntryList[ piNewMap[iNumAdi]].bDataType ) * piNewElemInfo[iNumAdi].bNumElements;
      iNumAdi++;
   }

   /*
   ** Build a success message with the new size as the response data
   */
   *((UINT16*)psMsg->abData) = iTOiLe( iPdSize );
   ABP_SetMsgResponse( psMsg, 2 );

   *piNewNumAdi = iNumAdi;
   return iPdSize;
;

} /* end of appd_RemapProcessDataCommand() */


void AD_RemapDone ( void )
{
   /*
   ** Start to use new map
   */
   if ( bLatestRemapCommand == ABP_APPD_REMAP_ADI_WRITE_AREA )
   {
      AD_WriteMapInfo = AD_RemapWriteMapInfo;
   }

   if ( bLatestRemapCommand == ABP_APPD_REMAP_ADI_READ_AREA )
   {
      AD_ReadMapInfo = AD_RemapReadMapInfo;
   }

   AD_NewWrPd();
}

#endif







/*******************************************************************************
** End of ad_obj.c
********************************************************************************
*/
