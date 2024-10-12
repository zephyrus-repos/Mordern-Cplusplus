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