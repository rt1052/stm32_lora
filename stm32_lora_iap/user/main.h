#ifndef _MAIN_H_
#define _MAIN_H_


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

/* flash分区起始地址，共64页 */
#define FLASH_BOOT   0x8000000   // 15页
#define FLASH_PARAM  0x8003C00   // 1页
#define FLASH_APP    0x8004000   // 48页
#define FLASH_END    0x8010000

#define PAGE_SIZE     0x400
#define APP_PAGE_NUM  48


typedef struct {
	uint8_t id;
	bool update;
	uint8_t reserve[2];  /* 保证结构体长度为4的倍数 */
} param_t;

typedef struct {
	param_t param;	
	uint8_t port;
	bool erase;
} sys_t;

typedef void (*run_func)(void);

extern sys_t sys;

#endif
