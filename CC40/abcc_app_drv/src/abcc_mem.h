/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** file_description
********************************************************************************
********************************************************************************
** Services:
** ABCC_MemCreatePool()        - Create memory resource pool
** ABCC_MemAlloc()             - Alloc memory resource
** ABCC_MemFree()              - Free memory resource
**
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_MEM_H_
#define ABCC_MEM_H_
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
 * Memory pool structure
 *
 * ------------------
 * iNumFreeItems = 3|
 * ------------------
 * Item 0 pointer   |---|
 * ------------------   |
 * Item 1 pointer   |---+--|
 * ------------------   |  |
 * Item 2 pointer   |---+--+--|
 * ------------------   |  |  |
 * Item 0           |<--|  |  |
 * ------------------      |  |
 * Item 1           |<-----|  |
 * ------------------         |
 * Item 2           |<--------|
 * ------------------
 */
typedef struct
{
   UINT16 iNumFreeItems;
   UINT8* pxFreeItemStack[1]; /* Dynamic length */
} ABCC_MemHandlerType;

/*
 * Required pool size in bytes
 */
#define ABCC_MEM_REQUIRED_POOLSIZE(iItemSize, iNumItems ) ( sizeof(ABCC_MemHandlerType) + ( iNumItems ) * sizeof(UINT8*) + sizeof( UINT8 )* (iItemSize) * (iNumItems) )


/*******************************************************************************
** Public Globals
********************************************************************************
*/
/*******************************************************************************
** Public Services
********************************************************************************
*/


/*------------------------------------------------------------------------------
** Creates a memory pool of buffers with a specific size. The memory is provided
** by the  user.
**------------------------------------------------------------------------------
** Arguments:
**          pxMem: Memory to create the pool in. Use privided ABCC_MEM_REQUIRED_POOLSIZE
**                 macro to calculate the required size.
** Returns:
**          Handle to the pool used as input used for alloc and free methods.
**          If the pool is empty NULL is returned.
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_MemHandlerType* ABCC_MemCreatePool(UINT8* pxMem, UINT16 iItemSize, UINT16 iNumItems );



/*------------------------------------------------------------------------------
** Allocates and return pointer to memory of predefined size (ABCC_MemCreatePool)
**------------------------------------------------------------------------------
** Arguments:
**         pxMemHandler: Handler for the pool where the memory is taken from
** Returns:
**          Pointer to allocated memory. NULL if pool is empty.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8* ABCC_MemAlloc(ABCC_MemHandlerType* pxMemHandler );

/*------------------------------------------------------------------------------
**  Return memory to the pool. Note that it is important that the returned memory
**  is belonging to the pool from the beginning.
**------------------------------------------------------------------------------
** Arguments:
**         pxMemHandler: Handler for the pool where the memory is returned.
**         pxItem:       Pointer the memory to be returned. The pointer is set
**                      to NULL.
** Returns:
**       -
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_MemFree(ABCC_MemHandlerType* pxMemHandler, UINT8** pxItem);




#endif  /* inclusion lock */
