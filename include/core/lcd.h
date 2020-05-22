#include <stdint.h>

void sc_lcd_init(uint8_t addr, uint8_t cols, uint8_t lines, uint8_t dotsize);
void sc_lcd_clear();
void sc_lcd_set_home();
void sc_lcd_set_cursor(uint8_t col, uint8_t row);
void sc_lcd_set_display(uint8_t enable);
void sc_lcd_set_cursor_visibility(uint8_t enable);
void sc_lcd_set_blink(uint8_t enable);
void sc_lcd_scroll_left();
void sc_lcd_scroll_right();
void sc_lcd_autoscroll(uint8_t enable);
void sc_lcd_set_backlight(uint8_t enable);
