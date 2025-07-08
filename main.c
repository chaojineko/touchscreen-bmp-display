#include "include/common.h"
#include "include/bmp_handler.h"
#include "include/lcd_control.h"
#include "include/touch_handler.h"
#include "include/file_scanner.h"
#include "include/user_interface.h"
#include "include/config.h"

// 显示程序版本信息
void show_version()
{
    printf("BMP图片查看器 - 模块化版本 v1.0.0\n");
    printf("作者: Copilot 2025-07-08\n");
    printf("=================================\n");
}

// 显示帮助信息
void show_help()
{
    printf("使用方法: bmp_viewer [选项] [目录]\n");
    printf("选项:\n");
    printf("  -h, --help            显示帮助信息\n");
    printf("  -v, --version         显示版本信息\n");
    printf("  -c, --config 文件路径   指定配置文件路径\n");
    printf("  -t, --touch 设备路径    指定触摸设备路径\n");
    printf("  -f, --fast            使用快速显示模式\n");
    printf("  -d, --delay 毫秒        设置显示延迟时间\n");
    printf("  -n, --no-touch        禁用触摸功能\n");
    printf("示例:\n");
    printf("  bmp_viewer                  扫描当前目录\n");
    printf("  bmp_viewer /path/to/images  扫描指定目录\n");
    printf("  bmp_viewer -c config.conf   使用指定配置文件\n");
    printf("=================================\n");
}

// 主函数
int main(int argc, char *argv[])
{
    // 解析命令行参数
    const char *scan_directory = ".";        // 默认扫描当前目录
    const char *config_file = "config.conf"; // 默认配置文件
    int i;

    // 显示欢迎信息
    show_version();

    // 解析命令行参数
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            show_help();
            return 0;
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            return 0;
        }
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0)
        {
            if (i + 1 < argc)
            {
                config_file = argv[++i];
            }
            else
            {
                fprintf(stderr, "错误: --config 选项需要一个参数\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--touch") == 0)
        {
            if (i + 1 < argc)
            {
                strncpy(g_config.touch_device, argv[++i], sizeof(g_config.touch_device) - 1);
                g_config.touch_device[sizeof(g_config.touch_device) - 1] = '\0';
            }
            else
            {
                fprintf(stderr, "错误: --touch 选项需要一个参数\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fast") == 0)
        {
            g_config.use_slow_refresh = 0;
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--delay") == 0)
        {
            if (i + 1 < argc)
            {
                g_config.default_refresh_delay = atoi(argv[++i]);
            }
            else
            {
                fprintf(stderr, "错误: --delay 选项需要一个参数\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--no-touch") == 0)
        {
            g_config.enable_touch = 0;
        }
        else if (argv[i][0] != '-')
        {
            // 非选项参数被视为目录
            scan_directory = argv[i];
        }
        else
        {
            fprintf(stderr, "未知选项: %s\n使用 --help 查看帮助\n", argv[i]);
            return 1;
        }
    }

    // 加载配置文件
    load_config(config_file);

    // 覆盖配置文件中的目录设置(如果命令行指定了目录)
    if (strcmp(scan_directory, ".") != 0)
    {
        printf("使用命令行指定的目录: %s\n", scan_directory);
    }

    // 如果开启调试模式，打印配置信息
    if (g_config.debug_mode || g_config.verbose_output)
    {
        print_config();
    }

    printf("扫描目录: %s\n", scan_directory);
    printf("触摸设备: %s\n", g_config.touch_device);
    printf("=================================\n");

    // 1. 从配置文件更新应用状态
    g_app_state.screen_w = g_config.screen_width;
    g_app_state.screen_h = g_config.screen_height;
    g_app_state.refresh_delay = g_config.default_refresh_delay;
    g_app_state.use_slow_refresh = g_config.use_slow_refresh;

    // 2. 扫描BMP文件
    g_app_state.bmp_count = scan_bmp_files(scan_directory, g_app_state.bmp_files, g_config.max_bmp_files);
    if (g_app_state.bmp_count <= 0)
    {
        printf("没有找到BMP文件，程序退出\n");
        return ERROR;
    }

    print_file_list(g_app_state.bmp_files, g_app_state.bmp_count);

    // 3. 初始化LCD设备
    lcd_device_t lcd;
    if (lcd_init(&lcd, g_app_state.screen_w, g_app_state.screen_h) != SUCCESS)
    {
        printf("LCD初始化失败，程序退出\n");
        return ERROR;
    }

    // 设置全局LCD地址
    g_app_state.map_addr = lcd.map_addr;

    // 4. 显示第一张图片
    g_app_state.current_index = 0;
    if (display_bmp(g_app_state.bmp_files[g_app_state.current_index],
                    g_app_state.map_addr, g_app_state.screen_w, g_app_state.screen_h,
                    g_app_state.use_slow_refresh, g_app_state.refresh_delay) < 0)
    {
        printf("显示初始图片失败\n");
    }

    // 5. 初始化触摸屏设备
    touch_device_t touch;
    int touch_enabled = 0;

    if (g_config.enable_touch)
    {
        if (touch_init(&touch, g_config.touch_device) == SUCCESS)
        {
            // 启动触摸屏处理线程
            if (pthread_create(&touch.thread_id, NULL, touch_handler_thread, &touch) == 0)
            {
                touch_enabled = 1;
                printf("触摸屏功能已启用\n");
            }
            else
            {
                perror("创建触摸屏处理线程失败");
                touch_cleanup(&touch);
            }
        }
        else
        {
            printf("触摸屏初始化失败，仅支持键盘控制\n");
        }
    }
    else
    {
        printf("触摸屏功能已禁用，仅支持键盘控制\n");
    }

    // 6. 显示帮助信息
    ui_print_help();
    ui_print_status(&g_app_state);

    // 7. 主循环 - 处理键盘输入
    printf("\n程序已启动，等待用户输入...\n");
    printf("按 'h' 键显示帮助信息\n");

    while (g_app_state.running)
    {
        if (!ui_handle_keyboard_input())
        {
            break;
        }

        // 如果开启了调试模式，可以添加一些统计信息显示
        if (g_config.debug_mode)
        {
            // TODO: 添加统计信息显示
        }
    }

    // 8. 清理资源
    printf("正在清理资源...\n");

    if (touch_enabled)
    {
        touch_cleanup(&touch);
        // 注意：触摸线程可能在无限循环中，实际应用中可能需要更优雅的退出机制
        // pthread_cancel(touch.thread_id);
        // pthread_join(touch.thread_id, NULL);
    }

    lcd_cleanup(&lcd);
    pthread_mutex_destroy(&g_app_state.refresh_mutex);

    // 保存当前配置（可选）
    if (g_config.debug_mode)
    {
        printf("保存当前配置...\n");
        save_default_config(config_file);
    }

    printf("程序已退出\n");
    return SUCCESS;
}
