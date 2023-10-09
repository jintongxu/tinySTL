#ifndef MYTINYSTL_ALGOBASE_H_
#define MYTINYSTL_ALGOBASE_H_

// 这个头文件包含了 mystl 的基本算法

#include <cstring>
#include "iterator"

namespace mystl
{


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


}
#endif