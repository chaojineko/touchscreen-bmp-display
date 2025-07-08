#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include "common.h"

// 函数声明
int scan_bmp_files(const char *directory, char files[][MAX_FILENAME_LEN], int max_files);
void print_file_list(char files[][MAX_FILENAME_LEN], int count);

#endif // FILE_SCANNER_H
