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



    while (1)
    {
        t=MPU6050_GetTemperature();
        MPU6050_GetGyroscope(&GX,&GY,&GZ);     // �õ�������ֵ
        MPU6050_GetAccelerometer(&AX,&AY,&AZ); // �õ����ٶ�ֵ
        printf("%d\n",t);



    }
}
