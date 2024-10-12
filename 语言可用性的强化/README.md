# 语言可用性的强化

当我们声明、定义一个变量或者常量，对代码进行流程控制、面向对象的功能、模板编程等这些都是运行时之前，可能发生在编写代码或编译器编译代码时的行为。为此，我们通常谈及**语言可用性**，是指那些发生在运行时之前的语言行为。

## 常量

### C++中的`nullptr`

本小节代码见文件夹`nullptr`。

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
 * nullptr_example.cpp
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

### constexpr常量表达式

本小节代码见文件夹`constexpr`。

C++ 本身已经具备了**常量表达式**的概念，比如 `1+2`, `3*4` 这种表达式总是会产生相同的结果并且没有任何副作用。如果编译器能够在编译时就把这些表达式直接优化并植入到程序运行时，将能*增加程序的性能*。一个非常明显的例子就是在数组的定义阶段：

```cpp
/**
 * array_definition.cpp
 */
#include <iostream>

#define LEN 10

int len_foo() {
    int i = 2;
    return i;
}

constexpr int len_foo_constexpr() {
    return 5;
}

constexpr int fibonacci(const int n) {
    return n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    char arr_1[10];  // legal
    char arr_2[LEN]; // legal

    int len = 10;
    char arr_3[len]; // illegal

    const int len_2 = len + 1;
    constexpr int len_2_constexpr = 1 + 2 + 3;

    char arr_4_[len_2];          // illegal
    char arr_4[len_2_constexpr]; // legal

    char arr_5[len_foo() + 5];           // illegal
    char arr_6[len_foo_constexpr() + 1]; // legal

    std::cout << fibonacci(10) << std::endl;

    return 0;
}
```

上面的例子中，`char arr_4_[len_2]` 可能比较令人困惑，因为 `len_2` 已经被定义为了常量。为什么`char arr_4_[len_2]` 仍然是非法的呢？这是因为 C++ 标准中数组的长度必须是一个常量表达式，而对于 `len_2` 而言，这是一个 `const` 常数，而不是一个常量表达式，因此（即便**这种行为在大部分编译器中都支持**，但是）它是一个非法的行为，我们需要使用接下来即将介绍的 C++11 引入的 `constexpr` 特性来解决这个问题；而对于 `arr_5` 来说，C++98 之前的编译器无法得知 len_foo() 在运行期实际上是返回一个常数，这也就导致了非法的产生。

> 注意，现在大部分编译器其实都带有自身编译优化，很多非法行为在编译器优化的加持下会变得合法，若需重现编译报错的现象需要使用老版本的编译器。



C++11 提供了 `constexpr` 让用户**显式的声明函数或对象构造函数在编译期会成为常量表达式**，这个关键字明确的告诉编译器应该去验证 `len_foo` 在编译期就应该是一个常量表达式。

此外，`constexpr` 修饰的函数可以使用递归：

```cpp
constexpr int fibonacci(const int n) {
  return n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2);
}
```

从 C++14 开始，constexpr 函数可以在内部使用局部变量、循环和分支等简单语句，例如下面的代码在 C++11 的标准下是不能够通过编译的：

```cpp
constexpr int fibonacci(const int n) {
	if(n == 1) return 1;
	if(n == 2) return 1;
	return fibonacci(n-1) + fibonacci(n-2);
}
```



## 变量及其初始化



### `if`和`switch`条件分支中的变量声明强化

本小节代码见文件夹`if-switch`。

在传统 C++ 中，变量的声明虽然能够位于任何位置，甚至于 for 语句内能够声明一个临时变量`int`，但始终没有办法在 `if` 和 `switch` 语句中声明一个临时的变量。例如：

```cpp
/**
 * if_before_c++17.cpp
 */
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4};

    // before C++17

    // define variable itr1
    const std::vector<int>::iterator itr1 = std::find(vec.begin(), vec.end(), 2);
    if (itr1 != vec.end()) {
        *itr1 = 3;
    }

    // define variable itr2
    const std::vector<int>::iterator itr2 = std::find(vec.begin(), vec.end(), 3);
    if (itr2 != vec.end()) {
        *itr2 = 4;
    }

    // output vec. your can define temp variables in the for loop
    for (std::vector<int>::iterator element = vec.begin(); element != vec.end(); ++element) {
        std::cout << *element << std::endl;
    }

    return 0;
}
```

