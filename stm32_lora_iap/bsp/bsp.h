#ifndef _BSP_H_
#define _BSP_H_

#include "stm32f10x.h"
#include "stdio.h"
#include <stdbool.h>


void led_init(void);

void usart1_init(void);

void spi1_init(void);
uint8_t spi_read_write_byte(u8 txdata);

void dht11_init(void);

void bsp_init(void);

void delay_ms(uint16_t ms);

#endif
