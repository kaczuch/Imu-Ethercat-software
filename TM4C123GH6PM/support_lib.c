/*
 * support_lib.c
 *
 *  Created on: 15 paü 2015
 *      Author: bkaczor
 */
#include "../TM4C123GH6PM/support_lib.h"

bool isInterruptOnPin(uint32_t port,uint32_t PinNr){
	uint32_t interruptSource;
	bool isOnPinNr=false;
	interruptSource =HWREG(port+0x418);

	if (interruptSource&PinNr){
		isOnPinNr=true;
	}
	return isOnPinNr;
}
