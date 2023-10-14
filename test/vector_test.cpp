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

    mystl::vector<int> stdVector;

    // 使用 insert 函数在 myVector 的第三个位置插入 stdVector 的元素
    myVector.insert(myVector.begin(), stdVector.begin(), stdVector.end());

    // 打印 myVector
    for (const auto& val : myVector) {
        std::cout << val << " ";    // 1 2 3 4 5
    }

}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
