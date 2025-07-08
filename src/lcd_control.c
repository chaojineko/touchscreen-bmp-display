#include "../include/lcd_control.h"

// 初始化LCD设备
int lcd_init(lcd_device_t *lcd, int width, int height)
{
    if (!lcd)
    {
        return ERROR;
    }

    // 打开LCD设备
    lcd->fd = open("/dev/fb0", O_RDWR);
    if (lcd->fd == -1)
    {
        perror("打开LCD失败");
        return ERROR;
    }

    lcd->width = width;
    lcd->height = height;
    lcd->bpp = 4; // 32位色深，4字节per pixel
    lcd->screen_size = width * height * lcd->bpp;

    // 内存映射
    lcd->map_addr = mmap(NULL, lcd->screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd->fd, 0);
    if (lcd->map_addr == MAP_FAILED)
    {
        perror("映射LCD内存失败");
        close(lcd->fd);
        return ERROR;
    }

    printf("LCD初始化成功: %dx%d, %d字节/像素\n", width, height, lcd->bpp);
    return SUCCESS;
}

// 清理LCD资源
void lcd_cleanup(lcd_device_t *lcd)
{
    if (!lcd)
    {
        return;
    }

    if (lcd->map_addr && lcd->map_addr != MAP_FAILED)
    {
        munmap(lcd->map_addr, lcd->screen_size);
        lcd->map_addr = NULL;
    }

    if (lcd->fd >= 0)
    {
        close(lcd->fd);
        lcd->fd = -1;
    }

    printf("LCD资源已清理\n");
}

// 清屏
void lcd_clear_screen(lcd_device_t *lcd, int color)
{
    if (!lcd || !lcd->map_addr)
    {
        return;
    }

    int *pixel = (int *)lcd->map_addr;
    int total_pixels = lcd->width * lcd->height;

    for (int i = 0; i < total_pixels; i++)
    {
        pixel[i] = color;
    }
}
