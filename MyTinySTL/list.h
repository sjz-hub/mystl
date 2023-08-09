//
// Created by 孙建钊 on 2023/6/14.
//

#ifndef MYSTL_LIST_H
#define MYSTL_LIST_H


#include <initializer_list>

#include "iterator.h"
#include "util.h"
#include "exceptdef.h"


namespace mystl{

    template <class T> struct list_node_base;
    template <class T> struct list_node;

    template <class T>
    struct node_traits{
        typedef list_node_base<T>* base_ptr;
        typedef list_node<T>* node_ptr;
    };

    //list节点结构只包含前后指针
    template <class T>
    struct list_node_base{

        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        base_ptr prev;//前节点
        base_ptr next;//后节点

        list_node_base() = default;

        virtual base_ptr self(){
            return static_cast<base_ptr>(&*this);
        }

        node_ptr as_node(){
            return static_cast<node_ptr>(self());
        }

        void unlink(){
            prev = next = self();
        }

    };

    template <class T>
    struct list_node: public list_node_base<T>{

        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        T value; // 数据域

        node_ptr self(){
            return static_cast<node_ptr>(&*this);
        }

        list_node() = default;
        list_node(const T& v):value(v){}
        list_node(T&& v):value(mystl::move(v)){}

        base_ptr as_base(){
            return static_cast<base_ptr>(&*this);
        }
    };

    //list 迭代器设计
    template <class T>
    struct list_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag,T>{
        typedef T                                 value_type;
        typedef T*                                pointer;
        typedef T&                                reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_iterator<T>                  self;

        base_ptr node_;//指向当前节点

        list_iterator() = default;
        list_iterator(base_ptr x):node_(x){}
        list_iterator(node_ptr x):node_(x->as_base()){}
        list_iterator(const list_iterator& rhs):node_(rhs.node_){}

        //重载操作符
        reference operator*() const{
            return node_->as_node()->value;
        }
        pointer operator->()const{
            return &(operator*());
        }
        //前置
        self& operator++(){
            MYSTL_DEBUG(node_!= nullptr);
            node_ = node_->next;
            return *this;
        }
        //后置
        self operator++(int){
            MYSTL_DEBUG(node_ != nullptr);
            self temp = static_cast<self>(*this);
            ++*this;
            return temp;
        }
        //前置
        self& operator--(){
            MYSTL_DEBUG(node_!= nullptr);
            node_ = node_->prev;
            return *this;
        }
        //后置
        self operator--(int){
            MYSTL_DEBUG(node_ != nullptr);
            self temp = static_cast<self>(*this);
            --*this;
            return temp;
        }

        //重载比较运算符
        bool operator==(const self& rhs) const{
            return node_ == rhs.node_;
        }
        bool operator!=(const self& rhs) const{
            return node_ != rhs.node_;
        }
    };

    //list迭代器的const版本
    template <class T>
    struct list_const_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag,T>{
        typedef T                                 value_type;
        typedef const T*                          pointer;
        typedef const T&                          reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_const_iterator<T>            self;

        base_ptr node_;//指向当前节点

        list_const_iterator() = default;
        list_const_iterator(base_ptr x):node_(x){}
        list_const_iterator(node_ptr x):node_(x->as_base()){}
        list_const_iterator(const list_iterator<T>& rhs)
                :node_(rhs.node_) {}
        list_const_iterator(const list_const_iterator& rhs)
                :node_(rhs.node_) {}
        //重载操作符
        reference operator*() const{
            return node_->as_node()->value;
        }
        pointer operator->()const{
            return &(operator*());
        }
        //前置
        self& operator++(){
            MYSTL_DEBUG(node_!= nullptr);
            node_ = node_->next;
            return *this;
        }
        //后置
        self operator++(int){
            MYSTL_DEBUG(node_ != nullptr);
            self temp = static_cast<self>(*this);
            ++*this;
            return temp;
        }
        //前置
        self& operator--(){
            MYSTL_DEBUG(node_!= nullptr);
            node_ = node_->prev;
            return *this;
        }
        //后置
        self operator--(int){
            MYSTL_DEBUG(node_ != nullptr);
            self temp = static_cast<self>(*this);
            --*this;
            return temp;
        }

