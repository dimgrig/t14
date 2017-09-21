#include "stm32l1xx.h"
#include "stm32.h"
//#include "t14-uart.h"
#include "t14-spi.h"
#include "t14-math.h"
#include "GLCD.h"
#include "TouchPanel.h"
//#include "ESP8266.h"
#include "t14-timer.h"
//#include "SMSD.h"
#include "t14-adc.h"
#include "t14-ssi.h"
#include "t14-power.h"
#include "GUI.h"

volatile static uint32_t s_Tick;
void LEDsSet (unsigned int);

uint8_t init_finished = 0; 

State_TypeDef STATE = IDLE;
uint16_t TouchDelay = 0;

double F = 0; //текущие
double A = 0;

double F1 = 0; //измерение
double A0 = 0; //касание
double A1 = 0; //измерение
double H = 0; //снятие
/*************************************************************************
 * Function Name: LEDsSet
 * Parameters: unsigned int State
 *
 * Return: none
 *
 * Description: Set LEDS State
 *
 *************************************************************************/
void LEDsSet (unsigned int State)
{

  if(State & 1)
  {
    STM32_LEDOn(LED1);
  }
  else
  {
    STM32_LEDOff(LED1);
  }
  if(State & 2)
  {
    STM32_LEDOn(LED2);
  }
  else
  {
    STM32_LEDOff(LED2);
  }
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{
  __disable_interrupt();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit();

  TIMERS_init();  
//  USART_init();  
//  UART_rx_buffer_clear();

  SPI1_init();
  SPI2_init();

  GPIO_SPI_init();
  SPI_TP_CS_HIGH(); 
  SPI_LCD_CS_HIGH(); 
  
  ADC_init();
  InitSSI();
    
  LCD_Initializtion();
  LCD_Clear(Black); 
    
  #ifndef TP_CALIBRATE
    TouchPanel_Calibrate();
  #endif
  
//  UART_rx_buffer_clear();
//  ESP8266_init();   
  
  LEDsSet(0x0);  

  placeMainScreen();
  placeState(STATE);
  placeReset();
  
  InitPower();
  EN_3V3_HIGH();
  EN_5V_HIGH();
  
  init_finished = 1;
  
  __enable_interrupt();
  
  while(1)
  {
    //DelayUS(100);

    LEDsSet(0x1);  

  }
}


void EXTI0_IRQHandler()
{
    EXTI_ClearFlag(EXTI_Line0);  
         
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
    {	
      if (TouchDelay == 0)
      {         
          
          getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
          
          if ((display.x > 240) || (display.y > 320) /*|| (display.x < 0) || (display.y < 0)*/) 
          {
            ;
          }
          else
          {
            #ifdef TP_TEST_DRAW
              TP_DrawPoint(display.x,display.y);  
            #endif
            
            if ((display.x < 120) && (display.y > 270))
            {
              TouchDelay = 1;
              
              //ACTION
              switch (STATE)
              {
                case IDLE:

                  placeValuesReset();
                  
                  STATE = TOUCH;
                  placeState(STATE);
                break;            
                case TOUCH:
                  A0 = A;//касание
                  placeValuesTouch(A0);
                  STATE = MEASURE;
                  placeState(STATE);                  
                break;
                case MEASURE:
                  F1 = F;//измерение
                  A1 = A;
                  placeValuesMeasurement(F, A1);
                  STATE = REMOVAL;
                  placeState(STATE);                 
                break;            
                case REMOVAL:
                  H = A;//снятие и далее расчёт
                  placeValuesRemoval(H);
                  
                  double E, HB, ST, SB;
                  T14math(&E, &ST, &SB, &HB, F1, A0, A1, H);
                  
                  placeValuesMath(E, ST, HB, SB);
                  
                  STATE = IDLE;
                  placeState(STATE);                 
                break;
              }
            }
            else if ((display.x > 120) && (display.y > 270))
            {
              
              TouchDelay = 1;
              //RESET
              STATE = IDLE;
              placeState(STATE);  
              
              placeValuesReset();
            }
            else
            {
              ;
            }              
           
          }           
      }
    }
}


void Timer2IntrHandler (void)
{    
  // Clear update interrupt bit
  TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
         
  float adc_value_f = get_adc_value();  

  //GUI_Text(10, 10, &adc_value_c[0], 6, White, Black);
  
  F = F1K/ADC_VOLTS * adc_value_f + F1B;
  A = A1K*ReadSSI()+A1B;//A1cal/adc1cal * adc_value_f;
  
  placeValuesADC(F, A);
  
  if (TouchDelay == 0)
  {
    ;
  }
  else if (TouchDelay > 2)
  {
    TouchDelay = 0;
  }
  else
  {
    TouchDelay++;
  } 

}