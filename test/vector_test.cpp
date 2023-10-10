#include <iostream>
#include "vector.h"
#include <vector>
#include <iterator>

// 真实 vector 测试
void sysVector() {
    std::vector<int> v(2);

    // std::cout << i << "  " << v[i] << std::endl;
    // std::cout << v[0] << std::endl;
    // std::cout << v.size() << std::endl;
    std::cout << "可用空间为：" << v.capacity() << std::endl;
    // for (int i = 0; i < 10; i ++ )
    // {
    //     std::cout << v[i] << std::endl;
    // }
}

// 自己的 vector 测试
void myselfVector() {
    int myArray[] = {1, 2, 3, 4, 5, 6};
    int* begin = myArray;
    int* end = myArray + 6;
    mystl::vector<int> myVector(begin, end);

//    for (int i = 0; i < 5; i++) {
//        std::cout << myVector[i] << std::endl;
//    }
    mystl::vector<int> test2;

    test2 = std::move(myVector);

    for (int i = 0; i < 6; i ++) {
        std::cout << test2[i] << std::endl;
    }


}


int main() {

    /* 系统的 vector 测试 */
    // sysVector();        
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
