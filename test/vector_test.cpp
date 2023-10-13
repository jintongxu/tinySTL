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


    // 测试 v.assign(Iter first, Iter last)
    /*
     *  assign前:
        0 0 0 0 0 0 0 0
        assign后:
        1 2 3 4 5 6
     */
    // 可以看到如果赋值的元素个数，小于原来的元素个数，那么就删除后面多余的元素
    mystl::vector<int> myVector_assign2 = {1, 2, 3, 4, 5, 6};
    mystl::vector<int> myVector_target2;

    std::cout << "assign前:" << std::endl;
    for (int* i = myVector_target2.begin(); i < myVector_target2.end(); i ++ ) std::cout << *i << " ";   // 0 0 0 0 0 0 0 0

    std::cout << "\n";

    std::cout << "assign后:" << std::endl;
    myVector_target2.assign(myVector_assign2.begin(), myVector_assign2.end());
    for (int* i = myVector_target2.begin(); i < myVector_target2.end(); i ++ ) std::cout << *i << " ";    // 1 2 3 4 5 6
    
    std::cout << "\n";








}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
