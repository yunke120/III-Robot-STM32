#ifndef __APP_PID_H
#define __APP_PID_H

#include "main.h"


typedef struct _pid
{
	float set_val;	  /* �趨ֵ 		*/
	float actual_val; /* ʵ��ֵ		*/
	float err;		  /* ƫ��ֵ		*/
	float err_last;	  /* ��һ��ƫ��ֵ	*/
	float err_next;	  /* ��һ��ƫ��ֵ	*/
	float Kp, Ki, Kd; /* �趨ֵ		*/
	float output;	  /* ʵ��ת��ֵ	*/
	float integral;	  /* �����ۻ�		*/
	float umax;		  /* ƫ������ֵ	*/
	float umin;		  /* ƫ������ֵ	*/
} PID_t;

void pid_init(PID_t *pid, float kp, float ki, float kd);
int pid_execute(PID_t *pid, float actual_val, float set_val);

//int Incremental_PI_LT(float Encoder, float Target);
//int Incremental_PI_RT(float Encoder, float Target);
//int Incremental_PI_LB(float Encoder, float Target);
//int Incremental_PI_RB(float Encoder, float Target);

//float target_limit_float(float insert, float low, float high);
//int target_limit_int(int insert, int low, int high);

#endif /* __APP_PID_H */
