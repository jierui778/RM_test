#ifndef __PID_H
#define __PID_H

#include "main.h"

/*定义PID结构体*/
#define PWM_MAX 2499 // 定义电机最大速度占空比
#define PWM_MIN 499  // 定义电机最小速度占空比

typedef struct
{
    float kp;
    float ki;
    float kd;
} PID;

// PID控制器参数结构体

// /*外部结构体声明*/
// extern PID x_dir;//定义x方向PID参数结构体，在这里设置速度PID参数
// extern PID y_dir;//定义yPID参数结构体，在这里设置转向PID参数
// /**/
extern PID SpeedDate;
extern int16_t PositionPID(float deviation, PID pid);
extern int16_t PositionPID2(float deviation, PID pid);
extern int16_t PositionPIDToSpeed_L(float deviation, PID pid); //
extern int16_t PositionPIDToSpeed_R(float deviation, PID pid);

// extern int Change_x(int Now_x_ValueL, int Target_x);
// extern int Change_y(int Now_y_ValueL, int Target_y);
#endif
