#ifndef __PID_H
#define __PID_H
#include "sys.h"
/*定义PID结构体*/
#define PWM_MAX 2499//定义电机最大速度占空比
#define PWM_MIN  499  //定义电机最小速度占空比

typedef struct
{
    float kp;
    float ki;
    float kd;
}PID;
//PID控制器参数结构体


extern PID SpeedDate;//声明全局结构体参数
extern int16_t PositionPID(float deviation, PID pid);//位置式PID控制器1
extern int16_t PositionPID2(float deviation,PID pid);//位置式PID控制器2
#endif
