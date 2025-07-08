#ifndef BMP_HANDLER_H
#define BMP_HANDLER_H

#include "common.h"

// BMP文件头结构
struct bitmap_header {
    int16_t type;          // 文件类型
    int32_t size;          // 图像文件大小
    int16_t reserved1;     // 保留
    int16_t reserved2;     // 保留
    int32_t offbits;       // bmp图像数据偏移量
} __attribute__((packed));

// BMP信息头结构
struct bitmap_info {
    int32_t size;          // 本结构体的大小
    int32_t width;         // 图像宽
    int32_t height;        // 图像高
    int16_t planes;
    int16_t bit_count;     // 色深
    int32_t compression;
    int32_t size_img;      // bmp数据大小
    int32_t X_pel;
    int32_t Y_pel;
    int32_t clrused;
    int32_t clrImportant;
} __attribute__((packed));

// 函数声明
int is_bmp_file(const char *filename);
int display_bmp(const char *filename, int *map_addr, int screen_w, int screen_h, int use_slow_refresh, int refresh_delay);

#endif // BMP_HANDLER_H
