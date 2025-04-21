// GNU Linker Script for STM32F072RB
// Based on CMSIS v5 Startup File
// Byron Theobald - 18 April 2025

#include "stm32f072.h"

#define WEAK_DEFAULT __attribute__ ((weak, alias("Default_Handler")))

extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

extern "C" __NO_RETURN void Reset_Handler  (void);
extern "C" void Default_Handler(void);

// Core Interrupts
extern "C" void HardFault_Handler     (void) WEAK_DEFAULT;
extern "C" void NMI_Handler           (void) WEAK_DEFAULT;
extern "C" void BusFault_Handler      (void) WEAK_DEFAULT;
extern "C" void UsageFault_Handler    (void) WEAK_DEFAULT;
extern "C" void SecureFault_Handler   (void) WEAK_DEFAULT;
extern "C" void SVC_Handler           (void) WEAK_DEFAULT;
extern "C" void DebugMon_Handler      (void) WEAK_DEFAULT;
extern "C" void PendSV_Handler        (void) WEAK_DEFAULT;
extern "C" void SysTick_Handler       (void) WEAK_DEFAULT;
// Device Interrupt
extern "C" void WWDG_Handler          (void) WEAK_DEFAULT;
extern "C" void PVD_Handler           (void) WEAK_DEFAULT;
extern "C" void RTC_Handler           (void) WEAK_DEFAULT;
extern "C" void FLASH_Handler         (void) WEAK_DEFAULT;
extern "C" void RCC_CRS_Handler       (void) WEAK_DEFAULT;
extern "C" void EXTI0_1_Handler       (void) WEAK_DEFAULT;
extern "C" void EXTI2_3_Handler       (void) WEAK_DEFAULT;
extern "C" void EXTI4_15_Handler      (void) WEAK_DEFAULT;
extern "C" void TSC_Handler           (void) WEAK_DEFAULT;
extern "C" void DMA_CH1_Handler       (void) WEAK_DEFAULT;
extern "C" void DMA_CH2_3_Handler     (void) WEAK_DEFAULT;
extern "C" void DMA_CH4_5_6_7_Handler (void) WEAK_DEFAULT;
extern "C" void ADC_COMP_Handler      (void) WEAK_DEFAULT;
extern "C" void TIM1_GENERAL_Handler  (void) WEAK_DEFAULT;
extern "C" void TIM1_CC_Handler       (void) WEAK_DEFAULT;
extern "C" void TIM2_Handler          (void) WEAK_DEFAULT;
extern "C" void TIM3_Handler          (void) WEAK_DEFAULT;
extern "C" void TIM6_DAC_Handler      (void) WEAK_DEFAULT;
extern "C" void TIM7_Handler          (void) WEAK_DEFAULT;
extern "C" void TIM14_Handler         (void) WEAK_DEFAULT;
extern "C" void TIM15_Handler         (void) WEAK_DEFAULT;
extern "C" void TIM16_Handler         (void) WEAK_DEFAULT;
extern "C" void TIM17_Handler         (void) WEAK_DEFAULT;
extern "C" void I2C1_Handler          (void) WEAK_DEFAULT;
extern "C" void I2C2_Handler          (void) WEAK_DEFAULT;
extern "C" void SPI1_Handler          (void) WEAK_DEFAULT;
extern "C" void SPI2_Handler          (void) WEAK_DEFAULT;
extern "C" void USART1_Handler        (void) WEAK_DEFAULT;
extern "C" void USART2_Handler        (void) WEAK_DEFAULT;
extern "C" void USART3_4_Handler      (void) WEAK_DEFAULT;
extern "C" void CEC_CAN_Handler       (void) WEAK_DEFAULT;
extern "C" void USB_Handler           (void) WEAK_DEFAULT;

typedef void(*VECTOR_T)(void);

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
const VECTOR_T __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_T)(&__INITIAL_SP),
  // Core Interrupts
  Reset_Handler,                     
  NMI_Handler,                       
  HardFault_Handler,                 
  0,                 
  BusFault_Handler,                  
  UsageFault_Handler,                
  SecureFault_Handler,               
  0,                                 
  0,                                 
  0,                                 
  SVC_Handler,                       
  DebugMon_Handler,                  
  0,                                 
  PendSV_Handler,                    
  SysTick_Handler,
  // Device Interrupt
  WWDG_Handler,                       
  PVD_Handler,                        
  RTC_Handler,                        
  FLASH_Handler,                      
  RCC_CRS_Handler,                    
  EXTI0_1_Handler,                    
  EXTI2_3_Handler,                    
  EXTI4_15_Handler,                   
  TSC_Handler,                        
  DMA_CH1_Handler,                    
  DMA_CH2_3_Handler,                  
  DMA_CH4_5_6_7_Handler,              
  ADC_COMP_Handler,                   
  TIM1_GENERAL_Handler,                   
  TIM1_CC_Handler,                    
  TIM2_Handler,                       
  TIM3_Handler,                       
  TIM6_DAC_Handler,                   
  TIM7_Handler,                       
  TIM14_Handler,                      
  TIM15_Handler,                      
  TIM16_Handler,                      
  TIM17_Handler,                      
  I2C1_Handler,                       
  I2C2_Handler,                       
  SPI1_Handler,                       
  SPI2_Handler,                       
  USART1_Handler,                     
  USART2_Handler,                     
  USART3_4_Handler,                   
  CEC_CAN_Handler,                    
  USB_Handler                        
};
#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

extern "C" __NO_RETURN void Reset_Handler(void) {
  __set_PSP((uint32_t)(&__INITIAL_SP));
  __PROGRAM_START();               /* Enter PreMain (C library entry point) */
}

#if defined(__ARMCC_VERSION)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

extern "C" void Default_Handler(void) {
  while(1) {
    __WFI();
  }
}

extern "C" void _exit(int status) {
  (void) status;
  while(1) {
    __WFI();
  }
}

#if defined(__ARMCC_VERSION)
  #pragma clang diagnostic pop
#endif
