
#include <stdio.h>
#include <stdlib.h>

#include "task_posture.h"
#include "delay.h"
#include "bsp_imu.h"
#include "protocol.h"

extern UART_HandleTypeDef huart1;

osThreadId_t posture_handle;
const osThreadAttr_t posture_attr = {
	.name = "posture",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityLow1,
};

static void posture_entry(void *param)
{
	uint8_t userdata[6] = {0};
	uint32_t lastWakeTime = getSysTickCnt();
	union UInt16Union data;
	imu_init();
	while (1)
	{
		vTaskDelayUntil(&lastWakeTime, F2T(RATE_10_HZ)); /* 以100ms时间运行 */

		if(data_check() == 0)
		{
			IMU_t imu;
			imu_get_posture(&imu);
			//app_printf("roll = %.3f\tpitch = %.3f\theading = %.3f\r\n", imu.roll, imu.pitch, imu.heading);
			
			unsigned short roll = imu.roll * 100;  // 放大1000倍，最大6280，只用两个字节就可以保存
			unsigned short pitch = imu.pitch * 100;
			unsigned short heading = imu.heading * 100;
			
			data.value = roll;
			userdata[0] = data.parts.lowByte;
			userdata[1] = data.parts.highByte;
			
			data.value = pitch;
			userdata[2] = data.parts.lowByte;
			userdata[3] = data.parts.highByte;
			
			data.value = heading;
			userdata[4] = data.parts.lowByte;
			userdata[5] = data.parts.highByte;
			
			//protocol_send(IMU, Posture, userdata, 6); //AA 05 02 06 E4 3C 00 00 01 3F 4D DD
			imu_rx_reset();
		}
	}
}

void create_posture_thread(void)
{
	posture_handle = osThreadNew(posture_entry, NULL, &posture_attr);
	if (posture_handle == NULL)
		printf("<create> [task]	: posture\t> 0\r\n");
	else
		printf("<create> [task]	: posture\t> 1\r\n");
}

