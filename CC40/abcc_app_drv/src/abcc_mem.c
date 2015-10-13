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
** Memory allocation implementation.
********************************************************************************
********************************************************************************
*/
#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abcc_mem.h"
#include "abcc_sys_adapt.h"


/*******************************************************************************
** Constants
********************************************************************************
*/


/*******************************************************************************
** Typedefs
********************************************************************************
*/


/*******************************************************************************
** Public Globals
********************************************************************************
*/


/*******************************************************************************
** Private Globals
********************************************************************************
*/


/*******************************************************************************
** Private Services
********************************************************************************
*/


/*******************************************************************************
** Public Services
********************************************************************************
*/
ABCC_MemHandlerType* ABCC_MemCreatePool(UINT8* pxMem, UINT16 iItemSize, UINT16 iNumItems )
{
   UINT16 i;
   UINT8* pbItem;
   ABCC_MemHandlerType* psMemHandler;
   psMemHandler = (ABCC_MemHandlerType*)pxMem;
   psMemHandler->iNumFreeItems = iNumItems;

   pbItem = (UINT8*)( psMemHandler->pxFreeItemStack + iNumItems );

   for( i = 0;  i < iNumItems ; i++ )
   {
      psMemHandler->pxFreeItemStack[i] = pbItem;
      pbItem += iItemSize;

   }
   return psMemHandler;
}


UINT8* ABCC_MemAlloc(ABCC_MemHandlerType* pxMemHandler)
{
   UINT8* pxItem = NULL;
   ABCC_SYS_UseCritical();
   ABCC_SYS_EnterCritical();
   if ( pxMemHandler->iNumFreeItems > 0 )
   {
      pxMemHandler->iNumFreeItems--;
      pxItem = pxMemHandler->pxFreeItemStack[pxMemHandler->iNumFreeItems ];
   }
   ABCC_SYS_ExitCritical();
   return pxItem;
}


void ABCC_MemFree(ABCC_MemHandlerType* pxMemHandler, UINT8** pxItem)
{
   ABCC_SYS_UseCritical();
   if( *pxItem )
   {
      ABCC_SYS_EnterCritical();
      pxMemHandler->pxFreeItemStack[pxMemHandler->iNumFreeItems] = *pxItem;
      pxMemHandler->iNumFreeItems++;
      *pxItem = NULL;
      ABCC_SYS_ExitCritical();
   }
}


/*******************************************************************************
** Tasks
********************************************************************************
*/
