#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "imu.h"
#include "timer.h"
#include "can.h"

int main(void)
{
    delay_init();
    // TIM3_Init(100-1, 8400-1); // 100HZ
    uart_init(115200);
    Control_Init();
        while (1)
    {
        Control_Run();
    }
}