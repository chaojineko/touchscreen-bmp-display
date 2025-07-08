#include "../include/file_scanner.h"
#include "../include/bmp_handler.h"

// 扫描目录中的BMP文件
int scan_bmp_files(const char *directory, char files[][MAX_FILENAME_LEN], int max_files)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(directory);
    if (!dir)
    {
        perror("无法打开目录");
        return ERROR;
    }

    printf("扫描目录 '%s' 中的BMP文件...\n", directory);

    while ((entry = readdir(dir)) != NULL && count < max_files)
    {
        if (is_bmp_file(entry->d_name))
        {
            strncpy(files[count], entry->d_name, MAX_FILENAME_LEN - 1);
            files[count][MAX_FILENAME_LEN - 1] = '\0'; // 确保字符串结束
            printf("找到BMP文件: %s\n", files[count]);
            count++;
        }
    }

    closedir(dir);

    if (count == 0)
    {
        printf("目录 '%s' 中没有找到任何BMP文件\n", directory);
        return 0;
    }

    printf("共找到 %d 个BMP文件\n", count);
    return count;
}

// 打印文件列表
void print_file_list(char files[][MAX_FILENAME_LEN], int count)
{
    printf("\n======== BMP文件列表 ========\n");
    for (int i = 0; i < count; i++)
    {
        printf("%2d. %s\n", i + 1, files[i]);
    }
    printf("============================\n\n");
}
