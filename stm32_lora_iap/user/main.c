#include "main.h"
#include "bsp.h"
#include "lora.h"


/* 
更新逻辑
   
应用程序：
1、启动后检查更新标志，如果是true，则改为false，否则不变
2、收到更新数据帧，将更新标志改成true

boot程序：
1、启动后检查更新标志，如果是true，则准备更新，否则跳转到应用程序
2、跳转到应用程序失败，将更新标志改成true

*/


sys_t sys;

void flash_set_param(param_t *param)
{
	FLASH_Unlock();
	FLASH_ErasePage(FLASH_PARAM);
	FLASH_ProgramWord(FLASH_PARAM, *(uint32_t *)param);
	FLASH_Lock();
}

param_t flash_get_param(void)
{
	return (*(param_t *)FLASH_PARAM);
}

void flash_erase(void)
{
	/* 检查是否已经擦除 */
	if (!sys.erase) {
		sys.erase = true;
		for (uint8_t i = 0; i < APP_PAGE_NUM; i++) {
			FLASH_ErasePage(FLASH_APP + PAGE_SIZE * i);
		}
	}
}

void flash_write(uint32_t addr, uint8_t *buf, uint8_t len)
{
	if (len % 4 || (addr+len) > FLASH_END) {
		return;
	}

	sys.erase = false;
	for (uint8_t i = 0; i < len; i += 4) {
		FLASH_ProgramWord(addr + i, *(uint32_t *)(buf + i));
	}		
}

void jump_app(void)
{
	run_func run_app;
	
	if (((*(uint32_t *)FLASH_APP) & 0x2FFE0000 ) == 0x20000000) {
		uint32_t app_addr = *(uint32_t *) (FLASH_APP + 4);
		run_app = (run_func)app_addr;
		__set_MSP(*(uint32_t *) FLASH_APP);
		run_app();
	} else {
		sys.param.update = true;
		flash_set_param(&sys.param);
	}
}

void update_app(void)
{
	uint16_t update_cnt = 0;
	uint8_t wait_cnt = 10;
	uint32_t update_addr;

    while(1)
    {        
		frame_t *frame = lora_recv();
    	if (frame) {
    		switch (frame->cmd) {
    		case UPDATE_START_REQUEST:
    			wait_cnt = 0;
    			update_cnt = 0;
    			update_addr = FLASH_APP;
    			sys.port = frame->port;
    			FLASH_Unlock();
    			flash_erase();
    			lora_send(sys.port, frame->id, UPDATE_DATA_REQUEST, (uint8_t *)&update_cnt, 2);    				
    			break;
    		case UPDATE_DATA_RESPONSE:
    			if (*(uint16_t *)frame->cnt == update_cnt
    				&& frame->port == sys.port) {

    				wait_cnt = 0;
    				uint8_t update_len = frame->len - 7;
    				flash_write(update_addr, frame->dat, update_len);
    				update_addr += update_len;  					
    				update_cnt++;
					/* 如果是最后一帧则跳转到应用程序 */
					if (update_len < 100) {
						FLASH_Lock();
						jump_app();
					} else {
						lora_send(sys.port, sys.param.id, UPDATE_DATA_REQUEST, (uint8_t *)&update_cnt, 2);
					}
    			}
    			break;
    		case GET_RELAY_REQUEST:
    			/* oxff表示需要更新 */
    			uint8_t state = 0xff;
    			lora_send(sys.port, sys.param.id, GET_RELAY_RESPONSE, &state, 1);
    			break;
			}
    	} else {
    		if (++wait_cnt > 10) {
    			wait_cnt = 10;
    		} else {
    			lora_send(sys.port, sys.param.id, UPDATE_DATA_REQUEST, (uint8_t *)&update_cnt, 2);
    		}
    	}								
    }
}

int main(void)
{
	/* 首次烧录 */
#if 0
	sys.param.update = true;
	sys.param.id = 1;
	flash_set_param(&sys.param);
#endif

	sys.param = flash_get_param();
	sys.erase = false;
	sys.port = 0;

	if (!sys.param.update) {
		jump_app();
	}
	
	bsp_init();
	/* 关闭继电器 */
	relay_ctrl(1);
	lora_init();
	
	update_app();
}

