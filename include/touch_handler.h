#ifndef TOUCH_HANDLER_H
#define TOUCH_HANDLER_H

#include "common.h"

// 触摸事件类型
typedef enum {
    TOUCH_EVENT_NONE,
    TOUCH_EVENT_PREV,
    TOUCH_EVENT_NEXT,
    TOUCH_EVENT_TOGGLE_MODE
} touch_event_t;

// 触摸坐标结构
typedef struct {
    int x;
    int y;
} touch_point_t;

// 触摸状态结构
typedef struct {
    int fd;
    touch_point_t start_pos;
    touch_point_t current_pos;
    int is_pressed;
    pthread_t thread_id;
    int running;
} touch_device_t;

// 函数声明
int touch_init(touch_device_t *touch, const char *device_path);
void touch_cleanup(touch_device_t *touch);
void *touch_handler_thread(void *arg);
touch_event_t process_touch_event(touch_point_t start, touch_point_t end);

#endif // TOUCH_HANDLER_H
