#include "app.h"

uint8_t check_sum(uint8_t *buf, uint16_t len) 
{
	uint16_t i;
	uint8_t sum = 0;

    for(i = 0; i < len; i++) {
        sum += buf[i];
    }

    return sum;        
}

void task_key(void *p_arg)
{
	uint8_t cnt;
	(void)p_arg;
	
	sys_ctrl.switch_state = key_get();
	sys_ctrl.relay_state = false;

	while(1) {		
		if (sys_ctrl.switch_state != key_get()) {
			/* 连续12次检测不到异常电平 */
			if (++cnt > 12) {
				cnt = 0;
				sys_ctrl.switch_state = !sys_ctrl.switch_state;
				sys_ctrl.relay_state = !sys_ctrl.relay_state;
				relay_ctrl(sys_ctrl.relay_state);
				led_ctrl(sys_ctrl.relay_state);
				lora_send(0, 0x0, SET_RELAY_NOTICE, &sys_ctrl.relay_state, 1);
				/* 防止频繁操作 */
				OSTimeDly(500);				
			}
		} else {
			cnt = 0;
		}
		
		OSTimeDly(10);
	}
}

void task_dht11(void *p_arg)
{
	while(1) {
		OSTimeDlyHMSM(0, 0, 5, 0);		
		memset(sys_ctrl.dht11_buf, 0, 5);
		dht11_read_data(sys_ctrl.dht11_buf);	
	}
}


extern uint8_t dat_ready;
extern uint32_t ir_dat;

void task_ir(void *p_arg)
{	
	while(1) {
		if (dat_ready) {
			dat_ready = 0;
        
			uint8_t *dat = (uint8_t *)(&ir_dat);
			
			/* stm32和stm8的大小端相反 */
			if (dat[3] == 0x00 && dat[2] == 0xfd) {
				if(dat[1] == 0x80) {
					sys_ctrl.relay_state = !sys_ctrl.relay_state;
					relay_ctrl(sys_ctrl.relay_state);
					led_ctrl(sys_ctrl.relay_state);
					lora_send(0, 0x0, SET_RELAY_NOTICE, &sys_ctrl.relay_state, 1);
					/* 防止频繁操作 */
					//OSTimeDly(500);		
				}
			}			
		}			
		OSTimeDly(100);		
	}
}
