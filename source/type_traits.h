#ifndef MYTINYSTL_TYPE_TRAITS_H_
#define MYTINYSTL_TYPE_TRAITS_H_


// 萃取器
// 这个头文件用于提取类型信息

#include <type_traits>

namespace mystl
{

// helper struct
template <class T, T v>
struct m_integral_constant
{
    static constexpr T value = v;
};


template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;

typedef m_bool_constant<true> m_true_type;
typedef m_bool_constant<false> m_false_type;


}
#endif