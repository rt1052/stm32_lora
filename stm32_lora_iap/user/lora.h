#ifndef _LORA_H_
#define _LORA_H_

#include <stdint.h>

#define SET_RELAY_REQUEST   0x1
#define SET_RELAY_RESPONSE  0x2
#define GET_RELAY_REQUEST   0x3
#define GET_RELAY_RESPONSE  0x4
#define GET_PARAM_REQUEST   0x5
#define GET_PARAM_RESPONSE  0x6

#define SET_RELAY_NOTICE    0x7

#define UPDATE_START_REQUEST  0x11
#define UPDATE_DATA_REQUEST   0X13
#define UPDATE_DATA_RESPONSE  0X14
#define UPDATE_END_REQUEST    0x15


typedef struct {
	uint8_t head;
	uint8_t len;
	uint8_t port;
	uint8_t id;
    uint8_t cmd;
    /* 更新命令专用格式 */
    uint8_t cnt[2];
    uint8_t dat[100];
} frame_t;


void lora_init(void);
frame_t *lora_recv(void);
void lora_send(uint8_t port, uint8_t id, uint8_t cmd, uint8_t *dat, uint8_t dat_len);


#endif
