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

	���ֱ�� 127mm



**--------------------------------------------------------------------------------------------------------*/

#include "bsp_encoder.h"
#include "tim.h"
#include "stdio.h"


/**
 * @brief ���ñ�����ʹ�ܺ���
 *
 * @param encoder ����������
 * @param state ʹ��״̬
 */
void encoder_set_enable(eEncoder encoder, eEncoderState state)
{
	eEncoder e = encoder;
	eEncoderState s = state;

	switch (e)
	{
	case Encoder_LT:
		(s == eEncoderEnable) ? HAL_TIM_Encoder_Start(&Encoder_LT_TIM, TIM_CHANNEL_ALL) : HAL_TIM_Encoder_Stop(&Encoder_LT_TIM, TIM_CHANNEL_ALL);

		break;
	case Encoder_RT:
		(s == eEncoderEnable) ? HAL_TIM_Encoder_Start(&Encoder_RT_TIM, TIM_CHANNEL_ALL) : HAL_TIM_Encoder_Stop(&Encoder_RT_TIM, TIM_CHANNEL_ALL);
		break;
	case Encoder_LB:
		(s == eEncoderEnable) ? HAL_TIM_Encoder_Start(&Encoder_LB_TIM, TIM_CHANNEL_ALL) : HAL_TIM_Encoder_Stop(&Encoder_LB_TIM, TIM_CHANNEL_ALL);
		break;
	case Encoder_RB:
		(s == eEncoderEnable) ? HAL_TIM_Encoder_Start(&Encoder_RB_TIM, TIM_CHANNEL_ALL) : HAL_TIM_Encoder_Stop(&Encoder_RB_TIM, TIM_CHANNEL_ALL);
		break;
	default:
		break;
	}
}

/**
 * @brief ��ȡ����������ֵ
 *
 * @param encoder ����������
 * @param t_ms ���ʱ��ms
 * @return short ����ֵ
 */
short encoder_get_counter(eEncoder encoder)
{
	eEncoder e = encoder;
	short vel = 0;

	switch (e)
	{
	case Encoder_LT:
	{
		vel = -(__HAL_TIM_GET_COUNTER(&Encoder_LT_TIM)); // ��ȡencoder�������ļ���ֵ
		__HAL_TIM_SET_COUNTER(&Encoder_LT_TIM, 0);
	}
	break;
	case Encoder_RT:
	{
		vel = (__HAL_TIM_GET_COUNTER(&Encoder_RT_TIM)); // ��ȡencoder�������ļ���ֵ
		__HAL_TIM_SET_COUNTER(&Encoder_RT_TIM, 0);
	}
	break;
	case Encoder_LB:
	{
		vel = -(__HAL_TIM_GET_COUNTER(&Encoder_LB_TIM)); // ��ȡencoder�������ļ���ֵ
		__HAL_TIM_SET_COUNTER(&Encoder_LB_TIM, 0);
	}
	break;
	case Encoder_RB:
	{
		vel = (__HAL_TIM_GET_COUNTER(&Encoder_RB_TIM)); // ��ȡencoder�������ļ���ֵ
		__HAL_TIM_SET_COUNTER(&Encoder_RB_TIM, 0);
	}
	break;
	}

	return vel;
}

/**
 * @brief ��ȡ�������ٶ�
 *
 * @param encoder
 * @param counter
 * @return float
 */
float encoder_get_velocity(eEncoder encoder, short counter)
{
	eEncoder e = encoder;
	float vel = 0.0f;

	switch (e)
	{
	case Encoder_LT:
	{
		vel = counter * CALC_VELO;
	}
	break;
	case Encoder_RT:
	{
		vel = counter * CALC_VELO;
	}
	break;
	case Encoder_LB:
	{
		vel = counter * CALC_VELO;
	}
	break;
	case Encoder_RB:
	{
		vel = counter * CALC_VELO;
	}
	break;
	}
	return vel;
}

/**
 * @brief ��ȡ����������
 *
 * @param encoder ����������
 * @return eEncoderDir ����ö��
 */
eEncoderDir encoder_get_dir(eEncoder encoder)
{
	eEncoder e = encoder;
	int dir = 0;

	switch (e)
	{
	case Encoder_LT:
		dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&Encoder_LT_TIM) == 0 ? 1 : 0;
		break;
	case Encoder_RT:
		dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&Encoder_RT_TIM);
		break;
	case Encoder_LB:
		dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&Encoder_LB_TIM) == 0 ? 1 : 0;
		break;
	case Encoder_RB:
		dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&Encoder_RB_TIM);
		break;
	}
	return (eEncoderDir)dir;
}
