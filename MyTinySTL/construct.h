//
// Created by 孙建钊 on 2023/6/13.
//

#ifndef MYSTL_CONSTRUCT_H
#define MYSTL_CONSTRUCT_H

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include "type_traits.h"
#include "iterator.h"

namespace mystl{

    //construct构造对象，在已经创建好的内存上创建对象，不需要申请空间了
    //所以这里使用的应该是全局placement new
    //调用空的构造函数
    template <class T>
    void construct(T* ptr){
        ::new ((void*)ptr) T();
    }
    //将T2类型的value作为构造函数的参数
    template <class T1, class T2>
    void construct(T1* ptr, const T2& value){
        ::new ((void*)ptr) T1(value);
    }


    //destroy将对象析构
    template <class T>
    void destroy_one(T*, std::true_type){}

    template <class T>
    void destroy_one(T* pointer, std::false_type){
        if(pointer != nullptr){
            pointer->~T();
        }
    }

    template <class ForwardIter>
    void destroy_cat(ForwardIter, ForwardIter, std::true_type){}

    template <class ForwardIter>
    void destroy_cat(ForwardIter first, ForwardIter last, std::false_type){
        for(; first != last; ++first)
            destroy(&*first);
    }

    template <class T>
    void destroy(T* pointer){
        destroy_one(pointer,std::is_trivially_destructible<T>{});
    }

    template <class ForwardIter>
    void destroy(ForwardIter first, ForwardIter last){
        destroy_cat(first, last, std::is_trivially_destructible<
                                 typename iterator_traits<ForwardIter>::value_type>{});
    }


}

#endif //MYSTL_CONSTRUCT_H
