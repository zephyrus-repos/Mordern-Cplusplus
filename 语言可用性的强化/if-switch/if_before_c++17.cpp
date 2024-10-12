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