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
     *   void push_back(const value_type& value);
     *   void push_back(value_type&& value)
     */
    mystl::vector<int> myVector_emplace = {1, 2};   // 1，2

    int* pos = myVector_emplace.begin();

    int value = 10;
    myVector_emplace.push_back(value);  // 1，2，10
    myVector_emplace.push_back(3);  // 1，2，10，3
    for (int i = 0; i < 4; i ++) std::cout << myVector_emplace[i] << " \n"[i == 6];  // 1 2 10 3

}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
