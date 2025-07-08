#!/bin/bash
#
# 简单交叉编译脚本 - 单命令编译BMP查看器
# 类似于 arm-gcc filename.c -o filename 的方式
#

# 默认交叉编译器
COMPILER="arm-linux-gcc"

# 检查命令行参数
if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "简单交叉编译脚本 - BMP查看器"
    echo "用法: $0 [编译器路径]"
    echo "例如: $0 arm-linux-gcc"
    echo "      $0 /opt/toolchains/arm-linux-gnueabihf-gcc"
    exit 0
fi

# 如果提供了参数，使用该参数作为编译器
if [ -n "$1" ]; then
    COMPILER="$1"
fi

# 检查编译器是否存在
if ! command -v $COMPILER &> /dev/null; then
    echo "错误: 找不到编译器 '$COMPILER'"
    echo "请确保编译器已安装并在路径中，或提供编译器的完整路径。"
    exit 1
fi

echo "=== 使用编译器: $COMPILER ==="

# 创建bin目录（如果不存在）
mkdir -p bin

# 执行编译
echo "编译所有源文件..."
$COMPILER main.c src/*.c -Iinclude -pthread -lm -o bin/bmp_viewer -Wall -O2

# 检查编译结果
if [ $? -eq 0 ]; then
    echo "=== 编译成功! ==="
    echo "可执行文件: bin/bmp_viewer"
    echo "文件大小: $(du -h bin/bmp_viewer | cut -f1)"
    echo ""
    echo "使用方法:"
    echo "./bin/bmp_viewer [选项] [图片目录]"
    echo ""
    echo "示例:"
    echo "./bin/bmp_viewer -c config.conf /path/to/images"
else
    echo "=== 编译失败! ==="
fi
