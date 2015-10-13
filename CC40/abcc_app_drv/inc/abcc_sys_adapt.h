
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
** Defines system specific interface.
********************************************************************************
********************************************************************************
** Services:
**       ABCC_SYS_AbccInterruptEnable         - Enable interrupts.
**       ABCC_SYS_AbccInterruptDisable        - Disable interrupts.
**       ABCC_SYS_HWReset()                   - Puts Anybus HW into reset.
**       ABCC_SYS_HWReleaseReset              - Pulls Anybus HW out of reset.
**       ABCC_SYS_ReadModuleId                - Read Module Identification pins from the ABCC interface.
**       ABCC_SYS_SetOpmode                   - Sets ABCC Operating Mode pins from the ABCC interface
**       ABCC_SYS_GetOpmode                   - Reads ABCC Operating Mode from hardware
**       ABCC_SYS_ModuleDetect                - Detects if a module is present by reading the Module Detection pins.
**       ABCC_SYS_CopyMemBytes()              - Copy a number of bytes, from the source pointer to the destination pointer.
**       ABCC_SYS_UseCritical()               - Used if any preparation is needed before calling "ABCC_SYS_EnterCritical()".
**       ABCC_SYS_EnterCritical()             - Disables all interrupts, if they are not already disabled.
**       ABCC_SYS_ExitCritical()              - Restore interrupts to the state they were in when "ABCC_SYS_EnterCritical()" was called.
**       ABCC_SYS_Init()                      - Hardware or system dependent initialization.
**       ABCC_SYS_Close()                     - Close or free all resources allocated in ABCC_SYS_Init
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_SYS_ADAPT
#define ABCC_SYS_ADAPT
#include "abcc_user_def.h"
#include "abcc_td.h"



/*******************************************************************************
** Defines
********************************************************************************
*/

/*******************************************************************************
** Public Services Definitions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Enable the ABCC HW interrupt (IRQ_N pin on the application interface )
** This function will be called by the driver when the ABCC interrupt shall be
** enabled.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_AbccInterruptEnable( void );


/*------------------------------------------------------------------------------
** Disable ABCC HW interrupt (IRQ_N pin on the application interface )
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_AbccInterruptDisable( void );


/*------------------------------------------------------------------------------
** Reset ABCC. Set the reset pin on the ABCC interface to low.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_HWReset( void );


/*------------------------------------------------------------------------------
** Release reset of ABCC. Sets the reset pin on the ABCC_ interface to high.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**-------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_HWReleaseReset( void );


/*------------------------------------------------------------------------------
** Read Module Identification pins on the host connector.
** If the identification pins are not connected this function must return 
** the correct value corresponding to the used device.
**          0  0 ( 0)  Active CompactCom 30-series
**          0  1 ( 1 ) Passive CompactCom
**          1  0 ( 2 ) Active CompactCom 40-series
**          1  1 ( 3 ) Customer specific
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Module Id
**
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_SYS_ReadModuleId( void );


/*------------------------------------------------------------------------------
** Sets ABCC Operating Mode pins on the ABCC interface. In case operating mode
** is fixed or set by dip switches this function could be left empty or used to
** verify that the expected operating mode.
**------------------------------------------------------------------------------
** Arguments:
**      bOpMode   - 1 SPI
**                - 2 Shift Register ( not supported )
**                - 3-6 Reserved
**                - 7 16 bit parallel
**                - 8 8 bit parallel
**                - 9 Serial 19.2 kbit/s
**                - 10 Serial 57.6 kbit/s
**                - 11 Serial 115.2 kbit/s
**                - 12 Serial 625 kbit/s
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_SetOpmode( UINT8 bOpMode );


/*------------------------------------------------------------------------------
** Read current ABCC Operating Mode from HW.
**------------------------------------------------------------------------------
** Arguments:
**          None.
** Returns:
**      bOpMode   - 1 SPI
**                - 2 Shift Register ( not supported )
**                - 3-6 Reserved
**                - 7 16 bit parallel
**                - 8 8 bit parallel
**                - 9 Serial 19.2 kbit/s
**                - 10 Serial 57.6 kbit/s
**                - 11 Serial 115.2 kbit/s
**                - 12 Serial 625 kbit/s
**
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_SYS_GetOpmode( void );


/*------------------------------------------------------------------------------
** Detects if a module is present by reading the Module Detection pins on the
** ABCC interface.
** If the detections pins are not connected this function must return true.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       TRUE if module is detected.
**       FALSE if module is not detected.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_SYS_ModuleDetect( void );


/*------------------------------------------------------------------------------
** ABCC_SYS_MemCpy()
** Copy a number of bytes, from the source pointer to the destination pointer.
** This function can be modified to use performance enhancing platform specific
** instructions. The default implementation is memcpy().
** 
**------------------------------------------------------------------------------
** Arguments:
**    pbDestinationPtr     - Pointer to the destination.
**    pbSourcePtr          - Pointer to source data.
**    iNbrOfBytes          - The number of bytes that shall be copied.
**
** Returns:
**    None.                -
**
**------------------------------------------------------------------------------
*/
#ifndef ABCC_SYS_MemCpy
#include "string.h"
#define ABCC_SYS_MemCpy( pbDestinationPtr, pbSourcePtr, iNbrOfBytes ) memcpy(pbDestinationPtr, pbSourcePtr, iNbrOfBytes)
#endif


