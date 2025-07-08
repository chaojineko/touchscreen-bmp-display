#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "common.h"

// 函数声明
void ui_print_help(void);
void ui_print_status(const app_state_t *state);
int ui_handle_keyboard_input(void);
void ui_show_next_image(void);
void ui_show_prev_image(void);
void ui_toggle_refresh_mode(void);
void ui_reload_current_image(void);
void ui_adjust_refresh_delay(int new_delay);

#endif // USER_INTERFACE_H
