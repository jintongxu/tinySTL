#ifndef MYTINYSTL_VECTOR_H_
#define MYTINYSTL_VECTOR_H_

// 这个头文件包含一个模板类 vector
// vector : 向量

// notes:
//
// 异常保证：
// mystl::vecotr<T> 满足基本异常保证，部分函数无异常保证，并对以下函数做强异常安全保证：
//   * emplace
//   * emplace_back
//   * push_back
// 当 std::is_nothrow_move_assignable<T>::value == true 时，以下函数也满足强异常保证：
//   * reserve
//   * resize
//   * insert

#include <initializer_list>

#include "allocator.h"
#include "memory.h"
#include "util.h"
#include "uninitialized.h"


namespace mystl
{

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

// 模板类: vector 
// 模板参数 T 代表类型
template <class T>
class vector
{
  static_assert(!std::is_same<bool, T>::value, "vector<bool> is abandoned in mystl");
public:
  // vector 的嵌套型别定义
  typedef mystl::allocator<T>                      allocator_type;
  typedef mystl::allocator<T>                      data_allocator;

  typedef typename allocator_type::value_type      value_type;
  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;

  typedef value_type*                              iterator;
  typedef const value_type*                        const_iterator;
//   typedef mystl::reverse_iterator<iterator>        reverse_iterator;
//   typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

  allocator_type get_allocator() { return data_allocator(); }

private:
  iterator begin_;  // 表示目前使用空间的头部
  iterator end_;    // 表示目前使用空间的尾部
  iterator cap_;    // 表示目前储存空间的尾部

public:
    // 构造、赋值、移动、析构函数
    // 默认构造函数 vector<T> v，不分配内存，直接初始化为0。
    vector() : begin_(0), end_(0), cap_(0) {}

    // explicit：不能隐式调用构造函数，例如 vector v = 10;
    explicit vector(size_type n)
    { fill_init(n, value_type()); }

    vector(size_type n, const value_type& value)
    { fill_init(n, value); }

    // template <class Iter, typename std::enable_if<
    //     mystl::is_input_iterator<Iter>::value, int>::type = 0>
    // vector(Iter first, Iter last)
    // {
    //     MYSTL_DEBUG(!(last < first));
    //     range_init(first, last);
    // }

    vector(const vector& rhs)
    {
        range_init(rhs.begin_, rhs.end_);
    }

    vector(vector&& rhs) noexcept
        :begin_(rhs.begin_),
        end_(rhs.end_),
        cap_(rhs.cap_)
    {
        rhs.begin_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
    }

    vector(std::initializer_list<value_type> ilist)
    {
        range_init(ilist.begin(), ilist.end());
    }

    vector& operator=(const vector& rhs);
    vector& operator=(vector&& rhs) noexcept;

    vector& operator=(std::initializer_list<value_type> ilist)
    {
        vector tmp(ilist.begin(), ilist.end());
        swap(tmp);
        return *this;
    }

    // ~vector()
    // {
    //     destroy_and_recover(begin_, end_, cap_ - begin_);
    //     begin_ = end_ = cap_ = nullptr;
    // }


public:

    // 迭代器相关操作



    // 访问元素相关操作
    reference operator[](size_type n) { return *(begin_ + n); };



private:
    // helper functions

    // initialize / destroy
    // void try_init() noexcept;

    void init_space(size_type size);

    void fill_init(size_type n, const value_type& value);
};


/*****************************************************************************************/
// helper function

// try_init 函数，若分配失败则忽略，不抛出异常
// template <class T>
// void vector<T>::try_init() noexcept
// {
//     try
//     {
//         begin_ = data_allocator::allocate(16);
//         end_ = begin_;
//         cap_ = begin_ + 16;
//     }
//     catch (...)
//     {
//         begin_ = nullptr;
//         end_ = nullptr;
//         cap_ = nullptr;
//     }
// }

// init_space 函数
template <class T>
void vector<T>::init_space(size_type size)
{
    try
    {
        begin_ = data_allocator::allocate(size);
        end_ = begin_ + size;
        cap_ = end_;
    }
    catch(...)
    {
        begin_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
        throw;
    }
    
}


// file_init函数
template <class T>
void vector<T>::
fill_init(size_type n, const value_type& value)
{
    // const size_type init_size = mystl::max(static_cast<size_type>(16), n);      // 最小分配16个元素空间
    init_space(n); 
    mystl::uninitialized_fill_n(begin_, n, value);
}





}
#endif