/*------------------------------------------------------------------------------
** ABCC_SYS_UseCritical()
** If any preparation is needed before calling "ABCC_SYS_EnterCritical()" or
** "ABCC_SYS_ExitCritical()" this macro is used to add HW specific necessities.
** This could for example be a local variable to store the current interrupt
** status. If critical sections are used in a nested way this macro need to
** be used.
** Ex.
**
** void DoSomething( void )
** {
**   UINT8 x;
**   ABCC_SYS_UseCritical();
**
**   ABCC_SYS_EnterCritical()
**   DoMoreStuff();
**   ABCC_SYS_ExitCritical()
**
** }
**
** void DoMoreStuff( void )
** {
**   UINT8 x;
**   ABCC_SYS_UseCritical();
**
**   ABCC_SYS_EnterCritical()
**   Do stuff
**   ABCC_SYS_ExitCritical()
** }
**
**
**
**------------------------------------------------------------------------------
*/
#ifndef ABCC_SYS_UseCritical 
#define ABCC_SYS_UseCritical() ABCC_SYS_UseCriticalImpl()
EXTFUNC void ABCC_SYS_UseCriticalImpl( void );
#endif

/*------------------------------------------------------------------------------
** ABCC_SYS_EnterCritical()
** This  function is called by the driver when there is a possibility of
** internal resource conflicts between the ABCC interrupt handler and the
** application thread or main loop. The function temporary disables interrupts
** to avoid conflict. Note that all interrupts that could lead to a driver
** access need to be disabled. If no interrupts are used and the driver is
** accessed by a single thread or main loop there is no need to implement this
** function. This function could also protect from conflicts caused by different
** threads accessing the driver at the same time by disabling all interrupts.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_SYS_EnterCritical
#define ABCC_SYS_EnterCritical() ABCC_SYS_EnterCriticalImpl()
EXTFUNC void ABCC_SYS_EnterCriticalImpl( void );
#endif

/*------------------------------------------------------------------------------
** ABCC_SYS_ExitCritical()
** Restore interrupts to the state when "ABCC_SYS_EnterCritical()"
** was called.
**------------------------------------------------------------------------------
*/
#ifndef ABCC_SYS_ExitCritical
#define ABCC_SYS_ExitCritical() ABCC_SYS_ExitCriticalImpl()
EXTFUNC void ABCC_SYS_ExitCriticalImpl( void );
#endif


/*------------------------------------------------------------------------------
** This function is called by the driver at the beginning ABCC_StartDriver().
** If there is a need any hardware or system dependent initialization it shall be
** done here. Note that ABCC_StartDriver() will also be called during restart of
** the driver.
**------------------------------------------------------------------------------
** Arguments: -
**
** Returns:
**          TRUE - Initialization succeeded.
**          FALSE - Initialization failed.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_SYS_Init( void );


/*------------------------------------------------------------------------------
**  Called from driver at the end of ABCC_ShutDown(). Note that the driver could
**  be started  again by calling ABCC_StartDriver().
**------------------------------------------------------------------------------
** Arguments:
**
** Returns:
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SYS_Close( void );


/*------------------------------------------------------------------------------
** iBeTOi(), iTOiBe(), iLeTOi(), iTOiLe()
** Transforms a big/little endian integer into a native format integer and a
** native format integer to big/little endian integer.
** If ABCC_USER_BIG_ENDIAN is not defined LITLLE_ENDIAN macros will be used.
** There are two reasons that endian conversion macros are needed.
** 1. The ABCC interface uses little endian. If the application uses a big endian
** environment conversion is needed.
** 2. Process data is transferred transparently  from the newtwork to the host
** application. Depending on network, little or big endian is used and a
** conversion may be needed.
**------------------------------------------------------------------------------
** Inputs:
**    xBeFoo                  - a big endian integer to be transformed
**    xLeFoo                  - a little endian integer to be transformed
**    xFoo                    - a native format integer to be transformed
**
** Outputs:
**    xFoo                    - a native format integer
**    xBeFoo                  - a big endian integer
**    xLeFoo                  - a little endian integer
**
** Usage:
**    xFoo   = xBeTOx( xBeFoo );
**    xBeFoo = xTOxBe( xFoo );
**    xFoo   = xLeTOx( xLeFoo );
**    xLeFoo = xTOxLe( xFoo );
**------------------------------------------------------------------------------
*/
#ifdef ABCC_USER_BIG_ENDIAN
   #define iBeTOi( iBeFoo )    (UINT16)( iBeFoo )
   #define iTOiBe( iFoo )    (BeUINT16)( iFoo )
   #define iLeTOi( iLeFoo )    (UINT16)( ( ( ( iLeFoo ) & 0x00FF ) << 8 ) | ( ( ( iLeFoo ) & 0xFF00 ) >> 8 ) )
   #define iTOiLe( iFoo )    (LeUINT16)( ( ( ( iFoo )   & 0x00FF ) << 8 ) | ( ( ( iFoo )   & 0xFF00 ) >> 8 ) )

   #define lBeTOl( lBeFoo )    (UINT32)( lBeFoo )
   #define lTOlBe( lFoo )    (BeUINT32)( lFoo )
   #define lLeTOl( lLeFoo )    (UINT32)(  (((lLeFoo) & 0x000000FFL) << 24) | (((lLeFoo) & 0xFF000000L) >> 24)  | (((lLeFoo) & 0x0000FF00L) << 8)  | (((lLeFoo) & 0x00FF0000L) >> 8)  )
   #define lTOlLe( lFoo )    (LeUINT32)(  (((lFoo) & 0x000000FFL) << 24) | (((lFoo) & 0xFF000000L) >> 24)  | (((lFoo) & 0x0000FF00L) << 8)  | (((lFoo) & 0x00FF0000L) >> 8)  )

