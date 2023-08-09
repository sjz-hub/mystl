//
// Created by 孙建钊 on 2023/7/6.
//

#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

#include "deque.h"
#include "util.h"
#include "vector.h"
#include "heap_algo.h"

namespace mystl{


    template <class T, class Container = mystl::deque<T> >
    class queue{
    public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;

        static_assert(std::is_same<T, value_type>::value, "tht value_type of Container should be same with T");


    private:
        container_type c_;//底层容器



    public:

        queue() = default;
        explicit queue(size_type n):c_(n){}
        queue(size_type n, const value_type& value):c_(n,value){}
        template <class Iter>
        queue(Iter first, Iter last):c_(first,last){}
        queue(std::initializer_list<T> ilist):c_(ilist){}
        queue(const Container& c):c_(c){}
        queue(Container&& c):c_(std::move(c)){}
        queue(const queue& rhs):c_(rhs.c_){}
        queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value):c_(rhs.c_){}
        queue& operator=(const queue& rhs){
            c_ = rhs.c_;
            return *this;
        }
        queue& operator=(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value){
            c_ = std::move(rhs.c_);
            return *this;
        }
        queue& operator=(std::initializer_list<T> ilist){
            c_ = ilist;
            return *this;
        }
        ~queue() =default;

        reference front(){
            return c_.front();
        }
        const_reference front() const{
            return c_.front();
        }
        reference back(){
            return c_.back();
        }
        const_reference back() const{
            return c_.back();
        }
        bool empty() const noexcept{
            return c_.empty();
        }
        size_type size() const noexcept{
            return c_.size();
        }
        template <class ...Args>
        void emplace(Args&& ...args){
            c_.emplace_back(std::move(args...));
        }
        void push(const value_type& value){
            c_.push_back(value);
        }
        void push(value_type&& value){
            c_.push_back(value);
        }
        void pop(){
            c_.pop_front();
        }
        void clear(){
            c_.clear();
        }
        void swap(queue& rhs) noexcept(noexcept(mystl::swap(c_,rhs.c_))){
            container_type temp = std::move(c_);
            c_ = std::move(rhs.c_);
            rhs.c_ = std::move(temp);
        }

