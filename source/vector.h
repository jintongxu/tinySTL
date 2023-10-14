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
#include "exceptdef.h"


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

    // assign
    void assign(size_type n, const value_type& value)
    { fill_assign(n, value); }

    template <class Iter, typename std::enable_if<
            mystl::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last)
    {
        MYSTL_DEBUG(!(last < first));
        copy_assign(first, last, iterator_category(first));
    }

    void assign(std::initializer_list<value_type> il)
    { copy_assign(il.begin(), il.end(), mystl::forward_iterator_tag{}); }

    // emplace / emplace_back

    template <class... Args>
    iterator emplace(const_iterator  pos, Args&& ...args);

    template <class... Args>
    void emplace_back(Args&& ...args);

    // push_back / pop_back
    void push_back(const value_type& value);
    void push_back(value_type&& value)
    { emplace_back(mystl::move(value)); }

    void pop_back();


    // insert
    iterator insert(const_iterator pos, const value_type& value);
    iterator insert(const_iterator pos, value_type&& value)
    { return emplace(pos, mystl::move(value)); }

    iterator insert(const_iterator pos, size_type n, const value_type& value)
    {
        MYSTL_DEBUG(pos >= begin() && pos <= end());
        return fill_insert(const_cast<iterator>(pos), n, value);
    }

    template <class Iter, typename std::enable_if<
            mystl::is_input_iterator<Iter>::value, int>::type = 0>
    void     insert(const_iterator pos, Iter first, Iter last)
    {
        MYSTL_DEBUG(pos >= begin() && pos <= end() && !(last < first));
        copy_insert(const_cast<iterator>(pos), first, last);
    }

    // erase / clear
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void clear() { erase(begin(), end()); }

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

// calculate the growth size
size_type get_new_cap(size_type add_size);


// assign
void fill_assign(size_type n, const value_type& value);

template <class IIter>
void copy_assign(IIter first, IIter last, input_iterator_tag);

template <class FIter>
void copy_assign(FIter fist, FIter last, forward_iterator_tag);

/* reallocate */
template <class... Args>
void reallocate_emplace(iterator pos, Args&& ...args);
void reallocate_insert(iterator pos, const value_type& value);

// insert
iterator fill_insert(iterator pos, size_type n, const value_type& value);
template <class IIter>
void copy_insert(iterator pos, IIter first, IIter last);

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


// 在 pos 位置就地构造元素，避免额外的复制或移动开销
template <class T>
template <class ...Args>
typename vector<T>::iterator
vector<T>::emplace(const_iterator pos, Args&& ...args)
{
    MYSTL_DEBUG(pos >= begin() && pos <= end());
    iterator xpos = const_cast<iterator>(pos);
    const size_type n = xpos - begin_;
    if (end_ != cap_ && xpos == end_)
    {
        data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
        ++end_;
    }
    else if (end_ != cap_)
    {
        auto new_end = end_;
        data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
        ++new_end;
        mystl::copy_backward(xpos, end_ - 1, end_);
        *xpos = value_type(mystl::forward<Args>(args)...);
        end_ = new_end;
    }
    else
    {
        reallocate_emplace(xpos, mystl::forward<Args>(args)...);
    }
    return begin() + n;
}

// 在尾部就地构造元素，避免额外的复制或制动开销
template <class T>
template <class ...Args>
void vector<T>::emplace_back(Args &&...args)
{
    if (end_ < cap_) {
        // 直接在 end_ 创建元素
        data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
        ++end_;
    } else {
        // 重新分配空间，并在 end_ 创建元素
        reallocate_emplace(end_, mystl::forward<Args>(args)...);
    }
}

// 在尾部插入元素
template <class T>
void vector<T>::push_back(const value_type& value)
{
    if (end_ != cap_)
    {
        // 如果没超过 cap，就直接在 end_处构建元素，然后++end_;
        data_allocator::construct(mystl::address_of(*end_), value);
        ++end_;
    } else {
        // 如果超过了cap，就重新分配内存，然后在构建元素
        reallocate_insert(end_, value);
    }
}

// 弹出尾部元素
template <class T>
void vector<T>::pop_back()
{
    MYSTL_DEBUG(!empty());
    // 直接销毁末尾元素，然后移动 end_
    data_allocator::destroy(end_ - 1);
    --end_;
}

