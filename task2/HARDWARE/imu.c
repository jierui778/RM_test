#include "imu.h"
#include "mpu6050.h"

#define Buf_SIZE 10 // 队列长度，越大，平滑性越高
#define OFFSET_COUNT 200 // 校准次数
#define IMU_IntegralCount 1000 // 积分次数
#define dt 1/IMU_IntegralCount

static float Pitch_offset;
static float Roll_offset;
static float Yaw_offset; // 陀螺仪偏移量
static uint8_t Wr_Index = 0; // 当前FIFO的写入下标

int16_t MPU6050_FIFO[6][Buf_SIZE]; // FIFO数组，用于存储原始数据



// 将val入队
/**
 * @brief 将val入队
 *
 * @param buf 队列
 * @param val 入队值
 */
void MPU6050_NewVal(int16_t *buf, int16_t val)
{
    buf[Wr_Index] = val;
}




/**
 * @brief 计算FIFO中的平均值
 *
 * @param buf 队列
 * @return int16_t 平均值
 */
static int16_t MPU6050_GetAvg(int16_t *buf)
{
    int i;
    int32_t sum = 0;
    for (i = 0; i < Buf_SIZE; i++)
        sum += buf[i];
    sum = sum / Buf_SIZE;
    return (int16_t)sum;
}




/**
 * @brief 获取陀螺仪和加速度计的原始数据
 *
 * @param gyro 陀螺仪数据
 * @param acc 加速度计数据
 */
void MPU6050_ReadGyro_Acc(int16_t *gyro, int16_t *acc)
{
    static short buf[6]; // 缓存原始数据：0-2：陀螺仪数据；3-5：加速度计数据
    static int16_t gx, gy, gz;
    static int16_t ax, ay, az;

    // 正点原子的库函数，读取传感器原始数据
    MPU6050_GetGyroscope(&buf[0], &buf[1], &buf[2]);
    MPU6050_GetAccelerometer(&buf[3], &buf[4], &buf[5]);

    // 将原始数据入队
    MPU6050_NewVal(&MPU6050_FIFO[0][0], buf[0]);
    MPU6050_NewVal(&MPU6050_FIFO[1][0], buf[1]);
    MPU6050_NewVal(&MPU6050_FIFO[2][0], buf[2]);

    MPU6050_NewVal(&MPU6050_FIFO[3][0], buf[3]);
    MPU6050_NewVal(&MPU6050_FIFO[4][0], buf[4]);
    MPU6050_NewVal(&MPU6050_FIFO[5][0], buf[5]);

    // 更新FIFO入口指针
    Wr_Index = (Wr_Index + 1) % Buf_SIZE;

    // 计算队列平均值
    gx = MPU6050_GetAvg(&MPU6050_FIFO[0][0]);
    gy = MPU6050_GetAvg(&MPU6050_FIFO[1][0]);
    gz = MPU6050_GetAvg(&MPU6050_FIFO[2][0]);

    // 陀螺仪数据要减去偏移量
    gyro[0] = gx - Roll_offset; // gyro
    gyro[1] = gy - Pitch_offset;
    gyro[2] = gz - Yaw_offset;

    ax = MPU6050_GetAvg(&MPU6050_FIFO[3][0]);
    ay = MPU6050_GetAvg(&MPU6050_FIFO[4][0]);
    az = MPU6050_GetAvg(&MPU6050_FIFO[5][0]);

    acc[0] = ax; // acc
    acc[1] = ay;
    acc[2] = az;
}
/**
 * @brief 对陀螺仪和加速度计进行校准
 *
 */
void MPU6050_Init_Offset(void)
{
    uint8_t i;
    int16_t temp[3], temp2[3];
    int32_t tempgx = 0, tempgy = 0, tempgz = 0;
    int32_t tempax = 0, tempay = 0, tempaz = 0;
    Pitch_offset = 0;
    Roll_offset = 0;
    Yaw_offset = 0;

    // wait the mpu
    for (i = 0; i < 100; i++)
    {
        delay_ms(10);
        MPU6050_ReadGyro_Acc(temp, temp2);
    }

    // read the mpu data for calculate the offset
    for (i = 0; i < OFFSET_COUNT; i++)
    {
        delay_ms(10);
        MPU6050_ReadGyro_Acc(temp, temp2);
        tempgx += temp[0];
        tempgy += temp[1];
        tempgz += temp[2];

        tempax += temp2[0];
        tempay += temp2[1];
        tempaz += temp2[2];
    }
    Pitch_offset = tempgy / OFFSET_COUNT;
    Roll_offset = tempgx / OFFSET_COUNT;
    Yaw_offset = tempgz / OFFSET_COUNT;
}

uint8_t IMU_Init(void)
{
    uint8_t flag;
    flag = MPU6050_Init();
    MPU6050_Init_Offset();
    if (flag != 0)
    {
        return flag;
    }
    MPU6050_Init_Offset();
    return flag;
}
void IMU_IntegralGyro(float *gyro)//获取积分后的角度
{
    int16_t Gyro[3], Acc[3];
    int i, j;
    MPU6050_ReadGyro_Acc(&Gyro[0], &Acc[0]);
//    for (i = 0; i < 3; i++)
//    {
//        Gyro[i] = ((float)Gyro[i]) / 16.4f; // gyro range +-2000; adc accuracy 2^16=65536; 65536/4000=16.4;
//    }
    for (j = 0; j < IMU_IntegralCount; j++)
    {
        gyro[0] += Gyro[0] / 16.4f * dt;
        gyro[1] += Gyro[1] / 16.4f* dt;
        gyro[2] += Gyro[2] / 16.4f* dt;
        if (gyro[0] > 360)
        {
            gyro[0] -= 360;
        }
        if (gyro[0] < -360)
        {
            gyro[0] += 360;
        }
        if (gyro[1] > 360)
        {
            gyro[1] -= 360;
        }
        if (gyro[1] < -360)
        {
            gyro[1] += 360;
        }
        if (gyro[2] > 360)
        {
            gyro[2] -= 360;
        }
        if (gyro[2] < -360)
        {
            gyro[2] += 360;
        }
    }
}