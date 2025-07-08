#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

// 配置结构
typedef struct {
    int screen_width;
    int screen_height;
    int default_refresh_delay;
    int use_slow_refresh;
    int max_bmp_files;
    int scan_subdirectories;
    char touch_device[256];
    int swipe_threshold;
    int enable_touch;
    char lcd_device[256];
    int bits_per_pixel;
    int debug_mode;
    int verbose_output;
} config_t;

// 默认配置
extern config_t g_config;

// 函数声明
int load_config(const char *config_file);
void print_config(void);
void save_default_config(const char *config_file);

#endif // CONFIG_H
