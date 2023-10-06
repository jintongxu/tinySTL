#include <iostream>
#include "vector.h"
#include <vector>


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
    mystl::vector<int> v(10);

    for (int i = 0; i < 20; i++) { std::cout << i << "  " << v[i] << std::endl;}
}

int main() {

    /* 系统的 vector 测试 */
    // sysVector();        

    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}