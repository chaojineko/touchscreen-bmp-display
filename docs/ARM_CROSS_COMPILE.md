# ARM交叉编译指南

本文档提供了如何使用ARM交叉编译器编译BMP图片查看器的详细说明。

## 环境准备

### 1. 安装交叉编译工具链

在大多数Linux发行版上，可以通过包管理器安装ARM交叉编译工具链：

**Ubuntu/Debian:**
```bash
sudo apt-get install gcc-arm-linux-gnueabi binutils-arm-linux-gnueabi
# 或者
sudo apt-get install gcc-arm-linux-gnueabihf binutils-arm-linux-gnueabihf
```

**Fedora:**
```bash
sudo dnf install arm-linux-gnu-gcc arm-linux-gnu-binutils
```

**Arch Linux:**
```bash
sudo pacman -S arm-linux-gnueabihf-gcc arm-linux-gnueabihf-binutils
```

### 2. 确认工具链可用

安装完成后，确认交叉编译工具链是否可用：

```bash
arm-linux-gcc --version
# 或者
arm-linux-gnueabi-gcc --version
# 或者
arm-linux-gnueabihf-gcc --version
```

## 编译步骤

### 基本编译

使用默认的arm-linux-gcc交叉编译器：

```bash
make clean
make arm
```

### 使用自定义交叉编译器

如果您的交叉编译器前缀与默认不同，可以通过CROSS_COMPILE变量指定：

```bash
make clean
make arm CROSS_COMPILE=arm-poky-linux-gnueabi-
```

常见的交叉编译器前缀包括：
- `arm-linux-`
- `arm-linux-gnueabi-`
- `arm-linux-gnueabihf-`
- `arm-none-eabi-`
- `arm-poky-linux-gnueabi-`
- `aarch64-linux-gnu-` (用于64位ARM)

### 额外编译选项

您也可以传递额外的编译选项：

```bash
make arm CFLAGS="-O3 -march=armv7-a -mfpu=neon"
```

## 部署到目标设备

编译完成后，二进制文件将位于`bin/bmp_viewer`。您需要将此文件以及任何配置文件（如`config.conf`）传输到目标设备。

### 使用SCP传输

```bash
scp bin/bmp_viewer root@target-device:/usr/local/bin/
scp config.conf root@target-device:/etc/bmp_viewer/
```

### 使用USB存储设备

另一种方法是将文件复制到USB存储设备，然后连接到目标设备：

```bash
cp bin/bmp_viewer /media/usb/
cp config.conf /media/usb/
```

## 运行程序

在目标设备上运行程序：

```bash
cd /path/to/bmp_files
/usr/local/bin/bmp_viewer
```

或者指定配置文件：

```bash
/usr/local/bin/bmp_viewer -c /etc/bmp_viewer/config.conf
```

## 可能的问题及解决方法

### 1. 动态链接库问题

如果程序依赖于特定的共享库，可能需要确保目标设备上安装了这些库。您可以使用静态链接来避免此问题：

```bash
make arm LDFLAGS="-static"
```

### 2. 权限问题

确保二进制文件有执行权限：

```bash
chmod +x /usr/local/bin/bmp_viewer
```

对于访问帧缓冲设备和输入设备的权限，可能需要root权限或将用户添加到相应的组：

```bash
sudo usermod -a -G video,input username
```

### 3. 硬件兼容性

如果目标设备使用特定版本的ARM架构，您可能需要指定相应的编译标志：

```bash
make arm CFLAGS="-march=armv7-a -mfpu=vfp -mfloat-abi=hard"
```

## 其他资源

- [ARM交叉编译工具链文档](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain)
- [嵌入式Linux帧缓冲文档](https://www.kernel.org/doc/Documentation/fb/framebuffer.txt)
- [Linux输入子系统文档](https://www.kernel.org/doc/Documentation/input/input.txt)
