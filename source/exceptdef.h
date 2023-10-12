//
// Created by simon on 2023-10-12.
//
#ifndef MYTINYSTL_EXCEPTDEF_H_
#define MYTINYSTL_EXCEPTDEF_H_

#include <stdexcept>

#include <cassert>

namespace mystl
{
#define MYSTL_DEBUG(expr) \
    assert(expr)
}










#endif
