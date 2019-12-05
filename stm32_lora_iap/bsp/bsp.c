#include "bsp.h"


gpio_t led = {GPIOA, GPIO_Pin_10};
gpio_t key = {GPIOA, GPIO_Pin_8};
gpio_t relay = {GPIOB, GPIO_Pin_5};


void led_ctrl(bool state)
{
	GPIO_WriteBit(led.port, led.pin, state);
}

bool key_get(void)
{
	return GPIO_ReadInputDataBit(key.port, key.pin);
}

void relay_ctrl(bool state)
{
	/* 0表示打开，1表示关闭 */
	GPIO_WriteBit(relay.port, relay.pin, !state);
}

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* led */
	GPIO_InitStructure.GPIO_Pin =  led.pin;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(led.port, &GPIO_InitStructure);		
	/* key */
	GPIO_InitStructure.GPIO_Pin =  key.pin;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(key.port, &GPIO_InitStructure);	
	/* relay */
	GPIO_InitStructure.GPIO_Pin =  relay.pin;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(relay.port, &GPIO_InitStructure);		
}

void delay_ms(uint16_t ms)
{     
   while(ms--)
   {
      uint16_t i = 12000;
      while(i--);    
   }
}

void bsp_init(void)
{	
	gpio_init();
	//usart1_init();
	spi1_init();
}
