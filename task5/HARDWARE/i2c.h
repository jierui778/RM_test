#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    extern I2C_HandleTypeDef hi2c2;

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

    extern void Error_Handler(void);

    void MX_I2C2_Init(void);

    /* USER CODE BEGIN Prototypes */

    /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif
