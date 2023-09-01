#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"


int main(void)
{
    delay_init();
    uart_init(115200);
    int t;

    MPU6050_Init();
    int16_t AX, AY, AZ, GX, GY, GZ;
    int16_t pitch, roll, yaw;
    float gyroX, gyroY, gyroZ;
    static const float dt = 8.0 / 1000;  // 间隔8ms
    static const int ZERO_OFFSET_COUN =1 / dt; // 1/8=125次每秒

    static int g_GetZeroOffset = 0;
    static float gyroX_offset = 0.0f, gyroY_offset = 0.0f, gyroZ_offset = 0.0f;

    static float integralX = 0.0f, integralY = 0.0f, integralZ = 0.0f;


    while (1)
    {
        t=MPU6050_GetTemperature();
        MPU6050_GetGyroscope(&GX,&GY,&GZ);     // 得到陀螺仪值
        MPU6050_GetAccelerometer(&AX,&AY,&AZ); // 得到加速度值
        /* IMU Data */


        gyroX =GX/ 16.384f; // 我的MPU6050初始化的±2000LSB量程 (32768 / 2000) = 16.384f
        gyroY =GY/ 16.384f;
        gyroZ =GZ/ 16.384f;

        if (g_GetZeroOffset++ < ZERO_OFFSET_COUN)
        {
            gyroX_offset += gyroX * dt;  // 每次8%积分，累计加权125次
            gyroY_offset += gyroY * dt;
            gyroZ_offset += gyroZ * dt;
        }
        gyroX -= gyroX_offset;
        gyroY -= gyroY_offset;
        gyroZ -= gyroZ_offset;
        if (g_GetZeroOffset > ZERO_OFFSET_COUN) // 统计完零偏后开始累计偏角
        {
            integralX += gyroX * dt;  // 每次8%权重累计偏转角度
            integralY += gyroY * dt;
            integralZ += gyroZ * dt;
            // 360°一个循环
            if (integralX > 360)
                integralX -= 360;
            if (integralX < -360)
                integralX += 360;
            if (integralY > 360)
                integralY -= 360;
            if (integralY < -360)
                integralY += 360;

            if (integralZ > 360)
                integralZ -= 360;
            if (integralZ < -360)
                integralZ += 360;
            printf("%f %f %f\n", integralX, integralY, integralZ);
        }
        // MPU6050_GetAngle(&pitch, &roll, &yaw);
        // printf("%d\n", t);
    }

    // 对mpu6050输出的角速度数据进行积分得出角度


}
