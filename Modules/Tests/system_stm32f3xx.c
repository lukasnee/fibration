#include "stm32f3xx.h"

/* Default value of the Internal/External oscillator in Hz. This value can be provided 
and adapted by the user application. */
#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000) 
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)8000000) 
#endif /* HSI_VALUE */

/* #define VECT_TAB_SRAM */

/* Vector Table base offset field. This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET  0x0 

  /* This variable is updated in three ways:
    1) by calling CMSIS function SystemCoreClockUpdate()
    2) by calling HAL API function HAL_RCC_GetHCLKFreq()
    3) each time HAL_RCC_ClockConfig() is called to configure the system clock 
    frequency Note: If you use this function to configure the system clock there 
    is no need to call the 2 first functions listed above, since SystemCoreClock 
    variable is updated automatically.
  */
uint32_t SystemCoreClock = 8000000;

const uint8_t AHBPrescTable[16] = {
  0, 0, 0, 0, 
  0, 0, 0, 0, 
  1, 2, 3, 4, 
  6, 7, 8, 9
};

const uint8_t APBPrescTable[8] = {
  0, 0, 0, 0, 
  1, 2, 3, 4
};

void SystemInit(void)
{
/* FPU settings --------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  
  /* set CP10 and CP11 Full Access */
#endif

#ifdef VECT_TAB_SRAM
  /* Vector Table Relocation in Internal SRAM */
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; 
#else
  /* Vector Table Relocation in Internal FLASH */
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; 
#endif
}


void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, predivfactor = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case RCC_CFGR_SWS_HSI:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case RCC_CFGR_SWS_HSE:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case RCC_CFGR_SWS_PLL:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & RCC_CFGR_PLLMUL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
      pllmull = ( pllmull >> 18) + 2;

#if defined (STM32F302xE) || defined (STM32F303xE) || defined (STM32F398xx)
        predivfactor = (RCC->CFGR2 & RCC_CFGR2_PREDIV) + 1;
      if (pllsource == RCC_CFGR_PLLSRC_HSE_PREDIV)
      {
        /* HSE oscillator clock selected as PREDIV1 clock entry */
        SystemCoreClock = (HSE_VALUE / predivfactor) * pllmull;
      }
      else
      {
        /* HSI oscillator clock selected as PREDIV1 clock entry */
        SystemCoreClock = (HSI_VALUE / predivfactor) * pllmull;
      }
#else      
      if (pllsource == RCC_CFGR_PLLSRC_HSI_DIV2)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
        predivfactor = (RCC->CFGR2 & RCC_CFGR2_PREDIV) + 1;
        /* HSE oscillator clock selected as PREDIV1 clock entry */
        SystemCoreClock = (HSE_VALUE / predivfactor) * pllmull;
      }
#endif /* STM32F302xE || STM32F303xE || STM32F398xx */
      break;
    default: /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK clock frequency ----------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}
