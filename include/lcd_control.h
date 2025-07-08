#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

#include "common.h"

// LCD控制结构
typedef struct {
    int fd;
    int *map_addr;
    int width;
    int height;
    int bpp;
    long screen_size;
} lcd_device_t;

// 函数声明
int lcd_init(lcd_device_t *lcd, int width, int height);
void lcd_cleanup(lcd_device_t *lcd);
void lcd_clear_screen(lcd_device_t *lcd, int color);

#endif // LCD_CONTROL_H
