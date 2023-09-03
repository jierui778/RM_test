#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "imu.h"
#include "timer.h"
#include "can.h"
// static IMU_Info IMU;
int main(void)
{
    delay_init();
    IMU_Init();
    uint8_t canbuf[8] = {2, 2, 3, 3, 4, 4, 5, 5};
    CAN1_Init(CAN_SJW_1tq, CAN_BS2_8tq, CAN_BS1_9tq, 4, CAN_Mode_LoopBack);

    // //    TIM3_Init(100-1, 8400-1); // 100HZ
    //     uart_init(115200);
    //     int t;

    //     int16_t AX, AY, AZ, GX, GY, GZ;
    //     int16_t g[3];
    //     int16_t a[3];

    while (1)
    {
        CAN1_Send_Msg(canbuf, 8, 0x12); // 发送8个字节
        delay_ms(100);
        // //        t = MPU6050_GetTemperature();
        //         //        MPU6050_GetGyroscope(&GX,&GY,&GZ);     // 得到陀螺仪值
        //         MPU6050_GetAccelerometer(&AX,&AY,&AZ); // 得到加速度值
        //         IMU_Update();
        //         MPU6050_ReadGyro_Acc(&g[0], &a[0]);
        // //        printf("%f %f %f\n", IMU.Pitch,IMU.Roll, IMU.Yaw);
        // //        printf("%d %d %d\n", g[0],g[1], g[2]);
        // //        printf("%d %d %d\n", a[0],a[1], a[2]);
        // //        printf("%d %d %d\n", AX,AY, AZ);
        //         //        printf("%d\n",t);
    }
}
