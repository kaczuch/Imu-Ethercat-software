/*
 * support_lib.c
 *
 *  Created on: 15 paü 2015
 *      Author: bkaczor
 */
#include "../TM4C123GH6PM/support_lib.h"
#include "../inc/adis.h"
#include "abcc_hardware_implementation.h"
#include "abcc.h"

bool isInterruptOnPin(uint32_t port,uint32_t PinNr){
	uint32_t interruptSource;
	bool isOnPinNr=false;
	interruptSource =HWREG(port+0x418);

	if (interruptSource&PinNr){
		isOnPinNr=true;
	}
	return isOnPinNr;
}

void intPortBRoutine(){

	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_0)){
		Hw_Int_disable();
		Hw_Int_enable();
		ABCC_ISR();

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_1)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_2)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_3)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_4)){
		adis_interupt();
	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_5)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_6)){

	}
	if(isInterruptOnPin(GPIO_PORTB_BASE,GPIO_PIN_7)){

	}

}
