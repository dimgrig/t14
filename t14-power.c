#include "t14-power.h"


void InitPower(void)
{
  GPIO_InitTypeDef  POWER_init_3;  // ������� ���������

  POWER_init_3.GPIO_Mode = GPIO_Mode_OUT;  // ����������� - �����
  POWER_init_3.GPIO_OType = GPIO_OType_PP;  // ����������� �����
  POWER_init_3.GPIO_PuPd = GPIO_PuPd_UP;  // 
  POWER_init_3.GPIO_Speed = GPIO_Speed_40MHz;  
  POWER_init_3.GPIO_Pin = GPIO_Pin_6; 

  GPIO_Init(GPIOC, &POWER_init_3);  // �������, ����������� ��������� ������
  
  GPIO_InitTypeDef  POWER_init_5;  // ������� ���������

  POWER_init_5.GPIO_Mode = GPIO_Mode_OUT;  // ����������� - �����
  POWER_init_5.GPIO_OType = GPIO_OType_PP;  // ����������� �����
  POWER_init_5.GPIO_PuPd = GPIO_PuPd_UP;  // 
  POWER_init_5.GPIO_Speed = GPIO_Speed_40MHz;  
  POWER_init_5.GPIO_Pin = GPIO_Pin_4; 

  GPIO_Init(GPIOC, &POWER_init_5);  // �������, ����������� ��������� ������
}

void EN_3V3_HIGH()
{
  GPIO_SetBits(GPIOC, GPIO_Pin_6);
}

void EN_3V3_LOW()
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}

void EN_5V_HIGH()
{
  GPIO_SetBits(GPIOC, GPIO_Pin_4);
}

void EN_5V_LOW()
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_4);
}
