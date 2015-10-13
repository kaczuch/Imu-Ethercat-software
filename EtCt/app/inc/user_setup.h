/*
 * user_setup.h
 *
 *  Created on: 5 sie 2015
 *      Author: bkaczor
 */

#ifndef ETCT_APP_INC_USER_SETUP_H_
#define ETCT_APP_INC_USER_SETUP_H_


#define US_NW_TYPE_SRC_ID     0
#define US_FIRMWARE_SRC_ID    1
#define US_SERIAL_NO_SRC_ID   2
#define US_DIP_SW1_SRC_ID     3
#define US_DIP_SW2_SRC_ID     4
#define US_ANB_EXC_SRC_ID     5
#define US_NW_EXC_SRC_ID      6



void US_Init( void );

void US_RunUserInitStateMachine( void );




#endif /* ETCT_APP_INC_USER_SETUP_H_ */
