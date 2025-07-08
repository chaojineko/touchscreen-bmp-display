# BMP图片查看器 - 模块化版本
# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
LDFLAGS = -pthread -lm

# 版本信息
VERSION = 1.0.0
BUILD_DATE = $(shell date +%Y-%m-%d)

# 目录结构
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DOC_DIR = docs
TEST_DIR = tests

# 目标文件
TARGET = $(BIN_DIR)/bmp_viewer

# 源文件
SOURCES = $(wildcard $(SRC_DIR)/*.c) main.c
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# 版本定义
CFLAGS += -DVERSION=\"$(VERSION)\" -DBUILD_DATE=\"$(BUILD_DATE)\"

# 包含目录
INCLUDES = -I$(INC_DIR)

# 默认目标
all: $(TARGET)

# 创建目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 编译目标
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "编译完成: $@"

# 编译源文件
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 清理
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "清理完成"

# 安装（可选）
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	@echo "安装完成"

# 调试版本
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# 发布版本
release: CFLAGS += -O2 -DNDEBUG
release: $(TARGET)

# 生成版本信息
version:
	@echo "BMP图片查看器 - 模块化版本 v$(VERSION) ($(BUILD_DATE))"

# 创建文档目录
docs:
	mkdir -p $(DOC_DIR)
	echo "# API文档" > $(DOC_DIR)/API.md
	echo "# 使用手册" > $(DOC_DIR)/Manual.md
	echo "文档已创建在 $(DOC_DIR) 目录"

# 检查代码风格（需要安装cppcheck）
check:
	cppcheck --enable=all --std=c99 $(SRC_DIR)/*.c main.c

# 运行
run: $(TARGET)
	cd $(BIN_DIR) && ./bmp_viewer

# 帮助
help:
	@echo "可用目标:"
	@echo "  all      - 编译程序 (默认)"
	@echo "  clean    - 清理生成的文件"
	@echo "  debug    - 编译调试版本"
	@echo "  release  - 编译发布版本"
	@echo "  install  - 安装到系统"
	@echo "  check    - 代码风格检查"
	@echo "  run      - 编译并运行程序"
	@echo "  help     - 显示此帮助信息"

.PHONY: all clean install debug release check run help version docs tests
