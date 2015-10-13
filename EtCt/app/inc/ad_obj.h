/*
 * ad_obj.h
 *
 *  Created on: 31 lip 2015
 *      Author: bkaczor
 */

#ifndef ETCT_INC_AD_OBJ_H_
#define ETCT_INC_AD_OBJ_H_
#include "abcc_td.h"
#include "abp.h"
#include "abcc_ad_if.h"
/*------------------------------------------------------------------------------
** ADI entry internal status bits (AD_AdiEntryType::bStatBits)
**
** AD_STATBIT_PD_READ_MAPPED        - Entry is mapped on read process data
** AD_STATBIT_PD_WRITE_MAPPED       - Entry is mapped on write process data
** AD_STATBIT_PD_READ_LIMIT_ERROR   - If set an read process data limit error exists on this entry
**------------------------------------------------------------------------------
*/
#define AD_STATBIT_PD_READ_MAPPED      0x01
#define AD_STATBIT_PD_WRITE_MAPPED     0x02
#define AD_STATBIT_PD_READ_LIMIT_ERROR 0x04


/*******************************************************************************
** Type Definitions
********************************************************************************
*/

/*
** Describes what elements in an array that is used for an ADI.
*/
typedef struct
{
   UINT8  bStartIndex;
   UINT8  bNumElements;
} AD_ArrayInfoType;


/*******************************************************************************
** Public Globals
********************************************************************************
*/


/*******************************************************************************
** Public Services Definitions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** AD_Init()
** Performe AD Init routines
**------------------------------------------------------------------------------
** Inputs:
**    None
**
** Outputs:
**    None
**
** Usage:
**    AD_Init();
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_Init( void );

/*------------------------------------------------------------------------------
**
** AD_AdiMappingReq()
**
**  Called by Anybus driver.
**  Retuns the pointers to the ADI list and default map list.
**
**------------------------------------------------------------------------------
**
** Inputs:
**    ppsAdiEntry          - The ADI list
**    ppsDefaultMap        - The default map
** Outputs:
**    None
**
** Usage:
**    AD_Init();
**
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_AdiMappingReq( const AD_AdiEntryType** ppsAdiEntry,
                                  const AD_DefaultMapType** ppsDefaultMap );

/*------------------------------------------------------------------------------
**
** AD_RemapDone()
**
** Called by Anybus driver. Indicates that the remap is finished.
**
**------------------------------------------------------------------------------
**
** Inputs:
**    None
** Outputs:
**    None
**
** Usage:
**    AD_RemapDone();
**
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_RemapDone( void );

/*------------------------------------------------------------------------------
** AD_GetAdiInstEntry()
** Called by Anybus driver to process an object request against the
** Application Data Object.
**------------------------------------------------------------------------------
** Inputs:
**    iInstance         - ADI instance number to fetch
**
** Outputs:
**    AD_AdiEntryTyp*   - Pointer to ADI entry;
**                        NULL if no entry was found
**
** Usage:
**    psAdiEntey = AD_GetAdiInstEntry( 1 );
**------------------------------------------------------------------------------
*/
EXTFUNC const AD_AdiEntryType* AD_GetAdiInstEntry( UINT16 iInstance );


/*------------------------------------------------------------------------------
** AD_GetAdiValue()
** Gets a specific value to an managed ADI entry.
**------------------------------------------------------------------------------
**  Arguments:
**    psAdiEntry     - Pointer to ADI entry
**    pabDataPtr     - Pointer to data
**
** Returns:
**    UINT16         - Size of get data in bytes
**------------------------------------------------------------------------------
*/
UINT16 AD_GetAdiValue( const AD_AdiEntryType* psAdiEntry, UINT8* pabDataPtr, AD_ArrayInfoType* const psArrayInfo );


/*------------------------------------------------------------------------------
** AD_SetAdiValue()
** Sets a specific value to an managed ADI entry. Function will execute
** value limit checks if setup.
**------------------------------------------------------------------------------
** Arguments:
**    psAdiEntry     - Pointer to ADI entry
**    pabDataPtr     - Pointer to data
**    fPdRead        - TRUE: Executed by Read process data from anybus
**                     FALSE: Ordinary execute
**
**  Returns:
**    INT16         - Size of set data (-1 if error)
**                     Note: If fPdReadData == TRUE the set size
**                           will always be returned.
**------------------------------------------------------------------------------
*/
INT16 AD_SetAdiValue( const AD_AdiEntryType* psAdiEntry, UINT8* pabDataPtr, AD_ArrayInfoType* const psArrayInfo, BOOL8 fPdRead );


/*------------------------------------------------------------------------------
** AD_ProcObjectRequest()
** Called by Anybus driver to process an object request against the
** Application Data Object.
**------------------------------------------------------------------------------
** Inputs:
**          None        - Works on the global driver buffers
**
** Outputs:
**          None        - Works on the global driver buffers
**
** Usage:
**          AD_ProcObjectRequest();
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_ProcObjectRequest( ABP_MsgType * psMsgBuffer );


/*------------------------------------------------------------------------------
** AD_UpdatePdReadData()
** Called by Anybus driver to update AD with new read process data received
** from Anybus
**------------------------------------------------------------------------------
** Inputs:
**    pabPdDataBuf   - Pointer to data buffer
**
** Outputs:
**    None
**
** Usage:
**    AD_UpdatePdReadData( abPdWriteBuffer );
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_UpdatePdReadData( UINT8* pabPdDataBuf );


/*------------------------------------------------------------------------------
** AD_UpdatePdWriteData()
** Called by Anybus driver to update the supplied buffer with the
** current write process data.
**------------------------------------------------------------------------------
** Inputs:
**    pabPdDataBuf   - Pointer to data buffer
**
** Outputs:
**    None
**
** Usage:
**    AD_UpdatePdWriteData( abPdWriteBuffer );
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL AD_UpdatePdWriteData( UINT8* pabPdDataBuf );


/*------------------------------------------------------------------------------
** AD_NewWrPd()
** Called by application when new write process data is available.
** Next time AD_UpdatePdWriteData() is is called the WrPd will be updated
** and sent to ABCC.
**------------------------------------------------------------------------------
** Inputs:
**    pabPdDataBuf   - Pointer to data buffer
**
** Outputs:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_NewWrPd(  void );





#endif /* ETCT_INC_AD_OBJ_H_ */
