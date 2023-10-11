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
  typedef mystl::reverse_iterator<iterator>        reverse_iterator;
  typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

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
    // vector<int> v(10)
    explicit vector(size_type n)
    { fill_init(n, value_type()); }

    // vector<int> v(10, 1)
    vector(size_type n, const value_type& value)
    { fill_init(n, value); }

    // 如果是输入迭代器才可以初始化，输出迭代器不能读取元素只能赋值，其他类型是继承的输入迭代器
    template <class Iter, typename std::enable_if<
    mystl::is_input_iterator<Iter>::value, int>::type = 0>
    vector(Iter first, Iter last)
    {
        // MYSTL_DEBUG(!(last < first));
        range_init(first, last);
    }

    // 传入一个vector的左值
    vector(const vector& rhs)
    {
        range_init(rhs.begin_, rhs.end_);
    }

    // 移动构造函数
    // 赋值后，同时将rhs设置一个为有效但不在拥有资源的状态，因为如果不释放资源
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

    vector& operator=(const vector& rhs);           // 拷贝赋值运算符
    vector& operator=(vector&& rhs) noexcept;       // 移动赋值运算符

    // 支持 initializer_list 类型赋值-----> mystl::vector<int> myVector = {1, 2, 3, 4};
    vector& operator=(std::initializer_list<value_type> ilist)
    {
        vector tmp(ilist.begin(), ilist.end()); // 根据initializer_list内的元素，初始化一个临时变量
        swap(tmp);  // 然后将临时变量交换给当前对象
        return *this;
    }

     ~vector()
     {
         destroy_and_recover(begin_, end_, cap_ - begin_);
         begin_ = end_ = cap_ = nullptr;
     }


public:

    // 迭代器相关操作
    iterator        begin()         noexcept
    { return begin_; }
    const_iterator  begin()         const noexcept
    { return begin_; }
    iterator        end()           noexcept
    { return end_; }
    const_iterator  end()           const noexcept
    { return end_; }

    reverse_iterator      rbegin()          noexcept
    { return reverse_iterator(end()); }
    const_reverse_iterator rbegin()      const noexcept
    { return const_reverse_iterator(end()); }
    reverse_iterator     rend()            noexcept
    { return reverse_iterator(begin()); }
    const_reverse_iterator rend()       const noexcept
    { return const_reverse_iterator(begin()); }
    
    const_iterator      cbegin()            const noexcept
    { return begin(); }
    const_iterator      cend()              const noexcept
    { return end(); }
    const_reverse_iterator  crbegin()       const noexcept
    { return rbegin(); }
    const_reverse_iterator  crend()         const noexcept
    { return rend(); }

    // 容器相关操作
    bool    empty()     const noexcept
    { return begin_ == end_; }
    size_type size()    const noexcept
    { return static_cast<size_type>(end_ - begin_); }
    size_type max_size()  const noexcept
    { return static_cast<size_type>(-1) / sizeof(T); }
    size_type capacity() const noexcept
    { return static_cast<size_type>(cap_ - begin_); }




    // 访问元素相关操作
    reference operator[](size_type n) { return *(begin_ + n); };

    const_reference operator[](size_type n) const
    {
        // 如果是类似 const mystl::vector<int> v 定义的只能调用这个方法，不能修改vector的内容。
        return *(begin_ + n);
    }
    reference at(size_type n)
    {
        // 普通定义的vector返回&，可以修改vector内容
        return (*this)[n];
    }
    const_reference at(size_type n) const 
    {
        // 如果是类似 const mystl::vector<int> v 定义的只能调用这个方法，不能修改vector的内容。
        return (*this)[n];
    }

    reference front()
    {
        // 普通定义的vector返回&，可以修改vector内容
        return *begin_;
    }
    const_reference front() const
    {
        // 如果是类似 const mystl::vector<int> v 定义的只能调用这个方法，不能修改vector的内容。
        return *begin_;
    }
    reference back()
    {
        // 普通定义的vector返回&，可以修改vector内容
        return *(end_ - 1);
    }
    const_reference back() const
    {
        // 如果是类似 const mystl::vector<int> v 定义的只能调用这个方法，不能修改vector的内容。
        return *(end_ - 1);
    }

    pointer     data()              noexcept  { return begin_; }
    const_pointer data()        const noexcept { return begin_; }

    /* 修改容器相关操作 */

    // swap
    void swap(vector& rhs) noexcept;