#ifdef ABCC_USER_64BIT_ADI_SUPPORT
   #define lBeTOl64( lBeFoo )  (UINT64)( lBeFoo )
   #define lTOlBe64( lFoo )    (UINT64)( lFoo )
   #define lLeTOl64( lLeFoo )  (UINT64)( ( (UINT64)lLeTOl( (UINT32)( lLeFoo & 0x00000000FFFFFFFFLL ) ) << 32 ) | (UINT64)lLeTOl( (UINT32)( ( lLeFoo & 0xFFFFFFFF00000000LL ) >> 32 ) ) )
   #define lTOlLe64( lFoo )    (UINT64)( ( (UINT64)lTOlLe( (UINT32)( lFoo   & 0x00000000FFFFFFFFLL ) ) << 32 ) | (UINT64)lTOlLe( (UINT32)( ( lFoo   & 0xFFFFFFFF00000000LL ) >> 32 ) ) )
#endif
#else
   #define iBeTOi( iBeFoo )    (UINT16)( ( ( ( iBeFoo ) & 0x00FF ) << 8 ) | ( ( ( iBeFoo ) & 0xFF00 ) >> 8 ) )
   #define iTOiBe( iFoo )    (BeUINT16)( ( ( ( iFoo )   & 0x00FF ) << 8 ) | ( ( ( iFoo )   & 0xFF00 ) >> 8 ) )
   #define iLeTOi( iLeFoo )    (UINT16)( iLeFoo )
   #define iTOiLe( iFoo )    (LeUINT16)( iFoo )

   #define lBeTOl( lBeFoo )   (UINT32)( (((lBeFoo) & 0x000000FFL) << 24) | (((lBeFoo) & 0xFF000000L) >> 24)  | (((lBeFoo) & 0x0000FF00L) << 8)  | (((lBeFoo) & 0x00FF0000L) >> 8) )
   #define lTOlBe( lFoo )     (BeUINT32)( (((lFoo) & 0x000000FFL) << 24) | (((lFoo) & 0xFF000000L) >> 24)  | (((lFoo) & 0x0000FF00L) << 8)  | (((lFoo) & 0x00FF0000L) >> 8)  )
   #define lLeTOl( lLeFoo )    (UINT32)( lLeFoo )
   #define lTOlLe( lFoo )    (LeUINT32)( lFoo )

#ifdef ABCC_USER_64BIT_ADI_SUPPORT
   #define lBeTOl64( lBeFoo )  (UINT64)( ( (UINT64)lBeTOl( (UINT32)( lBeFoo & 0x00000000FFFFFFFFLL ) ) << 32 ) | (UINT64)lBeTOl( (UINT32)( ( lBeFoo & 0xFFFFFFFF00000000LL ) >> 32 ) ) )
   #define lTOlBe64( lFoo )    (UINT64)( ( (UINT64)lTOlBe( (UINT32)( lFoo   & 0x00000000FFFFFFFFLL ) ) << 32 ) | (UINT64)lTOlBe( (UINT32)( ( lFoo   & 0xFFFFFFFF00000000LL ) >> 32 ) ) )
   #define lLeTOl64( lLeFoo )  (UINT64)( lLeFoo )
   #define lTOlLe64( lFoo )    (UINT64)( lFoo )
#endif

#endif



#endif  /* inclusion lock */



/*******************************************************************************
**
** End of sys_adpt.h
**
********************************************************************************
*/
