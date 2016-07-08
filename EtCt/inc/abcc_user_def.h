/*
 * abcc_user_def.h
 *
 *  Created on: 31 lip 2015
 *      Author: bkaczor
 */
#ifndef ETCT_ABCC_USER_DEF_H_
#define ETCT_ABCC_USER_DEF_H_

#define ABCC_USER_DRV_PARALLEL

#include <stdbool.h>
#include <stdint.h>
#include "utils/uartstdio.h"




#define ABCC_USER_MAX_NUM_APPL_CMDS     ( 2 )
#define ABCC_USER_MAX_NUM_ABCC_CMDS     ( 2 )
#define ABCC_USER_MAX_MSG_SIZE                       ( 255 )
#define ABCC_USER_MAX_PROCESS_DATA_SIZE              ( 512 )
#define ABCC_USER_ERR_REPORTING_ENABLED
//#define ABCC_USER_REMAP_SUPPORT_ENABLED

#define ABCC_USER_DEBUG_EVENT_ENABLED
#define ABCC_USER_DEBUG_ERR_ENABLED
#define ABCC_USER_DEBUG_PRINT( args)                 UARTprintf("Debug: %s\n",args);
#define ABCC_USER_POLL_WRPD

/*******************************************************************************
** Spi specific defines
********************************************************************************
*/


#ifdef ABCC_USER_DRV_SPI

#define ABCC_USER_INT_ENABLED
#define ABCC_USER_INT_ENABLE_MASK                    ( ABP_INTMASK_RDPDIEN | ABP_INTMASK_STATUSIEN | ABP_INTMASK_RDMSGIEN | ABP_INTMASK_WRMSGIEN | ABP_INTMASK_ANBRIEN  )
#define ABCC_USER_SPI_MSG_FRAG_LEN                   ( 2 )
#define ABCC_USER_WD_TIMEOUT_MS                      ( 100 )
#endif
/*
 * paralel specyfic defines
 * */
#ifdef ABCC_USER_DRV_PARALLEL
#define ABCC_USER_INT_ENABLED
#define ABCC_USER_INT_ENABLE_MASK                    ( ABP_INTMASK_RDPDIEN | ABP_INTMASK_STATUSIEN | ABP_INTMASK_RDMSGIEN | ABP_INTMASK_WRMSGIEN | ABP_INTMASK_ANBRIEN  )

#define ABCC_USER_MEMORY_MAPPED_ACCESS
#define ABCC_USER_PARALLEL_BASE_ADR             			( 0x60000000 )

#endif
#endif /* ETCT_ABCC_USER_DEF_H_ */
