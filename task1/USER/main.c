#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"

int16_t AX, AY, AZ, GX, GY, GZ;
int main(void)
{
    delay_init();//延时函数初始化
    uart_init(115200);//串口初始化
    int t;//温度
    MPU6050_Init();//MPU6050初始化
    while (1)
    {
        t=MPU6050_GetTemperature();
        MPU6050_GetGyroscope(&GX,&GY,&GZ);     // 得到陀螺仪值
        MPU6050_GetAccelerometer(&AX,&AY,&AZ); // 得到加速度值
        // printf("%d\n",t);
        printf("%d %d %d\n",GX,GY,GZ);
        // printf("%d %d %d\n",AX,AY,AZ);



    }
}
