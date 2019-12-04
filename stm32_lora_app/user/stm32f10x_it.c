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


void SysTick_Handler(void)
{  
   OS_CPU_SR  cpu_sr;


   OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断 Tell uC/OS-II that we are starting an ISR
   OSIntNesting++;
   OS_EXIT_CRITICAL();	  //恢复全局中断标志

   OSTimeTick();      //Call uC/OS-II's OSTimeTick(),在os_core.c文件里定义,主要判断延时的任务是否计时到

   OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换            
}

void TIM2_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
  
uint8_t dat_ready;
uint32_t ir_dat;
uint8_t flag;
uint8_t dat_cnt;
uint32_t temp;

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line1);
		
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)) {  /* rise eage */
			TIM_SetCounter(TIM2, 0);
		} else {  /* fall eage */
			uint16_t tim_cnt = TIM_GetCounter(TIM2);
			if (flag) {
				if (tim_cnt > 800 && tim_cnt < 1400) {   /* 580us */
					temp <<= 1;
				  
				} else if (tim_cnt > 3200 && tim_cnt < 3600) {  /* 1.7ms */
					temp <<= 1;              
					temp |= 0x1;
				} else if (tim_cnt > 4200 && tim_cnt < 6000) {  /* 2.5ms */
					flag = 0;
				} else {
					// GPIO_WriteLow(GPIOB, GPIO_PIN_5);
				}
				
				dat_cnt++;
				if (dat_cnt == 32) {
					dat_ready = 1;
					flag = 0;
					ir_dat = temp;
				}
			} else {  
				  if (tim_cnt > 8000 && tim_cnt < 10000) { /* 4.5ms */
					  flag = 1;
					  dat_cnt = 0;
					  temp = 0;
				  }
			}
		}  	
	}
}


void RTC_IRQHandler(void)
{
}



//一以下函数不用管
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
