#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "imu.h"
#include "timer.h"
//IMU_Info IMU;
int main(void)
{
    delay_init();
    IMU_Init();
    //    TIM3_Init(100-1, 8400-1); // 100HZ
    uart_init(115200);
    int t;

    int16_t AX, AY, AZ, GX, GY, GZ;
    int16_t g[3];
    int16_t a[3];

    while (1)
    {
        //        t = MPU6050_GetTemperature();
        //        MPU6050_GetGyroscope(&GX,&GY,&GZ);     // 得到陀螺仪值
        // MPU6050_GetAccelerometer(&AX,&AY,&AZ); // 得到加速度值
        IMU_Update();
        // MPU6050_ReadGyro_Acc(&g[0], &a[0]);
        //        printf("%f %f %f\n", IMU.Pitch,IMU.Roll, IMU.Yaw);
        printf("%f %f %f\n", Pitch, Roll, Yaw);
        //        printf("%d %d %d\n", g[0],g[1], g[2]);
        //        printf("%d %d %d\n", a[0],a[1], a[2]);
        //        printf("%d %d %d\n", AX,AY, AZ);
        //        printf("%d\n",t);
    }
}
