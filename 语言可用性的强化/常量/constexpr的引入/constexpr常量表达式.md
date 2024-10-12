# constexpr常量表达式

C++ 本身已经具备了**常量表达式**的概念，比如 `1+2`, `3*4` 这种表达式总是会产生相同的结果并且没有任何副作用。如果编译器能够在编译时就把这些表达式直接优化并植入到程序运行时，将能*增加程序的性能*。一个非常明显的例子就是在数组的定义阶段：

```cpp
#include <iostream>

#define LEN 10

int len_foo() {
  int i = 2;
  return i;
}

constexpr int len_foo_constexpr() { return 5; }

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

## C++11中的`constexpr`

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

