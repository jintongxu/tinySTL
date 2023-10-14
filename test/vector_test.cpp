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

    /* 可以看到 resize 后并不会影响capacity的大小。通过shrink_to_fit调整后 cap 就和 size一样大了。
     shrink_to_fit前：
        size大小：100000
        capacity大小：100000

    resize后：
        size大小：1
        capacity大小：100000

    shrink_to_fit后：
        size大小：1
        capacity大小：1
     */
    mystl::vector<int> myVector(100000);
    for (int i = 0; i < 100000; i++) myVector[i] = i;

    std::cout << "shrink_to_fit前：";
    std::cout << "size大小：" << myVector.size() << "\n";
    std::cout << "capacity大小：" << myVector.capacity() << "\n";

    std::cout << "\n";

    myVector.resize(1);
    std::cout << "resize后：";
    std::cout << "size大小：" << myVector.size() << "\n";
    std::cout << "capacity大小：" << myVector.capacity() << "\n";

    std::cout << "\n";

    myVector.shrink_to_fit();
    std::cout << "shrink_to_fit后：";
    std::cout << "size大小：" << myVector.size() << "\n";
    std::cout << "capacity大小：" << myVector.capacity();







}


int main() {

    /* 系统的 vector 测试 */
//     sysVector();
    /* 自己的 vector 测试 */
    myselfVector();

    return 0;
}