        //重载比较运算符
        bool operator==(const self& rhs) const{
            return node_ == rhs.node_;
        }
        bool operator!=(const self& rhs) const{
            return node_ != rhs.node_;
        }
    };





    //类模板：list
    template <class T>
    class list{
    public:
        //list的嵌套类型定义


        typedef list_iterator<T> iterator;
        typedef list_const_iterator<T> const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        typedef size_t size_type;
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;

    private:
        base_ptr node_;//指向末尾节点
        size_type size_;//list大小

    public:
        list(){
            fill_init(0, value_type());
        }
        list(size_type n){
            fill_init(n, value_type());
        }

        list(size_type n, const value_type& value){
            fill_init(n,value);
        }

        template <class Iter,typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type = 0>
        list(Iter first,Iter last){
            copy_init(first,last);
        }

        list(std::initializer_list<T> ilist){
            copy_init(ilist.begin(),ilist.end());
        }

        list(const list& rhs){
            copy_init(rhs.begin(),rhs.end());
        }

        list(list&& rhs) noexcept:node_(rhs.node_),size_(size_){
            rhs.node_ = nullptr;
            rhs.size_ = 0;
        }

        list& operator=(const list& rhs){
            if(this != &rhs){
                copy_assign(rhs.begin(),rhs.end());
            }
            return *this;
        }

        list& operator=(list&& rhs) noexcept{
            node_ = std::move(rhs.node_);
            size_ = std::move(rhs.size_);
            rhs.node_ = nullptr;
            rhs.size_ = 0;
            return *this;
        }



        list& operator=(std::initializer_list<T> ilist){

            copy_init(ilist.begin(),ilist.end());
            return *this;
        }

        ~list(){
            if(node_){
                clear();
                delete node_;
                node_ = nullptr;
                size_ = 0;
            }
        }




        size_type size() const{
            return size_;
        }

        bool empty() const{
            return size_ == 0;
        }

        size_type max_size(){
            return static_cast<size_type>(-1);
        }


        iterator begin() noexcept{
            return iterator(node_->next);
        }
        const_iterator begin() const{
            return const_iterator(node_->next);
        }

        iterator end() noexcept{
            return iterator(node_);
        }
        const_iterator end() const{
            return const_iterator(node_);
        }

