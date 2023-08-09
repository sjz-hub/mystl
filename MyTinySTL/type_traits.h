//
// Created by 孙建钊 on 2023/6/13.
//

//这个头文件用于提取类型信息
#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

//引入了标准库头文件
#include <type_traits>

namespace mystl{

    //传递一个类型，以及类型值，使得对应的value为这个类型值
    template <class T, T v>
    struct m_integral_constant{
        static constexpr T value = v;
    };

    template <bool b>
    using m_bool_constant = m_integral_constant<bool,b>;

    //创建了两个结构体，其对应的value分别为true和false
    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;

    template <class T1, class T2>
    struct pair;
// --- forward declaration end

    template <class T>
    struct is_pair : mystl::m_false_type {};

    template <class T1, class T2>
    struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type {};
//    template <class T1, class T2>
//    struct is_pair<std::pair<T1, T2>> : mystl::m_true_type {};



}


#endif //MYSTL_TYPE_TRAITS_H
