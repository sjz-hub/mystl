//
// Created by 孙建钊 on 2023/6/13.
//


#ifndef MYSTL_ITERATOR_H
#define MYSTL_ITERATOR_H

//这个头文件踊跃迭代器设计，包含了一些模板结构与全局函数

#include <cstddef>
#include <iostream>
#include "type_traits.h"

namespace mystl{

    //五种迭代器类型
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag:public input_iterator_tag{};
    struct bidirectional_iterator_tag:public forward_iterator_tag{};
    struct random_access_iterator_tag:public bidirectional_iterator_tag{};


    //迭代器模板
    // Category      迭代器的种类
    // T             迭代器元素的类型
    // Distance      迭代器之间的距离 ptrdiff_t 为 __int64
    // Pointer       元素的指针类型
    // Reference     元素的引用类型
    template<class Category, class T , class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator{
        typedef Category                 iterator_category;//迭代器的种类 上述5种之一
        typedef T                        value_type;//元素类型
        typedef Pointer                  pointer;//元素指针类型
        typedef Reference                reference;//元素引用类型
        typedef Distance                 difference_type;//迭代器的距离 __int64
    };


    //迭代器萃取

    //判断T是否有iterator_category这个成员，如果有则静态变量value为true，否则为false
    //这里加了一个U目的就是为了防止出现T没有iterator_category的情况
    template <class T>
    struct has_iterator_cat{
    private:
        struct two{char a;char b;};
        template <class U> static two test(...){}
        template <class U> static char test(typename U::iterator_category* = 0){return 'c';}
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template<class Iterator,bool>
    struct iterator_traits_impl{};

    template<class Iterator>
    struct iterator_traits_impl<Iterator,true>{
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
        typedef typename Iterator::difference_type      difference_type;
    };

    template<class Iterator,bool>
    struct iterator_traits_helper{};

    //std::is_convertible<from,to>表明from类型是否可以隐式转换为to类型
    template<class Iterator>
    struct iterator_traits_helper<Iterator,true>:public iterator_traits_impl<Iterator,
            std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value ||
            std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>{
    };

    //迭代器萃取器的特性
    template<class Iterator>
    struct iterator_traits:public iterator_traits_helper<Iterator,has_iterator_cat<Iterator>::value>{};

    //针对原生指针的偏特化版本
    template<class T>
    struct iterator_traits<T*>{
        typedef random_access_iterator_tag        iterator_category;
        typedef T                                 value_type;
        typedef T*                                pointer;
        typedef T&                                reference;
        typedef ptrdiff_t                         difference_type;
    };

    template<class T>
    struct iterator_traits<const T*>{
        typedef random_access_iterator_tag        iterator_category;
        typedef T                                 value_type;
        typedef const T*                          pointer;
        typedef const T&                          reference;
        typedef ptrdiff_t                         difference_type;
    };


    //判断T是否是迭代器，并且迭代器T是否可以转换为迭代器U
    template<class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of:
            public m_bool_constant<std::is_convertible<
            typename iterator_traits<T>::iterator_category,U>::value>{

    };
    template <class T,class U>
    struct has_iterator_cat_of<T,U,false>:public m_false_type {};

    //判断Iter是否是某种迭代器
    template <class Iter>
    struct is_input_iterator:public has_iterator_cat_of<Iter,input_iterator_tag>{};

    template <class Iter>
    struct is_output_iterator:public has_iterator_cat_of<Iter,output_iterator_tag>{};

    template <class Iter>
    struct is_forward_iterator:public has_iterator_cat_of<Iter,forward_iterator_tag>{};

    template <class Iter>
    struct is_bidirectional_iterator : public has_iterator_cat_of<Iter,bidirectional_iterator_tag>{};

    template <class Iter>
    struct is_random_access_iterator : public has_iterator_cat_of<Iter,random_access_iterator_tag>{};


    //判断Iterator是否是迭代器
    template <class Iterator>
    struct is_iterator: public m_bool_constant<
            is_input_iterator<Iterator>::value ||
            is_output_iterator<Iterator>::value>{

    };

    //已知Iterator是迭代器，萃取Iterator的某个成员
    //萃取category
    template <class Iterator>
    typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&){
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }
    //萃取distance_type
    template <class Iterator>
    typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);//空指针
    }
    //萃取value_type
    template <class Iterator>
    typename iterator_traits<Iterator>::value_type* value_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    //以下函数用于计算迭代器间的距离
    //计算input_iterator_tag版本 这个函数调用的前提需要保证fist一定小于等于last,否则会出问题
    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance_dispatch(InputIterator first,InputIterator last,input_iterator_tag){
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last){
            ++first;
            ++n;
        }
        return n;
    }

    //计算random_access_iterator_tag版本
    template <class RandomIterator>
    typename iterator_traits<RandomIterator>::difference_type
    distance_dispatch(RandomIterator first, RandomIterator last, random_access_iterator_tag){
        return last - first;
    }
    //使用一个函数将两种不同情况计算距离的方式合并在了一起，非常巧妙
    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last){
        return distance_dispatch(first, last, iterator_category(first));
    }

    //以下函数用于让迭代器前进n个距离
    //input_iterator_tag版本
    template <class InputIterator, class Distance>
    void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag){
        while(n--)
            ++i;
    }
    //bidirectional_iterator_tag版本
    template <class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag){
        if(n >= 0)
            while (n--) ++i;
        else while(n++) --i;
    }
    //random_access_iterator_tag版本
    template <class RandomIterator, class Distance>
    void advance_dispatch(RandomIterator& i, Distance n, random_access_iterator_tag){
        i+=n;
    }

    template <class InputIterator, class Distance>
    void advance(InputIterator& i, Distance n){
        advance_dispatch(i,n, iterator_category(i));
    }













    // 模板类 : reverse_iterator
// 代表反向迭代器，使前进为后退，后退为前进
    template <class Iterator>
    class reverse_iterator
    {
    private:
        Iterator current;  // 记录对应的正向迭代器

    public:
        // 反向迭代器的五种相应型别
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type        value_type;
        typedef typename iterator_traits<Iterator>::difference_type   difference_type;
        typedef typename iterator_traits<Iterator>::pointer           pointer;
        typedef typename iterator_traits<Iterator>::reference         reference;

        typedef Iterator                                              iterator_type;
        typedef reverse_iterator<Iterator>                            self;

    public:
        // 构造函数
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type i) :current(i) {}
        reverse_iterator(const self& rhs) :current(rhs.current) {}

    public:
        // 取出对应的正向迭代器
        iterator_type base() const
        { return current; }

        // 重载操作符
        reference operator*() const
        { // 实际对应正向迭代器的前一个位置
            auto tmp = current;
            return *--tmp;
        }
        pointer operator->() const
        {
            return &(operator*());
        }

        // 前进(++)变为后退(--)
        self& operator++()
        {
            --current;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            --current;
            return tmp;
        }
        // 后退(--)变为前进(++)
        self& operator--()
        {
            ++current;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            ++current;
            return tmp;
        }

        self& operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }
        self operator+(difference_type n) const
        {
            return self(current - n);
        }
        self& operator-=(difference_type n)
        {
            current += n;
            return *this;
        }
        self operator-(difference_type n) const
        {
            return self(current + n);
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }
    };

// 重载 operator-
    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type
    operator-(const reverse_iterator<Iterator>& lhs,
              const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() - lhs.base();
    }

// 重载比较操作符
    template <class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() < lhs.base();
    }

    template <class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs)
    {
        return rhs < lhs;
    }

    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs < rhs);
    }




}


#endif //MYSTL_ITERATOR_H
