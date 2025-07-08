#include "../include/config.h"

// 全局配置
config_t g_config = {
    .screen_width = 800,
    .screen_height = 480,
    .default_refresh_delay = 5,
    .use_slow_refresh = 1,
    .max_bmp_files = 100,
    .scan_subdirectories = 0,
    .touch_device = "/dev/input/event0",
    .swipe_threshold = 100,
    .enable_touch = 1,
    .lcd_device = "/dev/fb0",
    .bits_per_pixel = 32,
    .debug_mode = 0,
    .verbose_output = 1
};

// 加载配置文件
int load_config(const char *config_file)
{
    FILE *fp = fopen(config_file, "r");
    if (!fp) {
        printf("配置文件 %s 不存在，使用默认配置\n", config_file);
        // 创建一个默认配置文件
        save_default_config(config_file);
        return 0;
    }

    char line[256];
    char key[64], value[192];

    while (fgets(line, sizeof(line), fp)) {
        // 跳过注释和空行
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        // 解析key=value对
        if (sscanf(line, "%63[^=]=%191[^\n]", key, value) == 2) {
            // 去除首尾空格
            char *k = key;
            while (*k && isspace(*k)) k++;
            char *end = k + strlen(k) - 1;
            while (end > k && isspace(*end)) *end-- = '\0';

            char *v = value;
            while (*v && isspace(*v)) v++;
            end = v + strlen(v) - 1;
            while (end > v && isspace(*end)) *end-- = '\0';

            // 设置配置项
            if (strcmp(k, "screen_width") == 0) {
                g_config.screen_width = atoi(v);
            } else if (strcmp(k, "screen_height") == 0) {
                g_config.screen_height = atoi(v);
            } else if (strcmp(k, "default_refresh_delay") == 0) {
                g_config.default_refresh_delay = atoi(v);
            } else if (strcmp(k, "use_slow_refresh") == 0) {
                g_config.use_slow_refresh = atoi(v);
            } else if (strcmp(k, "max_bmp_files") == 0) {
                g_config.max_bmp_files = atoi(v);
            } else if (strcmp(k, "scan_subdirectories") == 0) {
                g_config.scan_subdirectories = atoi(v);
            } else if (strcmp(k, "touch_device") == 0) {
                strncpy(g_config.touch_device, v, sizeof(g_config.touch_device) - 1);
                g_config.touch_device[sizeof(g_config.touch_device) - 1] = '\0';
            } else if (strcmp(k, "swipe_threshold") == 0) {
                g_config.swipe_threshold = atoi(v);
            } else if (strcmp(k, "enable_touch") == 0) {
                g_config.enable_touch = atoi(v);
            } else if (strcmp(k, "lcd_device") == 0) {
                strncpy(g_config.lcd_device, v, sizeof(g_config.lcd_device) - 1);
                g_config.lcd_device[sizeof(g_config.lcd_device) - 1] = '\0';
            } else if (strcmp(k, "bits_per_pixel") == 0) {
                g_config.bits_per_pixel = atoi(v);
            } else if (strcmp(k, "debug_mode") == 0) {
                g_config.debug_mode = atoi(v);
            } else if (strcmp(k, "verbose_output") == 0) {
                g_config.verbose_output = atoi(v);
            } else {
                printf("未知配置项: %s = %s\n", k, v);
            }
        }
    }

    fclose(fp);
    printf("配置文件 %s 已加载\n", config_file);
    return 1;
}

// 打印当前配置
void print_config(void)
{
    printf("\n======== 当前配置 ========\n");
    printf("屏幕宽度: %d\n", g_config.screen_width);
    printf("屏幕高度: %d\n", g_config.screen_height);
    printf("刷新延迟: %d 毫秒/行\n", g_config.default_refresh_delay);
    printf("使用逐行刷新: %s\n", g_config.use_slow_refresh ? "是" : "否");
    printf("最大BMP文件数: %d\n", g_config.max_bmp_files);
    printf("扫描子目录: %s\n", g_config.scan_subdirectories ? "是" : "否");
    printf("触摸设备: %s\n", g_config.touch_device);
    printf("滑动阈值: %d 像素\n", g_config.swipe_threshold);
    printf("启用触摸功能: %s\n", g_config.enable_touch ? "是" : "否");
    printf("LCD设备: %s\n", g_config.lcd_device);
    printf("色深(BPP): %d\n", g_config.bits_per_pixel);
    printf("调试模式: %s\n", g_config.debug_mode ? "开启" : "关闭");
    printf("详细输出: %s\n", g_config.verbose_output ? "开启" : "关闭");
    printf("==========================\n\n");
}

// 保存默认配置
void save_default_config(const char *config_file)
{
    FILE *fp = fopen(config_file, "w");
    if (!fp) {
        perror("无法创建默认配置文件");
        return;
    }

    fprintf(fp, "# BMP图片查看器配置文件\n");
    fprintf(fp, "# 所有行以#开头的都是注释\n\n");

    fprintf(fp, "# 显示设置\n");
    fprintf(fp, "screen_width=%d\n", g_config.screen_width);
    fprintf(fp, "screen_height=%d\n", g_config.screen_height);
    fprintf(fp, "default_refresh_delay=%d\n", g_config.default_refresh_delay);
    fprintf(fp, "use_slow_refresh=%d\n", g_config.use_slow_refresh);
    fprintf(fp, "\n");

    fprintf(fp, "# 文件设置\n");
    fprintf(fp, "max_bmp_files=%d\n", g_config.max_bmp_files);
    fprintf(fp, "scan_subdirectories=%d\n", g_config.scan_subdirectories);
    fprintf(fp, "\n");

    fprintf(fp, "# 触摸设置\n");
    fprintf(fp, "touch_device=%s\n", g_config.touch_device);
    fprintf(fp, "swipe_threshold=%d\n", g_config.swipe_threshold);
    fprintf(fp, "enable_touch=%d\n", g_config.enable_touch);
    fprintf(fp, "\n");

    fprintf(fp, "# LCD设置\n");
    fprintf(fp, "lcd_device=%s\n", g_config.lcd_device);
    fprintf(fp, "bits_per_pixel=%d\n", g_config.bits_per_pixel);
    fprintf(fp, "\n");

    fprintf(fp, "# 调试设置\n");
    fprintf(fp, "debug_mode=%d\n", g_config.debug_mode);
    fprintf(fp, "verbose_output=%d\n", g_config.verbose_output);

    fclose(fp);
    printf("默认配置已保存到 %s\n", config_file);
}
