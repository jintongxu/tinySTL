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





}
#endif