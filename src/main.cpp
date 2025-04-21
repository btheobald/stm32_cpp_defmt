#include <stm32f072.h>

#include "hal_gpio.hpp"
#include "hal_uart.hpp"

#include <cstdlib>

#include "log.h"

uartConfig debugUART(USART2, 115200);

extern "C" void USART2_Handler() {
    debugUART.intr();
}

static uint32_t systick = 0;

extern "C" void SysTick_Handler() {
    systick++;
}

void SysTick_DelayMs(uint32_t ms) {
    uint32_t start = systick;
    while(systick < (start + ms)) {
        __WFI();
    }
}

static uint32_t log_header = 0x07E0;

void log_commit(uint16_t log_, void * ptr, size_t len) {
    debugUART.write((uint8_t*)&log_header, 2);
    debugUART.write((uint8_t*)&log_, 2);
    if(len > 0) {
        debugUART.write((uint8_t*)ptr, len);
    }
}

int main(void) {
    SysTick_Config(8000000 / 1000);

    gpioConfig bspUART_TX(GPIOA, 2, modeAlt, PA2_AF1_USART2_TX);
    gpioConfig bspUART_RX(GPIOA, 3, modeAlt, PA3_AF1_USART2_RX);
    NVIC_EnableIRQ(USART2_IRQn);

    gpioConfig bspLED(GPIOA, 5, modeOutput, speedLow);
    gpioConfig bspButton(GPIOC, 13, modeInput, pullNone);

    LOG_BUILD_ID();

    while(1) {
        if(!bspButton.read()) {
            LOGV_INF("Button Held", U32, &systick, 4);
            bspLED.toggle();
            SysTick_DelayMs(100);
        } else {
            bspLED.write(0);
            __WFI();
        }
    }
}