        reverse_iterator rbegin() noexcept{
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const{
            return const_reverse_iterator(begin());
        }

        const_iterator cbegin() const{
            return const_iterator(begin());
        }

        const_iterator cend() const{
            return const_iterator(end());
        }

        const_reverse_iterator crbegin() const{
            const_reverse_iterator(end());
        }

        const_reverse_iterator crend() const{
            const_reverse_iterator(begin());
        }


        reference front(){
            MYSTL_DEBUG(size_ > 0);
            return node_->next->as_node()->value;
        }

        const_reference front() const{
            MYSTL_DEBUG(size_ > 0);
            return *begin();
        }

        reference back(){
            MYSTL_DEBUG(size_ > 0);
            return node_->prev->as_node()->value;
        }

        const_reference back() const{
            MYSTL_DEBUG(size_ > 0);
            return *(--end());
        }

        void assign(size_type n, const value_type& value){
            fill_assign(n,value);
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type = 0>
        void assign(Iter first, Iter last){
            copy_init(first,last);
        }

        void assign(std::initializer_list<T> ilist){
            assign(ilist.begin(),ilist.end());
        }


        void push_front(const value_type& value){
            base_ptr  new_node = create_node(value);
            link_nodes(node_->next,new_node,new_node);
            size_++;
        }

        void push_front(const value_type&& value){
            base_ptr  new_node = create_node(std::move(value));
            link_nodes(node_->next,new_node,new_node);
            size_++;
        }

        void push_back(const value_type& value){
            base_ptr  new_node = create_node(value);
            link_nodes(node_,new_node,new_node);
            size_++;
        }

        void push_back(const value_type&& value){
            base_ptr  new_node = create_node(std::move(value));
            link_nodes(node_,new_node,new_node);
            size_++;
        }


        void pop_front(){
            erase(const_iterator(node_->next));
        }

        void pop_back(){
            erase(const_iterator(node_->prev));
        }


        iterator insert(const_iterator pos, const value_type& value){
            base_ptr new_node = create_node(value);
            size_++;
            return link_iter_node(pos,new_node);
        }

        iterator insert(const_iterator pos,value_type&& value){
            base_ptr new_node = create_node(std::move(value));
            size_++;
            return link_iter_node(pos,new_node);
        }

        iterator insert(const_iterator pos, size_type n, const value_type& value){
            return fill_insert(pos,n,value);
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type = 0 >
        iterator insert(const_iterator pos, Iter first, Iter last){
            size_type n = mystl::distance(first,last);
            return copy_insert(pos,n,first);
        }



        template <class ...Args>
        iterator emplace(const_iterator pos,Args&&... args){

            base_ptr new_node = create_node(std::forward<Args>(args)...);
            link_nodes(pos.node_,new_node,new_node);
            ++size_;
            return iterator(new_node);
        }

        template <class ...Args>
        iterator emplace_front(Args&& ...args){
            return emplace(begin(),args...);
        }

        template <class ...Args>
        iterator emplace_back(Args&& ...args){
            return emplace(end(),args...);
        }



        iterator erase(const_iterator pos);//删除指定位置的元素，并返回下一个位置的迭代器
        iterator erase(const_iterator first, const_iterator last);

        void clear(){
            if(size_ > 0) erase(begin(),end());
        }


        void resize(size_type new_size){ resize(new_size,value_type());}
        void resize(size_type new_size, const value_type& value);

        void     swap(list& rhs) noexcept
        {
            mystl::swap(node_, rhs.node_);
            mystl::swap(size_, rhs.size_);
        }

        void reverse();//进行反转

        void splice(const_iterator pos, list& x);
        void splice(const_iterator pos,list& x,const_iterator it);
        void splice(const_iterator pos, list& x, const_iterator first, const_iterator last);

        template <class UnaryPredicate>
        void remove_if(UnaryPredicate pred);

        void remove(const value_type& value){
            remove_if([&](const value_type& v){return v == value;});
        }

        template <class BinaryPredicate>
        void unique(BinaryPredicate pred);

        void unique(){
            unique([](const value_type& l, const value_type& r){return l == r;});
        }

        template <class Compare>
        void merge(list& x, Compare comp);

        void merge(list& x){
            merge(x,[](const value_type& l, const value_type& r){return l < r;});
        }

        void sort(){
            sort([](const value_type& l, const value_type& r){return l < r;});
        }

        template<class Compared>
        void sort(Compared comp){
            list_sort(begin(),end(),size(),comp);
        }





    public:
        //帮助函数否是私有的
        template <class ...Args>
        node_ptr create_node(Args&& ... args);
        node_ptr create_node(const T& value);//创建新节点，返回新节点的node_ptr
        void fill_init(size_type n, const value_type& value);//使用n个value元素初始化一个容器,在这里面修改了size_
        iterator fill_insert(const_iterator pos, size_type n, const value_type& value);//在pos前插入n个节点value,返回n个节点的第一个节点的迭代器，如果插入0个节点，则返回pos
        void fill_assign(size_type n, const value_type& value);//用n个value为容器重新赋值，如果容器的大小大于n则删除尾部指定元素；如果小于n则在尾部后面添加指定数量的元素
        template <class Iter>
        void copy_init(Iter first, Iter last);//使用[first,last)初始化容器，将这些值拷贝到容器中
        template <class Iter>
        iterator copy_insert(const_iterator pos,size_type n, Iter first);//在pos前面插入[first,last)的值节点，返回插入的n个节点的第一个节点，如果插入0个节点，则返回pos
        template <class Iter>
        void copy_assign(Iter f2, Iter l2);//copy[f2,l2)元素为容器赋值，如果容器的大小大于n则删除尾部指定元素；如果小于n则在尾部后面添加指定数量的元素
        iterator link_iter_node(const_iterator pos,base_ptr link_node);//在pos前面插入一个节点，返回插入节点的迭代器
        void link_nodes(base_ptr pos, base_ptr first, base_ptr last);//在pos前面插入[first,last]
        void unlink_nodes(base_ptr first, base_ptr last);//断开[first,last]与容器的连接
        void destroy_node(node_ptr p);//销毁一个节点
        template <class Compared>
        iterator list_sort(iterator f2, iterator l2, size_type n, Compared comp);//对list进行归并排序，并返回一个迭代器指向区间的最小元素的位置，使用comp进行排序
    };


    //*************************帮助函数***************************************


    template <class T>
    template <class ...Args>
    typename list<T>::node_ptr list<T>::create_node(Args &&...args) {
//        std::cout<<"b";
        node_ptr new_node = new list_node<T>(T(args...));//这里可能会出问题，源代码里面使用forward
        return new_node;
    }

    template <class T>
    typename list<T>::node_ptr list<T>::create_node(const T& value){
        node_ptr new_node = new list_node<T>(value);
        return new_node;
    }

    template <class T>
    void list<T>::fill_init(size_type n, const value_type &value) {
        node_ = new list_node_base<T>();
        node_->unlink();//前后都指向自己
        size_ = 0;
        for(int i = 0; i < n; ++i){
            node_ptr new_node = create_node(value);
            link_iter_node(const_iterator(node_),new_node);
        }
        size_ = n;
    }

    template <class T>
    typename list<T>::iterator list<T>::fill_insert(const_iterator pos, size_type n, const value_type& value){

        iterator start(pos.node_);
        if(n > 0){
            start = insert(pos,value);
            for(int i = 0; i < n - 1; ++i){
                start = insert(start,value);//在这里面已经进行了insert
            }
        }
        return start;
    }

    template <class T>
    void list<T>::fill_assign(size_type n, const value_type &value) {
        iterator first = begin();
        iterator last = end();
        while(first!=last && n > 0){
            *first = value;
            ++first;
            --n;
        }

        if(n==0){
            erase(first,last);
        }

        if(first == last){
            fill_insert(end(),n,value);
            n = 0;
        }


    }

    template <class T>
    template <class Iter>
    void list<T>::copy_init(Iter first, Iter last) {
        node_ = new list_node_base<T>();
        node_->unlink();
        size_ = 0;
        while(first != last){
            insert(end(),*first);
            ++first;
        }

    }


    template <class T>
    template <class Iter>
    typename list<T>::iterator list<T>::copy_insert(const_iterator pos, size_type n,
                                                    Iter first) {
        iterator start(pos.node_);
        if(n>0){
            start = insert(start,*first);
            ++first;
            for(int i = 0; i < n - 1; ++i,first++){
                start = insert(pos,*first);
            }
        }
        return start;
    }


    template <class T>
    template <class Iter>
    void list<T>::copy_assign(Iter f2, Iter l2) {
        iterator first = begin();
        size_type  len = 0;
        while(f2 != l2 && len < size()){
            first.node_->as_node()->value = *f2;
            ++first;
            ++f2;
            len++;
        }

        while(f2 != l2){
            insert(end(),*f2);
            ++f2;
        }
        if(len < size()){
            erase(first,end());

        }
    }

    template<class T>
    typename list<T>::iterator list<T>::link_iter_node(const_iterator pos,base_ptr link_node) {
        link_nodes(pos.node_,link_node,link_node);
        return iterator(link_node);
    }

    template<class T>
    void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last) {
        first->prev = pos->prev;
        last->next = pos;
        pos->prev->next = first;
        pos->prev = last;
    }

    template<class T>
    typename list<T>::iterator list<T>::erase(const_iterator pos) {
        MYSTL_DEBUG(size() > 0);
        base_ptr next = pos.node_->next;
        unlink_nodes(pos.node_,pos.node_);
        delete pos.node_;

        size_--;
        return iterator(next);
    }

    template<class T>
    void list<T>::unlink_nodes(base_ptr first, base_ptr last) {

        first->prev->next = last->next;
        last->next->prev = first->prev;

    }

    template <class T>
    void list<T>::destroy_node(node_ptr p) {
        delete p;
    }


    template <class T>
    template <class Compared>
    typename list<T>::iterator list<T>::list_sort(iterator f2, iterator l2,
                                                  size_type n, Compared comp) {
        if(n == 1) return f2;
        else if(n == 2){
            iterator temp = l2;
            --temp;
            if(comp(*temp,*f2)){
//                std::cout<<"q";
                base_ptr node = temp.node_;
                unlink_nodes(node,node);
                link_nodes(f2.node_,node,node);
                return iterator(f2.node_->prev);
            }
            else {
//                std::cout<<"z";
                return f2;
            }
        }
        else{
            size_type mid = n / 2;
            iterator f1 = f2;
            iterator l1 = f2;
            mystl::advance(l1,mid);
            f2 = l1;

//            std::cout<<"pre:";
//            for(auto it1 = f1; it1 != l1; ++it1){
//                std::cout<<*it1<<" ";
//            }
//            std::cout<<std::endl;
//            for(auto it1 = f2; it1 != l2; ++it1){
//                std::cout<<*it1<<" ";
//            }
//            std::cout<<std::endl;

            //在f2之前加入一个哨兵节点作为l1
            insert(f2,T());
            l1 = iterator(f2.node_->prev);

            f1 = list_sort(f1,l1,mid,comp);
//            std::cout<<"sort:";
//            for(auto it1 = f1; it1 != l1; ++it1){
//                std::cout<<*it1<<" ";
//            }
//            std::cout<<std::endl;


            f2 = list_sort(f2,l2,n - mid,comp);

//            for(auto it1 = f2; it1 != l2; ++it1){
//                std::cout<<*it1<<" ";
//            }
//            std::cout<<std::endl;

            iterator min_iterator = f1;
            bool flag = false;
            //合并两段有序区间
            while(f1 != l1 && f2 != l2){
                if(comp(*f1,*f2)){
//                    std::cout<<"f1:"<<*f1;
                    ++f1;
                }
                else {
                    iterator temp = iterator(f2.node_->next);
                    base_ptr node = f2.node_;
                    unlink_nodes(node,node);
                    link_nodes(f1.node_,node,node);
                    if(!flag){
                        min_iterator = iterator(node);
                    }
                    f2 = temp;
//                    std::cout<<"f2:"<<*f2;
                }
                flag = true;
            }

//            std::cout<<"min:"<<*min_iterator<<":";
//            if(f1!= l1){
//                std::cout<<"qwe";
//                for(auto it1 = min_iterator; it1 != l1; ++it1){
//                    std::cout<<*it1<<" ";
//                }
//                std::cout<<std::endl;
//            }
//            else{
//                for(auto it1 = min_iterator; it1 != l2; ++it1){
//                    std::cout<<*it1<<" ";
//                }
//                std::cout<<std::endl;
//            }

            erase(l1);



            return min_iterator;


        }

    }



    template<class T>
    typename list<T>::iterator list<T>::erase(const_iterator first,const_iterator last) {
        while(first!= last){
            first = erase(first);
        }
        return iterator(last.node_);
    }

    template <class T>
    void list<T>::resize(size_type new_size, const value_type &value) {
        if(new_size > size_){
            fill_insert(end(),new_size - size_, value);
        }
        else {
            iterator first = begin();
            while(new_size > 0){
                new_size--;
                ++first;
            }
            erase(first,end());
        }
    }

    template <class T>
    void list<T>::reverse() {


        iterator first = begin();
        iterator last = end();

        while(first != last){
            base_ptr now_node = first.node_;
            ++first;

            now_node->next = now_node->prev;
            now_node->prev = first.node_;

        }

        base_ptr next = node_->next;
        node_->next = node_->prev;
        node_->prev = next;

    }

    template <class T>
    void list<T>::splice(const_iterator pos, list<T> &x) {
        base_ptr prev = pos.node_->prev;
        base_ptr now = pos.node_;
        prev->next = x.node_->next;
        x.node_->next->prev = prev;


        now->prev = x.node_->prev;
        x.node_->prev->next = now;
        size_ += x.size_;
        delete x.node_;
        x.node_ = nullptr;
        x.size_ = 0;
    }

    template <class T>
    void list<T>::splice(const_iterator pos, list &x,const_iterator it) {
;
        base_ptr it_ptr = it.node_;

        it_ptr->prev->next = it_ptr->next;
        it_ptr->next->prev = it_ptr->prev;

        link_nodes(pos.node_,it_ptr,it_ptr);

        x.size_--;
        size_++;

    }

    template <class T>
    void list<T>::splice(const_iterator pos, list &x,const_iterator first, const_iterator last) {

        int n = mystl::distance(first,last);
        base_ptr f1 = first.node_;
        base_ptr l1 = last.node_->prev;

        f1->prev->next = l1->next;
        l1->next->prev = f1->prev;


        x.size_-=n;
        size_+=n;

        link_nodes(pos.node_,f1,l1);

    }

    template <class T>
    template <class UnaryPredicate>
    void list<T>::remove_if(UnaryPredicate pred) {
        iterator first = begin();
        iterator last = end();
        iterator next = iterator(first.node_->next);
        while(first != last){
            if(pred(*first)){
                erase(first);
            }
            first = next;
            ++next;
        }
    }

    template <class T>
    template <class BinaryPredicate>
    void list<T>::unique(BinaryPredicate pred) {
        iterator first = begin();
        iterator last = end();

        while(first != last){
            if(pred(*first,*(iterator(first.node_->next)))){
                erase(iterator(first.node_->next));
            }
            else{
                ++first;
            }
        }
    }

    template <class T>
    template <class Compare>
    void list<T>::merge(list&x, Compare comp){
        iterator f1 = begin();
        iterator l1 = end();
        iterator f2 = x.begin();
        iterator l2 = x.end();

        iterator temp;

        while(f1 != l1 && f2 != l2){
            if(comp(*f1,*f2)){
                ++f1;
            }
            else{
                temp = iterator(f2.node_->next);
                x.unlink_nodes(f2.node_,f2.node_);
                link_nodes(f1.node_,f2.node_,f2.node_);
                f2 = temp;
            }
        }


        if(f2!=l2){
            base_ptr xl = l2.node_->prev;
            x.unlink_nodes(f2.node_,xl);
            link_nodes(l1.node_,f2.node_,xl);
        }

//        for(auto it = begin(); it != end(); ++it){
//            std::cout<<*it<<" ";
//        }
//        std::cout<<std::endl;
//
//        for(auto it = x.begin(); it != x.end(); ++it){
//            std::cout<<*it<<" ";
//        }
//        std::cout<<std::endl;



        size_ += x.size_;
        x.size_ = 0;

    }


}





#endif //MYSTL_LIST_H
