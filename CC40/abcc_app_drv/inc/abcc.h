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
** ABCC driver API used by the the application.
********************************************************************************
********************************************************************************
** Services provided by ABCC driver:
** ABCC_StartDriver()                  - Make the driver ready for use.
** ABCC_isReadyForCommunication()      - Check if the driver is ready to communicate.
** ABCC_ShutdownDriver()               - Close the driver.
** ABCC_HWReset                        - Reset ABCC.
** ABCC_HWReleaseReset                 - Release the reset on ABCC.
** ABCC_RunTimerSystem()               - Timer information to ABCC.
** ABCC_RunDriver()                    - Main routine to be called cyclically during polling.
** ABCC_UserInitComplete()             - End of user specific setup sequence.
** ABCC_SendCmdMsg()                   - Sends a message command to ABCC.
** ABCC_SendRespMsg()                  - Sends a message response to ABCC.
** ABCC_SendRemapRespMsg()             - Send remap response message
** ABCC_SetAppStatus()                 - Sets the application status.
** ABCC_MsgAlloc()                     - Allocates the message buffer.
** ABCC_MsgFree()                      - Frees the message buffer.
** ABCC_ISR()                          - ABCC interrupt service routine.
** ABCC_RdPdEvent()                    - Triggers the RdPd read.
** ABCC_RdMsgEvent()                   - Triggers the message read.
** ABCC_NewWrPdEvent()                 - Triggers the WrPd update.
** ABCC_CheckAnbStatus()               - Check for Anybus status change
** ABCC_CheckSendMessage()             - Check sending queue
** ABCC_ModCap()                       - Reads the module capability.
** ABCC_LedStatus()                    - Reads the led status.
** ABCC_AnbStatus()                    - Reads the Anybus status.
** ABCC_NetworkType()                  - Get network type.
** ABCC_ModuleType()                   - Get module type.
** ABCC_DataFormatType()               - Get network endian.
** ABCC_ParameterSupport()             - Get parameter support.
** ABCC_GetOpmode()                    - Get operation mode.
** ABCC_GetAttribute()                 - Fills "Get Attribute" message
** ABCC_SetByteAttribute()             - Fills "Set Attribute" message
** ABCC_VerifyMessage()                - Checks if E-bit is set
** ABCC_GetDataTypeSize()              - Returns size of ABCC data type
**
** Services to be implemented by user:
** ABCC_CbfReceiveMsg()                - Delivery of the received message.
** ABCC_CbfAdiMappingReq()             - Request of the ADI mapping information.
** ABCC_CbfUserInitReq()               - User specific setup made by the application.
** ABCC_CbfUpdateWriteProcessData()    - Request of the latest write process data.
** ABCC_CbfNewReadPd()                 - Delivery of the latest read process data.
** ABCC_CbfWdTimeout()                 - Communication lost.
** ABCC_CbfWdTimeoutRecovered()        - Communication restored.
** ABCC_CbfEvent()                     - Events received. Called from ISR.
** ABCC_CbfRemapDone()                 - Acknowledge of remap has been sent.
** ABCC_CbfAnbStatusChanged()          - The anybus status has changed
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_H_
#define ABCC_H_

#include "abcc_user_def.h"
#include "abcc_td.h"
#include "abp.h"
#include "abcc_ad_if.h"


/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
**
** Defines for the ABCC driver version.
**
**------------------------------------------------------------------------------
*/
#define ABCC40_APP_DRV_VERSION            "V.2.02"



/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Function types used by user to deliver messages to the application.
**------------------------------------------------------------------------------
*/
typedef void (*ABCC_MsgHandlerFuncType)( ABP_MsgType* psMsg );


/*------------------------------------------------------------------------------
** Data format type.
**------------------------------------------------------------------------------
*/
typedef enum ABCC_DataFormatType
{
   ABCC_DF_LSB_FIRST = 0,
   ABCC_DF_MSB_FIRST = 1
}
ABCC_DataFormatType;


