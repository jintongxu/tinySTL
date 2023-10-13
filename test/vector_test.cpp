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

    // emplace_back(Args &&...args)
    /*
     * 在末尾插入元素，push_back是创建一个临时变量然后再复制，emplace_back是直接在末尾创建元素
     * 原vector 1,2
     * 插入后 1 2 3
     */
    mystl::vector<int> myVector_emplace = {1, 2};

    int* pos = myVector_emplace.begin();
    myVector_emplace.emplace_back(3);
    for (int i = 0; i < 3; i ++) std::cout << myVector_emplace[i] << " \n"[i == 6];  // 1 2 3

}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
