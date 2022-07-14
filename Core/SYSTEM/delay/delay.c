#include "delay.h"
#include "main.h"



void delay_us(int32_t nus) {
    uint16_t differ = 0xffff - nus - 5;
    //设置定时器2的技术初始值
    __HAL_TIM_SetCounter(&htim1, differ);
    //开启定时器
    HAL_TIM_Base_Start(&htim1);

    while (differ < 0xffff - 5) {
        differ = __HAL_TIM_GetCounter(&htim1);
    };
    //关闭定时器
    HAL_TIM_Base_Stop(&htim1);
}

void delay_ms(int16_t nms) {
//    int32_t temp;
//    SysTick->LOAD = (int32_t) nms * fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
//    SysTick->VAL = 0x00;           //��ռ�����
//    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;          //��ʼ����
//    do {
//        temp = SysTick->CTRL;
//    } while (temp & 0x01 && !(temp & (1 << 16)));//�ȴ�ʱ�䵽��
//    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
//    SysTick->VAL = 0X00;       //��ռ�����
    HAL_Delay(nms);
} 
