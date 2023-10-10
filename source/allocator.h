#ifndef TINYSTL_ALLOCATOR_H_
#define TINYSTL_ALLOCATOR_H_

#include <cstring>

#include "construct.h"
#include "util.h"


// 包含模板类 allocator，用于管理内存的分配、释放，对象的释放构造

namespace mystl {
    
// 模板类：allocator
// 模板函数代表数据类型
template <typename T>
class allocator {
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t    difference_type;
public:
    // 分配内存
    static T*   allocate();
    static T*   allocate(size_type n);

    // 清理内存
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);


    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
};

template <class T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}


// 分配内存，分配n个类型为 T 的内存，并强制转为 T*
// 返回一个指向该内存的指针 T*
template <class T>
T* allocator<T>::allocate(size_type n)
{   
    if (n == 0)
        return nullptr;
    // new 完返回一个void*类型，需要强制转化为 T*
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

// 释放内存
template <class T>
void allocator<T>::deallocate(T* ptr)
{
    if (ptr == nullptr)
        return;
    ::operator delete(ptr);
}

template <class T>
void allocator<T>::deallocate(T* ptr, size_type /*size*/)
{
    if (ptr == nullptr)
        return;
    ::operator delete(ptr);
}


// 销毁元素
template <class T>
void allocator<T>::destroy(T* ptr)
{
    mystl::destroy(ptr);
}

template <class T>
void allocator<T>::destroy(T* first, T* last)
{
    mystl::destroy(first, last);
}

}
#endif