/*------------------------------------------------------------------------------
** Driver severity codes indicated by ABCC_CbfDriverError.
**------------------------------------------------------------------------------
*/
typedef enum ABCC_SeverityType
{
   /* 
   ** Information about an event that has occurred (e.g., serial message lost).
   */
   ABCC_SEV_INFORMATION = 0,
   
   /*
   ** An error of minor importance has occurred. The system can recover from
   ** this error.
   */
   ABCC_SEV_WARNING,
   
   /*
   ** A fatal event has occurred, the system cannot recover (e.g., driver is
   ** out of timers).
   */
   ABCC_SEV_FATAL,
   
   /*
   ** Force the compiler to use a 16-bit variable as enumeration.
   */
   ABCC_SEV_SET_ENUM_ANSI_SIZE = 0x7FFF

} ABCC_SeverityType;


/*------------------------------------------------------------------------------
**  Driver error codes indicated by ABCC_CbfDriverError.
**------------------------------------------------------------------------------
*/
typedef enum ABCC_ErrorCodeType
{
   ABCC_EC_INTERNAL_ERROR,
   ABCC_EC_LINK_CMD_QUEUE_FULL,
   ABCC_EC_LINK_RESP_QUEUE_FULL,
   ABCC_EC_OUT_OF_MSG_BUFFERS,
   ABCC_EC_TRYING_TO_FREE_NULL_POINTER,
   ABCC_EC_INCORRECT_OPERATING_MODE,
   ABCC_EC_INCORRECT_STATE,
   ABCC_EC_RESP_MSG_E_BIT_SET,
   ABCC_EC_WRPD_SIZE_ERR,
   ABCC_EC_RDPD_SIZE_ERR,
   ABCC_EC_RDMSG_SIZE_ERR,
   ABCC_EC_INVALID_RESP_SOURCE_ID,
   ABCC_EC_MODULE_NOT_DECTECTED,
   ABCC_EC_PARAMETER_NOT_VALID,
   ABCC_EC_MODULE_ID_NOT_SUPPORTED,
   ABCC_EC_ERROR_IN_READ_MAP_CONFIG,
   ABCC_EC_ERROR_IN_WRITE_MAP_CONFIG,
   ABCC_EC_INTSTATUS_NOT_SUPPORTED_BY_DRV_IMPL,
   ABCC_EC_MODCAP_NOT_SUPPORTED_BY_DRV_IMPL,
   ABCC_EC_SPI_OP_NOT_ALLOWED_DURING_SPI_TRANSACTION,
   ABCC_EC_SET_ENUM_ANSI_SIZE       = 0x7FFF
} ABCC_ErrorCodeType;


/*------------------------------------------------------------------------------
** Type for command message sending status.
**  ABCC_MSG_SENT: The message is already delivered to the ABCC.
**  ABCC_IN_Q: The message is not yet sent but will be sent as soon as possible
**  ABCC_MSG_Q_FULL: The message is not handled at all due to lack of resources.
**  ABCC_MSG_SIZE_ERROR: The message has an unsupported size.
**  ABCC_MSG_ERROR: Internal error.
**------------------------------------------------------------------------------
*/
typedef enum
{
   ABCC_MSG_SENT = 0,   
   ABCC_MSG_IN_Q,
   ABCC_MSG_Q_FULL,
   ABCC_MSG_SIZE_ERR,
   ABCC_MSG_ERROR
} ABCC_MsgStatusType;


/*------------------------------------------------------------------------------
** ABCC_API_StatusType:
** Defines the current state of the driver.
**------------------------------------------------------------------------------
*/
typedef enum
{
   ABCC_OK,
   ABCC_NOK
} ABCC_StatusType;


/*------------------------------------------------------------------------------
** Used for storing the data format of the field bus.
** NET_UNKNOWN means that the Anybus-CC has not yet responded to our command to
** read the fieldbus data format.
**------------------------------------------------------------------------------
*/
typedef enum NetFormatType
{
   NET_LITTLEENDIAN,
   NET_BIGENDIAN,
   NET_UNKNOWN
}
NetFormatType;

/*------------------------------------------------------------------------------
** Type for indicate if parameter support is available or not.
**------------------------------------------------------------------------------
*/
typedef enum ParameterSupportType
{
   NOT_PARAMETER_SUPPORT,
   PARAMETER_SUPPORT,
   PARAMETER_UNKNOWN
}
ParameterSupportType;


/*******************************************************************************
** Public Globals
********************************************************************************
*/


