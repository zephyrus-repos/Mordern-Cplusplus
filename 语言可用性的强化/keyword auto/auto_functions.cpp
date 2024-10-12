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