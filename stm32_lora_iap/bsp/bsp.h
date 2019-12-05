#ifndef _BSP_H_
#define _BSP_H_


#include <stdbool.h>
#include "stm32f10x.h"


typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} gpio_t;


void led_ctrl(bool state);
bool key_get(void);
void relay_ctrl(bool state);
void delay_ms(uint16_t ms);
void bsp_init(void);

void usart1_init(void);
void spi1_init(void);
uint8_t spi_rw_byte(u8 txdata);


#endif
