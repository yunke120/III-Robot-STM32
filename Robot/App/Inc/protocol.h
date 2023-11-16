
#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "main.h"

#define PROTOCOL_MSG_LEN 12
#define PROTOCOL_USART_Handle huart3
#define PROTOCOL_USART_IRQHandler USART3_IRQHandler

#define FrameHeader 0xAA
#define FrameTail 0xDD

struct UInt16Parts {
    uint8_t lowByte;
    uint8_t highByte;
};

// 定义一个联合体，用于存储16位整数和结构
union UInt16Union {
    short value;
    struct UInt16Parts parts;
};


typedef enum
{
	DEVNone = 0x00,
	Robot,
	PTZ,
	RoboticArm,
	Battery,
	IMU,
	Sensor,
} eDEVICE;

/* command send prase */
typedef enum
{
	Reserved,
	RobotVelocity,
	RobotIMU,
	RobotBatVoltage,
	RobotGetMQ2,
}eSRobot;

typedef enum
{
	HandShake = 0xFF,
} eRobot;

typedef enum
{
	GetVoltage = 0x01,
} eBattery;

typedef enum
{
	AngularVelocity = 0x01, /* 角速度 */
	Posture,				/* 姿态 */
	Quaternion,				/* 四元数 */
}eIMU;

typedef enum
{
	GetMQ2 = 0x01,
}eSensor;

void protocol_init(void);
void protocol_send(eDEVICE device, unsigned char cmd, unsigned char *userdata, unsigned char len);
void app_printf(const char *format, ...);

#endif /* __PROTOCOL_H */
