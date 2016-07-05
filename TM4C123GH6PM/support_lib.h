/*
 * support_lib.h
 *
 *  Created on: 15 paü 2015
 *      Author: bkaczor
 */

#ifndef TM4C123GH6PM_SUPPORT_LIB_H_
#define TM4C123GH6PM_SUPPORT_LIB_H_
#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include "inc/hw_memmap.h"

bool isInterruptOnPin(uint32_t port,uint32_t PinNum);

void intPortBRoutine();
#endif /* TM4C123GH6PM_SUPPORT_LIB_H_ */
