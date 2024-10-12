# `if`和`switch`条件分支中的变量声明强化

在传统 C++ 中，变量的声明虽然能够位于任何位置，甚至于 for 语句内能够声明一个临时变量`int`，但始终没有办法在 `if` 和 `switch` 语句中声明一个临时的变量。例如`if_before_c++17.cpp`：

```cpp
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

