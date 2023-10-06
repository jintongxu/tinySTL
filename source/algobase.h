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





}
#endif