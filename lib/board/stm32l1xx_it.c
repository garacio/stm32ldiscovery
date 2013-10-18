/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Template/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    Apri-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "stm32l1xx_it.h"
#include "stm32l1xx_exti.h"
#include "discover_functions.h"
#include "discover_board.h"
#include "stm32l_discovery_lcd.h"
#include "stm32l1xx_rtc.h"

extern volatile bool KeyPressed;
extern uint8_t state_machine;
extern bool Auto_test;
extern bool UserButton;
extern volatile bool Idd_WakeUP;
extern uint8_t t_bar[2];

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{  
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
 /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    disableInterrupts();
    TimingDelay_Decrement();
    enableInterrupts();
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void UserButtonHandler (void)
{
  
  uint32_t i=0;	
   
  /* User button pressed */
  KeyPressed = TRUE;  
  
  if ((BUTTON_GPIO_PORT->IDR & USER_GPIO_PIN) == 1) 
  {
          
  /* If user button pressed 4 seconds (approx.) set autotest */
  /* Autotest doesn't execute in Interrupt handler */
    while ((BUTTON_GPIO_PORT->IDR & USER_GPIO_PIN) == 1 )
    {
      i++;
      if (i == 0x0100000)
      {
        AUTOTEST(TRUE) ; 
        return;
      }
    }
  }
  
  if (Auto_test)
    return ;
   
  for (i=0;i<0x8ffff;i++) {__NOP();}
     
/* To pass to next state*/
    state_machine++;
    
    if (state_machine == MAX_STATE)
    {	
      state_machine = STATE_VREF;
    }
          
    /* To update Bar graph & leds*/  
    switch (state_machine)
    {
      case STATE_VREF:
              GPIO_HIGH(LD_PORT,LD_GREEN);
              GPIO_LOW(LD_PORT,LD_BLUE);
              BAR0_OFF;
              BAR1_OFF;
              BAR2_OFF;
              BAR3_OFF;
              break;
              
      case STATE_SLIDER_VALUE:
             GPIO_LOW(LD_PORT,LD_BLUE);	
             GPIO_HIGH(LD_PORT,LD_GREEN);
              break;
              
      case STATE_SLIDER_BUTTON:
             GPIO_LOW(LD_PORT,LD_GREEN);	        
             GPIO_HIGH(LD_PORT,LD_BLUE);
           break;
          
      case STATE_ICC_RUN:
              GPIO_LOW(LD_PORT,LD_GREEN);	
              GPIO_LOW(LD_PORT,LD_BLUE);	        
              BAR0_ON;
              BAR1_OFF;
              BAR2_OFF;
              BAR3_OFF;
              break;
              
      case STATE_ICC_LP_RUN:
              GPIO_LOW(LD_PORT,LD_GREEN);	
              GPIO_LOW(LD_PORT,LD_BLUE);	
              BAR0_ON;
              BAR1_ON;
              BAR2_OFF;
              BAR3_OFF;
              break;

      case STATE_ICC_STOP:
              GPIO_LOW(LD_PORT,LD_GREEN);	
              GPIO_LOW(LD_PORT,LD_BLUE);	
              BAR0_ON;
              BAR1_ON;
              BAR2_ON;
              BAR3_OFF;
              break;              
      
      case STATE_ICC_STBY:
              GPIO_LOW(LD_PORT,LD_GREEN);	
              GPIO_LOW(LD_PORT,LD_BLUE);	
              BAR0_ON;
              BAR1_ON;
              BAR2_ON;
              BAR3_ON;
              break;					
    }	
    
}

void CurrentmeasurementHandler(void)
{

  Idd_WakeUP = TRUE;

}

void EXTI0_IRQHandler(void)
{
  disableInterrupts();
  if (UserButton)
  {
    UserButtonHandler();
  }
  else {
    CurrentmeasurementHandler();
  }  
  
  EXTI_ClearITPendingBit(EXTI_Line0);
  enableInterrupts();
}


void RTC_WKUP_IRQHandler (void)
{
  RTC_ClearITPendingBit(RTC_IT_WUT);
  EXTI_ClearITPendingBit(EXTI_Line20);
}
/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_md.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
