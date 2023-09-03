#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "timer.h"
#include "imu.h"

int main(void)
{
    delay_init();
    uart_init(115200);
    int t;

    IMU_Init();
    float a[3], g[3];




    while (1)
    {

        

        IMU_IntegralGyro(&g[0]);
        delay_ms(50);
        printf("%f,%f,%f\n", g[0], g[1], g[2]);
    }
}
