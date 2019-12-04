#include "app.h"
#include "bsp.h"

ctrl_t sys_ctrl;

void task_ctrl(void *p_arg)
{
	int key;	
	uint8_t *dat;
	INT8U err;
	
	(void)p_arg;
	
	while(1) {
		// OSMboxPend(lora_msg, 0, &err);	
		frame_t *frame = (frame_t *)OSQPend(lora_q, 0, &err);

		switch (frame->cmd) {
			case SET_RELAY_REQUEST:
				sys_ctrl.relay_state = frame->dat;
				relay_ctrl(sys_ctrl.relay_state);
				led_ctrl(sys_ctrl.relay_state);
				lora_send(frame->port, 0x0, SET_RELAY_RESPONSE, &sys_ctrl.relay_state, 1);
				break;
			
			case GET_RELAY_REQUEST:					
				/* 延时一会再发送，否则中控可能还没有切换到发送模式 */
				OSTimeDly(100);
				lora_send(frame->port, 0x0, GET_RELAY_RESPONSE, &sys_ctrl.relay_state, 1);
				break;
			
			case GET_PARAM_REQUEST:
				OSTimeDly(100);
				//lora_send(frame->port, 0x0, GET_PARAM_RESPONSE, sys_ctrl.dht11_buf, 4);
				break;
			
			case UPDATE_START_REQUEST:
				NVIC_SystemReset();
				break;
		}
		OSTimeDly(50);
	}
}

