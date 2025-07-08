#include "../include/common.h"

// 全局应用状态定义
app_state_t g_app_state = {
    .refresh_delay = 5,
    .use_slow_refresh = 1,
    .current_index = 0,
    .bmp_count = 0,
    .map_addr = NULL,
    .screen_w = WIDTH,
    .screen_h = HEIGHT,
    .running = 1,
    .refresh_mutex = PTHREAD_MUTEX_INITIALIZER};

// 毫秒级延时函数
void delay_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

// 获取字符函数（无缓冲输入）
int getch(void)
{
    struct termios old_settings, new_settings;
    int ch;

    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

    return ch;
}
