//
// Created by 孙建钊 on 2023/7/5.
//

#ifndef MYSTL_STACK_H
#define MYSTL_STACK_H

#include "deque.h"




namespace mystl{


template <class T, class Container = mystl::deque<T>>
class stack{
public:
    typedef Container container_type;
    //使用底层容器的型别
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    static_assert(std::is_same<T, value_type>::value, "the value_type of the Container should be same with T");

private:
    container_type c_;//用底层容器实现 stack

public:
    stack() = default;
    explicit stack(size_type n):c_(n){}
    stack(size_type n, const value_type& value):c_(n,value){}
    template<class Iter>
    stack(Iter first, Iter last):c_(first,last){}
    stack(std::initializer_list<T> ilist):c_(ilist){}
    stack(const Container& c):c_(c){}
    stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value):c_(std::move(c)){}
    stack(const stack& rhs):c_(rhs.c_){}
    stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) : c_(std::move(rhs.c_)){}

    stack& operator=(const stack& rhs){
        c_ = rhs.c_;
        return *this;
    }

    stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value){
        c_ = std::move(rhs.c_);
        return *this;
    }

    stack& operator=(std::initializer_list<T> ilist){
        c_ = ilist;
        return *this;
    }

    ~stack() = default;

    reference top(){
        return c_.back();
    }

    const_reference top() const{
        return c_.back();
    }

    bool empty() const noexcept{
        return c_.empty();
    }

    size_type size() const noexcept{
        return c_.size();
    }

    template<class... Args>
    void emplace(Args&& ...args){
        c_.emplace(c_.end(),std::forward<Args...>(args...));
    }

    void push(const value_type& value) {
        c_.push_back(value);
    }

    void push(const value_type&& value){
        c_.push_back(std::move(value));
    }

    void pop(){
        c_.pop_back();
    }

    void clear(){
        if(size() > 0){
            c_.clear();
        }
    }

    void swap(stack& rhs) noexcept(noexcept(mystl::swap(c_,rhs.c_))){
        Container temp = std::move(rhs.c_);
        rhs.c_ = std::move(c_);
        c_ = std::move(temp);
    }

    friend bool operator==(const stack& lhs, const stack& rhs){
        return lhs.c_ == rhs.c_;
    }

    friend bool operator<(const stack& lhs, const stack& rhs){
        return lhs.c_ < rhs.c_;
    }
};

template <class T, class Container>
bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
    return lhs == rhs;
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
    return lhs < rhs;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
    return !(lhs == rhs);
}


template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
    return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
    return !(rhs<lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
    return !(lhs<rhs);
}




}



#endif //MYSTL_STACK_H
