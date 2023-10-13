#ifndef MYTINYSTL_ALGOBASE_H_
#define MYTINYSTL_ALGOBASE_H_

// 这个头文件包含了 mystl 的基本算法

#include <cstring>
#include "iterator"

namespace mystl
{

/*****************************************************************************************/
// max
// 取二者中的较大值，语义相等时保证返回第一个参数
/*****************************************************************************************/
template <class T>
const T& max(const T& lhs, const T& rhs)
{
  return lhs < rhs ? rhs : lhs;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare comp)
{
  return comp(lhs, rhs) ? rhs : lhs;
}


/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
    {
        *first = value;
    }

    // 返回最后一个元素之后的第一个位置
    return first;
}

// 为 one-byte 类型提供特化版本
template <class Tp, class Size, class Up>
typename std::enable_if<
  std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
  !std::is_same<Tp, bool>::value &&
  std::is_integral<Up>::value && sizeof(Up) == 1,
  Tp*>::type
unchecked_fill_n(Tp* first, Size n, Up value)
{
  if (n > 0)
  {
    std::memset(first, (unsigned char)value, (size_t)(n));
  }
  return first + n;
}


template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    return unchecked_fill_n(first, n, value);
}


/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/
template <class ForwardIter, class T>
void fill_cat(ForwardIter first, ForwardIter last, const T& value,
              mystl::forward_iterator_tag)
{
    for (; first != last; ++first)
    {
        *first = value;
    }
}

template <class RandomIter, class T>
void fill_cat(RandomIter first, RandomIter last, const T& value,
              mystl::random_access_iterator_tag)
{
    // 如果是迭代器是 random_access_iterator_tag，那么可以直接推断出要填充多少个元素
    fill_n(first, last - first, value);
}

template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value)
{
    fill_cat(first, last, value, iterator_category(first));
}




/*****************************************************************************************/
// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/

// input_iterator_tag 版本
template <class InputIter, class OutputIter>
OutputIter
unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
                  mystl::input_iterator_tag)
{
  for (; first != last; ++first, ++result)
  {
    *result = *first;
  }
  return result;
}

// random_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter
unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result,
                  mystl::random_access_iterator_tag)
{
  for (auto n = last - first; n > 0; --n, ++first, ++result)
  {
    *result = * first;
  }
  return result;
}


template <class InputIter, class OutputIter>
OutputIter
unchecked_copy(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_copy_cat(first, last, result, iterator_category(first));
}



template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
  return unchecked_copy(first, last, result);
}


/*****************************************************************************************/
// copy_backward
// 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/*****************************************************************************************/
// unchecked_copy_backward_cat 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                            BidirectionalIter2 result, mystl::bidirectional_iterator_tag)
{
    while (first != last)
        *--result = *--last;
    return result;
}

// unchecked_copy_backward_cat 的 random_access_iterator_tag 版本
template <class RandomIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
                            BidirectionalIter2 result, mystl::random_access_iterator_tag)
{
    for (auto n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                        BidirectionalIter2 result)
{
    return unchecked_copy_backward_cat(first, last, result,
                                       iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_copy_assignable<Up>::value,
        Up*>::type
unchecked_copy_backward(Tp* first, Tp* last, Up* result)
{
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
    {
        result -= n;
        std::memmove(result, first, n * sizeof(Up));
    }
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return unchecked_copy_backward(first, last, result);
}

/*****************************************************************************************/
// move
// 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
/*****************************************************************************************/
// input_iterator_tag 版本
template <class InputIter, class OutputIter>
OutputIter
unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
                   mystl::input_iterator_tag)
{
    for (; first != last; ++first, ++result)
    {
        *result = mystl::move(*first);
    }
    return result;
}

// random_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter
unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
                   mystl::random_access_iterator_tag)
{
    for (auto n = last - first; n > 0; --n, ++first, ++result)
    {
        *result = mystl::move(*first);
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter
unchecked_move(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_move_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
unchecked_move(Tp* first, Tp* last, Up* result)
{
    const size_t n = static_cast<size_t>(last - first);
    if (n != 0)
        std::memmove(result, first, n * sizeof(Up));
    return result + n;
}



template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_move(first, last, result);
}



}
#endif