/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ABCC_StartDriver()
** This function will initiate the driver, enable interrupt, and set the operation
** mode.
** When this function has been called the timer system could be started.
** ( see ABCC_RunTimerSystem() )
** Note! This function will NOT release the reset of the ABCC.
** To release reset, ABCC_HwReleaseReset() has to be called by the application.
**------------------------------------------------------------------------------
** Arguments:
**       bOpmode           - Used operation mode according to abp.h
**       fPingPong         - Use ABCC30 compitable PingPong communication (serial  
**                           and 8bit parallel only) 
** Returns:
**       ABCC_StatusType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_StatusType ABCC_StartDriver( UINT8 bOpmode, BOOL fPingPong );


/*------------------------------------------------------------------------------
** ABCC_ShutdownDriver()
** Stops the driver and puts it into SHUTDOWN state. The ABCC will be reset.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_ShutdownDriver( void );


/*------------------------------------------------------------------------------
** ABCC_isReadyForCommunication()
** This function must be polled after the ABCC_StartDriver() until it returns
** the value TRUE.  This indicates that the ABCC is ready for communication and
** the ABCC setup sequence is started.
** If the ABCC interrupt is used the power up event will indicate that the ABCC
** is ready for communication and the function will return TRUE as soon as the
** ABCC has started.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       TRUE   - If driver is ready for communication.
**       FALSE  - Not ready for communication
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_isReadyForCommunication( void );


/*------------------------------------------------------------------------------
** ABCC_ISR()
** This function should be called from inside the ABCC interrupt routine to 
** acknowledge and handle received ABCC events ( Triggered by IRQ_n on the
** abcc application interface )
** The user configuration defines, ABCC_USER_INT_ENABLE_MASK and ABCC_USER_HANDLE_IN_ABCC_ISR_MASK,
** allows configuration of which events to handle by the ISR and which events to
** pass on to the application ( ABCC_CbfEvent() ).
**
**------------------------------------------------------------------------------
** Arguments: 
**       None.
**
** Returns: 
**       None.
**------------------------------------------------------------------------------
*/
/*EXTFUNC void ABCC_ISR( void );*/
EXTFUNC void ( *ABCC_ISR )( void );

/*------------------------------------------------------------------------------
** ABCC_RunTimerSystem()
** This function is responsible for handling all timers for the ABCC-driver. It 
** is recommended to call this function on a regular basis from a timer
** interrupt. Without this function no timeout and watchdog functionality will
** work. This function can be called after ABCC_StartDriver() has been called.
**------------------------------------------------------------------------------
** Arguments:
**       iTickMs           - Milliseconds since last call.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_RunTimerSystem( const INT16 iDeltaTimeMs );


/*------------------------------------------------------------------------------
** ABCC_HWReset()
** ABCC hardware reset.
** Note! This function will only set reset pin to low. It the responsibility of
** the caller to make sure that the reset time is long enough.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_HWReset( void );


/*------------------------------------------------------------------------------
** ABCC_HWReleaseReset()
** Releases the ABCC reset.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_HWReleaseReset( void );


/*------------------------------------------------------------------------------
** ABCC_RunDriver()
** This function drives the ABCC driver sending and receiving mechanism.
** The driver must be ready for communication before this function is called 
** (ABCC_isReadyForCommunication() must be TRUE). This function could be called 
** cyclically or be based on events from the ABCC. If all events are handled in 
** the interrupt context then there is no need to call this function.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       TRUE              - Normal operation
**       FALSE             - If the driver is stopped or not started for some 
**                           reason.
**
**------------------------------------------------------------------------------
*/
/*EXTFUNC BOOL ABCC_RunDriver( void );*/
EXTFUNC BOOL (*ABCC_RunDriver)( void );


