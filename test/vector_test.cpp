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

    // 测试 v.assign(n, value)
    mystl::vector<int> myVector_assign1;

    int value = 11;
    myVector_assign1.assign(5, value);

    for (int i = 0; i < 5; i ++) {
        std::cout << myVector_assign1[i] << std::endl;
    }


    /* 测试 assign(std::initializer_list<value_type> il) */

    // myVector_target2.assign({1, 2, 3, 4, 5, 6, 7});
    mystl::vector<int> myVector_target2;

    std::cout << "assign前:" << std::endl;
    for (int* i = myVector_target2.begin(); i < myVector_target2.end(); i ++ ) std::cout << *i << " ";

    std::cout << "\n";

    std::cout << "assign后:" << std::endl;
    myVector_target2.assign({1, 2, 3, 4, 5, 6, 7});
    for (int* i = myVector_target2.begin(); i < myVector_target2.end(); i ++ ) std::cout << *i << " ";    // 1 2 3 4 5 6 7
    
    std::cout << "\n";








}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
