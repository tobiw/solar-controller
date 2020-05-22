/*
 * Hardware-independent implementation of 16x2 LCD via I2C.
 */
#include "hw/io.h"
#include "core/lcd.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define EN 0b00000100  // Enable bit
#define RW 0b00000010  // Read/Write bit
#define RS 0b00000001  // Register select bit

static uint8_t lcd_displaymode = 0;
static uint8_t lcd_displaycontrol = 0;
static uint8_t lcd_numlines = 0;
static uint8_t lcd_backlight_value = 0;
static uint8_t lcd_addr = 0;

static void sc_lcd_expander_write(uint8_t data)
{
    hw_i2c_write(lcd_addr, (int)data | lcd_backlight_value);
}

static void sc_lcd_pulse_enable(uint8_t data)
{
    sc_lcd_expander_write(data | EN);
    hw_ussleep(1);
    sc_lcd_expander_write(data & ~EN);
    hw_ussleep(50);
}

static void sc_lcd_write4bits(uint8_t value)
{
    sc_lcd_expander_write(value);
    sc_lcd_pulse_enable(value);
}

static void sc_lcd_send(uint8_t value, uint8_t mode)
{
	const uint8_t hi = value & 0xf0;
	const uint8_t lo = (value << 4) & 0xf0;
    sc_lcd_write4bits(hi | mode);
    sc_lcd_write4bits(lo | mode);
}

inline static void sc_lcd_command(uint8_t value)
{
    sc_lcd_send(value, 0);
}

void sc_lcd_command_delay(uint8_t c, uint8_t delay_ms)
{
    sc_lcd_command(c);
    hw_mssleep(delay_ms);
}

void sc_lcd_init(uint8_t addr, uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    hw_i2c_init();

    lcd_addr = addr;

    lcd_displaymode = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

    if (lines > 1) {
        lcd_displaymode |= LCD_2LINE;
    }
    lcd_numlines = lines;

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way bef0r 4.5V so we'll wait 50
    hw_mssleep(50);

    // Now we pull both RS and R/W low to begin commands
    sc_lcd_expander_write(lcd_backlight_value);
    hw_mssleep(1000);

    // put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    sc_lcd_write4bits(0x03 << 4);
    hw_mssleep(5);

    // second try
    sc_lcd_write4bits(0x03 << 4);
    hw_mssleep(5);

    // third go!
    sc_lcd_write4bits(0x03 << 4);
    hw_mssleep(1);

    // finally, set to 4-bit interface
    sc_lcd_write4bits(0x02 << 4);


    // set # lines, font size, etc.
    sc_lcd_command(LCD_FUNCTIONSET | lcd_displaymode);

    // turn the display on with no cursor or blinking default
    lcd_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    sc_lcd_set_display(1);

    // clear it off
    sc_lcd_clear();

    // Initialize to default text direction (for roman languages)
    lcd_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    sc_lcd_command(LCD_ENTRYMODESET | lcd_displaymode);

    sc_lcd_set_home();
}

void sc_lcd_clear()
{
    sc_lcd_command_delay(LCD_CLEARDISPLAY, 2);
}

void sc_lcd_set_home()
{
    sc_lcd_command_delay(LCD_RETURNHOME, 2);
}

void sc_lcd_set_cursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    sc_lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void sc_lcd_set_display(uint8_t enable)
{
    if (enable == 0)
        lcd_displaycontrol &= ~LCD_DISPLAYON;
    else
        lcd_displaycontrol |= LCD_DISPLAYON;
    sc_lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}

void sc_lcd_set_cursor_visibility(uint8_t enable)
{
    if (enable == 0)
        lcd_displaycontrol &= ~LCD_CURSORON;
    else
        lcd_displaycontrol |= LCD_CURSORON;
    sc_lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}

void sc_lcd_set_blink(uint8_t enable)
{
    if (enable == 0)
        lcd_displaycontrol &= ~LCD_BLINKON;
    else
        lcd_displaycontrol |= LCD_BLINKON;
    sc_lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}

void sc_lcd_scroll_left()
{
    sc_lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void sc_lcd_scroll_right()
{
    sc_lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void sc_lcd_autoscroll(uint8_t enable)
{
    if (enable)
        lcd_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    else
        lcd_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	sc_lcd_command(LCD_ENTRYMODESET | lcd_displaymode);
}

void sc_lcd_set_backlight(uint8_t enable)
{
    lcd_backlight_value = enable == 0 ? LCD_NOBACKLIGHT : LCD_BACKLIGHT;
    sc_lcd_expander_write(0);
}
