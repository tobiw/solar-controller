#ifdef ARCH_AVR

/* I/O definitions */
#define PIN_SENSOR1 PC0 // ADC0
#define PIN_SENSOR2 PC1 // ADC1
#define PIN_SENSOR3 PC2 // ADC2
#define PIN_SENSOR4 PC3 // ADC3
#define PIN_LED1 PD2
#define PIN_LED2 PD3
#define PIN_SWITCH1 PB0
#define PIN_SWITCH2 PB1

#else

/* I/O definitions */
#define PIN_SENSOR1 0
#define PIN_SENSOR2 1
#define PIN_SENSOR3 2
#define PIN_SENSOR4 3
#define PIN_LED1 4
#define PIN_LED2 5
#define PIN_SWITCH1 6
#define PIN_SWITCH2 7

#endif

/* Features */
#define ENABLE_SPI_LCD 1        // operational output on LCD display attached via SPI
#define ENABLE_LCD_DEBUG 1      // additional debug information on display
#define ENABLE_SERIAL 1         // serial input and output on RXD/TXD pins
#define ENABLE_SERIAL_DEBUG 1   // additional debug information on serial
#define ENABLE_I2C_ETH 1        // Wiznet ethernet chip attached via I2C

