/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
//#include "stm32l0xx.h"
#include "osObjects.h"                      // RTOS object definitions
#include "init.h"
#include "main.h"

#define ToogleGREEN() 		GPIOB->ODR ^= (1 << 4)
#define ToogleRED()				GPIOA->ODR ^= (1 << 5)

void send_str(uint8_t string[], uint8_t lenghth) 
{
		DMA1_Channel2->CMAR = (uint32_t)string; /* (3) */
		DMA1_Channel2->CCR &=~ DMA_CCR_EN;
    DMA1_Channel2->CNDTR = lenghth;/* Data size */
    DMA1_Channel2->CCR |= DMA_CCR_EN;
	  DMA1_Channel3->CCR &=~ DMA_CCR_EN;
    DMA1_Channel3->CNDTR = 255;/* Data size */
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void Init_SIM800(void)
{
 			osDelay(7000);
			led_state = SND_GOOD;
			need_setup = 0;
		  send_str(ECHO, 5);
		  osDelay(1000);
		  send_str(SET_SMS_OP_1, 10);
		  osDelay(1000);
		  send_str(SET_SMS_OP_2, 15);;
		  osDelay(1000);
		  send_str(DEL_SMS, 19);
		  osDelay(1000);
		  send_str(CHK_GPRS, 10);
		  osDelay(1000);
	    send_str(GPRS_CONNECT, 11);
		  osDelay(1000);
      send_str(APN, 19);
		  osDelay(1000);
	    send_str(CHK_CONNECT, 9);
		  osDelay(1000);
	    send_str(CHK_IP, 9);
		  osDelay(1000);
	    send_str(SRV_CONNECT, 42);
		  osDelay(3000); 
}

void Start_Job (void const *argument)
{
	Init_SIM800();
	while(1)
	{
		ToogleGREEN();
		ToogleRED();
		osDelay(100);
	}
}

int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
  SystemCoreClockSetHSI();
	Configure_GPIO_LED();
	Configure_GPIO_USART1();
	Configure_GPIO_USART2();
	Configure_DMA1();
	Configure_USART1();	
	Configure_USART2();	
	tid_Start_Job = osThreadCreate (osThread(Start_Job), NULL);
	osKernelStart ();                         // start thread execution 
}
