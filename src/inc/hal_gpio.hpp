#include <stm32f072.h>

typedef enum {
    modeInput  = 0,
    modeOutput = 1,
    modeAlt    = 2,
    modeAnalog = 3
} GPIOMode;

typedef enum {
    pullNone = 0,
    pullUp   = 1,
    pullDown = 2
} GPIOPull;

typedef enum {
    drivePushPull  = 0,
    driveOpenDrain = 1
} GPIODriver;

typedef enum {
    speedLow = 0,
    speedMed = 1,
    speedMax = 2
} GPIOSpeed;

typedef enum {
    AF0 = 0,
    AF1 = 1,
    AF2 = 2,
    AF3 = 3,
    AF4 = 4,
    AF5 = 5,
    AF6 = 6,
    AF7 = 7
} GPIOAlt;

class gpioConfig {
    public:
        /* Input with Pull Up/Down */
        gpioConfig( GPIO_Type * port,
                    uint8_t bPin, 
                    GPIOMode Mode = modeInput, 
                    GPIOPull Pull = pullNone );

        /* Alternative Function with Driver Config */
        gpioConfig( GPIO_Type * port,
                    uint8_t bPin, 
                    GPIOMode Mode = modeAlt, 
                    GPIOAlt Alt = AF0,
                    GPIOSpeed Speed = speedLow,
                    GPIODriver Driver = drivePushPull );

        /* General Purpose Output with Driver Config */
        gpioConfig( GPIO_Type * port,
                    uint8_t bPin, 
                    GPIOMode Mode = modeOutput, 
                    GPIOSpeed Speed = speedLow,
                    GPIODriver Driver = drivePushPull );
        
        // Write value to output                    
        void write(bool val);

        // Invert present value of output
        void toggle();

        // Write value from input                    
        bool read();
    private:
        bool enable_ahb_clk(GPIO_Type * port);
        void config_pin(GPIO_Type * port, uint8_t bPin, GPIOMode bMode, GPIOPull bPull, GPIOSpeed bSpeed, GPIODriver bDriver);
        void config_alt(GPIO_Type * port, uint8_t bPin, GPIOAlt bAlt);
        volatile GPIO_Type * p_port;
        volatile uint8_t p_bPin;
};
