/****************************************Copyright (c)****************************************************

**--------------File Info---------------------------------------------------------------------------------
** File name:
** Last modified Date:
** Last Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:               yunke120
** Created date:           2023/04/09
** Version:                  1.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include "task_robot.h"	
#include "tim.h"
#include "bsp_motor.h"
#include "protocol.h"
#include "robot.h"

float robotSetVelocity = 0.5;

/* 创建互斥锁 */
osMutexId_t robotVelocityMutexHandle;
const osMutexAttr_t robotVelocityMutex_attributes = {
	.name = "robotVelocityMutex"};

/**
 * 电机运行任务
 */
osThreadId_t robot_handle;
const osThreadAttr_t robot_attr = {
	.name = "robot",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityLow1,
};

osMessageQueueId_t robotQueueHandle;
const osMessageQueueAttr_t robotQueue_attributes = {
	.name = "robotQueue"};

static void robot_entry(void *param)
{
	uint8_t buf[PROTOCOL_MSG_LEN] = {0};
	uint8_t data[6] = {0};
    union UInt16Union v;

	robot_set_dir(Robot_Stop);

	motor_set_velocity(Wheel_LT, 0);
	motor_set_enable(Wheel_LT, eWheelEnable);

	motor_set_velocity(Wheel_RT, 0);
	motor_set_enable(Wheel_RT, eWheelEnable);

	motor_set_velocity(Wheel_LB, 0);
	motor_set_enable(Wheel_LB, eWheelEnable);

	motor_set_velocity(Wheel_RB, 0);
	motor_set_enable(Wheel_RB, eWheelEnable);

	while (1)
	{
		if (osMessageQueueGet(robotQueueHandle, buf, NULL, osWaitForever) == osOK)
		{
			switch (buf[2])
			{
			case HandShake:
				protocol_send(Robot, HandShake, data, 0);
				break;
			case RobotVelocity:		/* 设置机器人运行速度 */
				v.parts.lowByte = buf[4];
				v.parts.highByte = buf[5];
				osMutexAcquire(robotVelocityMutexHandle, portMAX_DELAY);
				robotSetVelocity = (float)v.value/1000.0;
				osMutexRelease(robotVelocityMutexHandle);
//				app_printf("v = %.2f", robotSetVelocity);
				break;
			default:
				robot_set_dir((eRobotDir)buf[2]);
				break;
			}
		}
	}
}

void create_robot_thread(void)
{
	robotVelocityMutexHandle = osMutexNew(&robotVelocityMutex_attributes);
	if (robotVelocityMutexHandle == NULL)
	{
		// 创建失败
		printf("Failed to create robotVelocityMutexHandle.\r\n");
		return;
	}
	
	robotQueueHandle = osMessageQueueNew(3, PROTOCOL_MSG_LEN * sizeof(uint8_t), &robotQueue_attributes);
	if (robotQueueHandle)
		printf("<create> [queue] : robot\t> 1\r\n");
	else
		printf("<create> [queue] : robot\t> 0\r\n");

	robot_handle = osThreadNew(robot_entry, NULL, &robot_attr);

	if (robot_handle == NULL)
		printf("<create> [task]	: robot		> 0\r\n");
	else
		printf("<create> [task]	: robot		> 1\r\n");
}


