/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

#include "app.h"


void EXTI0_IRQHandler(void)
{	
}

void SysTick_Handler(void)
{  
   OS_CPU_SR  cpu_sr;


   OS_ENTER_CRITICAL();  //����ȫ���жϱ�־,�����ж� Tell uC/OS-II that we are starting an ISR
   OSIntNesting++;
   OS_EXIT_CRITICAL();	  //�ָ�ȫ���жϱ�־

   OSTimeTick();      //Call uC/OS-II's OSTimeTick(),��os_core.c�ļ��ﶨ��,��Ҫ�ж���ʱ�������Ƿ��ʱ��

   OSIntExit();  //��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л�            
}

void RTC_IRQHandler(void)
{
}



//һ���º������ù�
void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/