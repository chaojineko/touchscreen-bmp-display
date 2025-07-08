#!/bin/bash

# ARM交叉编译脚本 - 用于BMP图片查看器

# 显示帮助信息
show_help() {
    echo "ARM交叉编译脚本 - BMP图片查看器"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -c, --compiler <前缀>   指定交叉编译器前缀 (默认: arm-linux-)"
    echo "  -o, --optimize <级别>   优化级别 (默认: -O2)"
    echo "  -m, --march <架构>      目标ARM架构 (默认: armv7-a)"
    echo "  -f, --float <ABI>       浮点ABI (硬: hard, 软: soft, 默认: hard)"
    echo "  -s, --static            静态链接"
    echo "  -h, --help              显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 --compiler arm-linux-gnueabihf- --march armv7-a --float hard"
    echo "  $0 -c arm-none-eabi- -o 3 -m armv6 -s"
    echo ""
}

# 默认值
CROSS_PREFIX="arm-linux-"
OPTIMIZE="-O2"
MARCH="armv7-a"
FLOAT_ABI="hard"
STATIC=""

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--compiler)
            CROSS_PREFIX="$2"
            shift 2
            ;;
        -o|--optimize)
            OPTIMIZE="-O$2"
            shift 2
            ;;
        -m|--march)
            MARCH="$2"
            shift 2
            ;;
        -f|--float)
            FLOAT_ABI="$2"
            shift 2
            ;;
        -s|--static)
            STATIC="-static"
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
done

# 验证编译器是否存在
if ! command -v ${CROSS_PREFIX}gcc &> /dev/null; then
    echo "错误: 找不到交叉编译器 ${CROSS_PREFIX}gcc"
    echo "请安装ARM交叉编译工具链或指定正确的前缀。"
    exit 1
fi

# 构建CFLAGS
ARCH_FLAGS="-march=${MARCH} -mfloat-abi=${FLOAT_ABI}"
if [[ "$FLOAT_ABI" == "hard" ]]; then
    ARCH_FLAGS="${ARCH_FLAGS} -mfpu=vfp"
fi

EXTRA_CFLAGS="${OPTIMIZE} ${ARCH_FLAGS}"
EXTRA_LDFLAGS="${STATIC}"

echo "======================================"
echo "ARM交叉编译 - BMP图片查看器"
echo "======================================"
echo "使用交叉编译器: ${CROSS_PREFIX}gcc"
echo "架构标志: ${ARCH_FLAGS}"
echo "优化级别: ${OPTIMIZE}"
if [[ -n "$STATIC" ]]; then
    echo "静态链接: 是"
else
    echo "静态链接: 否"
fi
echo "======================================"

# 清理旧的构建文件
echo "清理旧的构建文件..."
make clean

# 开始编译
echo "开始编译..."
make arm CROSS_COMPILE=${CROSS_PREFIX} CFLAGS="${EXTRA_CFLAGS}" LDFLAGS="${EXTRA_LDFLAGS}"

# 检查编译结果
if [[ $? -eq 0 ]]; then
    echo "======================================"
    echo "编译成功！二进制文件位置: bin/bmp_viewer"
    echo "文件信息:"
    ${CROSS_PREFIX}objdump -f bin/bmp_viewer | grep "architecture"
    echo "文件大小: $(du -h bin/bmp_viewer | cut -f1)"
    
    # 检查是否为静态链接
    if [[ -z "$STATIC" ]]; then
        echo "动态库依赖:"
        ${CROSS_PREFIX}objdump -p bin/bmp_viewer | grep "NEEDED" || echo "无动态依赖"
    else
        echo "静态链接，无动态库依赖"
    fi
    
    echo "======================================"
    echo "要将程序部署到目标设备，请使用:"
    echo "scp bin/bmp_viewer user@target:/path/to/destination/"
    echo "======================================"
else
    echo "======================================"
    echo "编译失败！请检查错误信息。"
    echo "======================================"
    exit 1
fi