// 在 pos 处插入元素
template <class T>
typename vector<T>::iterator
vector<T>::insert(const_iterator pos, const value_type& value)
{
    MYSTL_DEBUG(pos >= begin() && pos <= end());
    iterator xpos = const_cast<iterator>(pos);
    const size_type n = pos - begin_;
    if (end_ != cap_ && xpos == end_)
    {
        // 如果插入位置正好是 end_，直接在end_地址处新建元素
        data_allocator::construct(mystl::address_of(*end_), value);
        ++end_;
    } else if (end_ != cap_) {
        auto new_end = end_;
        data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
        ++new_end;
        auto value_copy = value;    // 避免元素因以下复制操作而改变
        mystl::copy_backward(xpos, end_ - 1, end_);
        *xpos = mystl::move(value_copy);
        end_ = new_end;
    } else {
        reallocate_insert(xpos, value);
    }
    return begin_ + n;
}


// 删除 pos 位置上的元素
template <class T>
typename vector<T>::iterator
vector<T>::erase(const_iterator pos)
{
    MYSTL_DEBUG(pos >= begin() && pos <= end());
    iterator xpos = begin_ + (pos - begin());
    mystl::move(xpos + 1, end_, xpos);
    --end_;
    return xpos;
}

// 删除 [first, last)上的元素
template <class T>
typename vector<T>::iterator
vector<T>::erase(const_iterator first, const_iterator last)
{
    MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
    const auto n = first - begin();
    iterator r = begin_ + (first - begin());
    data_allocator::destroy(mystl::move(r + (last - first), end_, r), end_);
    end_ = end_ - (last - first);   // 更新 end_ 的位置
    return begin_ + n;
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

// get_new_cap 函数
// 计算容器拓展时需要的新容量
template <class T>
typename vector<T>::size_type 
vector<T>::
get_new_cap(size_type add_size)
{
  const auto old_size = capacity();
  THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size,
                        "vector<T>'s size too big");
  if (old_size > max_size() - old_size / 2)
  {
    return old_size + add_size > max_size() - 16
      ? old_size + add_size : old_size + add_size + 16;
  }
  const size_type new_size = old_size == 0
    ? mystl::max(add_size, static_cast<size_type>(16))
    : mystl::max(old_size + old_size / 2, old_size + add_size);
  return new_size;
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

// fill_assign 函数
template <class T>
void vector<T>::
fill_assign(size_type n, const value_type& value)
{
    if (n > capacity()) {
        vector tmp(n, value);  // 创建一个临时变量
        swap(tmp);     // 然后交换3个指针
    } else if (n > size()) {
        mystl::fill(begin(), end(), value);
        end_ = mystl::uninitialized_fill_n(end_, n - size(), value);  // 从 第一个参数开始，填充第二个参数个元素，返回填充结束的位置
    }
}

// copy_assign 函数
template <class T>
template <class IIter>
void vector<T>::
copy_assign(IIter first, IIter last, input_iterator_tag)
{
    auto cur = begin_;
    for (; first != last && cur != end_; ++first, ++cur)
    {
        *cur = *first;
    }
    if (first == last)
    {
        // 如果没到 end_ 就赋值结束了，就删除 cur 到 end_的元素
        erase(cur, end_);
    } else {
        // 如果扫到了end_，还有其他元素没赋值呢
       insert(end_, first, last);
    }

}

// 用 [first, last) 为容器赋值
template <class T>
template <class FIter>
void vector<T>::
copy_assign(FIter first, FIter last, forward_iterator_tag)
{
    const size_type len = mystl::distance(first, last); // 计算两个迭代器之间的距离
    if (len > capacity())
    {
        vector tmp(first, last);
        swap(tmp);
    } else if (size() >= len) {
        auto new_end = mystl::copy(first, last, begin_);
        data_allocator::destroy(new_end, end_);
        end_ = new_end;
    } else {
        auto mid = first;
        mystl::advance(mid, size());
        mystl::copy(first, mid, begin_);
        auto new_end = mystl::unchecked_copy(mid, last, end_);
        end_ = new_end;
    }
}

// 重新分配空间并在 pos 处就地构造元素
template <class T>
template <class ...Args>
void vector<T>::
reallocate_emplace(iterator pos, Args&& ...args)
{
    const auto new_size = get_new_cap(1);   // 新的 cap
    auto new_begin = data_allocator::allocate(new_size);    // 根据新的 cap 分配空间并返回起始气质
    auto new_end = new_begin;
    try
    {
        // 将 [begin_, pos) 的值从new_begin开始赋值，返回末尾元素的后一个地址
        new_end = mystl::uninitialized_move(begin_, pos, new_begin);

        data_allocator::construct(mystl::address_of(*new_end), mystl::forward<Args>(args)...); // 在末尾构造元素
        ++new_end;

        // 将 [pos, end_) 的元素从 new_end 开始赋值，并返回最后一个元素后面一个位置的地址。
        new_end = mystl::uninitialized_move(pos, end_, new_end);
    }
    catch (...)
    {
        data_allocator::deallocate(new_begin, new_size);
        throw;
    }
    // 销毁原来数组的元素，并且释放之前的内存
    destroy_and_recover(begin_, end_, cap_ - begin_);

    // 将指针指向新构造的区域
    begin_ = new_begin;
    end_ = new_end;
    cap_ = new_begin + new_size;
}



// 重新分配空间并在 pos 处插入元素
template <class T>
void vector<T>::reallocate_insert(iterator pos, const value_type& value)
{
    const auto new_size = get_new_cap(1);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    const value_type& value_copy = value;
    try
    {
        new_end = mystl::uninitialized_move(begin_, pos, new_begin);
        data_allocator::construct(mystl::address_of(*new_end), value_copy);
        ++new_end;
        new_end = mystl::uninitialized_move(pos, end_, new_end);
    }
    catch(...)
    {
        data_allocator::deallocate(new_begin, new_size);
        throw;
    }
    destroy_and_recover(begin_, end_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = new_begin + new_size;
    
}

/* fill_insert 函数 */
// 在指定位置插入 n 个相同的元素
template <class T>
typename vector<T>::iterator
vector<T>::
fill_insert(iterator pos, size_type n, const value_type& value)
{
    if (n == 0)
        return pos;
    const size_type xpos = pos - begin_;
    const value_type value_copy = value;    // 避免被覆盖
    if (static_cast<size_type>(cap_ - end_) >= n)
    {
        // 如果备用空间大于等于增加的空间
        const size_type after_elems = end_ - pos;
        auto old_end = end_;
        if (after_elems > n)
        {
            // 如果 pos 到 end_ 的个数超过要增加的元素个数

            // 将 pos 到 end_ 的元素向后移动n位，给新插入元素腾出位置
            mystl::uninitialized_copy(end_ - n, end_, end_);

            end_ += n; // 更新 end_ 位置
            mystl::uninitialized_fill_n(pos, n, value_copy);    // 将元素插入中间腾出的位置
        } else {
            end_ = mystl::uninitialized_fill_n(end_, n - after_elems, value_copy);
            end_ = mystl::uninitialized_move(pos, old_end, end_);
            mystl::uninitialized_fill_n(pos, after_elems, value_copy);
        }
    } else {
        // 如果备用空间不足
        const auto new_size = get_new_cap(n);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try
        {
            new_end = mystl::uninitialized_move(begin_, pos, new_begin);
            new_end = mystl::uninitialized_fill_n(new_end, n, value);
            new_end = mystl::uninitialized_move(pos, end_, new_end);
        }
        catch (...)
        {
            destroy_and_recover(new_begin, new_end, new_size);
            throw;
        }
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = begin_ + new_size;
    }
    return begin_ + xpos;
}

// copy_insert 函数
template <class T>
template <class IIter>
void vector<T>::
copy_insert(iterator pos, IIter first, IIter last)
{
    if (first == last)
        return;
    const auto n = mystl::distance(first, last);
    if ((cap_ - end_) >= n)
    {
        // 如果备用空间大小足够
        const auto after_elems = end_ - pos;
        auto old_end = end_;
        if (after_elems > n)
        {
//            把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
            end_ = mystl::uninitialized_copy(end_ - n, end_, end_);
            mystl::move_backward(pos, old_end - n, old_end);
            mystl::uninitialized_copy(first, last, pos);
        } else {
            auto mid = first;
            mystl::advance(mid, after_elems);
            end_ = mystl::uninitialized_copy(mid, last, end_);
            end_ = mystl::uninitialized_move(pos, old_end, end_);
            mystl::uninitialized_copy(first, mid, pos);
        }
    } else
    {
        // 备用空间不足
        const auto new_size = get_new_cap(n);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try
        {
            new_end = mystl::uninitialized_move(begin_, pos, new_begin);
            new_end = mystl::uninitialized_copy(first, last, new_end);
            new_end = mystl::uninitialized_move(pos, end_, new_end);
        }
        catch (...)
        {
            destroy_and_recover(new_begin, new_end, new_size);
            throw;
        }
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = begin_ + new_size;
    }
}


};






#endif