/*------------------------------------------------------------------------------
** ABCC_UserInitComplete()
** This function should be called by the application when the last response from 
** the user specific setup has been received. This will end the ABCC setup 
** sequence and ABCC_SETUP_COMPLETE will be sent. The user specific setup is a 
** part of the ABCC setup sequence and it is initiated by the driver by calling 
** the ABCC_CbfUserInitReq() function.
**------------------------------------------------------------------------------
** Arguments:
**       fSuccess          - TRUE if the user initialization was OK.
**
** Returns: 
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_UserInitComplete( BOOL fSuccess );


/*------------------------------------------------------------------------------
** ABCC_SendCmdMsg()
** Sends a command message to the ABCC.
** The function ABCC_MsgAlloc() must be used to allocate the message buffer.
** It is OK to re-use a previously received response buffer. If ABCC_MSG_Q_FULL
** is returned then the message buffer will still owned by the caller and needs
** to be freed by the ABCC_MsgFree() function or be re-sent later.
** The driver will use the sourceId to map the response to the correct response 
** handler. ABCC_GetNewSourceId() could be used to provide an new source id.
** Example where ABCC_CbfMessageReceived() function is used as response handler:
**
** eResp = ABCC_SendCmdMsg( psMsg, ABCC_CbfMessageReceived );
**------------------------------------------------------------------------------
** Arguments:
**       psWriteMsg        - Pointer to the command message.
**       pnMsgHandler      - Pointer to the function to handle the response
**                           message.
**
** Returns:
**       Message status ( ABCC_MsgStatusType )
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_MsgStatusType ABCC_SendCmdMsg( ABP_MsgType* psMsgCmd,
                                            ABCC_MsgHandlerFuncType pnMsgHandler );


/*------------------------------------------------------------------------------
** ABCC_GetCmdQueueSize()
** Retrieves the number of entries left in the command queue.
** Note! When sending a message the returned status must always be checked to
** verify that the message has in fact been sent.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Number queue entries left in command queue.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_GetCmdQueueSize( void );


/*------------------------------------------------------------------------------
** ABCC_SendRespMsg()
** Sends a response message to the ABCC.
** Note: The received command buffer can be reused as a response buffer. If a 
** new buffer is used then the function ABCC_MsgAlloc() must be used to allocate 
** the buffer. If ABCC_MSG_Q_FULL is returned then the message buffer will still 
** owned by the caller and needs to be freed by the ABCC_MsgFree() function or 
** be re-sent later.
**------------------------------------------------------------------------------
** Arguments:
**       psMsgResp         - Pointer to the command message.
**
** Returns:
**      Message status     ( ABCC_MsgStatusType )
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_MsgStatusType ABCC_SendRespMsg( ABP_MsgType* psMsgResp );


#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED
/*------------------------------------------------------------------------------
** ABCC_SendRemapRespMsg()
** Sends a remap response to the ABCC. When the response is sent the new process
** data sizes will be set and the function ABCC_CbfRemapDone() will be called to
** indicate the change.
**------------------------------------------------------------------------------
** Arguments:
**       psMsgResp         - Pointer to the response message.
**       iNewReadPdSize    - RdPd size when the remap is done.
**       iNewWritePdSize   - WrPd size when the remap is done.
**
** Returns:
**       ABCC_MsgStatusType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_MsgStatusType ABCC_SendRemapRespMsg( ABP_MsgType* psMsgResp,
                                                  UINT16 iNewReadPdSize,
                                                  const UINT16 iNewWritePdSize );
#endif


/*------------------------------------------------------------------------------
** ABCC_SetAppStatus()
** Sets the current application status.
** Note: This information is only supported in SPI and parallel operation mode.
**------------------------------------------------------------------------------
** Arguments:
**       eAppStatus        - Current application status
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetAppStatus(  ABP_AppStatusType eAppStatus  );


/*------------------------------------------------------------------------------
** ABCC_MsgAlloc()
** Allocates a message buffer. This function MUST be used when allocating
** message buffers. The size of the buffer  is controlled by ABCC_USER_MAX_MSG_SIZE.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       ABP_MsgType*      - Pointer to the message buffer.
**                           NULL is returned if no resource is available.
**------------------------------------------------------------------------------
*/
EXTFUNC ABP_MsgType* ABCC_MsgAlloc( void );


/*------------------------------------------------------------------------------
** ABCC_MsgFree()
** Returns the message buffer to the driver to for other messages.
**------------------------------------------------------------------------------
** Arguments:
**       ppsBuffer         - Pointer to the message buffer that was freed. 
**                           The buffer pointer will be set to NULL.
**
** Returns:
**       ABCC_StatusType
**------------------------------------------------------------------------------
*/
EXTFUNC ABCC_StatusType ABCC_MsgFree( ABP_MsgType** ppsBuffer );


