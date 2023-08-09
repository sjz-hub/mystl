//
// Created by 孙建钊 on 2023/6/13.
//

#ifndef MYSTL_EXCEPTDEF_H
#define MYSTL_EXCEPTDEF_H

#include <stdexcept>
#include <cassert>

namespace mystl{

//定义常见的不满足条件的assert
#define MYSTL_DEBUG(expr) assert(expr)

//抛出长度错误
#define THROW_LENGTH_ERROR_IF(expr, what) if((expr)) throw std::length_error(what)

//抛出越界错误
#define THROW_OUT_OF_RANGE_IF(expr, what) if((expr)) throw std::out_of_range(what)

//抛出运行时错误
#define THROW_RUNTIME_ERROR_IF(expr, what) if((expr)) throw std::runtime_error(what)


}


#endif //MYSTL_EXCEPTDEF_H
