# C++中的`nullptr`

`nullptr`出现的目的是为了替代 NULL。C 与 C++ 语言中有**空指针常量**，它们能被隐式转换成任何指针类型的空指针值，或 C++ 中的任何成员指针类型的空成员指针值。

在 C 中，`NULL` 由标准库实现提供，并被定义为实现定义的空指针常量。有些标准库会把 `NULL` 定义为 `((void*)0) `而有些将它定义为0。

C++ **不允许**直接将 `void *` 隐式转换到其他类型，从而 `((void*)0)` 不是 `NULL` 的合法实现。**如果**标准库尝试把 `NULL` 定义为 `((void*)0)`，那么下面这句代码中会出现编译错误：

```cpp
char *ch = NULL;
```

没有了 `void *` 隐式转换的 C++ 只好将 `NULL` 定义为 0。而这依然会产生新的问题，将 `NULL` 定义成 0 将导致 C++ 中重载特性发生混乱。考虑下面这两个 foo 函数：

```cpp
void foo(char *);
void foo(int);
```

那么 `foo(NULL);` 这个语句将会去调用 `foo(int)`，从而导致代码违反直觉。

为了解决这个问题，C++11 引入了 `nullptr` 关键字，专门用来区分空指针、0。而 nullptr 的类型为 `nullptr_t`，能够隐式的转换为任何指针或成员指针的类型，也能和他们进行相等或者不等的比较。示例代码如下：

```cpp
/**
 *nullptr_example.cpp
 */
#include <iostream>
#include <type_traits>

void foo(char *);
void foo(int);

int main() {
    if (std::is_same<decltype(NULL), decltype(0)>::value) {
        std::cout << "NULL == 0" << std::endl;
    }
    if (std::is_same<decltype(NULL), decltype((void *)0)>::value) {
        std::cout << "NULL == (void*)0" << std::endl;
    }
    if (std::is_same<decltype(NULL), std::nullptr_t>::value) {
        std::cout << "NULL == nullptr" << std::endl;
    }

    foo(0);
    foo(NULL);
    foo(nullptr);

    return 0;
}

void foo(char *) {
    std::cout << "foo(char*) is called" << std::endl;
}

void foo(int i) {
    std::cout << "foo(int) is called" << std::endl;
}
```

上述程序将输出：

```shell
NULL == 0
foo(int) is called
foo(int) is called
foo(char*) is called
```

从输出中我们可以看出，`NULL` 在我们电脑上的实现为 0，与 `nullptr`不同。所以，在C++编程中需要养成直接使用 `nullptr` 的习惯。