private:
// helper functions

// initialize / destroy
template <class Iter>
void range_init(Iter first, Iter last);

void destroy_and_recover(iterator first, iterator last, size_type n);   // 销毁元素并释放内存，这个 n 是用于释放内存的


// void try_init() noexcept;

void init_space(size_type size);

void fill_init(size_type n, const value_type& value);
};

/*****************************************************************************************/

// 复制赋值操作符
template <class T>
vector<T>& vector<T>::operator=(const vector& rhs)
{
    // 如果不是自赋值，即将 vector<int> v(v)，自己赋值给自己，避免不必要的资源浪费
    if (this != &rhs)
    {
        const auto len = rhs.size();
        if (len > capacity())
        {
            // 如果对象已经使用空间大小，比当前对象实际分配空间还大
            vector tmp(rhs.begin(), rhs.end());     // 利用迭代器创建一个临时对象。
            swap(tmp);  // 将临时变量交换给本变量，然后临时变量调用析构函数
        }
        else if (size() > len)
        {
            // 如果当前对象的大小能容纳需要赋值的数据
            // 直接 copy 到对象，然后返回当前对象的最后一个数据的下一个位置的指针 i
            auto i = mystl::copy(rhs.begin(), rhs.end(), begin());

            // 将多余的数据释放
            data_allocator::destroy(i, end_);
            end_ = begin_ + len;    // 因为后面那一段数据释放了，所以end_就变化了
        }
        else
        {
            // 如果 size() < len < capacity()
            // 先将 rhs 的 size()范围内数据赋值给当前对象
            mystl::copy(rhs.begin(), rhs.begin() + size(), begin_);
            // rhs + 当前对象的size 位置到结尾的数据分配内存构造
            mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end_);
            cap_ = end_ = begin_ + len;
        }
    }
    // 返回本对象
    return *this;
}

// 移动赋值操作符
template <class T>
vector<T>& vector<T>::operator=(vector<T>&& rhs) noexcept
{
    destroy_and_recover(begin_, end_, cap_ - begin_);   // 释放vector中所有的内存
    begin_ = rhs.begin_;
    end_ = rhs.end_;
    cap_ = rhs.cap_;
    rhs.begin_ = nullptr;
    rhs.end_ = nullptr;
    rhs.cap_ = nullptr;
    return *this;
}

// 与另一个 vector 交换
template <class T>
void vector<T>::swap(vector<T>& rhs) noexcept
{
    // 判断是否是自赋值情况，即将 vector<int> v(v)，自己赋值给自己，避免不必要的资源浪费
    // this 是指向当前对象的指针，判断两个地址是否相等来判断是不是同一个对象
    if (this != &rhs)
    {
        // 就是简单的交换
        mystl::swap(begin_, rhs.begin_);
        mystl::swap(end_, rhs.end_);
        mystl::swap(cap_, rhs.cap_);
    }
}


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

// range_init函数
template <class T>
template <class Iter>
void vector<T>::
range_init(Iter first, Iter last)
{
    const size_type len = mystl::distance(first, last); // 获取两个迭代器之间的距离
    init_space(len);
    mystl::uninitialized_copy(first, last, begin_);
}

// destroy_and_recover 函数
// 销毁元素并释放内存
template <class T>
void vector<T>::
destroy_and_recover(iterator first, iterator last, size_type n)
{
    data_allocator::destroy(first, last);
    data_allocator::deallocate(first, n);
}

// 重载 mystl 的 swap
template <class T>
void swap(vector<T>& lhs, vector<T>& rhs)
{
    lhs.swap(rhs);
}


};






#endif