    public:
        friend bool operator==(const queue& lhs, const queue& rhs){
            return lhs.c_ == rhs.c_;
        }
        friend bool operator<(const queue& lhs, const queue& rhs){
            return lhs.c_ < rhs.c_;
        }

    };

    //重载运算符
    template <class T, class Container>
    bool operator==(const queue<T,Container>& lhs, const queue<T, Container>& rhs){
        return lhs == rhs;
    }

    template <class T, class Container>
    bool operator!=(const queue<T, Container>& lhs, const queue<T,Container>& rhs){
        return !(lhs == rhs);
    }

    template <class T, class Container>
    bool operator<(const queue<T,Container>& lhs, const queue<T,Container>& rhs){
        return lhs < rhs;
    }

    template <class T, class Container>
    bool operator>(const queue<T,Container>& lhs, const queue<T,Container>& rhs){
        return rhs < lhs;
    }

    template <class T, class Container>
    bool operator<=(const queue<T,Container>& lhs, const queue<T, Container>& rhs){
        return !(rhs < lhs);
    }

    template <class T, class Container>
    bool operator>=(const queue<T,Container>& lhs, const queue<T, Container>& rhs){
        return !(lhs < rhs);
    }





    template <class T, class Container = mystl::vector<T>,
            class Compare = std::less<typename Container::value_type>>
    class priority_queue{
        public:
            typedef Container container_type;
            typedef Compare value_compare;
            typedef typename Container::value_type value_type;
            typedef typename Container::size_type size_type;
            typedef typename Container::reference reference;
            typedef typename Container::const_reference const_reference;
            static_assert(std::is_same<T, value_type>::value,
                      "the value_type of Container should be same with T");

        private:
            container_type c_;
            value_compare comp_;



        public:
            priority_queue() =default;
            priority_queue(const Compare& c):c_(),comp_(c){}
            explicit priority_queue(size_type n):c_(n){}
            priority_queue(size_type n, const value_type& value):c_(n, value){}
            priority_queue(std::initializer_list<T> ilist):c_(ilist){}
            template <class Iter>
            priority_queue(Iter first, Iter last):c_(first, last){}
            priority_queue(const Container& s):c_(s){
                make_heap(c_.begin(), c_.end(), comp_);
            }
            priority_queue(Container&& s):c_(std::move(s)){
                make_heap(c_.begin(),c_.end(), comp_);
            }
            priority_queue(const priority_queue& rhs):c_(rhs.c_),comp_(rhs.comp_){
                make_heap(c_.begin(), c_.end(), comp_);//疑问这里为什么要调用生成堆，rhs不是一定是一个堆了吗，为什么要再次生成一遍呢，很奇怪
            }
            priority_queue(priority_queue&& rhs):c_(rhs.c_),comp_(rhs.comp_){
                make_heap(c_.begin(), c_.end(), comp_);//疑问这里为什么要调用生成堆，rhs不是一定是一个堆了吗，为什么要再次生成一遍呢，很奇怪
            }
            priority_queue& operator=(const priority_queue& rhs){
                c_ = rhs.c_;
                comp_ = rhs.comp_;
                make_heap(c_.begin(), c_.end(), comp_);//疑问这里为什么要调用生成堆，rhs不是一定是一个堆了吗，为什么要再次生成一遍呢，很奇怪
                return *this;
            }

            priority_queue& operator=(priority_queue&& rhs){
                c_ = std::move(rhs.c_);
                comp_ = std::move(rhs.comp_);
                make_heap(c_.begin(), c_.end(), comp_);//疑问这里为什么要调用生成堆，rhs不是一定是一个堆了吗，为什么要再次生成一遍呢，很奇怪
                return *this;
            }

            ~priority_queue() = default;


        public:
            const_reference top() const{return c_.front();}
            bool empty() const {return c_.empty();}
            size_type size() const {return c_.size();}
            template <class ...Args>
            void emplace(Args&& ...args){
                c_.emplace_back(std::move(args...));
                push_heap(c_.begin(), c_.end(), comp_);
            }
            void push(const value_type& value){
                c_.push(value);
                push_heap(c_.begin(), c_.end(), comp_);
            }
            void push(value_type&& value){
                c_.push_back(std::move(value));
                push_heap(c_.begin(), c_.end(), comp_);
            }

            void pop(){
                pop_heap(c_.begin(), c_.end(), comp_);
                c_.pop_back();
            }

            void clear(){//但是这里为什么不直接调用c_.clear() 速度还快，每次调用pop() 都需要维持一个堆，比较耗时
                while(!c_.empty())
                    pop();
            }

            void swap(priority_queue& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_)) && noexcept(mystl::swap(comp_, rhs.comp_))){
                priority_queue temp(std::move(rhs));
                rhs.c_ = std::move(c_);
                rhs.comp_ = std::move(comp_);

                c_ = std::move(temp.c_);
                comp_ = std::move(temp.comp_);
            }

        public:
            friend bool operator==(const priority_queue& lhs, const priority_queue& rhs){
                return lhs.c_ == rhs.c_;
            }
            friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs){
                return lhs.c_ != rhs.c_;
            }
    };

    template <class T, class Container, class Compare> bool operator==(priority_queue<T, Container, Compare>& lhs,priority_queue<T, Container, Compare>& rhs){
        return lhs == rhs;
    }
    template <class T, class Container, class Compare> bool operator!=(priority_queue<T, Container, Compare>& lhs,priority_queue<T, Container, Compare>& rhs){
        return lhs != rhs;
    }








}



#endif //MYSTL_QUEUE_H
