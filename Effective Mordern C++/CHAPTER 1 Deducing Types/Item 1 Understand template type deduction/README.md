# Item 1: Understand template type deduction

**模板类型推断**是C++模板编程中的一个重要特性，它允许编译器根据函数参数的实际类型来自动推断模板参数的类型。一个函数模板如下：

```cpp
template<typename T>
void f(ParamType param);
```

调用该函数：

```cpp
f(expr); // call f with some expression
```

在模板函数的调用过程中，会通过表达式`expr`来推导模板参数`T`和参数类型`ParamType`。`T`和`ParamType`一般是不一样的，因为`ParamType`会带有修饰符，例如`const`等。例如：

```cpp
template<typename T>
void f(const T& param); // ParamType is const T&

int x = 0;
f(x); 		// call f with an int
```

上述例子中，`T`被推导为`int`，`ParamType`被推导为`const T&`。

在模板编程中，我们可能会直觉地认为模板参数`T`的类型推断结果应该与传递给函数的参数`expr`的类型相同,这种直觉有时候是错的。`T`的类型推断不仅与`expr`的类型有关，也和`ParamType`的形式都有关系，共有三种不同的情况。

- **`ParamType` 是指针或引用类型，但不是通用引用**
- **`ParamType` 是通用引用**
- **`ParamType` 既不是指针也不是引用**

## `ParamType` 是指针或引用类型，但不是通用引用

当`ParamType`是一个引用类型或者指针类型，但不是一个通用引用时，类型推断遵循以下规则：

1. **忽略引用部分**：如果传递给模板函数的表达式`expr`是一个引用类型，比如 `int&` 或 `const int&`，那么在类型推断时，编译器会忽略这个引用，将其视为一个普通的 `int` 类型。这意味着，无论 `expr` 是一个左值引用还是一个右值引用，类型推断都不会将 `T` 推断为引用类型。
2. **模式匹配**：编译器会将忽略引用后的 `expr` 类型与 `ParamType` 进行比较，以确定模板参数 `T` 的类型。这个过程称为模式匹配，它确保 `ParamType` 中的任何修饰符（如 `const` 或 `volatile`）都能正确地应用到 `T` 上。

## `ParamType` 是通用引用



## `ParamType` 既不是指针也不是引用