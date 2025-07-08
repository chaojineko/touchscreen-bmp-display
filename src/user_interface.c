#include "../include/user_interface.h"
#include "../include/bmp_handler.h"
#include "../include/config.h"

// 打印帮助信息
void ui_print_help(void)
{
    printf("\n");
    printf("======== 控制说明 ========\n");
    printf("键盘控制:\n");
    printf("  n/N: 下一张图片\n");
    printf("  p/P: 上一张图片\n");
    printf("  +: 增加刷新延迟（显示变慢）\n");
    printf("  -: 减少刷新延迟（显示变快）\n");
    printf("  0-9: 直接设置刷新速度（0最快，9最慢）\n");
    printf("  s/S: 切换显示模式（%s）\n", g_app_state.use_slow_refresh ? "当前:逐行显示" : "当前:快速显示");
    printf("  r/R: 重新加载当前图片\n");
    printf("  h/H: 显示帮助信息\n");
    printf("  i/I: 显示当前图片信息\n");
    printf("  d/D: 切换调试模式\n");
    printf("  q/Q/ESC: 退出程序\n");
    printf("触摸屏控制:\n");
    printf("  点击左侧1/3区域: 上一张图片\n");
    printf("  点击右侧1/3区域: 下一张图片\n");
    printf("  左滑: 下一张图片\n");
    printf("  右滑: 上一张图片\n");
    printf("  上下滑: 切换显示模式\n");
    printf("当前刷新延迟: %d 毫秒/行\n", g_app_state.refresh_delay);
    printf("========================\n");
}

// 打印状态信息
void ui_print_status(const app_state_t *state)
{
    printf("当前图片: (%d/%d) %s\n", 
           state->current_index + 1, state->bmp_count, 
           state->bmp_files[state->current_index]);
    printf("显示模式: %s\n", state->use_slow_refresh ? "逐行显示" : "快速显示");
    printf("刷新延迟: %d 毫秒/行\n", state->refresh_delay);
}

// 显示下一张图片
void ui_show_next_image(void)
{
    pthread_mutex_lock(&g_app_state.refresh_mutex);
    g_app_state.current_index = (g_app_state.current_index + 1) % g_app_state.bmp_count;
    printf("显示下一张图片 (%d/%d): %s\n", 
           g_app_state.current_index + 1, g_app_state.bmp_count, 
           g_app_state.bmp_files[g_app_state.current_index]);
    
    if (display_bmp(g_app_state.bmp_files[g_app_state.current_index], 
                    g_app_state.map_addr, g_app_state.screen_w, g_app_state.screen_h,
                    g_app_state.use_slow_refresh, g_app_state.refresh_delay) < 0) {
        printf("显示图片失败，尝试下一张\n");
    }
    pthread_mutex_unlock(&g_app_state.refresh_mutex);
}

// 显示上一张图片
void ui_show_prev_image(void)
{
    pthread_mutex_lock(&g_app_state.refresh_mutex);
    g_app_state.current_index = (g_app_state.current_index - 1 + g_app_state.bmp_count) % g_app_state.bmp_count;
    printf("显示上一张图片 (%d/%d): %s\n", 
           g_app_state.current_index + 1, g_app_state.bmp_count, 
           g_app_state.bmp_files[g_app_state.current_index]);
    
    if (display_bmp(g_app_state.bmp_files[g_app_state.current_index], 
                    g_app_state.map_addr, g_app_state.screen_w, g_app_state.screen_h,
                    g_app_state.use_slow_refresh, g_app_state.refresh_delay) < 0) {
        printf("显示图片失败，尝试上一张\n");
    }
    pthread_mutex_unlock(&g_app_state.refresh_mutex);
}

