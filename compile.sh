#!/bin/bash

# BMP图片查看器 - 模块化版本编译脚本

echo "==================================="
echo "BMP图片查看器 - 模块化版本编译脚本"
echo "==================================="

# 检查是否存在Makefile
if [ -f "Makefile" ]; then
    echo "使用Makefile进行编译..."
    make clean
    make
    
    if [ $? -eq 0 ]; then
        echo "编译成功！"
        echo "可执行文件位置: bin/bmp_viewer"
        echo ""
        echo "使用方法:"
        echo "  cd bin && ./bmp_viewer [图片目录] [触摸设备]"
        echo ""
        echo "示例:"
        echo "  cd bin && ./bmp_viewer . /dev/input/event0"
    else
        echo "编译失败！"
        exit 1
    fi
else
    echo "未找到Makefile，使用手动编译..."
    
    # 创建必要的目录
    mkdir -p obj/src bin
    
    # 编译参数
    CC=gcc
    CFLAGS="-Wall -Wextra -std=c99 -pthread"
    INCLUDES="-Iinclude"
    LDFLAGS="-pthread -lm"
    
    # 编译所有源文件
    echo "编译源文件..."
    
    for file in src/*.c; do
        if [ -f "$file" ]; then
            obj_file="obj/${file%.c}.o"
            echo "编译 $file -> $obj_file"
            $CC $CFLAGS $INCLUDES -c "$file" -o "$obj_file"
            if [ $? -ne 0 ]; then
                echo "编译 $file 失败！"
                exit 1
            fi
        fi
    done
    
    # 编译主文件
    echo "编译 main.c -> obj/main.o"
    $CC $CFLAGS $INCLUDES -c main.c -o obj/main.o
    if [ $? -ne 0 ]; then
        echo "编译 main.c 失败！"
        exit 1
    fi
    
    # 链接
    echo "链接生成可执行文件..."
    $CC obj/*.o obj/src/*.o -o bin/bmp_viewer $LDFLAGS
    
    if [ $? -eq 0 ]; then
        echo "编译成功！"
        echo "可执行文件位置: bin/bmp_viewer"
        echo ""
        echo "使用方法:"
        echo "  cd bin && ./bmp_viewer [图片目录] [触摸设备]"
        echo ""
        echo "示例:"
        echo "  cd bin && ./bmp_viewer . /dev/input/event0"
    else
        echo "链接失败！"
        exit 1
    fi
fi

echo "==================================="
