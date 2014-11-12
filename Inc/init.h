#include "stm32l0xx.h"


extern void SystemCoreClockSetHSI(void);
extern void Configure_GPIO_LED(void);
extern void Configure_USART1(void);
extern void Configure_USART2(void);
extern void Configure_GPIO_USART1(void);
extern void Configure_GPIO_USART2(void);
extern void Configure_DMA1(void);

extern uint8_t stringtosend[];
extern uint8_t stringtoreceive[];
extern uint8_t RX_Buffer[16];
