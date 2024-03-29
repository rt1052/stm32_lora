#include "app.h"
#include "bsp.h"

static OS_STK task_start_stk[TASK_START_STK_SIZE];
static OS_STK task_key_stk[TASK_KEY_STK_SIZE];
static OS_STK task_lora_stk[TASK_LORA_STK_SIZE];
static OS_STK task_ctrl_stk[TASK_CTRL_STK_SIZE];
static OS_STK task_dht11_stk[TASK_DHT11_STK_SIZE];
static OS_STK task_ir_stk[TASK_IR_STK_SIZE];

void task_start(void *p_arg);

OS_EVENT *lora_msg;
OS_EVENT *lora_q;

void *msg_arr[3];

param_t param;

int main(void)
{	
	SCB->VTOR = 0x08004000;
	
	param = flash_get_param();
	if (param.update == true) {
		param.update = false;
		flash_set_param(&param);
	}

	bsp_init();

	OSInit();

	OSTaskCreate(task_start, (void *)0, &task_start_stk[TASK_START_STK_SIZE-1], 4);
	OSStart();
}

static void task_start(void *p_arg)
{
	(void)p_arg;
	
	OSTaskCreate(task_key, 0, &task_key_stk[TASK_KEY_STK_SIZE-1], TASK_KEY_PRIO);
	OSTaskCreate(task_lora, 0, &task_lora_stk[TASK_LORA_STK_SIZE-1], TASK_LORA_PRIO);
	OSTaskCreate(task_ctrl, 0, &task_ctrl_stk[TASK_CTRL_STK_SIZE-1], TASK_CTRL_PRIO);
	/* dht11 hsa the highest prio */	
	//OSTaskCreate(task_dht11, 0, &task_dht11_stk[TASK_DHT11_STK_SIZE-1], TASK_DHT11_PRIO);
	OSTaskCreate(task_ir, 0, &task_ir_stk[TASK_IR_STK_SIZE-1], TASK_IR_PRIO);
	
	// lora_msg = OSMboxCreate((void *)0);
	lora_q = OSQCreate(msg_arr, 3);

	OSTaskDel(OS_PRIO_SELF);
}



