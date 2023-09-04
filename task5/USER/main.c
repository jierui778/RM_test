#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mpu6050.h"



int main(void)
{
	int16_t GX,GY,GZ,T,AX,AY,AZ;	//�������������ǡ�������ٶȡ��¶ȱ���

    HAL_Init();                    	 			//��ʼ��HAL��
    Stm32_Clock_Init(RCC_PLL_MUL9);   			//����ʱ��,72M
    uart_init(115200);							//��ʼ�����ڲ�����Ϊ115200
    MPU6050_Init();

    while (1) {
        MPU6050_GetGyroscope(&GX, &GY, &GZ);		//�õ�������ֵ
        printf("%d,%d,%d\n", GX,GY,GZ);
    }
}
