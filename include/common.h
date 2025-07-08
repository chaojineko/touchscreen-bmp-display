#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE  // 启用GNU扩展，包括strcasecmp

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>  // 包含strcasecmp声明
#include <dirent.h>
#include <termios.h>
#include <ctype.h>
#include <time.h>
#include <linux/input.h>
#include <math.h>
#include <pthread.h>

// 颜色定义
#define WHITE 0xffffff

// 屏幕配置
#define WIDTH 800
#define HEIGHT 480

// 文件配置
#define MAX_BMP_FILES 100
#define MAX_FILENAME_LEN 256

// 触摸区域定义
#define LEFT_CLICK_AREA (WIDTH / 3)
#define RIGHT_CLICK_AREA (WIDTH * 2 / 3)

// 滑动检测阈值
#define SWIPE_THRESHOLD 100

// 返回值定义
#define SUCCESS 0
#define ERROR -1

// 应用程序状态结构
typedef struct {
    int refresh_delay;
    int use_slow_refresh;
    int current_index;
    int bmp_count;
    char bmp_files[MAX_BMP_FILES][MAX_FILENAME_LEN];
    int *map_addr;
    int screen_w;
    int screen_h;
    int running;
    pthread_mutex_t refresh_mutex;
} app_state_t;

// 全局应用状态
extern app_state_t g_app_state;

// 工具函数
void delay_ms(int milliseconds);
int getch(void);

#endif // COMMON_H
