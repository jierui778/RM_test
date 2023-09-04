#include "imu.h"
#include "mpu6050.h"
#include "MahonyAHRS.h"

static float Pitch_offset;
static float Roll_offset;
static float Yaw_offset;//陀螺仪偏移量

#define PI 3.14159265358979f//圆周率
#define OFFSET_COUNT 100//校准次数
#define Buf_SIZE 10 // 队列长度，越大，平滑性越高

int16_t MPU6050_FIFO[6][Buf_SIZE];//FIFO数组，用于存储原始数据
// int16_t lastAx, lastAy, lastAz, lastGx, lastGy, lastGz;
static uint8_t Wr_Index = 0; // 当前FIFO的写入下标
static IMU_Info imu;





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
 * @brief
 *
 * @param values
 */
static void Get_IMU_Data(float *values)
{
    int16_t gyro[3], acc[3];
    int i;

    MPU6050_ReadGyro_Acc(&gyro[0], &acc[0]);
    for(i = 0; i < 3; i++)
    {
        values[i] = ((float)gyro[i]) / 16.4f; // gyro range +-2000; adc accuracy 2^16=65536; 65536/4000=16.4;
        values[3 + i] = (float)acc[i];
    }
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
        MPU6050_ReadGyro_Acc(temp, temp2);
    }

    // read the mpu data for calculate the offset
    for (i = 0; i < OFFSET_COUNT; i++)
    {
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
 * @brief IMU初始化
 *
 * @return uint8_t 0:成功 1:失败
 */

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
/**
 * @brief 读取经过滤波的角速度和加速度
 *
 * @param values 存储角速度和加速度的数组
 */
void IMU_GetValues(float *values)
{
    int16_t gyro[3], acc[3];
    int i;
    MPU6050_ReadGyro_Acc(&gyro[0], &acc[0]);
    for(i = 0; i < 3; i++)
    {
        values[i] = ((float)gyro[i]) / 16.4f; // gyro range +-2000; adc accuracy 2^16=65536; 65536/4000=16.4;
        values[3 + i] = (float)acc[i]/16384.0f;
    }
}

// asin
/**
 * @brief acsin函数
 *
 * @param v 输入值
 * @return float 输出值
 */
static float safe_asin(float v)
{
    if (isnan(v))
    {
        return 0.0f;
    }
    if (v >= 1.0f)
    {
        return PI / 2;
    }
    if (v <= -1.0f)
    {
        return -PI / 2;
    }
    return asin(v);
}

/**
 * @brief 更新IMU数据
 *
 */
void IMU_Update(void)
{
    static float q[4];
    float Values[6];
    IMU_GetValues(Values);
    MahonyAHRSupdateIMU(Values[0] * PI / 180, Values[1] * PI / 180, Values[2] * PI / 180,Values[3], Values[4], Values[5]);
    q[0] = q0;
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;

    imu.ax = Values[3];
    imu.ay = Values[4];
    imu.az = Values[5];

    imu.Pitch_v = Values[0];
    imu.Roll_v = Values[1];
    imu.Yaw_v = Values[2];
    //四元数计算欧拉角
    imu.Roll = (atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), 1 - 2.0f * (q[1] * q[1] + q[2] * q[2]))) * 180 / PI;
    imu.Pitch = asin(2.0f * (q[0] * q[2] - q[1] * q[3])) * 180 / PI;
    imu.Yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / PI; // yaw
//    printf("%f %f %f\n", imu.Pitch,imu.Roll, imu.Yaw);
}


