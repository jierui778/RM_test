#include "mpu6050.h"
#include "sys.h"
#include "delay.h"

uint8_t MPU6050_Init()
{
    uint8_t res;
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // PB10——SCL PB11——SDA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_DeInit(I2C2);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;					//设置为IIC模式
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;			//设置IIC的占空比，低电平除以高电平值为2
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;		//指定第一个设备的地址为7位地址
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;					//使能ACK信号
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//指定7位地址
	I2C_InitStructure.I2C_ClockSpeed = 400000;

    I2C_Cmd(I2C2, ENABLE); // 初始化IIC总线
    I2C_Init(I2C2, &I2C_InitStructure);


    MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050
    delay_ms(10);
    MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050
    MPU6050_SetGyroFsr(3);                      // 陀螺仪传感器,±2000dps
    MPU6050_SetAccelFsr(0);                     // 加速度传感器，±2g
    MPU6050_SetRate(50);                        // 设置采样率50hz
    MPU6050_WriteByte(MPU_INT_EN_REG, 0X00);    // 关闭所有中断
    MPU6050_WriteByte(MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
    MPU6050_WriteByte(MPU_FIFO_EN_REG, 0X00);   // 关闭FIFO
    MPU6050_WriteByte(MPU_INTBP_CFG_REG, 0X80); // INT引脚低电平有效

    res = MPU6050_ReadByte(MPU_DEVICE_ID_REG);
    if (res == MPU_ADDR) // 器件ID正确,即res = MPU_ADDR = 0x68
    {
        MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
        MPU6050_WriteByte(MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
        MPU6050_SetRate(50);                        // 设置采样率为50Hz
    }
    else
        return 1; // 地址设置错误,返回1
                  //    IIC_Init();//初始化IIC总线
                  //	MPU6050_WriteByte(MPU_PWR_MGMT1_REG,0x00);//解除休眠状态
                  //	MPU6050_WriteByte(MPU_SAMPLE_RATE_REG,0x07);//陀螺仪采样率，典型值：0x07(125Hz)
                  //	MPU6050_WriteByte(MPU_CFG_REG,0x06);//低通滤波频率，典型值：0x06(5Hz)
                  //	MPU6050_WriteByte(MPU_GYRO_CFG_REG,0x18);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
                  //	MPU6050_WriteByte(MPU_ACCEL_CFG_REG,0x01);//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
    return 0;     // 地址设置正确,返回0
}

/**
 * @description:设置MPU6050陀螺仪传感器满量程范围
 * @param {uint8_t} fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
 * @return {*}0,设置成功  其他,设置失败
 */
uint8_t MPU6050_SetGyroFsr(uint8_t fsr)
{
    return MPU6050_WriteByte(MPU_GYRO_CFG_REG, fsr << 3); // 设置陀螺仪满量程范围
}

/**
 * @description:设置MPU6050加速度传感器满量程范围
 * @param {uint8_t} fsr:0,±2g;1,±4g;2,±8g;3,±16g
 * @return {*}0,设置成功  其他,设置失败
 */
uint8_t MPU6050_SetAccelFsr(uint8_t fsr)
{
    return MPU6050_WriteByte(MPU_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}

/**
 * @description: 设置MPU6050的数字低通滤波器
 * @param {uint16_t} lpf：数字低通滤波频率(Hz)
 * @return {*}0,设置成功  其他,设置失败
 */
uint8_t MPU6050_SetLPF(uint16_t lpf)
{
    uint8_t data = 0;
    if (lpf >= 188)
        data = 1;
    else if (lpf >= 98)
        data = 2;
    else if (lpf >= 42)
        data = 3;
    else if (lpf >= 20)
        data = 4;
    else if (lpf >= 10)
        data = 5;
    else
        data = 6;
    return MPU6050_WriteByte(MPU_CFG_REG, data); // 设置数字低通滤波器
}

uint8_t MPU6050_SetRate(uint16_t rate)
{
    uint8_t data;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    data = MPU6050_WriteByte(MPU_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
    return MPU6050_SetLPF(rate / 2);                     // 自动设置LPF为采样率的一半
}
/**
 * @description: 获取温度传感器的值
 * @return {*}温度值
 */
short MPU6050_GetTemperature(void)
{
    uint8_t buf[2];
    short raw;
    float temp;

    MPU6050_ReadLen(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf); // 读取温度值
    raw = ((uint16_t)buf[0] << 8) | buf[1];               // 合成数据
    temp = 36.53 + ((double)raw) / 340;                   // 转换为实际温度值
    return temp;
}

// 得到陀螺仪值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
uint8_t MPU6050_GetGyroscope(short *gx, short *gy, short *gz)
{
    u8 buf[6], res;
    res = MPU6050_ReadLen(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *gx = ((u16)buf[0] << 8) | buf[1];
        *gy = ((u16)buf[2] << 8) | buf[3];
        *gz = ((u16)buf[4] << 8) | buf[5];
    }
    return res;
    ;
}

// 得到加速度值(原始值)
// gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
// 返回值:0,成功
//     其他,错误代码
uint8_t MPU6050_GetAccelerometer(short *ax, short *ay, short *az)
{
    uint8_t buf[6], res;
    res = MPU6050_ReadLen(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *ax = ((u16)buf[0] << 8) | buf[1];
        *ay = ((u16)buf[2] << 8) | buf[3];
        *az = ((u16)buf[4] << 8) | buf[5];
    }
    return res;
    ;
}

/**
 * @description: IIC连续写
 * @param {uint8_t} addr器件地址
 * @param {uint8_t} len数据长度
 * @param {uint8_t} *buf数据区
 * @return {*}0：成功，1：失败
 */
uint8_t MPU6050_WriteLen(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{

    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
        ; // 主机判忙
    I2C_GenerateSTART(I2C2, ENABLE);
    // check EV5
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;

    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter);
    // check EV6
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
        ;

    I2C_SendData(I2C2, reg);
    // check EV8_2
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
        ;

    /* 参考上图，页写入与单个写入的差别，仅仅是停止信号的发送 */
    /* 加入判断即可 */
    while (len)
    {
        I2C_SendData(I2C2, *buf);
        // check EV8_2
        while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
            ;
        buf++;
        len--;
    }
    I2C_GenerateSTOP(I2C2, ENABLE);
    return 0;
}
/**
 * @description: IIC连续读
 * @param {uint8_t} addr器件地址
 * @param {uint8_t} reg寄存器地址
 * @param {uint8_t} len数据长度
 * @param {uint8_t} *buf数据区
 * @return {*}
 */
uint8_t MPU6050_ReadLen(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
        ;                            // 主机判忙
    I2C_GenerateSTART(I2C2, ENABLE); // 发送起始信号
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;
    I2C_Send7bitAddress(I2C2, MPU_ADDR, I2C_Direction_Transmitter);

    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
        ;
    I2C_Cmd(I2C2, ENABLE);
    I2C_SendData(I2C2, reg);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
        ;

    I2C_GenerateSTART(I2C2, ENABLE);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;
    I2C_Send7bitAddress(I2C2, MPU_ADDR, I2C_Direction_Receiver);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS)
        ;
    while (len)
    {
        if (len == 1)
        {
            I2C_AcknowledgeConfig(I2C2, DISABLE);
            I2C_GenerateSTOP(I2C2, ENABLE);
        }
        while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
            ;
        {
            *buf = I2C_ReceiveData(I2C2);
            buf++;
            len--;
        } // 这个中括号表示连续读取
    }
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    return 0;
}
/**
 * @description:
 * @param {uint8_t} reg寄存器地址
 * @param {uint8_t} data写入数据
 * @return {*}0：成功，1：失败
 */
uint8_t MPU6050_WriteByte(uint8_t addr, uint8_t data)
{
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C2, ENABLE); // 发送起始信号
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;                                                           // 等待EV5
    I2C_Send7bitAddress(I2C2, MPU_ADDR, I2C_Direction_Transmitter); // 发送挂在在I2C上设备的地址
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
        ; // 等待EV6
    // EV8_1事件不需要等待，它是告诉该写入DR发送数据了，下一步直接写入数据
    I2C_SendData(I2C2, addr); // 发送写入设备的内部地址
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
        ;                     // 等待EV8
    I2C_SendData(I2C2, data); // 发送写入的数据
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
        ;                           // 等待EV8_2事件
    I2C_GenerateSTOP(I2C2, ENABLE); // 发送停止信号
    return 0;
}
uint8_t MPU6050_ReadByte(uint8_t addr)
{
    uint8_t data = 0;
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
        ;                            // 主机判忙
    I2C_GenerateSTART(I2C2, ENABLE); // 发送起始信号
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;                                                           // 等待EV5                                                                 // 应答
    I2C_Send7bitAddress(I2C2, MPU_ADDR, I2C_Direction_Transmitter); // 发送设备地址
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
        ;                     // 等待EV6
    I2C_Cmd(I2C2, ENABLE);    // 清除事件EV6
    I2C_SendData(I2C2, addr); // 发送要读取设备的内部地址
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)
        ;                            // 等待EV8
    I2C_GenerateSTART(I2C2, ENABLE); // 发送第二次起始信号

    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
        ;                                                        // 等待EV5
    I2C_Send7bitAddress(I2C2, MPU_ADDR, I2C_Direction_Receiver); // 发送设备地址
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS)
        ;                                 // 等待EV6
    I2C_AcknowledgeConfig(I2C2, DISABLE); // 关闭ACK使能
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
        ;                                // 等待EV7
    data = I2C_ReceiveData(I2C2);        // 读取数据
    I2C_GenerateSTOP(I2C2, ENABLE);      // 发送停止信号
    I2C_AcknowledgeConfig(I2C2, ENABLE); // //传输完毕，再次打开ACK使能
    return data;
    // 返回读取数据
}
uint8_t MPU6050_GetAngle(int16_t *pitch, int16_t *roll, int16_t *yaw)
{
    int16_t GyroX, GyroY, GyroZ;
    static float integralX = 0.0f, integralY = 0.0f, integralZ = 0.0f;

    static int g_GetZeroOffset = 0;
    static float GyroX_Offset = 0.0f, GyroY_Offset = 0.0f, GyroZ_Offset = 0.0f;

    static const float dt = 8.0 / 1000;      // 间隔8ms
    static const float ZERO_OFFSET_COUN=1 / dt; // 1/8=125次每秒

    MPU6050_GetGyroscope(&GyroX, &GyroY, &GyroZ); // 得到陀螺仪值
    *pitch /= 16.384f;
    GyroY /= 16.384f;
    GyroZ /= 16.384f;

    // 统计125次，共1秒时间
    if (g_GetZeroOffset++ < ZERO_OFFSET_COUN)
    {
        GyroX_Offset += GyroX * dt; // 每次8%积分，累计加权125次
        GyroY_Offset += GyroY * dt;
        GyroZ_Offset += GyroZ * dt;
    }

    // 除去零偏
    GyroX -= GyroX_Offset;
    GyroY -= GyroY_Offset;
    GyroZ -= GyroZ_Offset;

    if (g_GetZeroOffset++ > ZERO_OFFSET_COUN) // 统计完零偏后开始累计偏角
    {
        integralX += GyroX * dt; // 每次8%权重累计偏转角度
        integralY += GyroY * dt;
        integralZ += GyroZ * dt;
        // 360°一个循环
        if (*pitch > 360)
            *pitch -= 360;
        if (*pitch < -360)
            *pitch += 360;

        if (*roll > 360)
            *roll -= 360;
        if (*roll < -360)
            *roll += 360;

        if (*yaw > 360)
            *yaw -= 360;
        if (*yaw < -360)
            *yaw += 360;
    }
}

