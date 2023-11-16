// Header:
// File Name: 
// Author:	yunke120
// Date:	2023/04/15

#ifndef __BSP_INA226_H
#define	__BSP_INA226_H

#include "main.h"

#define INA226_USART_IRQHandler		USART2_IRQHandler


#define INA226_UART_RX_BUF_SIZE		128
#define INA226_UART_TX_BUF_SIZE		128


#define INA226_AT_TEST					"AT"
#define INA226_RESET						"AT+RESET"
#define INA226_GET_VOLTAGE			"AT+V"
#define INA226_GET_CURRENT			"AT+C"
#define INA226_GET_POWER				"AT+P"

void ina226_uart_initEx(void);  
void ina226_uart_printf(char *fmt, ...);       
void ina226_uart_rx_restart(void);            
   
uint8_t	ina226_get_finish_flag(void);
uint8_t ina226_get_response(float * data);


#endif /* __BSP_INA226_H */


