#include "../include/touch_handler.h"
#include "../include/user_interface.h"

// 初始化触摸设备
int touch_init(touch_device_t *touch, const char *device_path)
{
    if (!touch || !device_path) {
        return ERROR;
    }

    // 打开触摸设备
    touch->fd = open(device_path, O_RDONLY);
    if (touch->fd < 0) {
        perror("打开触摸屏设备失败");
        return ERROR;
    }

    touch->is_pressed = 0;
    touch->running = 1;
    touch->start_pos.x = 0;
    touch->start_pos.y = 0;
    touch->current_pos.x = 0;
    touch->current_pos.y = 0;

    printf("触摸屏设备初始化成功: %s\n", device_path);
    return SUCCESS;
}

// 清理触摸设备
void touch_cleanup(touch_device_t *touch)
{
    if (!touch) {
        return;
    }

    touch->running = 0;

    if (touch->fd >= 0) {
        close(touch->fd);
        touch->fd = -1;
    }

    printf("触摸屏资源已清理\n");
}

// 处理触摸事件
touch_event_t process_touch_event(touch_point_t start, touch_point_t end)
{
    // 计算滑动距离
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    // 判断是滑动还是点击
    if (abs(dx) > SWIPE_THRESHOLD || abs(dy) > SWIPE_THRESHOLD) {
        // 滑动操作
        if (abs(dx) > abs(dy)) {
            // 水平滑动
            if (dx > SWIPE_THRESHOLD) {
                printf("触摸屏向右滑动 - 显示上一张图片\n");
                return TOUCH_EVENT_PREV;
            } else if (dx < -SWIPE_THRESHOLD) {
                printf("触摸屏向左滑动 - 显示下一张图片\n");
                return TOUCH_EVENT_NEXT;
            }
        } else {
            // 垂直滑动 - 切换显示模式
            if (abs(dy) > SWIPE_THRESHOLD) {
                printf("触摸屏上下滑动 - 切换显示模式\n");
                return TOUCH_EVENT_TOGGLE_MODE;
            }
        }
    } else {
        // 点击操作
        if (start.x < LEFT_CLICK_AREA) {
            printf("点击屏幕左侧 - 显示上一张图片\n");
            return TOUCH_EVENT_PREV;
        } else if (start.x > RIGHT_CLICK_AREA) {
            printf("点击屏幕右侧 - 显示下一张图片\n");
            return TOUCH_EVENT_NEXT;
        } else {
            printf("点击屏幕中间区域\n");
            // 中间区域可以用于其他功能
        }
    }

    return TOUCH_EVENT_NONE;
}

// 触摸屏处理线程
void *touch_handler_thread(void *arg)
{
    touch_device_t *touch = (touch_device_t *)arg;
    if (!touch) {
        return NULL;
    }

    struct input_event event;
    printf("触摸屏处理线程已启动\n");

    while (touch->running) {
        int ret = read(touch->fd, &event, sizeof(event));
        if (ret < 0) {
            if (touch->running) {
                perror("读取触摸事件失败");
            }
            break;
        }

        // 处理输入事件数据
        if (event.type == EV_ABS && event.code == ABS_X) {
            touch->current_pos.x = event.value * 800 / 1024;
        } else if (event.type == EV_ABS && event.code == ABS_Y) {
            touch->current_pos.y = event.value * 480 / 600;
        } else if (event.type == EV_KEY && event.code == BTN_TOUCH) {
            if (event.value == 1) {
                printf("触摸屏被触摸在坐标: (%d, %d)\n", touch->current_pos.x, touch->current_pos.y);
                touch->start_pos = touch->current_pos;
                touch->is_pressed = 1;
            } else if (event.value == 0) {
                printf("触摸屏释放\n");
                printf("起始坐标: (%d, %d), 结束坐标: (%d, %d)\n",
                       touch->start_pos.x, touch->start_pos.y,
                       touch->current_pos.x, touch->current_pos.y);

                // 处理触摸事件
                touch_event_t touch_event = process_touch_event(touch->start_pos, touch->current_pos);
                
                switch (touch_event) {
                    case TOUCH_EVENT_PREV:
                        ui_show_prev_image();
                        break;
                    case TOUCH_EVENT_NEXT:
                        ui_show_next_image();
                        break;
                    case TOUCH_EVENT_TOGGLE_MODE:
                        ui_toggle_refresh_mode();
                        break;
                    case TOUCH_EVENT_NONE:
                    default:
                        break;
                }

                touch->is_pressed = 0;
            }
        }
    }

    return NULL;
}
