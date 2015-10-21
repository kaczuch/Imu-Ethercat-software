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
** Anybus-CC Protocol Definitions for SERCOS III.
** This file contains network specific definitions used by the Anybus-CC
** SERCOS III module as well as applications designed to use such module.
********************************************************************************
********************************************************************************
** Services:
********************************************************************************
********************************************************************************
*/
#ifndef ABP_SRC3_H
#define ABP_SRC3_H


/*******************************************************************************
** Anybus-CC SERCOS III object constants.
** Object revision: 1.
********************************************************************************
*/

/*------------------------------------------------------------------------------
** The Anybus-CC SERCOS III Object instance attributes.
**------------------------------------------------------------------------------
*/
#define ABP_SRC3_COMPONENT_NAME              1
#define ABP_SRC3_IA_VENDOR_CODE              2
#define ABP_SRC3_IA_DEVICE_NAME              3
#define ABP_SRC3_IA_VENDOR_ID                4
#define ABP_SRC3_IA_SW_VERSION               5
#define ABP_SRC3_IA_SERIAL_NUMBER            6
#define ABP_SRC3_IA_MAJOR_DIAG_EV_LATCHING   7


/*------------------------------------------------------------------------------
** The data size of the Anybus-CC SERCOS III Object instance attributes (in bytes).
**------------------------------------------------------------------------------
*/
#define ABP_SRC3_COMPONENT_NAME_DS                 ( ABP_SRC3_TRUNC_VALUE * ABP_UINT8_SIZEOF )
#define ABP_SRC3_IA_VENDOR_CODE_DS                 ABP_UINT16_SIZEOF
#define ABP_SRC3_IA_DEVICE_NAME_DS                 ( ABP_SRC3_TRUNC_VALUE * ABP_UINT8_SIZEOF )
#define ABP_SRC3_IA_VENDOR_ID_DS                   ( ABP_SRC3_TRUNC_VALUE * ABP_UINT8_SIZEOF )
#define ABP_SRC3_IA_SW_VERSION_DS				      ( ABP_SRC3_TRUNC_VALUE * ABP_UINT8_SIZEOF )
#define ABP_SRC3_IA_SERIAL_NUMBER_DS               ( ABP_SRC3_TRUNC_VALUE * ABP_UINT8_SIZEOF )
#define ABP_SRC3_IA_MAJOR_DIAG_EV_LATCHING_DS      ABP_BOOL_SIZEOF


/*------------------------------------------------------------------------------
** The Anybus-CC SERCOS III Object specific message commands.
**------------------------------------------------------------------------------
*/
#define ABP_SRC3_TRUNC_VALUE                 32


/*------------------------------------------------------------------------------
** The Anybus-CC SERCOS III Object specific exception codes.
**------------------------------------------------------------------------------
*/


#endif  /* inclusion lock */

/*******************************************************************************
** End of abp_src3.h
********************************************************************************
*/
