#pragma once

#include <stm32f072.h>
#include <cstdlib>

class uartConfig {
    public:
        uartConfig(USART_Type * inst, uint32_t baud);
        void write(uint8_t * data, size_t len);
        void intr();
    private:
        bool enable_abp_clk(USART_Type * inst);
        USART_Type * p_inst;
        volatile uint8_t uartBuffer[256];
        volatile uint8_t bufferBase;
        volatile uint8_t bufferTop;
};