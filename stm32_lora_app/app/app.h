#ifndef _APP_H_
#define _APP_H_


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ucos_ii.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

typedef struct {
	uint8_t port;
    uint8_t cmd;
    uint8_t dat;
} frame_t;

typedef struct {
	bool relay_state;
	bool switch_state;
	frame_t frame[3];
	uint8_t frame_cnt;
	uint8_t dht11_buf[5];
} ctrl_t;

extern frame_t lora_frame;
extern OS_EVENT *lora_msg;
extern OS_EVENT *lora_q;
extern ctrl_t sys_ctrl;

#define SET_RELAY_REQUEST   0x1
#define SET_RELAY_RESPONSE  0x2
#define GET_RELAY_REQUEST   0x3
#define GET_RELAY_RESPONSE  0x4
#define GET_PARAM_REQUEST   0x5
#define GET_PARAM_RESPONSE  0x6

#define SET_RELAY_NOTICE    0x7

#define UPDATE_START_REQUEST  0x11


uint8_t check_sum(uint8_t *buf, uint16_t len);

void task_led(void *p_arg);
void task_dht11(void *p_arg);
void task_lora(void *p_arg);
void task_ctrl(void *p_arg);
void task_ir(void *p_arg);

#endif
