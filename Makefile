# 定义编译器
CXX = g++

# 定义编译标志，-g用于调试信息，-Wall开启大多数警告
CXXFLAGS = -g -Wall $(shell root-config --cflags) -O3

# 定义链接器标志，这里引入ROOT库
LDFLAGS = $(shell root-config --libs)

# 定义目标可执行文件
TARGET = main

# 默认目标
all: $(TARGET)

# 链接规则
$(TARGET): main.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# 编译规则
main.o: main.cxx Particle.h Event.h par.h
	$(CXX) $(CXXFLAGS) -c $<

# 清理编译生成的文件
clean:
	rm -f $(TARGET) main.o

