
#include "pid.h"
#include "usart.h"
//速度PI参数
PID SpeedDate=
{
	-40,		//赋值比例值
	-5,			//赋值积分值
	 0			//赋值微分值
};

/*@brief:位置式PID控制器1
 *        [in] deviation:偏差
 * @return:调节电机的PWM
 */
int16_t PositionPID(float deviation,PID pid)
{
	float Position_KP=pid.kp,Position_KI=pid.ki,Position_KD=pid.kd;
	int Pwm;
	static float Bias,Integral_bias,Last_Bias;
	Bias=deviation;
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;//上一次偏差保存
	return Pwm;
}

/*@brief:位置式PID控制器2
 *        [in] deviation:偏差
 * @return:调节电机的PWM
 */
int16_t PositionPID2(float deviation,PID pid)
{
	float Position_KP=pid.kp,Position_KI=pid.ki,Position_KD=pid.kd;
	int Pwm;
	static float Bias,Integral_bias,Last_Bias;
	Bias=deviation;
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
	Last_Bias=Bias;
	return Pwm;
}
