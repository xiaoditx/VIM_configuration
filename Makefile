# Windows平台下的Makefile文件，用于编译Vim配置工具

# 编译器设置
CC = g++
CXXFLAGS = -std=c++11 -Wall -O2 -D_UNICODE -DUNICODE
LDFLAGS = -mwindows  # 不显示控制台窗口

# 源文件和目标文件
SRCS = main.cpp vimconfig.cpp guirenderer.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = vim_config_tool.exe

# 默认目标
all: $(TARGET)

# 链接目标文件
$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# 编译源文件
%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	del $(OBJS) $(TARGET) 2>nul || echo 没有需要清理的文件

# 运行程序
run: $(TARGET)
	$(TARGET)

# 检查编译环境
check-env:
	@echo 检查编译环境...
	@g++ --version >nul 2>&1 || (echo 错误: 未找到g++编译器，请安装MinGW && exit 1)
	@make --version >nul 2>&1 || (echo 警告: 未找到make工具，可能需要使用mingw32-make && exit 0)

# 依赖关系
main.o: main.cpp vimconfig.h guirenderer.h
vimconfig.o: vimconfig.cpp vimconfig.h
guirenderer.o: guirenderer.cpp guirenderer.h vimconfig.h

.PHONY: all clean run check-env