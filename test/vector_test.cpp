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
     *   insert(const_iterator pos, Iter first, Iter last) 测试
     */
    mystl::vector<int> myVector = {1, 2, 3, 4, 5};

    std::cout << "原来size大小\n" << myVector.size() << std::endl;  // 5

    myVector.resize(10);

    std::cout << "resize之后大小\n" << myVector.size() << "\n";     // 10
    std::cout << "resize之后元素："; // resize之后元素：1 2 3 4 5 0 0 0 0 0
    for (const int& value : myVector) {
        std::cout << value << " ";
    }


}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
