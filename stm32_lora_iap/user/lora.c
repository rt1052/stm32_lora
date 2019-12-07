#include "lora.h"

#include "sx1276.h"
#include "radio.h"
#include "main.h"
#include "bsp.h"


uint8_t lora_buf[128];

uint8_t check_sum(uint8_t *buf, uint8_t len) 
{
	uint8_t sum = 0;

    for(uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }

    return sum;
}

void lora_init(void)
{
	SX1276Init();
    SX1276StartRx();
}

/**
 * @dat: 数据指针
 * @dat_len: 数据长度
 */
void lora_send(uint8_t port, uint8_t id, uint8_t cmd, uint8_t *dat, uint8_t dat_len)
{
	uint8_t buf[64];
	uint8_t len = 5 + dat_len;
	
	buf[0] = 0x42;
	buf[1] = len;
	buf[2] = port;
	buf[3] = sys.param.id;
	buf[4] = cmd;
	memcpy(buf+5, dat, dat_len);
	buf[len] = check_sum(buf, len);
	
	SX1276SetTxPacket(buf, len+1);	
	/* 等发送完成 */
	do {
		delay_ms(10);
	} while (SX1276Process() != RF_TX_DONE);
	
	SX1276StartRx();
}

frame_t *lora_recv(void)
{
	uint8_t *buf = lora_buf;
	uint16_t len;

	for (uint8_t i = 0; i < 100; i++) {
		if (SX1276Process() == RF_RX_DONE) {
			memset(buf, 0, sizeof(buf));
			SX1276GetRxPacket(buf, &len);
			/* 减去校验占用的长度 */
			len -= 1;
			frame_t *frame = (frame_t *)buf;
			if (frame->head == 0x42
				&& frame->len == len
				&& buf[len] == check_sum(buf, len)
				&& frame->id == sys.param.id) {

				return frame;
			} else {
				return NULL;
			}
		}
		delay_ms(10);
	}

	return NULL;
}
