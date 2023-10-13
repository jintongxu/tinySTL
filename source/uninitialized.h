#ifndef MYTINYSTL_UNINITIALIZED_H_
#define MYTINYSTL_UNINITIALIZED_H_


#include <type_traits>

#include "algobase.h"
#include "memory.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace mystl
{

/*****************************************************************************************/
// uninitialized_copy
// 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置 迭代器 first 和 last
/*****************************************************************************************/

// 元素具有平凡的赋值操作 
// 就是类似 int float 这些基本类型的数据，系统直接赋值就行
template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
  return mystl::copy(first, last, result);
}

// 元素不具有平凡的赋值操作
// 需要调用类自己的构造函数
template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
  auto cur = result;
  try
  {
    for (; first != last; ++first, ++cur)
    {
      mystl::construct(&*cur, *first);
    }
  }
  catch (...)
  {
    for (; result != cur; --cur)
      mystl::destroy(&*cur);
  }
  return cur;
}


template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
  // 判断迭代器内的元素是具有平凡的赋值操作，然后调用相应的重载函数
  return mystl::unchecked_uninit_copy(first, last, result, 
                                     std::is_trivially_copy_assignable<
                                     typename iterator_traits<ForwardIter>::
                                     value_type>{});
}




/*****************************************************************************************/
// uninitialized_fill_n
// 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
/*****************************************************************************************/
// 如果类型具有非凡析构函数
template <class ForwardIter, class Size, class T>
ForwardIter
unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::true_type)
{
    return mystl::fill_n(first, n, value);
}

// 如果类型具有的是平凡析构函数
template <class ForwardIter, class Size, class T>
ForwardIter
unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::false_type)
{
    auto cur = first;
    try
    {
        mystl::construct(&*cur, value);
    }
    catch(...)
    {
        for (; first != cur; ++first)
            mystl::destroy(&*first);
    }
    return cur;
}


template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value)
{
    return mystl::unchecked_uninit_fill_n(first, n, value, 
                                        std::is_trivially_copy_assignable<
                                        typename iterator_traits<ForwardIter>::
                                        value_type>{});
}


/*****************************************************************************************/
// uninitialized_move
// 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
// 如果是普通元素类型
template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
  return mystl::move(first, last, result);
}

// 如果不是普通元素类型
template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
  ForwardIter cur = result;
  try
  {
    for (; first != last; ++first, ++cur)
    {
      mystl::construct(&*cur, mystl::move(*first));
    }
  }
  catch(const std::exception& e)
  {
    mystl::destroy(result, cur);
  }
  
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
{
  return mystl::unchecked_uninit_move(first, last, result,
                                    std::is_trivially_move_assignable<
                                    typename iterator_traits<InputIter>::
                                    value_type>{});
}



}
#endif