在上面的代码中，我们可以看到 `itr1` 这一变量是定义在整个 `main()` 的作用域内的，这导致当我们需要再次遍历整个 `std::vector` 时，需要重新命名另一个变量。C++17 消除了这一限制，使得我们可以在 `if`（或 `switch`）中完成这一操作，例如`if_after_c++17.cpp`：

```cpp
/**
 *if_after_c++17.cpp
 */
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4};

    // after C++17

    // define variable itr1
    // const std::vector<int>::iterator itr1 = std::find(vec.begin(), vec.end(), 2);
    if (const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 2);
        itr != vec.end()) {
        *itr = 3;
    }

    // define variable itr2
    if (const std::vector<int>::iterator itr = std::find(vec.begin(), vec.end(), 3);
        itr != vec.end()) {
        *itr = 4;
    }

    // output vec. your can define temp variables in the for loop
    for (std::vector<int>::iterator element = vec.begin(); element != vec.end(); ++element) {
        std::cout << *element << std::endl;
    }

    return 0;
}
```



### 初始化列表

本小节代码见文件夹`initialization`。

初始化是一个非常重要的语言特性，最常见的就是在对象进行初始化时进行使用。在传统 C++ 中，不同的对象有着不同的初始化方法，例如普通数组、POD （**P**lain **O**ld **D**ata，即没有构造、析构和虚函数的类或结构体）类型都可以使用 `{}` 进行初始化，也就是我们所说的**初始化列表**。而对于类对象的初始化，要么需要通过拷贝构造、要么就需要使用 `()` 进行。这些不同方法都针对各自对象，不能通用。例如：

```cpp
/**
 *initialization_classic.cpp
 */
#include <iostream>
#include <vector>

class Foo {
  public:
    int value_a;
    int value_b;
    Foo(int a, int b) :
        value_a(a), value_b(b) {
    }
};

int main() {
    // before C++11
    int arr[3] = {1, 2, 3};
    Foo foo(1, 2);
    std::vector<int> vec = {1, 2, 3, 4, 5};

    std::cout << "arr[0]: " << arr[0] << std::endl;
    std::cout << "foo:" << foo.value_a << ", " << foo.value_b << std::endl;
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << std::endl;
    }

    return 0;
}
```

为解决这个问题，C++11 首先把初始化列表的概念绑定到类型上，称其为 `std::initializer_list`，允许构造函数或其他函数像参数一样使用初始化列表，这就为类对象的初始化与普通数组和 POD 的初始化方法提供了统一的桥梁，

#### 在对象构造中使用初始化列表

示例如下：

```cpp
/**
 *class_initialization_mordern.cpp
 */
#include <iostream>
#include <vector>
#include <initializer_list>

class MagicFoo {
  public:
    std::vector<int> vec;
    MagicFoo(std::initializer_list<int> list) {
        for (std::initializer_list<int>::iterator it = list.begin();
             it != list.end();
             ++it) {
            vec.push_back(*it);
        }
    }
};

int main() {
    // after C++11
    MagicFoo magicFoo = {1, 2, 3, 4, 5};

    std::cout << "magicFoo: \n";
    for (std::vector<int>::iterator it = magicFoo.vec.begin();
         it != magicFoo.vec.end();
         ++it)
        std::cout << *it << std::endl;
}
```

上面这种构造函数被叫做初始化列表构造函数，具有这种构造函数的类型将在初始化时被特殊关照。

#### 在普通函数中使用初始化列表

初始化列表除了用在对象构造上，还能将其作为普通函数（包括类中的成员函数）的形参，例如`function_initialization_mordern.cpp`：

