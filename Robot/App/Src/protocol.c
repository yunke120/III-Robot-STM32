#include "protocol.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern UART_HandleTypeDef huart3;
extern osMessageQueueId_t protocolQueueHandle;

/* 创建互斥锁 */
osMutexId_t protocolMutexHandle;
const osMutexAttr_t protocolMutex_attributes = {
	.name = "protocolMutex"};

static uint8_t USART1_RX_BUF[PROTOCOL_MSG_LEN]; /* 接收缓存区 */
static uint8_t rx_count;						/* 接收计数 */

void protocol_init(void)
{
	protocolMutexHandle = osMutexNew(&protocolMutex_attributes);
	if (protocolMutexHandle == NULL)
	{
		// 创建失败
		printf("Failed to create protocolMutexHandle.\r\n");
		return;
	}
//	osMutexRelease(protocolMutexHandle);
	__HAL_UART_CLEAR_FLAG(&PROTOCOL_USART_Handle, UART_FLAG_RXNE);
	__HAL_UART_ENABLE_IT(&PROTOCOL_USART_Handle, UART_IT_RXNE);
}

/*
	线程安全的printf
*/

void app_printf(const char *format, ...) {
    osMutexAcquire(protocolMutexHandle, portMAX_DELAY);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    osMutexRelease(protocolMutexHandle);
}

static uint8_t check_data(uint8_t *buf, unsigned char len)
{
	unsigned char check_sum = 0, i;
	for (i = 0; i < len; i++)
	{
		check_sum = check_sum ^ buf[i];
	}
	return check_sum;
}

/*
	发送数据
*/
void protocol_send(eDEVICE device, unsigned char cmd, unsigned char *userdata, unsigned char len)
{
#if 1
	unsigned char buf[PROTOCOL_MSG_LEN] = {0};

	buf[0] = FrameHeader;
	buf[1] = device;
	buf[2] = cmd;
	buf[3] = len;
	
	for(int i = 0; i < len; i++)
	{
		buf[4+i] = userdata[i];
	}
	
	buf[10] = check_data(buf, 10);
	buf[11] = 0xDD;

	osMutexAcquire(protocolMutexHandle, portMAX_DELAY);
	HAL_UART_Transmit(&PROTOCOL_USART_Handle, buf, PROTOCOL_MSG_LEN, HAL_MAX_DELAY);
	osMutexRelease(protocolMutexHandle);
#endif
}

void PROTOCOL_USART_IRQHandler(void)
{
	if (__HAL_UART_GET_FLAG(&PROTOCOL_USART_Handle, UART_FLAG_RXNE) != RESET)
	{ // 接收中断：接收到数据

		uint8_t data;
		data = READ_REG(PROTOCOL_USART_Handle.Instance->DR);
		USART1_RX_BUF[rx_count] = data;
		if (data == FrameHeader || rx_count > 0)
		{
			rx_count++;
		}
		else
		{
			rx_count = 0;
		}

		if (rx_count == PROTOCOL_MSG_LEN)
		{
			rx_count = 0;
//			if (USART1_RX_BUF[10] == check_data(USART1_RX_BUF, 10))
//			{
#if 0
				for(int i = 0; i < PROTOCOL_MSG_LEN; i++)
				{
					printf("%02x ", USART1_RX_BUF[i]);
				}
#endif
				osMessageQueuePut(protocolQueueHandle, USART1_RX_BUF, 0U, 0U);
//			}
		}
	}

	HAL_UART_IRQHandler(&PROTOCOL_USART_Handle);
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

int fgetc(FILE *f)
{
	uint8_t ch = 0;
	HAL_UART_Receive(&huart3, &ch, 1, HAL_MAX_DELAY);
	return ch;
}

#if 0

// gcc protocol.c -o protocol -I ../inc

#include <stdint.h>
#include <stdio.h>

int main() {
    uint8_t buf[] = {0xaa, 0x55, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xdd};
    int len = sizeof(buf) / sizeof(buf[0]);
    unsigned char result = _data_check(buf, len);
    if(result == 0) {
        printf("Data check succeeded.\n");
    } else {
        printf("Data check failed.\n");
    }
    return 0;
}

#endif
