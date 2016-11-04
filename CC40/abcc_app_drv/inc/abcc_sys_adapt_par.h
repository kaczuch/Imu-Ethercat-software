/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
**                                                                            **
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, this code can be   **
** modified, reproduced and distributed in binary form without any            **
** restrictions.                                                              **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
**  Target dependent interface for parallel 8 / 16 operation mode.
********************************************************************************
********************************************************************************
** Services:
**       ABCC_SYS_ParallelRead()     - Reads an amount of bytes from the ABCC
**       ABCC_SYS_ParallelRead8()    - Reads a byte from the ABCC
**       ABCC_SYS_ParallelRead16()   - Reads a word from the ABCC
**
**       ABCC_SYS_ParallelWrite()    - Writes an amount of bytes
**       ABCC_SYS_ParallelWrite8()   - Writes a byte to the ABCC
**       ABCC_SYS_ParallelWrite16()  - Writes a word to the ABCC
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_SYS_ADAPT_PARI_
#define ABCC_SYS_ADAPT_PARI_
#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abp.h"

/*******************************************************************************
**
** Defines
**
********************************************************************************
*/

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
** Public Services
********************************************************************************
*/



/*------------------------------------------------------------------------------
** ABCC_SYS_ParallelRead()
** Reads an amount of bytes from the ABCC memory.
** This function will be used by the driver when reading process data or message
** data from the ABCC memory.
** See also the ABCC_SYS_READ_RDPD macro description below.
**------------------------------------------------------------------------------
** Arguments:
**    iMemOffset  - Memory offset to start reading from.
**    pbData      - Pointer where the read data should be stored.
**    iLength     - The amount of data to be read.
** Returns:
**        -
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_ParallelRead( UINT16 iMemOffset, UINT8* pbData, UINT16 iLength );


/*******************************************************************************
**  ABCC_SYS_ParallelRead8()
**  Reads a byte from the ABCC memory.
**  In case of a memory mapped system this function does not need not be
**  implemented. See ABCC_SYS_READ8 macro description below.
********************************************************************************
** Arguments:
**          iMemOffset: Offset from ABCC base address.
** Returns:
**          Read UINT8
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset );

/*******************************************************************************
**  ABCC_SYS_ParallelRead16()
**  Reads a word from the ABCC memory.
**  In case of a memory mapped system this function does not need not be
**  implemented. See ABCC_SYS_READ16 macro description below.
********************************************************************************
** Arguments:
**          iMemOffset: Offset from ABCC base address
** Returns:
**          Read UINT16
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset );


/*******************************************************************************
**  ABCC_SYS_READ8, ABCC_SYS_READ16
**  The driver will use the ABCC_SYS_READ8 and ABCC_SYS_READ16 macros to access the ABCC
**  registers. In case of a memory mapped system (ABCC_USER_MEMORY_MAPPED_ACCESS
**  is defined ) an direct memory access will be done using
**  ABCC_USER_PARALLEL_BASE_ADR to calculate the absolute address.
**  In case of a non memory mapped system the ABCC_SYS_ParallelRead8/16 will be
**  called.
********************************************************************************
** Arguments:
**          iMemOffset: Offset from ABCC base address.
** Returns:
**          Read UINT8/UINT16
**------------------------------------------------------------------------------
*/

#ifdef ABCC_USER_MEMORY_MAPPED_ACCESS
#define ABCC_SYS_READ8( iMemOffset ) *(volatile UINT8*)( ABCC_USER_PARALLEL_BASE_ADR + ( iMemOffset ) )
#define ABCC_SYS_READ16( iMemOffset ) *(volatile UINT16*)( ABCC_USER_PARALLEL_BASE_ADR + ( iMemOffset ) )
#else
#define ABCC_SYS_READ8( iMemOffset ) ABCC_SYS_ParallelRead8( iMemOffset )
#define ABCC_SYS_READ16( iMemOffset ) ABCC_SYS_ParallelRead16( iMemOffset )
#endif


/*------------------------------------------------------------------------------
** ABCC_SYS_ParallelWrite()
** Writes an amount of bytes to the ABCC memory
** This function will be used by the driver when writing process data or message
** data to the ABCC memory.
** See also the ABCC_SYS_WRITE_WRPD macro description below.
**------------------------------------------------------------------------------
** Inputs:
**    iMemgOffset    - Memory offset to start writing to.
**    pbData    - Pointer to the data to be written.
**    iLength    - The amount of data to write.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, UINT8* pbData, UINT16 iLength );

/*******************************************************************************
**  ABCC_SYS_ParallelWrite8()
**  Writes a byte to the ABCC memory.
**  In case of a memory mapped system this function does not need not be
**  implemented. See ABCC_SYS_WRITE8 macro description below.
********************************************************************************
** Arguments:
**          iMemOffset: Offset from ABCC base address.
**          pbData: Data to be written to ABCC
** Returns:
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 pbData );

/*******************************************************************************
**  ABCC_SYS_ParallelWrite16()
**  Writes a word to the ABCC memory.
**  In case of a memory mapped system this function does not need not be
**  implemented. See ABCC_SYS_WRITE16 macro description below.
********************************************************************************
** Arguments:
**          iMemOffset: Offset from ABCC base address.
**          pbData: Data to be written to ABCC
** Returns:
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 pbData );


/*******************************************************************************
**  ABCC_SYS_WRITE8, ABCC_SYS_WRITE16
**  The driver will use the ABCC_SYS_WRITE8 and ABCC_SYS_WRITE16 macros to access the ABCC
**  registers. In case of a memory mapped system
**  ( MEMORY_MAPPED_ACCESS is defined ) an direct memory access will be done
**  using ABCC_USER_PARALLEL_BASE_ADR to calculate the absolute address. In case
**  of a non memory mapped system the ABCC_SYS_ParallelWrite8/16 will be called.
********************************************************************************
** Arguments:
**          iMemOffset: Offset from ABCC base address.
**          pbData: Data to be written to ABCC
** Returns:
**------------------------------------------------------------------------------
*/
#ifdef ABCC_USER_MEMORY_MAPPED_ACCESS
#define ABCC_SYS_WRITE8( iMemOffset, pbData ) *(volatile UINT8*)( ABCC_USER_PARALLEL_BASE_ADR + ( iMemOffset ) ) = pbData
#define ABCC_SYS_WRITE16( iMemOffset, pbData ) *(volatile UINT16*)( ABCC_USER_PARALLEL_BASE_ADR + ( iMemOffset ) ) = pbData
#else
#define ABCC_SYS_WRITE8( iMemOffset, pbData ) ABCC_SYS_ParallelWrite8(iMemOffset, pbData )
#define ABCC_SYS_WRITE16( iMemOffset, pbData ) ABCC_SYS_ParallelWrite16(iMemOffset, pbData )
#endif



