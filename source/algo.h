//
// Created by simon on 2023-10-14.
//

#ifndef MYSTL_ALGO_H
#define MYSTL_ALGO_H

#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "memory.h"



/*****************************************************************************************/
// reverse
// 将[first, last)区间内的元素反转
/*****************************************************************************************/

namespace mystl
{


/*****************************************************************************************/
// reverse
// 将[first, last)区间内的元素反转
/*****************************************************************************************/

// reverse_dispatch 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter>
void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,
                      bidirectional_iterator_tag)
{
    while(true)
    {
        // last 会先减一
        if (first == last || first == --last) {
            return;
        }
        mystl::iter_swap(first++, last);
    }
}

// reverse_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter>
void reverse_dispatch(RandomIter first, RandomIter last,
                      random_access_iterator_tag)
{
    while (first < last)
        mystl::iter_swap(first++, --last);
}

template <class BidirectionalIter>
void reverse(BidirectionalIter first, BidirectionalIter last)
{
    mystl::reverse_dispatch(first, last, iterator_category(first));
}
};

#endif //MYSTL_ALGO_H
