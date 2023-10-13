#include <iostream>
#include "vector.h"
#include <vector>
#include <iterator>

// 真实 vector 测试
void sysVector() {
    std::vector<int> v;

    int value = 10;
    v.assign(5, value);
//    for (int i = 0; i < 5; i ++) {
//        std::cout << v[i] << std::endl;
//    }
    std::vector<int>::iterator begin;
    std::vector<int>::iterator  end;

    begin = v.begin();
    end = v.end();
    std::cout << end - begin << std::endl;
}

// 自己的 vector 测试
void myselfVector() {

    /*
     *   pop_back()
     */
    mystl::vector<int> myVector_emplace = {1, 2, 3, 4};

    myVector_emplace.pop_back();

    int* begin = myVector_emplace.begin();  // 迭代器，因为 vector 的迭代器是原生指针
    int* end = myVector_emplace.end();

    for (; begin != end; begin ++) std::cout << *begin << " ";  // 1 2 3

    std::cout << "\n";

}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
