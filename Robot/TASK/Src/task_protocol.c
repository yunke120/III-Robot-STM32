#include "task_protocol.h"
#include "protocol.h"
#include "stdio.h"

#include "bsp_oled.h"

extern osMessageQueueId_t robotQueueHandle;

osThreadId_t protocol_handle;
const osThreadAttr_t protocol_attr = {
	.name = "protocol",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityLow1,
};

osMessageQueueId_t protocolQueueHandle;
const osMessageQueueAttr_t protocolQueue_attributes = {
	.name = "protocolQueue"};

static void protocol_entry(void *param)
{
	uint8_t rx_buf[PROTOCOL_MSG_LEN];
	eDEVICE device = DEVNone;
	protocol_init();
	
	OLED_Init();
	OLED_ShowString(0, 0, (uint8_t *)"III-LAB");

	while (1)
	{
		if (osMessageQueueGet(protocolQueueHandle, rx_buf, NULL, osWaitForever) == osOK)
		{
#if 0
			for(int i = 0; i < PROTOCOL_MSG_LEN; i++)
			{
				printf("%02x ", rx_buf[i]);
			}
			printf("\r\n");
#endif
			// Todo:
			device = (eDEVICE)rx_buf[1]; // 根据设备地址不同进行数据分发
			switch (device)
			{
			case Robot:
				osMessageQueuePut(robotQueueHandle, rx_buf, 0U, 0U);
				break;
			case PTZ:
				break;
			case RoboticArm:
				break;
			case Battery:
				break;
			case DEVNone:
				break;
			}
		}
	}
}

void create_protocol_thread(void)
{
	protocolQueueHandle = osMessageQueueNew(10, PROTOCOL_MSG_LEN * sizeof(uint8_t), &protocolQueue_attributes);
	if (protocolQueueHandle)
		printf("<create> [queue] : protocol\t> 1\r\n");
	else
		printf("<create> [queue] : protocol\t> 0\r\n");

	protocol_handle = osThreadNew(protocol_entry, NULL, &protocol_attr);
	if (protocol_handle == NULL)
		printf("<create> [task]	: protocol\t> 0\r\n");
	else
		printf("<create> [task]	: protocol\t> 1\r\n");
}