// 切换显示模式
void ui_toggle_refresh_mode(void)
{
    pthread_mutex_lock(&g_app_state.refresh_mutex);
    g_app_state.use_slow_refresh = !g_app_state.use_slow_refresh;
    printf("切换显示模式为: %s\n", g_app_state.use_slow_refresh ? "逐行显示" : "快速显示");
    printf("重新加载图片以应用新设置...\n");
    
    if (display_bmp(g_app_state.bmp_files[g_app_state.current_index], 
                    g_app_state.map_addr, g_app_state.screen_w, g_app_state.screen_h,
                    g_app_state.use_slow_refresh, g_app_state.refresh_delay) < 0) {
        printf("重新加载图片失败\n");
    }
    pthread_mutex_unlock(&g_app_state.refresh_mutex);
}

// 重新加载当前图片
void ui_reload_current_image(void)
{
    printf("重新加载图片: %s\n", g_app_state.bmp_files[g_app_state.current_index]);
    pthread_mutex_lock(&g_app_state.refresh_mutex);
    
    if (display_bmp(g_app_state.bmp_files[g_app_state.current_index], 
                    g_app_state.map_addr, g_app_state.screen_w, g_app_state.screen_h,
                    g_app_state.use_slow_refresh, g_app_state.refresh_delay) < 0) {
        printf("重新加载图片失败\n");
    }
    pthread_mutex_unlock(&g_app_state.refresh_mutex);
}

// 调整刷新延迟
void ui_adjust_refresh_delay(int new_delay)
{
    if (new_delay < 0) new_delay = 0;
    if (new_delay > 200) new_delay = 200;
    
    g_app_state.refresh_delay = new_delay;
    printf("设置行刷新延迟时间为: %d 毫秒/行\n", g_app_state.refresh_delay);
}

// 显示当前图片信息
void show_image_info(void) 
{
    if (g_app_state.bmp_count <= 0 || g_app_state.current_index < 0) {
        printf("没有可显示的图片\n");
        return;
    }
    
    printf("\n======== 当前图片信息 ========\n");
    printf("文件名: %s\n", g_app_state.bmp_files[g_app_state.current_index]);
    printf("索引: %d/%d\n", g_app_state.current_index + 1, g_app_state.bmp_count);
    
    // 获取文件大小
    struct stat st;
    if (stat(g_app_state.bmp_files[g_app_state.current_index], &st) == 0) {
        printf("文件大小: %.2f KB\n", st.st_size / 1024.0);
    }
    
    printf("显示模式: %s\n", g_app_state.use_slow_refresh ? "逐行显示" : "快速显示");
    printf("刷新延迟: %d 毫秒/行\n", g_app_state.refresh_delay);
    printf("==============================\n\n");
}

// 处理键盘输入
int ui_handle_keyboard_input(void)
{
    int ch = getch();

    switch (ch) {
        case 'n':
        case 'N':
        case '\n': // 回车键
            ui_show_next_image();
            break;

        case 'p':
        case 'P':
            ui_show_prev_image();
            break;

        case 'q':
        case 'Q':
        case 27: // ESC键
            printf("退出程序\n");
            g_app_state.running = 0;
            return 0;

        // 调节显示速度
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            ui_adjust_refresh_delay((ch - '0') * 10);
            break;

        case '+': // 增加延迟（变慢）
            ui_adjust_refresh_delay(g_app_state.refresh_delay + 5);
            break;

        case '-': // 减少延迟（变快）
            ui_adjust_refresh_delay(g_app_state.refresh_delay - 5);
            break;

        case 'r':
        case 'R': // 重新加载当前图片
            ui_reload_current_image();
            break;

        case 's':
        case 'S': // 切换显示模式
            ui_toggle_refresh_mode();
            break;
            
        case 'h':
        case 'H': // 显示帮助
            ui_print_help();
            break;
            
        case 'i':
        case 'I': // 显示图片信息
            show_image_info();
            break;
            
        case 'd':
        case 'D': // 切换调试模式
            g_config.debug_mode = !g_config.debug_mode;
            printf("调试模式: %s\n", g_config.debug_mode ? "开启" : "关闭");
            break;

        default:
            // 忽略其他键
            break;
    }

    return 1; // 继续运行
}
