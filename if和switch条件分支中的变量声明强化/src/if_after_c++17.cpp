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