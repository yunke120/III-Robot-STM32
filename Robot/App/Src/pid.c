#include "pid.h"

void pid_init(PID_t *pid, float kp, float ki, float kd)
{
    pid->set_val = 0.0f;
    pid->actual_val = 0.0f;
    pid->err = 0.0f;
    pid->err_last = 0.0f;
    pid->err_next = 0.0f;
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->output = 0.0f;
    pid->integral = 0.0f;
    pid->umax = 0.0f;
    pid->umin = 0.0f;
}

/*
    PID
	pwm += Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
*/
int pid_execute(PID_t *pid, float actual_val, float set_val)
{
    pid->set_val = set_val;
	pid->actual_val = actual_val;
	pid->err = set_val - actual_val;
	pid->output += pid->Kp * (pid->err - pid->err_last) + pid->Ki * pid->err;
	pid->err_last = pid->err;
	
    if (pid->output > 16800)
        pid->output = 16800;
    if (pid->output < -16800)
        pid->output = -16800;
    return pid->output;
}

#if 0

// �ٶȿ���PID����
float Velocity_KP = 1000, Velocity_KI = 100;

/**************************************************************************
    �������ܣ�����ʽPI������
    ��ڲ���������������ֵ(ʵ���ٶ�)��Ŀ���ٶ�
    ����  ֵ�����PWM
    ��������ʽ��ɢPID��ʽ
    pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
    e(k)������ƫ��
    e(k-1)������һ�ε�ƫ��  �Դ�����
    pwm�����������
    �����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
    pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_LT(float Encoder, float Target)
{
    static float Bias, Pwm, Last_bias;
    Bias = Target - Encoder; // Calculate the deviation //����ƫ��
    Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;
    if (Pwm > 16800)
        Pwm = 16800;
    if (Pwm < -16800)
        Pwm = -16800;
    Last_bias = Bias; // Save the last deviation //������һ��ƫ��
    return Pwm;
}

int Incremental_PI_RT(float Encoder, float Target)
{
    static float Bias, Pwm, Last_bias;
    Bias = Target - Encoder; // Calculate the deviation //����ƫ��
    Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;
    if (Pwm > 16800)
        Pwm = 16800;
    if (Pwm < -16800)
        Pwm = -16800;
    Last_bias = Bias; // Save the last deviation //������һ��ƫ��
    return Pwm;
}

int Incremental_PI_LB(float Encoder, float Target)
{
    static float Bias, Pwm, Last_bias;
    Bias = Target - Encoder; // Calculate the deviation //����ƫ��
    Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;
    if (Pwm > 16800)
        Pwm = 16800;
    if (Pwm < -16800)
        Pwm = -16800;
    Last_bias = Bias; // Save the last deviation //������һ��ƫ��
    return Pwm;
}

int Incremental_PI_RB(float Encoder, float Target)
{
    static float Bias, Pwm, Last_bias;
    Bias = Target - Encoder; // Calculate the deviation //����ƫ��
    Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;
    if (Pwm > 16800)
        Pwm = 16800;
    if (Pwm < -16800)
        Pwm = -16800;
    Last_bias = Bias; // Save the last deviation //������һ��ƫ��
    return Pwm;
}



float target_limit_float(float insert, float low, float high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}
int target_limit_int(int insert, int low, int high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}

#endif

