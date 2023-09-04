#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"


void Control_Init(void);//初始化控制
void Control_Run(void);//控制代码运行
int16_t ChangeSpeedMotorL(int NowEncoderValueL, int TargetSpeedL);//根据PID参数调节左电机速度到目标速度
int16_t ChangeSpeedMotorR(int NowEncoderValueR, int TargetSpeedR);//根据PID参数调节右电机速度到目标速度
#endif
