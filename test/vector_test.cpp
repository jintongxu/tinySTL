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

    // emplace(const_iterator pos, Args&& ...args)
    /*
     * 在指定位置插入元素，其他元素顺移。
     * 原vector 1,2
     * 插入后 3 1 2
     */
    mystl::vector<int> myVector_emplace = {1, 2};

    int* pos = myVector_emplace.begin();
    myVector_emplace.emplace(pos, 3);
    for (int i = 0; i < 3; i ++) std::cout << myVector_emplace[i] << " \n"[i == 6];  // 3 1 2

}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
