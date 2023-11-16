// Header:		__BSP_ENCODER_H
// File Name: 	bsp_encoder.h
// Author:		yunke120
// Date:		2023/04/09

#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H

#include "main.h"

#define Encoder_LT_TIM htim4
#define Encoder_RT_TIM htim2
#define Encoder_LB_TIM htim5
#define Encoder_RB_TIM htim3

#define EncoderMultiples 4	/* ��������Ƶ����ȡ���ڱ�������ʼ������ */
#define ENCODER_ACCURACY 13 /* ����������13 */
#define GEARRATIO 27		/* ������ٱ� */

#define WHEEL_DIAMETER 0.127f /* ������ֱ̥�� */
#ifndef PI
#define PI 3.14159265358979f
#endif

#define CONTROL_FREQUENCY 100											/* ���������ݶ�ȡƵ�� */
#define WHEEL_PERIMETER WHEEL_DIAMETER *PI /* �������ܳ� */				// 0.4m
#define ENCODER_PRECISION EncoderMultiples *ENCODER_ACCURACY *GEARRATIO /* ���(����)ת1Ȧ��Ӧ�ı�������ֵ 1404 */

#define CALC_VELO 0.02846f /* CONTROL_FREQUENCY * WHEEL_PERIMETER / ENCODER_PRECISION */

#define VelocityMin 600
#define VelocityMax 1000

typedef enum
{
	eEncoderEnable,
	eEncoderDisable,
} eEncoderState;

typedef enum
{
	Encoder_LT,
	Encoder_RT,
	Encoder_LB,
	Encoder_RB,
} eEncoder;

typedef enum
{
	eEncoderForeward,
	eEncoderBackward,
	//	eEncoderStop,
} eEncoderDir;

void encoder_set_enable(eEncoder encoder, eEncoderState state);
short encoder_get_counter(eEncoder encoder);
float encoder_get_velocity(eEncoder encoder, short counter);
eEncoderDir encoder_get_dir(eEncoder encoder);

#endif /* __BSP_ENCODER_H */
