

#include <stdio.h>
#include <stdlib.h>

#include "task_encoder.h"
#include "delay.h"
#include "bsp_encoder.h"
#include "bsp_motor.h"
#include "robot.h"
#include "pid.h"
#include "protocol.h"

extern float robotSetVelocity;
extern osMutexId_t robotVelocityMutexHandle;

osThreadId_t encoder_handle;
const osThreadAttr_t encoder_attr = {
	.name = "encoder",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityLow1,
};

/**
 * @brief 求绝对值
 *
 * @param val
 * @return uint32_t
 */
static inline uint32_t _abs_(int32_t val)
{
	if (val < 0)
		return -val;
	return val;
}

static void encoder_entry(void *param)
{
	uint8_t userdata[6] = {0};
	union UInt16Union data;
	int pwm_LT=0, pwm_RT=0, pwm_LB=0, pwm_RB=0;
	uint32_t cnt = 0;
	float set_velocity = 0.0f;
	uint32_t lastWakeTime = getSysTickCnt();
	/* 使能4路编码器 */
	encoder_set_enable(Encoder_LT, eEncoderEnable);
	encoder_set_enable(Encoder_RT, eEncoderEnable);
	encoder_set_enable(Encoder_LB, eEncoderEnable);
	encoder_set_enable(Encoder_RB, eEncoderEnable);

	// 速度环通常使用PI调节器
	PID_t *pid_LT = (PID_t *)malloc(sizeof(PID_t));
	pid_init(pid_LT, 1000,100,0);
	PID_t *pid_RT = (PID_t *)malloc(sizeof(PID_t));
	pid_init(pid_RT, 1000,100,0);
	PID_t *pid_LB = (PID_t *)malloc(sizeof(PID_t));
	pid_init(pid_LB, 1000,100,0);
	PID_t *pid_RB = (PID_t *)malloc(sizeof(PID_t));
	pid_init(pid_RB, 1000,100,0);
	
	if(pid_LT == NULL || pid_LB == NULL || pid_RT == NULL || pid_RB == NULL)
	{
		printf("Failed to allocate memory for PID controller.");
//		osThreadDetach(encoder_handle);
		return;
	}

	while (1)
	{

		vTaskDelayUntil(&lastWakeTime, F2T(RATE_100_HZ)); /* 以10ms时间运行 */
		
		short counter_LT = encoder_get_counter(Encoder_LT);
		float vel_LT = encoder_get_velocity(Encoder_LT, counter_LT);
		eEncoderDir dir_LT = encoder_get_dir(Encoder_LT);

		short counter_RT = encoder_get_counter(Encoder_RT);
		float vel_RT = encoder_get_velocity(Encoder_RT, counter_RT);
		eEncoderDir dir_RT = encoder_get_dir(Encoder_RT);

		short counter_LB = encoder_get_counter(Encoder_LB);
		float vel_LB = encoder_get_velocity(Encoder_LB, counter_LB);
		eEncoderDir dir_LB = encoder_get_dir(Encoder_LB);

		short counter_RB = encoder_get_counter(Encoder_RB);
		float vel_RB = encoder_get_velocity(Encoder_RB, counter_RB);
		eEncoderDir dir_RB = encoder_get_dir(Encoder_RB);

		
		osMutexAcquire(robotVelocityMutexHandle, portMAX_DELAY);
		set_velocity = robotSetVelocity;
		osMutexRelease(robotVelocityMutexHandle);
		
		eRobotDir dir = robot_get_dir();
		switch(dir)
		{
			case Robot_Stop:
				pid_init(pid_LT, 1000,100,0);
				pid_init(pid_RT, 1000,100,0);
				pid_init(pid_LB, 1000,100,0);
				pid_init(pid_RB, 1000,100,0);
				pwm_LT /= 2, pwm_RT /= 2, pwm_LB /= 2, pwm_RB /= 2; // PWM缓慢减少
				break;
			case Robot_Foreward:
				pwm_LT = pid_execute(pid_LT, vel_LT, set_velocity);
				pwm_RT = pid_execute(pid_RT, vel_RT, set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, set_velocity);
				pwm_RB = pid_execute(pid_RB, vel_RB, set_velocity);
				break;

			case Robot_Backward:
				pwm_LT = pid_execute(pid_LT, vel_LT, -set_velocity);
				pwm_RT = pid_execute(pid_RT, vel_RT, -set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, -set_velocity);
				pwm_RB = pid_execute(pid_RB, vel_RB, -set_velocity);	
				break;
#if 1
			case Robot_Right_Up:
				pid_init(pid_LT, 1000,100,0);
				pwm_RT = pid_execute(pid_RT, vel_RT, set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, set_velocity);
				pid_init(pid_RB, 1000,100,0);	
				pwm_LT /= 2, pwm_RB /= 2;
				break;
			case Robot_Left_Up:
				pwm_LT = pid_execute(pid_LT, vel_LT, set_velocity);
				pid_init(pid_RT, 1000,100,0);
				pid_init(pid_LB, 1000,100,0);
				pwm_RB = pid_execute(pid_RB, vel_RB, set_velocity);
				pwm_RT /= 2, pwm_LB /= 2;
				break;
			case Robot_Right_Down:
				pwm_LT = pid_execute(pid_LT, vel_LT, -set_velocity);
				pid_init(pid_RT, 1000,100,0);
				pid_init(pid_LB, 1000,100,0);
				pwm_RB = pid_execute(pid_RB, vel_RB, -set_velocity);
				pwm_RT /= 2, pwm_LB /= 2;
				break;
			case Robot_Left_Down:
				pid_init(pid_LT, 1000,100,0);
				pwm_RT = pid_execute(pid_RT, vel_RT, -set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, -set_velocity);
				pid_init(pid_RB, 1000,100,0);	
				pwm_LT /= 2, pwm_RB /= 2;
				break;
			case Robot_Shift_Right:
				pwm_LT = pid_execute(pid_LT, vel_LT, set_velocity);
				pwm_RT = pid_execute(pid_RT, vel_RT, -set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, -set_velocity);
				pwm_RB = pid_execute(pid_RB, vel_RB, set_velocity);
				break;
			case Robot_Shift_Left:
				pwm_LT = pid_execute(pid_LT, vel_LT, -set_velocity);
				pwm_RT = pid_execute(pid_RT, vel_RT, set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, set_velocity);
				pwm_RB = pid_execute(pid_RB, vel_RB, -set_velocity);
				break;
			case Robot_Rotate_Right:
				pwm_LT = pid_execute(pid_LT, vel_LT, set_velocity);
				pwm_RT = pid_execute(pid_RT, vel_RT, -set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, set_velocity);
				pwm_RB = pid_execute(pid_RB, vel_RB, -set_velocity);
				break;
			case Robot_Rotate_Left:
				pwm_LT = pid_execute(pid_LT, vel_LT, -set_velocity);
				pwm_RT = pid_execute(pid_RT, vel_RT, set_velocity);
				pwm_LB = pid_execute(pid_LB, vel_LB, -set_velocity);
				pwm_RB = pid_execute(pid_RB, vel_RB, set_velocity);
				break;
#endif
		}
		
		motor_set_velocity(Wheel_LT, _abs_(pwm_LT));
		motor_set_velocity(Wheel_RT, _abs_(pwm_RT));
		motor_set_velocity(Wheel_LB, _abs_(pwm_LB));
		motor_set_velocity(Wheel_RB, _abs_(pwm_RB));

//		app_printf("\r\n");
//		app_printf("pwm_LT = %d, vel = %.3fm/s, counter_LT = %d\r\n", pwm_LT, vel_LT, counter_LT);
//		app_printf("pwm_RT = %d, vel = %.3fm/s\r\n", pwm_RT, vel_RT);
//		app_printf("pwm_LB = %d, vel = %.3fm/s\r\n", pwm_LB, vel_LB);
//		app_printf("pwm_RB = %d, vel = %.3fm/s\r\n", pwm_RB, vel_RB);
//		app_printf("%d %d %d %d\r\n", counter_LT, counter_RT, counter_LB, counter_RB);
		float robot_velocity = (vel_LT+vel_RT+vel_LB+vel_RB)/4.0f;
		data.value = robot_velocity * 1000;
		
		userdata[0] = data.parts.lowByte;
		userdata[1] = data.parts.highByte;
		if(((++cnt) % 50) == 0) // 10ms*50 = 500ms , 每500ms上传一次
		{
			//app_printf("robot_velocity = %.3f\r\n", robot_velocity);
			protocol_send(Robot, RobotVelocity, userdata, 2); // 目前只有前后运动的速度
			cnt = 0;
		}
	}
}

void create_encoder_thread(void)
{
	encoder_handle = osThreadNew(encoder_entry, NULL, &encoder_attr);
	if (encoder_handle == NULL)
		printf("<create> [task]	: encoder\t> 0\r\n");
	else
		printf("<create> [task]	: encoder\t> 1\r\n");
}
