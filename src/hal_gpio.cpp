#include "hal_gpio.hpp"

#define GPIO_ALT_FUNC_HIGH 16

#define GPIO_REG_MSK(x) ~(0x03 << (x * 2))
#define GPIO_REG_SET(x, s)  (s << (x * 2))

#define GPIO_ALT_MSK(x) ~(0x0F << (x * 4))
#define GPIO_ALT_SET(x, s)  (s << (x * 4))

// Input with Pull Up/Down
gpioConfig::gpioConfig( GPIO_Type * port, uint8_t bPin, GPIOMode bMode, 
                        GPIOPull bPull) 
                        : p_port(port), p_bPin(bPin) {
    enable_ahb_clk(port);
    config_pin(port, bPin, bMode, bPull, speedLow, drivePushPull);
}

// Alternative Function with Driver Config
gpioConfig::gpioConfig( GPIO_Type * port, uint8_t bPin, GPIOMode bMode, 
                        GPIOAlt bAlt, GPIOSpeed bSpeed, GPIODriver bDriver )
                        : p_port(port), p_bPin(bPin) {
    enable_ahb_clk(port);
    config_pin(port, bPin, bMode, pullNone, bSpeed, bDriver);
    config_alt(port, bPin, bAlt);
}

// General Purpose Output with Driver Config
gpioConfig::gpioConfig( GPIO_Type * port, uint8_t bPin, GPIOMode bMode, 
                        GPIOSpeed bSpeed, GPIODriver bDriver )
                        : p_port(port), p_bPin(bPin) {
    enable_ahb_clk(port);
    config_pin(port, bPin, bMode, pullNone, bSpeed, bDriver);
}

void gpioConfig::write(bool val) {
    if(val) {
        p_port->BSRR.reg = (1 << p_bPin);
    } else {
        p_port->BRR.reg  = (1 << p_bPin);
    }
}

bool gpioConfig::read() {
    bool ret = p_port->IDR.reg & (1 << p_bPin);
    return ret;
}

void gpioConfig::toggle() {
    p_port->ODR.reg ^= (1 << p_bPin);
}

bool gpioConfig::enable_ahb_clk(GPIO_Type * port) {
    switch((const int)port) {
        case GPIOA_BASE:
            RCC->AHBENR.bit.IOPAEN = 1;
            return false;
        case GPIOB_BASE:
            RCC->AHBENR.bit.IOPBEN = 1;
            return false;
        case GPIOC_BASE:
            RCC->AHBENR.bit.IOPCEN = 1;
            return false;
        case GPIOD_BASE:
            RCC->AHBENR.bit.IOPDEN = 1;
            return false;
        case GPIOF_BASE:
            RCC->AHBENR.bit.IOPFEN = 1;
            return false;
        default:
            return true;
    }
}

void gpioConfig::config_pin(GPIO_Type * port,
                            uint8_t bPin, 
                            GPIOMode bMode, 
                            GPIOPull bPull, 
                            GPIOSpeed bSpeed, 
                            GPIODriver bDriver) {
    port->MODER.reg = GPIO_REG_SET(bPin, bMode) | (port->MODER.reg & GPIO_REG_MSK(bPin));
    port->PUPDR.reg = GPIO_REG_SET(bPin, bPull) | (port->PUPDR.reg & GPIO_REG_MSK(bPin));
    port->OSPEEDR.reg = GPIO_REG_SET(bPin, bSpeed) | (port->OSPEEDR.reg & GPIO_REG_MSK(bPin));
    port->OTYPER.reg = GPIO_REG_SET(bPin, bDriver) | (port->OTYPER.reg & GPIO_REG_MSK(bPin));
}

void gpioConfig::config_alt(GPIO_Type * port, uint8_t bPin, GPIOAlt bAlt) {
    if(bPin > GPIO_ALT_FUNC_HIGH) {
        port->AFRH.reg = GPIO_ALT_SET(bPin - GPIO_ALT_FUNC_HIGH, bAlt) | (port->AFRH.reg & GPIO_ALT_MSK(bPin - GPIO_ALT_FUNC_HIGH));
    } else {
        port->AFRL.reg = GPIO_ALT_SET(bPin, bAlt) | (port->AFRL.reg & GPIO_ALT_MSK(bPin));
    }
}