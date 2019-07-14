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

void task_led(void *p_arg)
{
	(void)p_arg;	

	while(1) {
		if (sys_ctrl.switch_state != key_get()) {
			OSTimeDly(50);
			if (sys_ctrl.switch_state != key_get()) {
				sys_ctrl.switch_state = key_get();
				sys_ctrl.relay_state = !sys_ctrl.relay_state;
				relay_ctrl(sys_ctrl.relay_state);
				//led_ctrl(sys_ctrl.relay_state);
				lora_send(0, 0x0, SET_RELAY_NOTICE, &sys_ctrl.relay_state, 1);
				/* ·ÀÖ¹Æµ·±²Ù×÷ */
				OSTimeDly(500);
			}
		}
		OSTimeDly(50);	
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

void task_ir(void *p_arg)
{
	while(1) {
		//relay_ctrl(0);
		OSTimeDly(1000);
		//OSTimeDlyHMSM(0, 0, 1, 0);	
		//relay_ctrl(1);
		//OSTimeDlyHMSM(0, 0, 1, 0);			
		OSTimeDly(1000);
	}
}