/*------------------------------------------------------------------------------
** ABCC_ReadModuleId()
** Reads the module ID.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       ModuleId
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_ReadModuleId( void );


/*------------------------------------------------------------------------------
** ABCC_ModuleDetect()
** Detects if a module is present.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       TRUE              - Module detected.
**       FALSE             - No module detected
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_ModuleDetect( void );



#ifdef ABCC_USER_DRV_PARALLEL
/*------------------------------------------------------------------------------
** ABCC_ModCap()
** Reads the module capability. This function is only supported by the ABCC40 
** parallel operation mode.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Module capability.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_ModCap( void );
#endif


#if defined(ABCC_USER_DRV_PARALLEL) || defined(ABCC_USER_DRV_SPI)
/*------------------------------------------------------------------------------
** ABCC_LedStatus()
** Reads the LED status. Only supported in SPI and parallel operation mode.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       LED status according to the software design guide.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_LedStatus( void );
#endif


/*------------------------------------------------------------------------------
** ABCC_AnbState()
** Reads the current Anybus state.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Anybus state.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_AnbState( void);


/*------------------------------------------------------------------------------
** ABCC_IsSupervised()
** Returns the current status of the supervision bit.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       TRUE           - Is supervised by another network device.
**       FALSE          - Not supervised.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_IsSupervised( void);


/*------------------------------------------------------------------------------
** ABCC_NetworkType()
** Retrieves the network type.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been 
** called by the driver. If called earlier the function will return 0xFFFF which
** indicates that the network is unknown. The different newtwork types could
** be found in abp.h.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Network type ( 0xFFFF if the network is unknown )
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_NetworkType( void );


/*------------------------------------------------------------------------------
** ABCC_ModuleType()
** Retrieves the module type.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been 
** called by the driver. If called earlier the function will return 0xFFFF which
** indicates that the moduleType is unknown. The different module types types
** could be found in abp.h.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Module type (0x04XX for Anybus-CC modules).
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 ABCC_ModuleType( void );


/*------------------------------------------------------------------------------
** ABCC_NetFormatType()
** Retrieves the network format.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been 
** called by the driver. If called earlier the function will return NET_UNKNOWN.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       Network format type (NET_LITTLEENDIAN, NET_BIGENDIAN).
**------------------------------------------------------------------------------
*/
EXTFUNC NetFormatType ABCC_NetFormatType( void );


/*------------------------------------------------------------------------------
** ABCC_ParameterSupport()
** Retrieves the parameter support.
** This function will return a valid value after ABCC_CbfAdiMappingReq has been 
** called by the driver. If called earlier PARAMETR_UNKNOWN will be returned.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**      ParamemterSupportType
**------------------------------------------------------------------------------
*/
EXTFUNC ParameterSupportType ABCC_ParameterSupport( void );

