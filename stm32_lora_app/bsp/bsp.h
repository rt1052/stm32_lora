#ifndef _BSP_H_
#define _BSP_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stdbool.h>


/* flash分区起始地址 */
#define FLASH_BOOT   0x8000000
#define FLASH_PARAM  0x8002C00
#define FLASH_APP    0x8003000


typedef struct {
	uint8_t id;
	bool update;
	uint8_t reserve[2];  /* 保证结构体长度为4的倍数 */
} param_t;

extern param_t param;

void led_init(void);

void usart1_init(void);

void spi1_init(void);
uint8_t spi_read_write_byte(u8 txdata);

void dht11_init(void);

void bsp_init(void);


void flash_set_param(param_t *param);
param_t flash_get_param(void);

#endif
