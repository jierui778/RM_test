#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mpu6050.h"



int main(void)
{
	int16_t GX,GY,GZ,T,AX,AY,AZ;	//定义三轴陀螺仪、三轴加速度、温度变量

    HAL_Init();                    	 			//初始化HAL库
    Stm32_Clock_Init(RCC_PLL_MUL9);   			//设置时钟,72M
    uart_init(115200);							//初始化串口波特率为115200
    MPU6050_Init();

    while (1) {
        MPU6050_GetGyroscope(&GX, &GY, &GZ);		//得到陀螺仪值
        printf("%d,%d,%d\n", GX,GY,GZ);
    }
}
