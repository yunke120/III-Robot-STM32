
#include "bsp_imu.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

#ifndef PI
#define PI	3.141592653
#endif	

#define Rad2Degree	360/2/PI
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

/******************************************* USART DMA RECEIVE ***********************************************/
imu_rx_t imu_rx;

void imu_rx_reset(void)
{
	memset(&imu_rx, 0, sizeof(imu_rx));
	HAL_UARTEx_ReceiveToIdle_DMA(&IMU_USART_Handle, imu_rx.buf, IMU_RX_MAX_LEN);
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == IMU_USART_Handle.Instance)
	{
		imu_rx.sta.len = Size;
		imu_rx.sta.finsh = 1;
				//HAL_UART_Transmit(&huart1, imu_rx.buf, imu_rx.sta.len, HAL_MAX_DELAY);
	}
}

uint8_t imu_get_finished(void)
{
	return imu_rx.sta.finsh;
}

void imu_init(void)
{
	imu_rx_reset();
}

/******************************************* IMU DATA TRANSFORM ***********************************************/

uint8_t data_check(void)
{
	if(imu_get_finished() == 1)
	{
		if(imu_rx.buf[0] == 0xFC && imu_rx.buf[1] == 0x63 && imu_rx.buf[imu_rx.sta.len-1] == 0xFD)
		{
			return 0;
		}
		else imu_rx_reset();
	}
	return 1;
}

static float data_trans(uint8_t data_1, uint8_t data_2, uint8_t data_3, uint8_t data_4)
{
    uint32_t transition_32;
	float tmp=0;
	int sign=0;
	int exponent=0;
	float mantissa=0;
    transition_32 = 0;
    transition_32 |=  data_4<<24;   
    transition_32 |=  data_3<<16; 
	transition_32 |=  data_2<<8;
	transition_32 |=  data_1;
    sign = (transition_32 & 0x80000000) ? -1 : 1;
	exponent = ((transition_32 >> 23) & 0xff) - 127;
	mantissa = 1 + ((float)(transition_32 & 0x7fffff) / 0x7fffff);
	tmp=sign * mantissa * pow(2, exponent);
	return tmp;
}

void imu_get_posture(IMU_t *imu)
{
//	memcpy(imu->raw_roll,    imu_rx.buf + 7,  4);
//	memcpy(imu->raw_pitch,   imu_rx.buf + 11, 4);
//	memcpy(imu->raw_heading, imu_rx.buf + 15, 4);
	
	float a = data_trans(imu_rx.buf[7], imu_rx.buf[8], imu_rx.buf[9], imu_rx.buf[10]);
	float b = data_trans(imu_rx.buf[11], imu_rx.buf[12], imu_rx.buf[13], imu_rx.buf[14]);
	float c = data_trans(imu_rx.buf[15], imu_rx.buf[16], imu_rx.buf[17], imu_rx.buf[18]);
	
	imu->roll    = a * Rad2Degree;
	imu->pitch   = b * Rad2Degree;
	imu->heading = c * Rad2Degree;
}























