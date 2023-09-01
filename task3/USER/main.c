#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "imu.h"


int main(void)
{
     delay_init();
    uart_init(115200);
    int t;

    IMU_Init();
    int16_t AX, AY, AZ, GX, GY, GZ;
    int16_t g[3];
    int16_t a[3];



    while (1)
    {
        t=MPU6050_GetTemperature();
//        MPU6050_GetGyroscope(&GX,&GY,&GZ);     // 得到陀螺仪值
//        MPU6050_GetAccelerometer(&AX,&AY,&AZ); // 得到加速度值
        MPU6050_ReadGyro_Acc(&g[0], &a[0]);
        printf("%d %d %d\n",g[0],g[1],g[2]);
//        printf("%d\n",t);



    }
}
