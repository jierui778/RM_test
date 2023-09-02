#ifndef __IMU_H
#define __IMU_H
#include "sys.h"

uint8_t MPU6050_IntegralGetAngle(float *pitch, float *roll, float *yaw);//获取积分后的角度


void MPU6050_ReadGyro_Acc(int16_t *gyro, int16_t *acc);//获取滤波后的陀螺仪和加速度计的原始数据
void MPU6050_NewVal(int16_t *buf, int16_t val);//将val入队
int16_t MPU6050_GetAvg(int16_t *buf);//计算FIFO中的平均值
void MPU6050_Offset(void);//陀螺仪校准
void IMU_Update(void);//更新姿态

void IMU_IntegralGyro(float *gyro);
uint8_t IMU_Init(void); // 初始化

#endif