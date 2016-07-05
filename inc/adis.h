#ifndef INC_ADIS_H_
#define INC_ADIS_H_

#include <stdbool.h>
#include <stdint.h>
#include "adis_regs.h"
#include "abcc_hardware_implementation.h"

bool interr;


uint32_t adis_init();
void adis_io_init();
void adis_interupt();

uint32_t adis_self_test();
void adis_reset();
void initTimer();
void TimerInt();
#endif
