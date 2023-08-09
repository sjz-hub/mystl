//
// Created by 孙建钊 on 2023/6/13.
//

#ifndef MYSTL_ALGOBASE_H
#define MYSTL_ALGOBASE_H

// 这个头文件包含了 mystl 的基本算法

#include <cstring>

#include "type_traits.h"
#include "util.h"

namespace mystl{


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

}


#endif //MYSTL_ALGOBASE_H
