#include "smsd.h"


void GPIO_SMSD_init()
{
  GPIO_InitTypeDef  STEP_SMSD_init;  // ������� ���������

  STEP_SMSD_init.GPIO_Mode = GPIO_Mode_OUT;  // ����������� - �����
  STEP_SMSD_init.GPIO_OType = GPIO_OType_PP;  // ����������� �����
  STEP_SMSD_init.GPIO_PuPd = GPIO_PuPd_UP;  // ��� ��������
  STEP_SMSD_init.GPIO_Speed = GPIO_Speed_40MHz;  // �������� ������
  STEP_SMSD_init.GPIO_Pin = GPIO_Pin_10; // ��������� �� 15� ������

  GPIO_Init(GPIOB, &STEP_SMSD_init);  // �������, ����������� ��������� ������

  GPIO_InitTypeDef  DIR_SMSD_init;  // ������� ���������

  DIR_SMSD_init.GPIO_Mode = GPIO_Mode_OUT;  // ����������� - �����
  DIR_SMSD_init.GPIO_OType = GPIO_OType_PP;  // ����������� �����
  DIR_SMSD_init.GPIO_PuPd = GPIO_PuPd_UP;  // ��� ��������
  DIR_SMSD_init.GPIO_Speed = GPIO_Speed_40MHz;  // �������� ������
  DIR_SMSD_init.GPIO_Pin = GPIO_Pin_11; // ��������� �� 15� ������

  GPIO_Init(GPIOB, &DIR_SMSD_init);  // �������, ����������� ��������� ������
    
}     

void SMSD_STEP_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void SMSD_STEP_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

  
void SMSD_DIR_HIGH()
{
  GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

void SMSD_DIR_LOW()
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}
