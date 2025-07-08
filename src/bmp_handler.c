#include "../include/bmp_handler.h"

// 检查文件是否为BMP格式
int is_bmp_file(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (ext && strcasecmp(ext, ".bmp") == 0)
    {
        return 1;
    }
    return 0;
}

// 显示BMP图片到LCD屏幕
int display_bmp(const char *filename, int *map_addr, int screen_w, int screen_h, int use_slow_refresh, int refresh_delay)
{
    // 1. 打开并读取BMP文件信息
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("打开图片失败");
        return ERROR;
    }

    struct bitmap_header head = {0};
    if (fread(&head, sizeof(head), 1, fp) != 1)
    {
        perror("读取BMP文件头失败");
        fclose(fp);
        return ERROR;
    }

    if (head.type != 0x4D42)
    { // 'BM'
        fprintf(stderr, "不是一个有效的BMP文件\n");
        fclose(fp);
        return ERROR;
    }

    struct bitmap_info info = {0};
    if (fread(&info, sizeof(info), 1, fp) != 1)
    {
        perror("读取BMP信息头失败");
        fclose(fp);
        return ERROR;
    }

    printf("图片信息: %s\n", filename);
    printf("宽度: %d, 高度: %d, 色深: %d\n", info.width, info.height, info.bit_count);

    if (info.bit_count != 24 && info.bit_count != 32)
    {
        fprintf(stderr, "不支持的色深: %d, 只支持24位和32位\n", info.bit_count);
        fclose(fp);
        return ERROR;
    }

    // 2. 读取BMP像素数据
    int bytes_per_pixel = info.bit_count / 8;
    int row_size = ((info.width * bytes_per_pixel + 3) / 4) * 4; // 4字节对齐的行大小
    int image_size = row_size * abs(info.height);

    char *bgr_buf = malloc(image_size);
    if (bgr_buf == NULL)
    {
        perror("为BMP数据分配内存失败");
        fclose(fp);
        return ERROR;
    }

    fseek(fp, head.offbits, SEEK_SET);
    size_t bytes_read = fread(bgr_buf, 1, image_size, fp);
    if (bytes_read != (size_t)image_size)
    {
        perror("读取BMP像素数据失败");
        free(bgr_buf);
        fclose(fp);
        return ERROR;
    }
    fclose(fp);

    // 3. 转换像素并写入屏幕内存
    int w = info.width;
    int h = abs(info.height);

    // 确定要绘制的区域大小（防止图片超出屏幕）
    int draw_w = (w < screen_w) ? w : screen_w;
    int draw_h = (h < screen_h) ? h : screen_h;

    // 清屏为黑色
    memset(map_addr, 0, screen_w * screen_h * sizeof(int));

    // 根据模式选择显示方式
    if (use_slow_refresh)
    {
        printf("正在逐行加载图片...\n");

        // 逐行显示图片，并添加延迟
        for (int y = 0; y < draw_h; y++)
        {
            for (int x = 0; x < draw_w; x++)
            {
                int bmp_y = (info.height > 0) ? (h - 1 - y) : y; // 处理垂直翻转
                char *src_pixel = bgr_buf + bmp_y * row_size + x * bytes_per_pixel;
                int *dst_pixel = map_addr + y * screen_w + x;

                // 将BGR(A)转换为ARGB
                char b = src_pixel[0];
                char g = src_pixel[1];
                char r = src_pixel[2];
                *dst_pixel = (0 << 24) | (r << 16) | (g << 8) | b;
            }

            // 每显示一行后延迟
            if (refresh_delay > 0)
            {
                delay_ms(refresh_delay);
            }

            // 每10行输出一次进度
            if (y % 10 == 0 || y == draw_h - 1)
            {
                printf("\r加载进度: %.1f%%", (float)(y + 1) * 100 / draw_h);
                fflush(stdout);
            }
        }
        printf("\n图片加载完成!\n");
    }
    else
    {
        // 直接显示整张图片（无延迟）
        printf("直接加载整张图片...\n");
        for (int y = 0; y < draw_h; y++)
        {
            for (int x = 0; x < draw_w; x++)
            {
                int bmp_y = (info.height > 0) ? (h - 1 - y) : y; // 处理垂直翻转
                char *src_pixel = bgr_buf + bmp_y * row_size + x * bytes_per_pixel;
                int *dst_pixel = map_addr + y * screen_w + x;

                // 将BGR(A)转换为ARGB
                char b = src_pixel[0];
                char g = src_pixel[1];
                char r = src_pixel[2];
                *dst_pixel = (0 << 24) | (r << 16) | (g << 8) | b;
            }
        }
        printf("图片加载完成!\n");
    }

    free(bgr_buf);
    return SUCCESS;
}
