#include "sys.h"

void NVIC_Configuration(void)
{

//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
}