```cpp
/**
 *function_initialization_mordern.cpp
 */
#include <iostream>
#include <vector>
#include <initializer_list>

std::vector<int> foo(std::initializer_list<int> list) {
    std::vector<int> ret;
    for (std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it) {
        ret.push_back(*it);
    }
    return ret;
}

int main() {
    std::vector<int> test_value = foo({6, 7, 8, 9});

    for (auto element : test_value) {
        std::cout << element << std::endl;
    }

    return 0;
}
```

### 结构化绑定

本小节代码见文件夹`tuple`。

结构化绑定提供了类似其他语言中提供的多返回值的功能。在容器一章中，我们会学到 C++11 新增了 `std::tuple` 容器用于构造一个元组，进而囊括多个返回值。但缺陷是，C++11/14 并没有提供一种简单的方法直接从元组中拿到并定义元组中的元素，尽管我们可以使用 `std::tie` 对元组进行拆包，但我们依然必须非常清楚这个元组包含多少个对象，各个对象是什么类型，非常麻烦。

C++17 完善了这一设定，给出的结构化绑定可以从元组、结构体、数组等数据结构中提取多个变量。示例代码如下：

```cpp
/**
 *example_tuple.cpp
 */
#include <iostream>
#include <tuple>

std::tuple<int, double, std::string> f() {
    return std::make_tuple(1, 2.3, "456");
}

int main() {
    auto [x, y, z] = f();
    std::cout << x << ", " << y << ", " << z << std::endl;

    return 0;
}
```



## 类型推导

在传统 C 和 C++ 中，参数的类型都必须明确定义，这其实对我们快速进行编码没有任何帮助，尤其是当我们面对一大堆复杂的模板类型时，必须明确的指出变量的类型才能进行后续的编码，这不仅拖慢我们的开发效率，也让代码变得又臭又长。

C++11 引入了 `auto` 和 `decltype` 这两个关键字实现了类型推导，让编译器来操心变量的类型。这使得 C++ 也具有了和其他现代编程语言一样，某种意义上提供了无需操心变量类型的使用习惯。

### `auto`关键字

本小节代码见文件夹`keyword auto`。

`auto` 在很早以前就已经进入了 C++，但是他始终作为一个存储类型的指示符存在，与 `register` 并存。在传统 C++ 中，如果一个变量没有声明为 `register` 变量，将自动被视为一个 `auto` 变量。而随着 `register` 被弃用（在 C++17 中作为保留关键字，以后使用，目前不具备实际意义），对 `auto` 的语义变更也就非常自然了。

`auto`的常见用法包括但不限于：

```cpp
auto i = 5;	// i 被推导为 int
auto arr = new auto(10);	// arr被推导为 int *
......
```

从C++11开始，可以使用`auto`推导迭代器类型。示例如下：

```cpp
/**
 *auto-iterator.cpp
 */
#include <iostream>
#include <vector>
#include <initializer_list>

class MagicFoo {
  public:
    std::vector<int> vec;
    MagicFoo(std::initializer_list<int> list) {
        // 从C++11开始，使用auto关键字进行类型推导
        for (auto it = list.begin(); it != list.end(); ++it) {
            vec.push_back(*it);
        }
    }
};

int main() {
    MagicFoo magicFoo = {1, 2, 3, 4, 5};
    std::cout << "magicFoo: \n";
    for (auto element : magicFoo.vec) {
        std::cout << element << std::endl;
    }

    return 0;
}
```

从 C++ 14 起，`auto` 能用于 `lambda` 表达式中的函数传参，而 C++ 20 起该功能推广到了一般的函数。示例如下：

```cpp
/**
 * auto_functions.cpp
 */
#include <iostream>

// introduced in C++14
auto add14 = [](auto x, auto y) -> int {
    return x + y;
};

// introduced in C++20
int add20(auto x, auto y) {
    return x + y;
}

int main() {
    auto i = 5;
    auto j = 5;

    std::cout << add14(i, j) << std::endl;
    std::cout << add20(i, j) << std::endl;

    return 0;
}
```



### `decltype`关键字

本小节代码见文件夹。



### 尾返回类型推导

本小节代码见文件夹。

### `decltype(auto)`

本小节代码见文件夹。

## 控制流



## 模板



## 面向对象

