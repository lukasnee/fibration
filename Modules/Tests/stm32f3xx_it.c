#include "stm32f3xx_it.h"

// This function is executed in case of error occurrence.
void Error_Handler(void)
{

}


// This function handles Non maskable interrupt.
void NMI_Handler(void)
{
}


// This function handles Hard fault interrupt.
void HardFault_Handler(void)
{
  while (1)
  {
  }
}


// This function handles Memory management fault.
void MemManage_Handler(void)
{
  while (1)
  {
  }
}


// This function handles Pre-fetch fault, memory access fault.
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

// This function handles Undefined instruction or illegal state.
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}


// This function handles System service call via SWI instruction.
void SVC_Handler(void)
{

}


// This function handles Debug monitor.
void DebugMon_Handler(void)
{
 
}


// This function handles Pendable request for system service.
void PendSV_Handler(void)
{

}


// This function handles System tick timer.
void SysTick_Handler(void)
{
  HAL_IncTick();
}
