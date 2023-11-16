
#include <stdio.h>
#include <string.h>

#include "task_led.h"
#include "protocol.h"
#include "adc.h"
#include "dma.h"
#include "bsp_ina226.h"

#define FILTER_NUM			2	/* 接收FILTER_NUM次滤波 */
#define ADC_CHANNEL_NUM		2

uint8_t DMA_RECV_FLAG = 0;	/* DMA接收完成标志 */
uint16_t ADCBuf[FILTER_NUM][ADC_CHANNEL_NUM] = {0}; 	/* ADC_CHANNEL_NUM路ADC值存储区 */

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

osThreadId_t led_handle;
const osThreadAttr_t led_attr = {
  .name = "led",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};

//static float get_bat_voltage()
//{
//	float voltage;
//	HAL_ADC_Start(&hadc1);
//	if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
//		uint32_t adc_value = HAL_ADC_GetValue(&hadc1);

//		voltage=adc_value*3.24*11.0*1.0/1.0/4096;
////		app_printf("volatge = %.2f\r\n", voltage);
//	}
//	HAL_ADC_Stop(&hadc1);
//	return voltage;
//}

void DMA2_Stream0_IRQHandler(void)
{
	HAL_ADC_Stop_DMA(&hadc1); /* 接收一次就关闭 */
	DMA_RECV_FLAG = 1;
	HAL_DMA_IRQHandler(&hdma_adc1);
}

/*
	volatge = 23.620
	current = 0.135
	power = 3.200
*/
static void led_entry(void *param)
{
	uint8_t ret;
	uint8_t userdata[6] = {0};
	union UInt16Union data;
	uint32_t sum_vol, sum_mq2;
	float f_vol, f_mq2;
	float number;
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCBuf, FILTER_NUM*ADC_CHANNEL_NUM);
	ina226_uart_initEx();
	while(1)
	{
#if 1
		if(DMA_RECV_FLAG)
		{
			DMA_RECV_FLAG = 0;
			sum_vol = 0, sum_mq2 = 0;
			for(int i = 0; i < FILTER_NUM-1; i++)
			{
//				sum_vol += ADCBuf[i][0];
				sum_mq2 += ADCBuf[i][1];
//				app_printf("%d\t%d\r\n",  ADCBuf[i][0], ADCBuf[i][1]);
			}
//			app_printf("\r\n");
			memset(ADCBuf, 0, sizeof(ADCBuf));
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCBuf, FILTER_NUM*ADC_CHANNEL_NUM);
			
//			f_vol = (float)(sum_vol/(FILTER_NUM-1))*3.24*11.0*1.0/1.0/4096;
			f_mq2 = (float)(sum_mq2/(FILTER_NUM-1))/4096.0*3.3;
			
//			app_printf("vol = %d, mq2 = %d\r\n", (sum_vol/FILTER_NUM), (sum_mq2/FILTER_NUM));
//			app_printf("vol = %.2f, mq2 = %.2f\r\n\r\n", f_vol, f_mq2);
//			data.value = f_vol * 1000;
//			userdata[0] = data.parts.lowByte;
//			userdata[1] = data.parts.highByte;
//			protocol_send(Battery, GetVoltage, userdata, 2);
			
			data.value = f_mq2 * 1000;
			userdata[0] = data.parts.lowByte;
			userdata[1] = data.parts.highByte;
			protocol_send(Robot, RobotGetMQ2, userdata, 2);
			
		}
		
		ina226_uart_rx_restart();
		ina226_uart_printf("%s\r\n", INA226_GET_VOLTAGE);	/* 获取输入电压值 */
		osDelay(100);
		f_vol = 0;
		ret = ina226_get_response(&f_vol);
		if(ret == 0)
		{
			data.value = f_vol * 1000;
			userdata[0] = data.parts.lowByte;
			userdata[1] = data.parts.highByte;
			protocol_send(Robot, RobotBatVoltage, userdata, 2);
		}
//		if(ret != 0 )
//		{
//			app_printf("get voltage failed.\r\n");
//		}
//		else
//		{
//			app_printf("volatge = %.3f\r\n", number);
//		}
		
//		ina226_uart_rx_restart();
//		ina226_uart_printf("%s\r\n", INA226_GET_CURRENT);	/* 获取输入电流值 */
//		osDelay(100);
//		number = 0;
//		ret = ina226_get_response(&number);
//		if(ret != 0 )
//		{
//			app_printf("get current failed.\r\n");
//		}
//		else
//		{
//			app_printf("current = %.3f\r\n", number);
//		}
//		
//		ina226_uart_rx_restart();
//		ina226_uart_printf("%s\r\n", INA226_GET_POWER);	/* 获取输入功率值 */
//		osDelay(100);
//		number = 0;
//		ret = ina226_get_response(&number);
//		if(ret != 0 )
//		{
//			app_printf("get power failed.\r\n");
//		}
//		else
//		{
//			app_printf("power = %.3f\r\n", number);
//		}
		
#else
		app_printf("get voltage failed.\r\n");
#endif
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); /* 运行指示 */
		osDelay(2000);
	}
}


void create_led_thread(void)
{
	led_handle = osThreadNew(led_entry, NULL, &led_attr);
	if(led_handle == NULL)
		printf("<create> [task]	: led		> 0\r\n");
	else
		printf("<create> [task]	: led		> 1\r\n");
}


