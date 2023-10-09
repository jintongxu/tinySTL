#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_

// 这个头文件用于迭代器设计，包含了一些模板结构体与全局函数

#include <cstddef>

#include "type_traits.h"

namespace mystl
{

// 五种迭代器类型
// 可以看到后面三个是继承的前面的
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


// iterator 模板
template <class Category, class T, class Distance = ptrdiff_t,
    class Pointer = T*, class Reference = T&>
    struct iterator
{
    typedef Category                iterator_category;
    typedef T                       value_type;
    typedef Pointer                 pointer;
    typedef Reference               reference;
    typedef Distance                difference_type;
};


// iterator traits

// 判断 iterator 迭代器是否有 iterator_category 
template <class T>
struct has_iterator_cat
{
private:
    struct two { char a; char b; };
    template <class U> static two test(...);

    // typename U::iterator_category* 指向 U::iterator_category 的一个指针，默认值为0
    // 为什么要用指针呢？
    /*
        因为 U::iterator_category = 0是不对的，左边是类型不能赋值为0。
        如果类型 U 具有名为 iterator_category 的成员类型，那么这个指针就会成功匹配，因为编译器能够推导出 typename U::iterator_category* 的类型。
    */
    template <class U> static char test(typename U::iterator_category* = 0);  
public:
    // 如果类型有 iterator_category，value = true
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};


template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
    typedef typename Iterator::difference_type   difference_type;
};


template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true>
    : public iterator_traits_impl<Iterator,
    /*
    如果 Iterator::iterator_category 能转换为 input_iterator_tag 
    或者 Iterator::iterator_category 能转换为 output_iterator_tag
    就继承自 iterator_traits_impl<Iterator, true>
    因为 迭代器的5个属性中，有3个属于继承的，所以只要判断这两个就可以判断是否有 iterator_category 了、
    has_iterator_cat 判断是否有 iterator_category，这个判断 iterator_category 是不是这 5 个
  */
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>       // 如果 Iterator::iterator_category 能转化为 output_iterator_tag 返回一个true
{
};


// 萃取迭代器的特性
template <class Iterator>
struct iterator_traits
    : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};   // 根据迭代器是否有 iterator_category 选择不同函数


// 针对原生指针的偏特化版本
template <class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag             iterator_category;
    typedef T                                      value_type;
    typedef T*                                     pointer;
    typedef T&                                     reference;
    typedef ptrdiff_t                              difference_type;   
};

// 针对 const * 的偏特化
template <class T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag          iterator_category;
    typedef T                                   value_type;
    typedef const T*                            pointer;
    typedef const T&                            reference;
    typedef ptrdiff_t                           difference_type;                                  
};


/* 以下都是语法糖 不是标准库中的 */

// 萃取某种迭代器
template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
  : public m_bool_constant<std::is_convertible<
  typename iterator_traits<T>::iterator_category, U>::value>
{
};

template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

template <class Iter>
struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator :
    public m_bool_constant<is_input_iterator<Iterator>::value || 
        is_output_iterator<Iterator>::value>
{
};


// 萃取某个迭代器的 category
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}



/*  以下函数用于计算迭代器间的距离 */

// distance 的 input_iterator_tag 的版本
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
        ++first;
        ++n;
    }
    return n;
}


// distance 的 random_access_iterator_tag 的版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type
distance_dispatch(RandomIter first, RandomIter last, random_access_iterator_tag)
{
    return last - first;
}


template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    return distance_dispatch(first, last, iterator_category(first));
}


}
#endif