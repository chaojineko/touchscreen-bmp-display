#!/bin/bash
#
# ARM交叉编译脚本 - 简化版 - 适用于WSL环境
# 

# 显示帮助
show_help() {
    echo "========================================================="
    echo "ARM交叉编译脚本 (WSL版) - BMP图片查看器"
    echo "========================================================="
    echo "用法: $0 [参数]"
    echo ""
    echo "参数:"
    echo "  -c, --compiler=PREFIX  指定交叉编译器前缀 (默认: arm-linux-)"
    echo "  -s, --static           静态链接 (可选)"
    echo "  -d, --debug            编译调试版本 (可选)"
    echo "  -r, --release          编译发布版本 (默认)"
    echo "  -h, --help             显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 --compiler=arm-linux-gnueabihf- --static"
    echo "  $0 -c arm-none-eabi- -d"
    echo "========================================================="
}

# 默认值
CROSS_COMPILER="arm-linux-"
BUILD_TYPE="release"
STATIC_BUILD=""

# 参数解析
for i in "$@"; do
    case $i in
        -c=*|--compiler=*)
            CROSS_COMPILER="${i#*=}"
            shift
            ;;
        -c|--compiler)
            if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
                CROSS_COMPILER=$2
                shift 2
            else
                echo "错误: --compiler 选项需要一个参数"
                exit 1
            fi
            ;;
        -s|--static)
            STATIC_BUILD="-static"
            shift
            ;;
        -d|--debug)
            BUILD_TYPE="debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="release"
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            # 未知选项
            show_help
            exit 1
            ;;
    esac
done

# 检查交叉编译器
if ! command -v "${CROSS_COMPILER}gcc" &> /dev/null; then
    echo "错误: 找不到交叉编译器 ${CROSS_COMPILER}gcc"
    echo "请确保已安装正确的交叉编译工具链，或者通过 --compiler 选项指定正确的前缀"
    exit 1
fi

echo "========================================================="
echo "ARM交叉编译脚本 (WSL版) - BMP图片查看器"
echo "========================================================="
echo "使用交叉编译器: ${CROSS_COMPILER}gcc"
echo "版本信息: $(${CROSS_COMPILER}gcc --version | head -n 1)"
echo "构建类型: ${BUILD_TYPE}"
if [ -n "$STATIC_BUILD" ]; then
    echo "静态链接: 是"
else
    echo "静态链接: 否"
fi
echo "========================================================="

# 清理
echo "清理旧的构建文件..."
make clean

# 构建命令
BUILD_CMD="make arm CROSS_COMPILE=${CROSS_COMPILER}"

# 添加构建类型
if [ "$BUILD_TYPE" == "debug" ]; then
    BUILD_CMD="${BUILD_CMD} debug"
elif [ "$BUILD_TYPE" == "release" ]; then
    BUILD_CMD="${BUILD_CMD} release"
fi

# 添加静态链接
if [ -n "$STATIC_BUILD" ]; then
    BUILD_CMD="${BUILD_CMD} LDFLAGS=${STATIC_BUILD}"
fi

# 执行构建
echo "执行: ${BUILD_CMD}"
echo "========================================================="
eval ${BUILD_CMD}

# 检查构建结果
if [ $? -eq 0 ]; then
    echo "========================================================="
    echo "编译成功!"
    echo "可执行文件: bin/bmp_viewer"
    
    # 文件信息
    echo -n "文件类型: "
    file bin/bmp_viewer | cut -d':' -f2-
    
    echo -n "文件大小: "
    ls -lh bin/bmp_viewer | awk '{print $5}'
    
    # 静态/动态链接信息
    if [ -n "$STATIC_BUILD" ]; then
        echo "链接类型: 静态链接"
    else
        echo "链接类型: 动态链接，依赖以下库:"
        ${CROSS_COMPILER}readelf -d bin/bmp_viewer | grep "NEEDED" || echo "  (无动态库依赖)"
    fi
    
    echo "========================================================="
    echo "在ARM设备上使用:"
    echo "1. 将可执行文件复制到ARM设备"
    echo "   scp bin/bmp_viewer user@arm-device:/path/to/destination/"
    echo ""
    echo "2. 确保文件具有执行权限"
    echo "   chmod +x /path/to/destination/bmp_viewer"
    echo ""
    echo "3. 运行程序"
    echo "   ./bmp_viewer [选项] [图片目录]"
    echo "========================================================="
else
    echo "========================================================="
    echo "编译失败! 请检查错误信息"
    echo "========================================================="
    exit 1
fi
