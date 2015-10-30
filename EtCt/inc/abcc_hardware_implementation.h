/*
 * abcc_hardware_implementation.h
 *
 *  Created on: 31 lip 2015
 *      Author: bkaczor
 */

#ifndef ETCT_ABCC_HARDWARE_IMPLEMENTATION_H_
#define ETCT_ABCC_HARDWARE_IMPLEMENTATION_H_
#include "abcc_td.h"

void init_anybus_hardware();

void anybus_interrupt_routine();

void anybus_SPI_SEND_Receive(void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength);

void Hw_Reset();

void Hw_Release_Reset();

void Hw_Int_enable();
void Hw_Int_disable();
void Hw_SyncIntRout();
void initTimer();
void startCounting();
void stopCounting();

#endif /* ETCT_ABCC_HARDWARE_IMPLEMENTATION_H_ */
