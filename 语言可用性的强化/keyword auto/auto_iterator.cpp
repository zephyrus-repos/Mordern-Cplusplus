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