/****************************************Copyright (c)****************************************************

**--------------File Info---------------------------------------------------------------------------------
** File name:
** Last modified Date:
** Last Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:               yunke120
** Created date:           2023/04/08
** Version:                  1.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------*/

#include "robot.h"

/* ����С���˶�����Ĭ��ֹͣ */
static eRobotDir g_robot_Dir = Robot_Stop;

/**
 * @brief  ����С������
 *
 * @param dir С������
 */
void robot_set_dir(eRobotDir dir)
{
	eRobotDir r = dir;
	if (g_robot_Dir == r)
		return;
	switch (r)
	{
	case Robot_Stop: /* ֹͣ */
		motor_set_dir(Wheel_LT, Wheel_Stop);
		motor_set_dir(Wheel_RT, Wheel_Stop);
		motor_set_dir(Wheel_LB, Wheel_Stop);
		motor_set_dir(Wheel_RB, Wheel_Stop);
		break;
	case Robot_Foreward: /* �ĸ�����ͬʱͬ��ǰ�� */
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	case Robot_Backward: /* �ĸ�����ͬʱͬ�ٺ��� */
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Right_Up:
		motor_set_dir(Wheel_LT, Wheel_Stop);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Stop);
		break;
	case Robot_Left_Up:
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Stop);
		motor_set_dir(Wheel_LB, Wheel_Stop);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	case Robot_Right_Down:
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Stop);
		motor_set_dir(Wheel_LB, Wheel_Stop);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Left_Down:
		motor_set_dir(Wheel_LT, Wheel_Stop);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Stop);
		break;
	case Robot_Shift_Right:
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	case Robot_Shift_Left:
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Rotate_Right:
		motor_set_dir(Wheel_LT, Wheel_Foreward);
		motor_set_dir(Wheel_RT, Wheel_Backward);
		motor_set_dir(Wheel_LB, Wheel_Foreward);
		motor_set_dir(Wheel_RB, Wheel_Backward);
		break;
	case Robot_Rotate_Left:
		motor_set_dir(Wheel_LT, Wheel_Backward);
		motor_set_dir(Wheel_RT, Wheel_Foreward);
		motor_set_dir(Wheel_LB, Wheel_Backward);
		motor_set_dir(Wheel_RB, Wheel_Foreward);
		break;
	}
	g_robot_Dir = r;
}

/**
 * @brief ��ȡС����ǰ�˶�����
 *
 * @return erobotDir С������
 */
eRobotDir robot_get_dir(void)
{
	return g_robot_Dir;
}
