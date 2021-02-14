#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

static void Error_Handler()
{
    // todo think again
    UsageFault_Handler();
}

// Initializes the Global MSP.
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}
