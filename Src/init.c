#include "init.h"
#include "Sendings.h"

uint8_t stringtosend[] = "DMA\n";
uint8_t stringtoreceive[32] = {0};
uint8_t RX_Buffer[16] = {0};
uint8_t TX_Buffer[904] = {0};


void SystemCoreClockSetHSI(void) {

  RCC->CR |= ((uint32_t)RCC_CR_HSION);                      /* Enable HSI                        */
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                   /* Wait for HSI Ready                */

  RCC->CFGR = RCC_CFGR_SW_HSI;                              /* HSI is system clock               */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);   /* Wait for HSI used as system clock */

  /*  PLL configuration: PLLCLK = (HSI * 6)/3 = 32 MHz */
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC |
                 RCC_CFGR_PLLMUL |
                 RCC_CFGR_PLLDIV  );
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI |
                 RCC_CFGR_PLLMUL4    |
                 RCC_CFGR_PLLDIV2     );

  FLASH->ACR |= FLASH_ACR_PRFTEN;                           /* Enable Prefetch Buffer            */
  FLASH->ACR |= FLASH_ACR_LATENCY;                          /* Flash 1 wait state                */

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;                        /* Enable the PWR APB1 Clock         */
  PWR->CR = PWR_CR_VOS_0;                                   /* Select the Voltage Range 1 (1.8V) */
  while((PWR->CSR & PWR_CSR_VOSF) != 0);                    /* Wait for Voltage Regulator Ready  */

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                          /* HCLK = SYSCLK                     */
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;                         /* PCLK1 = HCLK                      */
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                         /* PCLK2 = HCLK                      */

  RCC->CR &= ~RCC_CR_PLLON;                                 /* Disable PLL */

  RCC->CR |= RCC_CR_PLLON;                                  /* Enable PLL                        */
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();            /* Wait till PLL is ready            */

  RCC->CFGR &= ~RCC_CFGR_SW;                                /* Select PLL as system clock source */
  RCC->CFGR |=  RCC_CFGR_SW_PLL;

  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);   /* Wait till PLL is system clock src */
}

void Configure_GPIO_LED(void)
{
  /* (1) Enable the peripheral clock of GPIOA and GPIOB */
  /* (2) Select output mode (01) on GPIOA pin 5 */
  /* (3) Select output mode (01) on GPIOB pin 4 */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN; /* (1) */  
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE5)) 
               | (GPIO_MODER_MODE5_0); /* (2) */  
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE4)) 
               | (GPIO_MODER_MODE4_0); /* (3) */  
}

void Configure_GPIO_USART2(void)
{
  /* Enable the peripheral clock of GPIOA */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	
  /* GPIO configuration for USART1 signals */
  /* (1) Select AF mode (10) on PA2 and PA3 */
  /* (2) AF4 for USART1 signals */
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE2|GPIO_MODER_MODE3))\
                 | (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1); /* (1) */
  GPIOA->AFR[0] = (GPIOA->AFR[0] &~ (0x00000FF0))\
                  | (4 << (2 * 4)) | (4 << (3 * 4)); /* (2) */
}

void Configure_GPIO_USART1(void)
{
  /* Enable the peripheral clock of GPIOA */
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	
  /* GPIO configuration for USART1 signals */
  /* (1) Select AF mode (10) on PB6 and PB7 */
  /* (2) AF4 for USART1 signals */
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE6|GPIO_MODER_MODE7))\
                 | (GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1); /* (1) */
  GPIOB->AFR[0] = (GPIOB->AFR[0] &~ (0x00000FF0))\
                  | (0 << (6 * 4)) | (0 << (7 * 4)); /* (2) */
}

void Configure_USART1(void)
{
  //Configure_GPIO_USART1();
	/* Enable the peripheral clock USART1 */
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  /* Configure USART1 */
  /* (1) oversampling by 16, 115200 baud */
  /* (2) Enable DMA in reception and transmission */
  /* (3) 8 data bit, 1 start bit, 1 stop bit, no parity, reception and transmission enabled */
  USART1->BRR = 320000 / 1152; /* (1) */
  USART1->CR3 = USART_CR3_DMAT | USART_CR3_DMAR; /* (2) */
  USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; /* (3) */
  
  while((USART1->ISR & USART_ISR_TC) != USART_ISR_TC)/* polling idle frame Transmission */
  { 
    /* add time out here for a robust application */
  }
  USART1->ICR = USART_ICR_TCCF;/* Clear TC flag */
}

