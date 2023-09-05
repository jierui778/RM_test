#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * These parameters and more are described within the 'configuration' section of the
 * FreeRTOS API documentation available on the FreeRTOS.org web site.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/* Section where include file can be added */
/* USER CODE END Includes */

/* Ensure definitions are only used by the compiler, and not by the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
  #include <stdint.h>
  extern uint32_t SystemCoreClock;
#endif
#define configUSE_PREEMPTION                     1//使用抢占式调度
#define configSUPPORT_STATIC_ALLOCATION          1//支持静态内存分配
#define configSUPPORT_DYNAMIC_ALLOCATION         1//支持动态内存分配
#define configUSE_IDLE_HOOK                      0//空闲任务钩子函数
#define configUSE_TICK_HOOK                      0//系统节拍钩子函数
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )//CPU时钟频率
#define configTICK_RATE_HZ                       ((TickType_t)1000)//系统节拍频率
#define configMAX_PRIORITIES                     ( 56 )//最大优先级
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)//最小任务堆栈大小
#define configTOTAL_HEAP_SIZE                    ((size_t)3072)//系统堆大小
#define configMAX_TASK_NAME_LEN                  ( 16 )//任务名长度
#define configUSE_TRACE_FACILITY                 1//使用可视化跟踪调试
#define configUSE_16_BIT_TICKS                   0//使用16位节拍计数器
#define configUSE_MUTEXES                        1//使用互斥信号量
#define configQUEUE_REGISTRY_SIZE                8//队列注册表大小
#define configUSE_RECURSIVE_MUTEXES              1//使用递归互斥信号量
#define configUSE_COUNTING_SEMAPHORES            1//使用计数信号量
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0//使用端口优化任务选择

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                    0//使用协程
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )  //协程优先级

/* Software timer definitions. */
#define configUSE_TIMERS                         1//使用软件定时器
#define configTIMER_TASK_PRIORITY                ( 2 )    //定时器任务优先级
#define configTIMER_QUEUE_LENGTH                 10   //定时器队列长度
#define configTIMER_TASK_STACK_DEPTH             256  //定时器任务堆栈大小

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet            1   //包含vTaskPrioritySet函数
#define INCLUDE_uxTaskPriorityGet           1   //包含uxTaskPriorityGet函数
#define INCLUDE_vTaskDelete                 1   //包含vTaskDelete函数
#define INCLUDE_vTaskCleanUpResources       0   //包含vTaskCleanUpResources函数
#define INCLUDE_vTaskSuspend                1   //包含vTaskSuspend函数
#define INCLUDE_vTaskDelayUntil             1   //包含vTaskDelayUntil函数
#define INCLUDE_vTaskDelay                  1   //包含vTaskDelay函数
#define INCLUDE_xTaskGetSchedulerState      1   //包含xTaskGetSchedulerState函数
#define INCLUDE_xTimerPendFunctionCall      1   //包含xTimerPendFunctionCall函数
#define INCLUDE_xQueueGetMutexHolder        1       //包含xQueueGetMutexHolder函数
#define INCLUDE_uxTaskGetStackHighWaterMark 1   //包含uxTaskGetStackHighWaterMark函数
#define INCLUDE_eTaskGetState               1   //包含eTaskGetState函数


#define USE_FreeRTOS_HEAP_4 //使用heap_4.c


#ifdef __NVIC_PRIO_BITS//
 /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS         4
#endif


#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15//最低中断优先级


#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5//最高系统调用优先级


#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//内核中断优先级

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//系统调用优先级

#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}//断言

#define vPortSVCHandler    SVC_Handler//SVC调度函数
#define xPortPendSVHandler PendSV_Handler//PendSV调度函数

#define xPortSysTickHandler SysTick_Handler//SysTick调度函数



#endif /* FREERTOS_CONFIG_H */
