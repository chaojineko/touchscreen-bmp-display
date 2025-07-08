# BMP图片查看器 API文档

本文档描述了BMP图片查看器的各个模块API，供开发者参考。

## 1. 公共模块 (common.h)

### 1.1 数据结构

```c
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
```

### 1.2 公共函数

```c
void delay_ms(int milliseconds);
int getch(void);
```

## 2. 配置模块 (config.h)

### 2.1 数据结构

```c
typedef struct {
    int screen_width;
    int screen_height;
    int default_refresh_delay;
    int use_slow_refresh;
    int max_bmp_files;
    int scan_subdirectories;
    char touch_device[256];
    int swipe_threshold;
    int enable_touch;
    char lcd_device[256];
    int bits_per_pixel;
    int debug_mode;
    int verbose_output;
} config_t;
```

### 2.2 配置函数

```c
int load_config(const char *config_file);
void print_config(void);
void save_default_config(const char *config_file);
```

## 3. BMP处理模块 (bmp_handler.h)

### 3.1 数据结构

```c
struct bitmap_header {
    int16_t type;          // 文件类型
    int32_t size;          // 图像文件大小
    int16_t reserved1;     // 保留
    int16_t reserved2;     // 保留
    int32_t offbits;       // bmp图像数据偏移量
} __attribute__((packed));

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
```

### 3.2 BMP处理函数

```c
int is_bmp_file(const char *filename);
int display_bmp(const char *filename, int *map_addr, int screen_w, int screen_h, int use_slow_refresh, int refresh_delay);
```

## 4. LCD控制模块 (lcd_control.h)

### 4.1 数据结构

```c
typedef struct {
    int fd;
    int *map_addr;
    int width;
    int height;
    int bpp;
    long screen_size;
} lcd_device_t;
```

### 4.2 LCD控制函数

```c
int lcd_init(lcd_device_t *lcd, int width, int height);
void lcd_cleanup(lcd_device_t *lcd);
void lcd_clear_screen(lcd_device_t *lcd, int color);
```

## 5. 触摸屏模块 (touch_handler.h)

### 5.1 数据结构

```c
typedef enum {
    TOUCH_EVENT_NONE,
    TOUCH_EVENT_PREV,
    TOUCH_EVENT_NEXT,
    TOUCH_EVENT_TOGGLE_MODE
} touch_event_t;

typedef struct {
    int x;
    int y;
} touch_point_t;

typedef struct {
    int fd;
    touch_point_t start_pos;
    touch_point_t current_pos;
    int is_pressed;
    pthread_t thread_id;
    int running;
} touch_device_t;
```

### 5.2 触摸屏函数

```c
int touch_init(touch_device_t *touch, const char *device_path);
void touch_cleanup(touch_device_t *touch);
void *touch_handler_thread(void *arg);
touch_event_t process_touch_event(touch_point_t start, touch_point_t end);
```

## 6. 文件扫描模块 (file_scanner.h)

### 6.1 文件扫描函数

```c
int scan_bmp_files(const char *directory, char files[][MAX_FILENAME_LEN], int max_files);
void print_file_list(char files[][MAX_FILENAME_LEN], int count);
```

## 7. 用户界面模块 (user_interface.h)

### 7.1 用户界面函数

```c
void ui_print_help(void);
void ui_print_status(const app_state_t *state);
int ui_handle_keyboard_input(void);
void ui_show_next_image(void);
void ui_show_prev_image(void);
void ui_toggle_refresh_mode(void);
void ui_reload_current_image(void);
void ui_adjust_refresh_delay(int new_delay);
void show_image_info(void);
```
