#include "control.h"
#include "pid.h"
/**
 * @description: 模块全部初始化
 * @return {*}
 */
void Control_Init(void)
{

}
/**
 * @description: 根据PID参数调节左电机速度到目标速度
 * @param {int} NowAngleValue
 * @param {int} TargetAngleValueSpeed
 * @return {*}
 */

int16_t ChangeSpeedMotorL(int NowAngleValueL, int TargetSpeedL)
{
    int pwm=0;
    int bias=0;
    int TargetAngleSpeedL;
    TargetAngleSpeedL = TargetSpeedL; // 目标速度后期在考虑是否转换
    bias =NowAngleValueL-TargetAngleSpeedL;
    pwm = PositionPID(bias, SpeedDate);
    if (pwm > PWM_MAX)
    {
        pwm = PWM_MAX;
    }
    else if (pwm < PWM_MIN)
    {
        pwm = PWM_MIN;
    }
    return pwm;
}


/**
 * @brief 根据PID参数调节右电机速度到目标速度
 *
 * @param NowAngleValueR
 * @param TargetSpeedR 目标速度
 * @return int16_t 调节电机的PWM
 */
int16_t ChangeSpeedMotorR(int NowAngleValueR, int TargetSpeedR)
{
    int pwm=0;
    int bias=0;
    int TargetAngleSpeedR;
    TargetAngleSpeedR = TargetSpeedR; // 目标速度后期在考虑是否转换
    bias =NowAngleValueR-TargetAngleSpeedR;
    pwm = PositionPID2(bias, SpeedDate); // 传入偏差和速度PID参数
    if (pwm > PWM_MAX)//限制最大最小值
    {
        pwm = PWM_MAX;
    }
    else if (pwm < PWM_MIN)
    {
        pwm = PWM_MIN;
    }
    return pwm;
}

