#ifndef INC_ADIS_H_
#define INC_ADIS_H_

#include <stdbool.h>
#include <stdint.h>
#include "adis_regs.h"
#include "abcc_hardware_implementation.h"

#define ADIS_OP_MODE_AUTO 0
#define ADIS_OP_MODE_MANUAL 1
#define ADIS_MODE_SET_ERROR 1
#define ADIS_MODE_SET_SUCCESS 0

bool interr;


uint32_t adis_init();
void adis_io_init();
void adis_interupt();

uint32_t adis_self_test();
void adis_reset();
void initTimer(uint32_t ui32SysClock,uint32_t timeNs);
void initTimer1(uint32_t ui32SysClock,uint32_t timeNs);
void initTimer2Enable(uint32_t ui32SysClock);
void TimerInt();
void TimCLKTimeout();
int SetOperatingMode(UINT16 mode);
void SyncInt();
void TimerEn();
int TimerSt();
void ReqMeasure();
void ReadGTemp();
void ReadGBias();
int WriteGBias();
void ReadABias();
int WriteABias();
void ReadDFilter();
int WriteDFilter();
int GlobCmmdAutonullSet();
int GlobCmmdFactoryCalSet();
int GlobCmmdPrecisionAutonullSet();
void GlobCmmdGet();
void GetIDandSerNr();
void WriteReg(INT16 Reg,INT16 data);
void WriteHalfReg(INT16 Reg,INT16 half,INT16 data);
void ReadGyroRange();
int WriteGyroRange();
#endif