/*------------------------------------------------------------------------------
** ABCC_GetOpmode()
** This function will call ABCC_SYS_GetOpmode() to read the operation mode from
** HW. If the operation is known and fixed or in any other way decided by the
** application this function could be ignored.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       The ABCC40 4 bit operation mode according to abp.h
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_GetOpmode( void );




/*******************************************************************************
** Callback Functions.
** These function must be implemented by the application. The context of the
** callback will differ depending on implementation.
** If, for example, the read process data is chosen to be interrupt driven and
** the message handling chosen to be polled ( see ABCC_USER_INT_ENABLE_MASK and
** HANDLE_ABCC_USER_IN_ABCC_ISR in abcc_user_def.h ), the  ABCC_CbfNewReadPd() will be
** called from interrupt context and ABCC_CbfReceiveMsg() will be called from
** the same context as ABCC_RunDriver().
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ABCC_CbfEvent()
** This function is called from ABCC_ISR() when events specified in
** ABCC_USER_INT_ENABLE_MASK have occurred. The function returns a mask with the currently
** active events according to interrupt the status flags defined in abp.h.
** This function is always called from interrupt context.
**------------------------------------------------------------------------------
** Arguments:
**       iEvents           - Mask according to the int status flags in abp.h
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfEvent( UINT16 iEvents);



/*------------------------------------------------------------------------------
** ABCC_CbfUserInitReq()
** This function needs to be implemented by the application. The function is 
** called to trigger a user specific setup during the ABCC setup state. The ABCC 
** driver will remain in ABCC_USER_INIT state until ABCC_UserInitComplete() is called 
** by the application.  If no user specific setup is required,
** ABCC_UserInitComplete() must be called inside this function otherwise setup
** complete will never be sent.
**
** This function call will be invoked in same context as the read message handling.
** ( See comment for callback section above )
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfUserInitReq( void );


/*------------------------------------------------------------------------------
** ABCC_CbfReceiveMsg()
** A message has been received from the ABCC. This is the receive function for
** all received commands from the ABCC. It could also be used as a response 
** handler if passed on as an argument to the ABCC_SendCmdMsg() function.
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**       psReceivedMsg       - Pointer to received message.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfReceiveMsg( ABP_MsgType* psReceivedMsg );


/*------------------------------------------------------------------------------
** ABCC_CbfUpdateWriteProcessData()
** This function needs to be implemented by the application. The function 
** updates the current write process data. The data must be copied into the 
** buffer before returning from the function.
** The data will only be sent to the ABCC if the return value is TRUE.
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**       pbWritePd         - Pointer to the process data to be sent.
**
** Returns:
**       TRUE              - If the process data has been changed since last
**                           call.
**       FALSE             - Process data not changed.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_CbfUpdateWriteProcessData( UINT8* pbWritePd );


/*------------------------------------------------------------------------------
** ABCC_CbfNewReadPd()
** This function needs to be implemented by the application. The function is
** called when new process data has been received. The process data needs to
** be copied to the application ADI:s before returning from the function. Note
** that the data may not be changed since last time.
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**       pbReadPd          - Pointer to the received process data.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfNewReadPd( UINT8* pbReadPd );



#if defined(ABCC_USER_DRV_SPI) || defined(ABCC_USER_DRV_SERIAL) || defined(ABCC_USER_DRV_PARALLEL_30)
/*------------------------------------------------------------------------------
** ABCC_CbfWdTimeout()
** This function needs to be implemented by the application. The function is
** called when communication with the ABCC module has been lost. The watchdog
** timeout is defined by ABCC_USER_WD_TIMEOUT_MS.
** Note! No watch functionality is provided for parallel 8/16 bit operation
** mode.
** This function is invoked in the same context as ABCC_RunTimerSystem().
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfWdTimeout( void );


/*------------------------------------------------------------------------------
** ABCC_CbfWdTimeoutRecovered()
** This function needs to be implemented by the application. The function 
** indicates that we recently had an ABCC watchdog timeout but now the 
** communication is working again.
** This function will be invoked from same context as the receive handling.
**( see comment for callback section above ).
**
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfWdTimeoutRecovered( void );
#endif


/*------------------------------------------------------------------------------
** ABCC_CbfAdiMappingReq()
** This function need to be implemented by the application. The function is 
** called when the driver is about to start the automatic process data mapping. 
** If no automatic configuration is desired then the pointers are set to NULL. 
** Otherwise the pointers are set to point at the structures containing mapping 
** information. The mapping structures are defined in abcc_ad_if.h.
** This function will be invoked in same context as the read message handling.
** ( See comment for callback section above )
**------------------------------------------------------------------------------
** Arguments:
**       ppsAdiEntry       - Pointer to the requested configuration structure
**                           pointer.
**       ppsDefaultMap     - Pointer to default mapping table.
**
** Returns:
**       -
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfAdiMappingReq ( const AD_AdiEntryType** const ppsAdiEntry,
                                     const AD_DefaultMapType**  const ppsDefaultMap );

/*------------------------------------------------------------------------------
** ABCC_CbfDriverError()
** This function needs to be implemented by the application.
** The context of the call is depending on where the error has occured.
**------------------------------------------------------------------------------
** Arguments:
**       eSeverity         - Severity of the event (see ABCC_SeverityType).
**       iErrorCode        - Error code.
**       lAddInfo          - Depending on error different additional information
**                           can be added.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfDriverError(  ABCC_SeverityType eSeverity,
                                   ABCC_ErrorCodeType  iErrorCode,
                                   UINT32  lAddInfo );



/*------------------------------------------------------------------------------
** ABCC_CbfAnbStatusChanged()
** This callback is invoked if the anybus changes status i.e. if anybus state or
** supervision state is changed.
** Regarding callback context, see comment for callback section above.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfAnbStatusChanged( void );


/*******************************************************************************
** REMAP Related functions
********************************************************************************
*/