/*------------------------------------------------------------------------------
** ABCC_SYS_ParallelGetRdPdBuffer()
** Get the address to the received read process data.
** For a memory mapped system a pointer to the ABCC read process data
** memory will be provided where the process data could be read directly by the
** driver.
** For a non memory mapped system the system adaption layer need to provide a
** buffer where the read process data can be stored.
**------------------------------------------------------------------------------
** Argument:
** Returns:
**    Address to RdPdBuffer.
**
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8* ABCC_SYS_ParallelGetRdPdBuffer( void );


/*------------------------------------------------------------------------------
** ABCC_SYS_READ_RDPD()
** This macro will be used by the driver to read process data. In case of a
** memory mapped system the macro will be empty since the buffer already contains
** the received data. ( See  ABCC_SYS_ParallelGetRdPdBuffer ).
** For a non memory mapped system ABCC_SYS_ParallelRead will be called with the buffer
** received from ABCC_SYS_ParallelGetRdPdBuffer()
** The buffer will be valid until the next read process data read is done.
** For ABCC30 ( ABCC_USER_DRV_PARALLEL_30 defined ) the legacy address offset is used.
**------------------------------------------------------------------------------
** Arguments:
**          buffer: Pointer where to store received process data
**          size: Size of read process data
** Returns:
**------------------------------------------------------------------------------
*/
#ifdef ABCC_USER_MEMORY_MAPPED_ACCESS
#define ABCC_SYS_READ_RDPD( buffer, size )
#else
#ifdef ABCC_USER_DRV_PARALLEL_30
#define ABCC_SYS_READ_RDPD( buffer, size ) ABCC_SYS_ParallelRead( ABP_RDPDM_ADR_OFFSET, buffer, size )
#else
#define ABCC_SYS_READ_RDPD( buffer, size ) ABCC_SYS_ParallelRead( ABP_RDPD_ADR_OFFSET, buffer, size )

#endif
#endif


/*------------------------------------------------------------------------------
** ABCC_SYS_ParallelGetWrPdBuffer()
** Get the address to store the write process data.
** For a memory mapped system a pointer to the ABCC write process data
** memory will be provided where the process data could be stored directly by
** the driver.
** For a non memory mapped system the system adaption layer need to provide a
** buffer where the write process data can be stored.
**------------------------------------------------------------------------------
** Argument:
** Returns:
**    Address to WrPdBuffer
**
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8* ABCC_SYS_ParallelGetWrPdBuffer( void );


/*------------------------------------------------------------------------------
** ABCC_SYS_WRITE_WRPD()
** This macro will be used by the driver to write process data. In case of a
** memory mapped system the macro will be empty since the buffer already
** contains the written data. ( See  ABCC_SYS_ParallelGetWrPdBuffer ).
** For a non memory mapped system ABCC_SYS_ParallelWrite will be called with the
** buffer received from ABCC_SYS_ParallelGetWrPdBuffer().
** When function has returned new process data can be written to the buffer.
** For ABCC30 ( ABCC_USER_DRV_PARALLEL_30 defined ) the legacy address offset is
** used.
**------------------------------------------------------------------------------
** Arguments:
**          buffer: Pointer to write process data
**          size: Size of write process data
** Returns:
**------------------------------------------------------------------------------
*/
#ifdef ABCC_USER_MEMORY_MAPPED_ACCESS
#define ABCC_SYS_WRITE_WRPD( buffer, size ) ABCC_SYS_ParallelWrite( ABP_WRPDM_ADR_OFFSET, buffer, size )
#else
#ifdef ABCC_USER_DRV_PARALLEL_30
#define ABCC_SYS_WRITE_WRPD( buffer, size ) ABCC_SYS_ParallelWrite( ABP_WRPDM_ADR_OFFSET, buffer, size )
#else
#define ABCC_SYS_WRITE_WRPD( buffer, size ) ABCC_SYS_ParallelWrite( ABP_WRPD_ADR_OFFSET, buffer, size )
#endif

#endif


#endif  /* inclusion lock */
