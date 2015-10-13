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
** Common defines error codes and functions for abcc driver and application.
********************************************************************************
********************************************************************************
** Services :
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_COMMON_H_
#define ABCC_COMMON_H_

#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abp.h"
#include "abcc_sys_adapt.h"
#include "abcc.h"

typedef void (*ABCC_ErrorReporter)( ABCC_SeverityType eSeverity,
                                    ABCC_ErrorCodeType  iErrorCode,
                                    UINT32  lAddInfo );

void SetErrorReporter( ABCC_ErrorReporter nFunc );
EXTFUNC ABCC_ErrorReporter ErrorReporter;


/*------------------------------------------------------------------------------
** Help macros for debugging and error reporting.
**------------------------------------------------------------------------------
*/

/*
** Create constant strings for file and line information.
*/
#define STR( x )        #x
#define XSTR( x )       STR( x )
#define FileLine        "Line nr: " XSTR( __LINE__ ) "\nFile: " __FILE__

/*
** Error reporting.
*/
#ifdef ABCC_USER_ERR_REPORTING_ENABLED
 #define _ABCC_ERROR( eSeverity, iErrorCode, lAddInfo ) ErrorReporter( eSeverity, iErrorCode, lAddInfo )
#else
#define _ABCC_ERROR( eSeverity, iErrorCode, lAddInfo )
#endif

/*
** DEBUG Level macros.
*/
#ifdef ABCC_USER_DEBUG_EVENT_ENABLED
#define DEBUG_EVENT( args ) ABCC_USER_DEBUG_PRINT (args)
#else
#define DEBUG_EVENT( args )
#endif

#ifdef ABCC_USER_DEBUG_ERR_ENABLED
#define DEBUG_ERR( args )   ABCC_USER_DEBUG_PRINT (args)
#else
#define DEBUG_ERR( args )
#endif


#define ABCC_ASSERT( x ) if ( !( x ) ){                                        \
        DEBUG_ERR((FileLine));                                                   \
        _ABCC_ERROR( ABCC_SEV_FATAL , ABCC_EC_INTERNAL_ERROR, 0 );}

#define ABCC_ASSERT_ERR(x, eSeverity, iErrorCode, lAddInfo ) if (!( x )){      \
        DEBUG_ERR((FileLine));                                                   \
        _ABCC_ERROR( eSeverity , iErrorCode, lAddInfo );}

 #define ABCC_ERROR( eSeverity, iErrorCode, lAddInfo )                         \
         DEBUG_ERR((FileLine));                                                  \
        _ABCC_ERROR( eSeverity , iErrorCode, lAddInfo )




#endif

/*******************************************************************************
** End of abcc_common.h
********************************************************************************
*/
