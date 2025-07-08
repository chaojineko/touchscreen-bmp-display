#!/bin/bash
#
# Windows下WSL环境交叉编译简单脚本
# 对于习惯使用简单命令如 arm-gcc filename.c -o filename 的用户
#

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 默认值
COMPILER="arm-linux-gcc"
OPTIMZE="-O2"
ARCH="-march=armv7-a"
OUTPUT="bin/bmp_viewer"

# 打印彩色信息
print_info() {
    echo -e "${YELLOW}[信息]${NC} $1"
}

print_error() {
    echo -e "${RED}[错误]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[成功]${NC} $1"
}

# 显示帮助
if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "===================================================="
    echo "BMP查看器 - 简易交叉编译脚本 (适用于WSL环境)"
    echo "===================================================="
    echo "用法: $0 [参数]"
    echo ""
    echo "参数:"
    echo "  编译器路径         指定ARM交叉编译器路径"
    echo "  -h, --help        显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0                  使用默认arm-linux-gcc编译"
    echo "  $0 arm-linux-gnueabihf-gcc   使用指定编译器"
    echo "===================================================="
    exit 0
fi

# 如果提供了参数，使用该参数作为编译器
if [ -n "$1" ]; then
    COMPILER="$1"
fi

# 检查编译器
print_info "检查编译器: $COMPILER"
if ! command -v $COMPILER &> /dev/null; then
    print_error "找不到编译器 '$COMPILER'!"
    echo "请确保编译器已安装并在PATH中，或提供完整路径"
    echo "例如:"
    echo "  ./win_simple_compile.sh /opt/arm-toolchain/bin/arm-linux-gcc"
    exit 1
fi

# 创建输出目录
mkdir -p bin

print_info "使用编译器: $COMPILER"
print_info "编译所有源文件..."

# 完整编译命令
COMPILE_CMD="$COMPILER main.c src/*.c -Iinclude -pthread -lm -Wall $OPTIMZE $ARCH -o $OUTPUT"

# 显示编译命令
echo "执行: $COMPILE_CMD"
echo "===================================================="

# 执行编译
eval $COMPILE_CMD

# 检查结果
if [ $? -eq 0 ]; then
    print_success "编译成功!"
    echo "可执行文件: $OUTPUT"
    echo "文件大小: $(du -h $OUTPUT | cut -f1)"
    
    echo "===================================================="
    echo "使用方法:"
    echo "1. 将文件复制到ARM设备"
    echo "   scp $OUTPUT user@arm-device:/path/to/destination/"
    echo ""
    echo "2. 在ARM设备上运行"
    echo "   ./bmp_viewer [选项] [图片目录]"
    echo "===================================================="
else
    print_error "编译失败! 请检查错误信息。"
    exit 1
fi
