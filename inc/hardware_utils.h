/*
 * hardware_config.c
 *
 *  Created on: 16 lip 2015
 *      Author: bkaczor
 */
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include <driverlib/gpio.h>
#include "driverlib/pin_map.h"
#include "adis.h"
#include "user_adi.h"
#include <inc/hw_types.h>
#include "ad_obj.h"
#include "sync_obj.h"
#include "inc/hw_timer.h"

void io_init();
void initTimer(uint32_t ui32SysClock,uint32_t timeNs);
void initTimer1(uint32_t ui32SysClock,uint32_t timeNs);
void initTimer2Enable(uint32_t ui32SysClock);
void TimerInt();
void TimCLKTimeout();
void SyncInt();
void TimerEn();
int TimerSt();
void ReqMeasure();
bool isInterruptOnPin(uint32_t port,uint32_t PinNr);
void intPortBRoutine();
