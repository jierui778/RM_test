#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

void Control_Init(void); // 初始化控制
int16_t ChangeSpeedMotorL(int NowEncoderValueL, int TargetSpeedL);
int16_t ChangeSpeedMotorR(int NowEncoderValueR, int TargetSpeedR);
#endif
