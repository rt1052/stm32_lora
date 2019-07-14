#include "bsp.h"


void relay_ctrl(bool state)
{
	/* 0表示开灯，1表示关灯 */
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, !state);
}

void led_ctrl(bool state)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, state);
}

bool key_get(void)
{
	bool value;
	value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
	return value;
}

void systick_init(void)
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency/1000);
}

void delay_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInitStruct.TIM_Period = 100-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = (84-1);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
    
    while((TIM3->SR & TIM_FLAG_Update)!=SET);
    TIM3->SR = (uint16_t)~TIM_FLAG_Update;
}

void delay_us(uint32_t us_cnt)
{
    TIM3->CNT = us_cnt-1;
    TIM3->CR1 |= TIM_CR1_CEN;    
    while((TIM3->SR & TIM_FLAG_Update)!=SET);
    TIM3->SR = (uint16_t)~TIM_FLAG_Update;
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //先占和从占都是[0-3]
	
	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;  //系统时钟中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器	
}

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* key */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8; // 9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	/* led */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	/* relay */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void bsp_init(void)
{	
	nvic_init();
	systick_init();
	
	gpio_init();
	//usart1_init();
	spi1_init();	
	
	//delay_init();
}
