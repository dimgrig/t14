#include "t14-adc.h"

uint16_t valueVoltsMem[6] = {2048, 2048, 2048, 2048, 2048, 2048};
uint8_t initADC = 0;
         
void ADC_init()
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_Mode_AIN);
        
        
  RCC_HSICmd(ENABLE);
  /*!< Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);       
        
        
//������������� ADC
  ADC_InitTypeDef ADC_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //�������� ������������ ADC
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;  //���������� ADC - 12 ���
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //������������ ���������� ������
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  
  
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;  
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  
  ADC_InitStruct.ADC_NbrOfConversion = 1;
  
  ADC_Init(ADC1, &ADC_InitStruct);  //�������������� ������ ADC �������� ����������
  /*����� ������ ��������� ��������� ��� ����������� ������: 
  ����� ADC - ADC1,
  ����� � 5,
  ����� �������������� � ������������������ - 1, 
  sample time - 16 ������*/

////  ADC_Cmd(ADC1, ENABLE); //� �������� ADC
////  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET); //���� ���������� ADC
////  ADC1->CR2 |= ADC_CR2_CONT; //����� ������������ ��������������.
//// ADC1->CR2 |= ADC_CR2_SWSTART;
  
  ADC1->CR1;
  ADC1->CR2;
   /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /* Wait until the ADC1 is ready */
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}

}


float get_adc_value()
{   
    float valueVolt = 0;
      
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_4Cycles);  

    ADC_SoftwareStartConv(ADC1); /*ADC1->CR2 |= ADC_CR2_SWSTART;*/
    
    if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
    {

      if (initADC == 0)
      {
        initADC = 1;
        for (int k = 0; k <= 5 ; k++)
        {
          valueVoltsMem[k] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/ 
        }
      }
      else
      {
        // convert valueADC to valueVolt -> valueADC * (MAX VOLT / 2^12)
        // and also :
        // ADC_SoftwareStartConvCmdsoftwareS,
        // ADC_GetCalibrationStatus
        // with
        // return (uint16_t) ADCx->DR;
        valueVoltsMem[0] = ADC_GetConversionValue(ADC1);/*ADC1->DR;*/      
        
        /*for (int k = 0; k <= 5 ; k++)
        {
          valueVoltsMem[k] = ADC_GetConversionValue(ADC1);
          
          for (int j = 0; j <= 10; )
            j++;
        }*/
      
 
      }
      
      // ����������       
      for (int k = 0; k <= 5 ; k++)
      {
        valueVolt += valueVoltsMem[k];
      }
      valueVolt = valueVolt/6;

      for (int k = 5; k > 0; k--)
      {
        valueVoltsMem[k] = valueVoltsMem[k - 1];
      }


      // convert  // Volt = 3.3
      // ADC = 12 bits, so 2^12 = 4096
      valueVolt = valueVolt * (ADC_VOLTS / ADC_MAX_BITS);
      
      return valueVolt;
    }
    else
    {
      return 0;
    }
}