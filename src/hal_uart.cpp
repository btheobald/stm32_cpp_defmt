#include "hal_uart.hpp"

uartConfig::uartConfig(USART_Type * inst, uint32_t baud) {
    p_inst = inst;
    bufferBase = 0;
    bufferTop = 0;

    enable_abp_clk(inst);

    p_inst->BRR.reg = (8000000 / baud);
    p_inst->CR1.bit.TE = 1;
    p_inst->CR1.bit.UE = 1;
    p_inst->CR1.bit.TCIE = 1;
    p_inst->TDR.reg = 0;
}

void uartConfig::write(uint8_t * data, size_t len) {
    for(size_t i = 0; i < len; i++) {
        uartBuffer[bufferTop] = data[i];
        bufferTop += 1;
    }

    if(p_inst->ISR.bit.TXE && p_inst->CR1.bit.UE) {
        p_inst->TDR.reg = uartBuffer[bufferBase];
        bufferBase += 1;
    }
}

void uartConfig::intr() {
    if(bufferBase != bufferTop) {
        p_inst->TDR.reg = uartBuffer[bufferBase];
        bufferBase += 1;
    } else {
        p_inst->ICR.bit.TCCF = 1;
    }
}

bool uartConfig::enable_abp_clk(USART_Type * inst) {
    // Enable peripheral clock
    switch((uint32_t)inst) {
        case USART1_BASE:
            RCC->APB2ENR.bit.USART1EN = 1;
            return false;
        case USART2_BASE:
            RCC->APB1ENR.bit.USART2EN = 1;
            return false;
        case USART3_BASE:
            RCC->APB1ENR.bit.USART3EN = 1;
            return false;
        case USART4_BASE:
            RCC->APB1ENR.bit.USART4EN = 1;
            return false;
        default:
            return true;
    }
}