void Configure_USART2(void)
{
  //Configure_GPIO_USART1();
	/* Enable the peripheral clock USART1 */
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

  /* Configure USART1 */
  /* (1) oversampling by 16, 115200 baud */
  /* (2) Enable DMA in reception and transmission */
  /* (3) 8 data bit, 1 start bit, 1 stop bit, no parity, reception and transmission enabled */
  USART2->BRR = 320000 / 1152; /* (1) */
  USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR; /* (2) */
  USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; /* (3) */
  
  while((USART2->ISR & USART_ISR_TC) != USART_ISR_TC)/* polling idle frame Transmission */
  { 
    /* add time out here for a robust application */
  }
  USART2->ICR = USART_ICR_TCCF;/* Clear TC flag */
}

void Configure_DMA1(void)
{
  /* Enable the peripheral clock DMA1 */
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
   
  /* DMA1 Channel2 USART1_TX config */
  /* (1)  Map USART1_TX DMA channel */
  /* (2)  Peripheral address */
  /* (3)  Memory address */
  /* (4)  Memory increment */
  /*      Memory to peripheral */
  /*      8-bit transfer */
  /*      Transfer complete IT */
  DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C2S) | (3 << (1 * 4)); /* (1) */
  DMA1_Channel2->CPAR = (uint32_t)&(USART1->TDR); /* (2) */
  DMA1_Channel2->CMAR = (uint32_t)stringtosend; /* (3) */
  DMA1_Channel2->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE; /* (4) */
  
  /* DMA1 Channel2 USART_RX config */
  /* (5)  Map USART1_RX DMA channel */
  /* (6)  Peripheral address */
  /* (7)  Memory address */
  /* (8)  Data size */
  /* (9)  Memory increment */
  /*      Peripheral to memory*/
  /*      8-bit transfer */
  /*      Transfer complete IT */
  DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C3S) | (3 << (2 * 4)); /* (5) */
  DMA1_Channel3->CPAR = (uint32_t)&(USART1->RDR); /* (6) */
  DMA1_Channel3->CMAR = (uint32_t)stringtoreceive; /* (7) */
  DMA1_Channel3->CNDTR = sizeof(stringtoreceive); /* (8) */
  DMA1_Channel3->CCR = DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN; /* (9) */
	
	DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C4S) | (4 << (3 * 4)); /* (5) */
  DMA1_Channel4->CPAR = (uint32_t)&(USART2->TDR); /* (6) */
  DMA1_Channel4->CMAR = (uint32_t)TX_Buffer; /* (7) */
  DMA1_Channel4->CNDTR = 10; /* (8) */
  DMA1_Channel4->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE; /* (9) */

  DMA1_CSELR->CSELR = (DMA1_CSELR->CSELR & ~DMA_CSELR_C5S) | (4 << (4 * 4)); /* (5) */
  DMA1_Channel5->CPAR = (uint32_t)&(USART2->RDR); /* (6) */
  DMA1_Channel5->CMAR = (uint32_t)RX_Buffer; /* (7) */
  DMA1_Channel5->CNDTR = 16; /* (8) */
  DMA1_Channel5->CCR = DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN; /* (9) */
	
  /* Configure IT */
  /* (10) Set priority for DMA1_Channel2_3_IRQn */
  /* (11) Enable DMA1_Channel2_3_IRQn */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0); /* (10) */
	NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 1); /* (10) */
  NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn); /* (11) */
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn); /* (11) */
}

void DMA1_Channel2_3_IRQHandler(void)
{
  if((DMA1->ISR & DMA_ISR_TCIF2) == DMA_ISR_TCIF2)
  {
    DMA1->IFCR = DMA_IFCR_CTCIF2;/* Clear TC flag */
  }
  else if((DMA1->ISR & DMA_ISR_TCIF3) == DMA_ISR_TCIF3)
  {
    DMA1->IFCR = DMA_IFCR_CTCIF3;/* Clear TC flag */
    DMA1_Channel3->CCR &=~ DMA_CCR_EN;
    DMA1_Channel3->CNDTR = sizeof(stringtoreceive);/* Data size */
    DMA1_Channel3->CCR |= DMA_CCR_EN;
  }
  else
  {
    NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);/* Disable DMA1_Channel2_3_IRQn */
  }
}

void DMA1_Channel4_5_6_7_IRQHandler(void)
{
	if((DMA1->ISR & DMA_ISR_TCIF4) == DMA_ISR_TCIF4)
  {
    DMA1->IFCR = DMA_IFCR_CTCIF4;/* Clear TC flag */
  }
	else if((DMA1->ISR & DMA_ISR_TCIF5) == DMA_ISR_TCIF5)
  {
    DMA1->IFCR = DMA_IFCR_CTCIF5;/* Clear TC flag */
    DMA1_Channel5->CCR &=~ DMA_CCR_EN;
    DMA1_Channel5->CNDTR = sizeof(RX_Buffer);/* Data size */
    DMA1_Channel5->CCR |= DMA_CCR_EN;
  }
  else
  {
    NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);/* Disable DMA1_Channel2_3_IRQn */
  }
}
