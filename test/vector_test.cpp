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

    // 重载比较操作符测试
    mystl::vector<int> myVector1 = {1, 2, 3};
    mystl::vector<int> myVector2 = {1, 2, 3};
    mystl::vector<int> myVector3 = {4, 5, 6};

    /*
     *  myVector1和myVector2相等
     *  myVector2和myVector3不相等
     */
    if (myVector1 == myVector2) {
        std::cout << "myVector1和myVector2相等" << std::endl;
    } else {
        std::cout << "myVector1和myVector2不相等" << std::endl;
    }

    if (myVector2 == myVector3) {
        std::cout << "myVector2和myVector3相等" << std::endl;
    } else {
        std::cout << "myVector2和myVector3不相等" << std::endl;
    }


}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
