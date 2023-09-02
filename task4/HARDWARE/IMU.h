#ifndef __IMU_H
#define __IMU_H
#include "sys.h"

typedef struct
{
	float Pitch;
	float Roll;
	float Yaw;

	float Pitch_v;
	float Roll_v;
	float Yaw_v;

	float ax;
	float ay;
	float az;

} IMU_Info;

IMU_Info *IMU_GetInfo(void);
uint8_t IMU_Init(void);
void IMU_Update(void);
void MPU6050_ReadGyro_Acc(int16_t *gyro, int16_t *acc);

#endif
