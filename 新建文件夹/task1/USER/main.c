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
    



    while (1)
    {
        t=MPU6050_GetTemperature();
        printf("%d\n",t);



    }
}