#ifdef ABCC_USER_REMAP_SUPPORT_ENABLED
/*------------------------------------------------------------------------------
** ABCC_CbfRemapDone()
** This callback is invoked when REMAP response is successfully sent to the
** ABCC.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CbfRemapDone( void );


#endif


/*******************************************************************************
** Event related functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ABCC_RdPdEvent()
** This function is called to trigger a RdPd read. If the read process data is 
** available then a call to the function ABCC_CbfNewReadPd() will be triggered.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_RdPdEvent( void );


/*------------------------------------------------------------------------------
** ABCC_RdMsgEvent()
** This function is called to trigger a message receive read. If a read message 
** is available then the corresponding message handler will be called.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC  void ABCC_RdMsgEvent( void );


/*------------------------------------------------------------------------------
** ABCC_NewWrPdEvent()
** This function indicates that new process data from the application is 
** available and will be sent to the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_NewWrPdEvent( void);


/*------------------------------------------------------------------------------
** ABCC_CheckAnbStatus()
** Check if current anybus status has changed.
** If the status  is changed  ABCC_CbfAnbStatusChanged() will be invoked.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None:
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CheckAnbStatus( void );



/*------------------------------------------------------------------------------
** ABCC_CheckSendMessage()
** Checks if there are any messages to send.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None:
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_CheckSendMessage( void );



/*******************************************************************************
** Message support functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** ABCC_GetAttribute()
** This function fills an ABCC message with parameters to get an attribute.
**------------------------------------------------------------------------------
** Arguments:
**       psMsg           - Pointer to message buffer.
**       bObject        - Object number.
**       iInstance      - Instance number.
**       bAttribute     - Attribute number.
**       bSourceId      - Source identifier.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_GetAttribute( ABP_MsgType* const psMsg,
                                UINT8 bObject,
                                UINT16 iInstance,
                                UINT8 bAttribute,
                                UINT8 bSourceId );


/*------------------------------------------------------------------------------
** ABCC_SetByteAttribute()
** This function fills an ABCC message with parameters in order to set an
** attribute.
**------------------------------------------------------------------------------
** Arguments:
**       psMsg          - Pointer to message buffer.
**       bObject       - Object number.
**       iInstance     - Instance number.
**       bAttribute    - Attribute number.
**       bVal          - Value to set.
**       bSourceId     - Source identifier.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetByteAttribute( ABP_MsgType* const psMsg,
                                    UINT8 bObject,
                                    UINT16 iInstance,
                                    UINT8 bAttribute,
                                    UINT8 bVal,
                                    UINT8 bSourceId );


/*------------------------------------------------------------------------------
** ABCC_VerifyMessage()
** This function verifies an ABCC response message.
**------------------------------------------------------------------------------
** Arguments:
**       psMsg          - Pointer to message buffer.
**
** Returns:
**       TRUE  Error bit is set.
**       FALSE No error
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_VerifyMessage( const ABP_MsgType* const psMsg );


/*------------------------------------------------------------------------------
** ABCC_GetNewSourceId()
** This function returns a new source id that could used when sending a command
** message. It is guaranteed be unique if this function is used every time a new
** command is sent. The alternative would be that the user uses fixed source
** id:s.
**------------------------------------------------------------------------------
** Arguments:
**       -
**
** Returns:
**       New SourceId
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_GetNewSourceId( void );


/*------------------------------------------------------------------------------
** ABCC_GetDataTypeSize()
** This function returns the size of an ABP data type.
**------------------------------------------------------------------------------
** Arguments:
**       bDataType     - Data type number.
**
** Returns:
**       Data type size in bytes.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8 ABCC_GetDataTypeSize( UINT8 bDataType );




#endif  /* inclusion lock */

/*******************************************************************************
** End of abcc.h
********************************************************************************
*/
