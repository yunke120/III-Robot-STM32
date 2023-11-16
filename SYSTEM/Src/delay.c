#include "delay.h"


#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��		  
#include "task.h"
#endif


static uint32_t sysTickCnt=0;

/********************************************************
	*getSysTickCnt()
	*���ȿ���֮ǰ ���� sysTickCnt
	*���ȿ���֮ǰ ���� xTaskGetTickCount()
*********************************************************/
uint32_t getSysTickCnt(void)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)	/*ϵͳ�Ѿ�����*/
		return xTaskGetTickCount();
	else
		return sysTickCnt;
}		


uint32_t delay_init(void)
{
    /* ���� TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* ���� TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
 
    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* ����  ʱ�����ڼ����� */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
 
    /* ����ʱ�����ڼ�����ֵ */
    DWT->CYCCNT = 0;
 
    /* 3 NO OPERATION instructions 3�� */
    
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
 
    /* ���ʱ�����ڼ������Ƿ�������*/
    if(DWT->CYCCNT)
    {
       return 0; /*clock cycle counter started*/
    }
    else
    {
      return 1; /*clock cycle counter not started*/
    }
}

 
inline void delay_ms(volatile uint32_t milliseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000);
  milliseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < milliseconds);
}

inline void delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
 
  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
 
  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}




































