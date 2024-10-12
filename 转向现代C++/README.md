# 转向现代C++

## C++与C的兼容性

出于一些历史原因，C++ 中不可避免的需要使用一些 C 语言代码（甚至古老的 C 语言代码），例如 Linux 系统调用。在C++ 11出现之前，大部分人当谈及C与C++的区别是什么时，普遍除了回答面向对象的类特性、泛型编程的模板特性外，就没有其他的看法了，甚至直接回答『差不多』，也是大有人在。

从现在开始，你的脑子里应该树立『**C++** **不是** **C** **的一个超集**』这个观念，在编写 C++ 时，也应该尽可能的避免使用诸如 void* 之类的程序风格。而在不得不使用 C 时，应该注意使用 extern "C" 这种特性，将 C 语言的代码与 C++ 代码进行分离编译，再统一链接这种做法，例如：

```c
/**
 * foo.h
 */

#ifdef __cplusplus
extern "C" {
#endif

int add(int x, int y);

#ifdef __cplusplus
}
#endif
```

```c
/**
 * foo.c
 */
#include "foo.h"

int add(int x, int y) {
    return x + y;
}
```

```cpp
/**
 * main.cpp
 */
#include "foo.h"
#include <iostream>
#include <functional>

int main() {
    [out = std::ref(std::cout << "Result from C code: " << add(1, 1))]() {
        out.get() << ".\n";
    }();
    return 0;
}
```

在这个例子中，应先使用 gcc 编译 C 语言的代码：`gcc -c foo.c`，编译出 `foo.o` 文件，再将 C++ 代码和 .o 文件链接起来（或者都编译为 .o 再统一链接）:`g++ main.cpp foo.o -std=c++2a -o main.exe`。完整的`Makefile`文件如下：

```makefile
C = gcc
CXX = g++

SOURCE_C = foo.c
OBJECTS_C = foo.o

SOURCE_CXX = main.cpp

TARGET = main 
LDFLAGS_COMMON = -std=c++2a

all:
	$(C) -c $(SOURCE_C)
	$(CXX) $(SOURCE_CXX) $(OBJECTS_C) $(LDFLAGS_COMMON) -o $(TARGET)

clean:
	rm -rf *.o $(TARGET)
```







