
#ifndef __BSP_IMU_H
#define	__BSP_IMU_H

#include "main.h"

#define IMU_RX_MAX_LEN		128
#define IMU_USART_Handle	huart2


typedef struct {
	uint8_t buf[IMU_RX_MAX_LEN];
    struct
    {
        uint16_t len    : 15;                               /* ֡���ճ��ȣ�sta[14:0] */
        uint16_t finsh  : 1;                                /* ֡������ɱ�־��sta[15] */
    } sta;                                                  /* ֡״̬��Ϣ */
}imu_rx_t;

typedef struct {
	uint8_t raw_roll[4];
	uint8_t raw_pitch[4];
	uint8_t raw_heading[4];
	float roll;		// ��λ rad תΪ���� rad * 360/2/PI
	float pitch;
	float heading;
}IMU_t;

void imu_init(void);
void imu_rx_reset(void);
uint8_t imu_get_finished(void);

uint8_t data_check(void);
void imu_get_posture(IMU_t *imu);

#endif /* __BSP_IMU_H */

