#include "sx1276-Hal.h"
#include "radio.h"
#include "app.h"
#include "bsp.h"


#define SYS_ID  0X4


void lora_send(uint8_t port, uint8_t id, uint8_t cmd, uint8_t *dat, uint8_t cnt)
{
	uint8_t buf[64];
	uint8_t len = cnt + 5;
	
	buf[0] = 0x42;
	buf[1] = len;
	buf[2] = port;
	buf[3] = SYS_ID;
	buf[4] = cmd;
	memcpy(buf+5, dat, cnt);
	buf[len] = check_sum(buf, len);
	
	SX1276SetTxPacket(buf, len+1);	
	OSTimeDly(100);
}

void task_lora(void *p_arg)
{	
	uint8_t buf[64];
	uint16_t len;
	
	(void)p_arg;

    SX1276Init();
    SX1276StartRx();	
		
    while(1)
    {        
        switch(SX1276Process()) {
        case RF_RX_TIMEOUT:
            // printf("timeout \r\n");
            break;
        case RF_RX_DONE:
			SX1276GetRxPacket(buf, &len);
						
			if ((buf[0] == 0x42) && (len < 10)) {
				if (buf[len-1] == check_sum(buf, len-1)) {
					/* check id */
					if (buf[3] == SYS_ID) {
						sys_ctrl.frame_cnt = (++sys_ctrl.frame_cnt) % 3;
						sys_ctrl.frame[sys_ctrl.frame_cnt].port = buf[2];
						sys_ctrl.frame[sys_ctrl.frame_cnt].cmd = buf[4];
						sys_ctrl.frame[sys_ctrl.frame_cnt].dat = buf[5];
             		    // OSMboxPost(lora_msg, &lora_frame);	
             		    OSQPost(lora_q, &sys_ctrl.frame[sys_ctrl.frame_cnt]);								
					}
				}
			}
            break;
        case RF_TX_DONE:
			SX1276StartRx();
            break;
        default:
            break;
        }
								
        OSTimeDly(10);
    }		
}

