CXX = g++
CXXFLAGS = -Wall -std=c++2a

TARGETS = nullptr

all: $(TARGETS)

# 使用模式规则，% 代表目标文件的名称，%.cpp 代表对应的源文件。
# $@ 是一个自动变量，它代表规则中的目标文件
# $< 是另一个自动变量，它代表规则中的第一个依赖文件
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean