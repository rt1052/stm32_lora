#include "sx1276.h"
#include "radio.h"
#include "app.h"
#include "bsp.h"

#include "stm32f10x_flash.h"

#define SYS_ID  0X1


typedef  void (*pFunction)(void);

pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t FlashProtection = 0;
extern uint32_t FlashDestination;

uint32_t CurrentAddr;

#define ApplicationAddress    0x8004000

#define PageSize        0x400  // 0x800


void jump_app(void)
{
	if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000) {
		JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) ApplicationAddress);
		Jump_To_Application();
	} else {
		while(1) {
			delay_ms(100);
			led_ctrl(1);		
			delay_ms(100);
			led_ctrl(0);
		}
	}
}

uint8_t check_sum(uint8_t *buf, uint8_t len) 
{
	uint8_t sum = 0;

    for(uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }

    return sum;
}


void lora_send(uint8_t port, uint8_t id, uint8_t cmd, uint16_t cnt)
{
	uint8_t buf[64];
	uint8_t len = 6;
	
	buf[0] = 0x42;
	buf[1] = len;
	buf[2] = port;
	buf[3] = SYS_ID;
	buf[4] = cmd;
	buf[5] = cnt;
	buf[len] = check_sum(buf, len);
	
	SX1276SetTxPacket(buf, len+1);	
	/* 等发送完成 */
	do {
		delay_ms(10);
	} while (SX1276Process() != RF_TX_DONE);
	
	SX1276StartRx();
}

typedef struct {
	uint8_t head;
	uint8_t len;
	uint8_t port;
	uint8_t id;
    uint8_t cmd;
    uint8_t dat[101];
} frame_t;

typedef struct {
	uint8_t cnt;
	uint8_t dat[100];	
} boot_data_t;

typedef struct {
	uint16_t boot_cnt;
	uint8_t wait_cnt;
} sys_t;


uint8_t lora_buf[128];

frame_t *lora_recv(void)
{
	uint8_t *buf = lora_buf;
	uint16_t len;

	for (uint8_t i = 0; i < 100; i++) {
		if (SX1276Process() == RF_RX_DONE) {
			memset(buf, 0, sizeof(buf));
			SX1276GetRxPacket(buf, &len);

			len -= 1;
			frame_t *frame = (frame_t *)buf;
			if (frame->head == 0x42
				&& frame->len == len
				&& buf[len] == check_sum(buf, len)
				&& frame->id == SYS_ID) {

				return frame;
			} else {
				return NULL;
			}
		}
		delay_ms(10);
	}

	return NULL;
}

sys_t sys;

#define START 0x1
#define DATA 0x2

int main()
{
	uint8_t buf[64];
	uint16_t len;	
	uint8_t state = START;
	uint8_t port;
	
	bsp_init();
	
	led_ctrl(1);		
	
	SX1276Init();
    SX1276StartRx();

	
	FLASH_Unlock();
    while(1)
    {        
		frame_t *frame = lora_recv();
    	switch(state) {
    	case START:
    		if (frame) {
    			switch (frame->cmd) {
    			case UPDATE_START_REQUEST:
    				sys.boot_cnt = 0;
    				lora_send(frame->port, frame->id, UPDATE_DATA_REQUEST, sys.boot_cnt);
					delay_ms(500);
					lora_send(frame->port, frame->id, UPDATE_DATA_REQUEST, sys.boot_cnt);
    				break;
    			case UPDATE_DATA_RESPONSE:
    				boot_data_t *boot_data = (boot_data_t *)frame->dat;
    				if (boot_data->cnt == 0) {

    					for (uint8_t i = 0; i < 26; i++) {
    						FLASH_ErasePage(ApplicationAddress + PageSize * i);
    					}
    					CurrentAddr = ApplicationAddress;

    					//sys.boot_cnt++;
    					lora_send(frame->port, frame->id, UPDATE_DATA_REQUEST, sys.boot_cnt);
    					state = DATA;
    				}
    				break;
    			}
    		}
    		break;
    	case DATA:
    		if (frame) {
    			sys.wait_cnt = 0;
				port = frame->port;
    			switch (frame->cmd) {
    			case UPDATE_START_REQUEST:
    				state = START;
    				break;
    			case UPDATE_DATA_RESPONSE:
    				boot_data_t *boot_data = (boot_data_t *)frame->dat;
    				if (boot_data->cnt == sys.boot_cnt) {

						uint8_t word_cnt = (frame->len - 6) / 4;
						for (uint8_t i = 0; i < word_cnt; i++) {
							FLASH_ProgramWord(CurrentAddr, *(uint32_t *)(boot_data->dat + i * 4));
							CurrentAddr += 4;
						}	    					
    					sys.boot_cnt++;
						
						if ((frame->len - 6) < 100) {
							FLASH_Lock();
							jump_app();	
						}
    				}
    				lora_send(frame->port, SYS_ID, UPDATE_DATA_REQUEST, sys.boot_cnt);
    				break;
				}
    		} else {
    			if (++sys.wait_cnt > 10) {
    				state = START;
    			} else {
    				lora_send(100, SYS_ID, UPDATE_DATA_REQUEST, sys.boot_cnt);
    			}
    		}
    		break;
    	}
								
        delay_ms(10);
    }
}














