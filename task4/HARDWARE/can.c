#include "can.h"

/**
 * @brief CAN1初始化
 *
 * @param tsjw 重新同步跳跃宽度为一个时间单位
 * @param tbs2 时间段2占用8个时间单位
 * @param tbs1 时间段1占用6个时间单位
 * @param brp 分频系数（Fdiv）
 * @param mode 模式设置：mode:0,普通模式;1,回环模式;
 * @return uint8_t
 */
uint8_t CAN1_Init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;           // 定义结构体GPIO_InitStructure，用来配置CAN的引脚
    CAN_InitTypeDef CAN_InitStructure;             // 定义结构体CAN_InitStructure，用来配置CAN的参数
    CAN_FilterInitTypeDef CAN_FilterInitStructure; // 定义结构体CAN_FilterInitStructure，用来配置CAN的过滤器
    // NVIC_InitTypeDef  NVIC_InitStructure;//定义结构体NVIC_InitStructure，用来配置CAN的中断

    // 使能相关时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PORTA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  // 使能CAN1时钟

    // CAN1_TX   PA12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12; // PA12|PA11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;          // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // 速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          // 推挽复用输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   // 初始化PA12|PA11

    // 引脚复用映射配置
    //    GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
    //    GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1

    // CAN单元设置
    CAN_InitStructure.CAN_TTCM = DISABLE; // 非时间触发通信模式
    CAN_InitStructure.CAN_ABOM = DISABLE; // 软件自动离线管理
    CAN_InitStructure.CAN_AWUM = DISABLE; // 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART = ENABLE;  // 禁止报文自动传送
    CAN_InitStructure.CAN_RFLM = DISABLE; // 报文不锁定,新的覆盖旧的
    CAN_InitStructure.CAN_TXFP = DISABLE; // 优先级由报文标识符决定
    CAN_InitStructure.CAN_Mode = mode;    // 模式设置：mode:0,普通模式;1,回环模式;
    // 设置波特率
    // CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //重新同步跳跃宽度为一个时间单位
    // CAN_InitStructure.CAN_BS1=CAN_BS1_6tq; //时间段1占用6个时间单位
    // CAN_InitStructure.CAN_BS2=CAN_BS2_8tq; //时间段2占用8个时间单位
    // CAN_InitStructure.CAN_Prescaler=5; //分频系数（Fdiv）
    CAN_InitStructure.CAN_SJW = tsjw;      // 重新同步跳跃宽度为一个时间单位
    CAN_InitStructure.CAN_BS1 = tbs1;      // 时间段1占用6个时间单位
    CAN_InitStructure.CAN_BS2 = tbs2;      // 时间段2占用8个时间单位
    CAN_InitStructure.CAN_Prescaler = brp; // 分频系数（Fdiv）
    CAN_Init(CAN1, &CAN_InitStructure);    // 初始化CAN1

    // 配置过滤器
    CAN_FilterInitStructure.CAN_FilterNumber = 0;                    // 过滤器0
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;  // 标识符屏蔽模式
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; // 32位
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;               // 32位ID
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; // 32位MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; // 过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;               // 激活过滤器0
    CAN_FilterInit(&CAN_FilterInitStructure);                            // 滤波器初始化

    // 中断配置
    // CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.
    // CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);//发送邮箱空中断允许.
    // CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);//FIFO1消息挂号中断允许.
    // CAN_ITConfig(CAN1,CAN_IT_FF0,ENABLE);//FIFO0满中断允许.

    // 中断向量表配置
    // NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级1
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
    // NVIC_Init(&NVIC_InitStructure);//根据指定的参数初始化VIC寄存器

    return 0;
}

/**
 * @brief can1发送一组数据(固定格式:ID为0X12,标准帧,数据帧)
 *
 * @param msg 数据指针
 * @param len 数据长度(最大为8)
 * @param stdid 标准ID()
 * @return uint8_t
 */
uint8_t CAN1_Send_Msg(uint8_t *msg, uint8_t len, uint32_t stdid)
{
    uint8_t mbox;
    uint16_t i = 0;
    CanTxMsg TxMessage;
    TxMessage.StdId = stdid;      // 标准标识符为0
    TxMessage.ExtId = 0x00;       // 扩展标识符为0
    TxMessage.IDE = CAN_ID_STD;   // 使用标准标识符
    TxMessage.RTR = CAN_RTR_DATA; // 数据帧
    TxMessage.DLC = len;          // 数据长度为len
    for (i = 0; i < len; i++)
        TxMessage.Data[i] = msg[i];        // 第一帧数据
    mbox = CAN_Transmit(CAN1, &TxMessage); // 发送数据
    i = 0;
    while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
        i++; // 等待发送结束
    if (i >= 0XFFF)
        return 1;
    return 0;
}

/**
 * @brief can1接收一组数据
 *
 * @param buf 数据缓存区
 * @return uint8_t 返回数据长度
 */
uint8_t CAN1_Receive_Msg(uint8_t *buf)
{
    uint32_t i;
    CanRxMsg RxMessage;
    if (CAN_MessagePending(CAN1, CAN_FIFO0) == 0)
        return 0;                             // 没有接收到数据,直接退出
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); // 读取数据
    for (i = 0; i < RxMessage.DLC; i++)
        buf[i] = RxMessage.Data[i]; // 读取数据
    return RxMessage.DLC;
}
