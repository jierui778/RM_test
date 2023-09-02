#ifndef __MAHONYAHRS_H
#define __MAHONYAHRS_H
#include "math.h"

#define IMU_Update_Freq 500.0f //更新频率

extern volatile float q0, q1, q2, q3; //框架四元数

void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz); //磁场数据的融合
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);//无磁场数据的融合
#endif
