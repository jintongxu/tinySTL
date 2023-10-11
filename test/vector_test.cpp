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

    mystl::vector<int> myVector = {1, 2, 3, 4};

    // for (int i = 0; i < 4; i ++) {
    //     std::cout << myVector[i] << std::endl;
    // }
    std::cout << *(myVector.rend() - 1)<< std::endl;



}


int main() {

    /* 系统的 vector 测试 */
    // sysVector();        
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
