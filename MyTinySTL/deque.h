//
// Created by 孙建钊 on 2023/6/29.
//

#ifndef MYSTL_DEQUE_H
#define MYSTL_DEQUE_H

#include "iterator.h"
#include "exceptdef.h"

//deque map初始化大小
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

namespace mystl{

    //一个buf的元素的个数
    //buf的中元素的个数最小为16个，最小内存为4kb
    template <class T>
    struct deque_buf_size{
        static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
    };

    // deque的迭代器设计
    template <class T, class Ref, class Ptr>
    struct deque_iterator : public iterator<random_access_iterator_tag, T>
    {
        typedef deque_iterator<T, T& , T*> iterator;
        typedef deque_iterator<T, const T&, const T*> const_iterator;
        typedef deque_iterator self;

        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference ;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T* value_pointer;
        typedef T** map_pointer;

        static const size_type buffer_size = deque_buf_size<T>::value;//一个buf的元素的大小

        //迭代器所包含的成员
        value_pointer cur; //指向所在缓冲区的当前元素
        value_pointer first;//指向所在缓冲区的头部,不是该缓冲区真实存放的头部，而是缓冲区开辟的头部
        value_pointer last; //指向所在缓冲区的尾部
        map_pointer node;//缓冲区所在节点

        deque_iterator() noexcept:cur(nullptr), first(nullptr),last(nullptr),node(nullptr){}
        deque_iterator(value_pointer v, map_pointer n):cur(v),first(*n),last(*n+buffer_size),node(n){} //这个函数表明缓冲区全部装满
        deque_iterator(const iterator& rhs):cur(rhs.cur),first(rhs.first),last(rhs.last),node(rhs.node){}
        deque_iterator(iterator&& rhs):cur(std::move(rhs.cur)),first(std::move(rhs.first)),last(std::move(rhs.last)),node(std::move(rhs.node)){
            rhs.cur = nullptr;
            rhs.first = nullptr;
            rhs.last = nullptr;
            rhs.node = nullptr;
        }
        deque_iterator(const const_iterator& rhs):cur(rhs.cur),first(rhs.first),last(rhs.last),node(rhs.node){}

        self& operator=(const iterator& rhs){
            if(this != &rhs){
                cur = rhs.cur;
                first = rhs.first;
                last = rhs.last;
                node = rhs.node;
            }
            return *this;
        }

        //转到另一个缓冲区，对node,first,last进行重置，但是不对cur重置
        void set_node(map_pointer new_node){
            first = *new_node;
            last = *new_node + buffer_size;
            node= new_node;
        }



        reference operator*() const{
            return *cur;
        }
        pointer operator->() const{
            return cur;
        }

        difference_type operator-(const self& x) const{
            return static_cast<difference_type>(buffer_size*(node - x.node) + (cur - first) - (x.cur - x.first));
        }

        self& operator++(){
            ++cur;
            if(cur == last){
                set_node(node+1);
                cur = first;
            }
            return *this;
        }

        self operator++(int){
            self temp = *this;
            ++*this;
            return temp;
        }

        self& operator--(){
            if(cur == first){
                set_node(node-1);
                cur = last;
            }
            --cur;
            return *this;
        }

        self operator--(int){
            self temp = *this;
            --*this;
            return temp;
        }

        self& operator+=(difference_type n){
            difference_type offset = n + (cur - first);
            if(offset >= 0 && offset < static_cast<difference_type>(buffer_size)){
                cur += n;
            }
            else{
                auto node_offset = offset > 0 ?
                        offset / static_cast<difference_type>(buffer_size):
                       -static_cast<difference_type>((-offset - 1) / buffer_size) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
            }
            return *this;
        }

        self operator+(difference_type n) const{
            self temp = *this;
            temp+=n;
            return temp;
        }

        self& operator-=(difference_type n) {
            return *this += -n;
        }

        self operator-(difference_type n){
            self temp = *this;
            temp += -n;
            return temp;
        }

        reference operator[](difference_type n) const{
            return *(*this + n);
        }

        bool operator==(const self& rhs) const{
            return cur == rhs.cur;
        }
        bool operator<(const self& rhs) const{
            return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node);
        }
        bool operator!=(const self& rhs) const{
            return !(*this == rhs);
        }
        bool operator>(const self& rhs) const{
            return rhs < *this;
        }
        bool operator<=(const self& rhs) const{
            return !(*this > rhs);
        }
        bool operator>=(const self& rhs) const{
            return !(*this < rhs);
        }
    };



    //模板类 deque
    //模板参数代表数据类型
    template <class T>
    class deque{
    public:

        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef pointer* map_pointer;
        typedef const_pointer* const_map_pointer;

        typedef deque_iterator<T, T&, T*> iterator;
        typedef deque_iterator<T, const T&, const T*> const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

        static const size_type buffer_size = deque_buf_size<T>::value;

    private:
        iterator begin_;//指向第一个节点
        iterator end_;//指向最后一个节点
        map_pointer map_;//指向一块map,map中的每个元素都是一个指针，指向一个缓冲区
        size_type map_size_; //map内指针的数目
    public:

        deque(){
            fill_init(0, value_type());
        }

        explicit deque(size_type n) {
            fill_init(n, value_type());
        }

        deque(size_type n, const value_type& value){
            fill_init(n,value);
        }

        deque(const deque& rhs){
            copy_init(rhs.begin(),rhs.end());
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type=0>
        deque(Iter first, Iter last){
            copy_init(first,last);
        }

        deque(std::initializer_list<value_type> ilist){
            copy_init(ilist.begin(),ilist.end());
        }

        deque(deque&& rhs) noexcept:begin_(std::move(rhs.begin_)),end_(std::move(rhs.end_)),map_(std::move(rhs.map_)),map_size_(std::move(rhs.map_size_)){
            rhs.map_size_ = 0;
            rhs.map_ = nullptr;
        }

        deque& operator=(const deque& rhs){

            copy_assign(rhs.begin(),rhs.end());


            return *this;
        }

        deque& operator=(deque&& rhs){
            clear();
            begin_ = std::move(rhs.begin_);
            end_ = std::move(rhs.end_);
            map_ = std::move(rhs.map_);
            map_size_ = std::move(rhs.map_size_);
            rhs.map_ = nullptr;
            rhs.map_size_ = 0;
            return *this;
        }

        deque& operator=(std::initializer_list<value_type> ilist){
            copy_init(ilist.begin(),ilist.end());
            return *this;
        }

        void clear(){//暂时还未完成
            end_ = begin_;
            shrink_to_fit();

        }

        ~deque(){
            if(map_ != nullptr){//这里需要进行判断，因为有可能这个deque被move给另一个，如果不判断会出问题
                clear();
                delete[] *begin_.node;
                *begin_.node = nullptr;
                delete[] map_;
                map_ = nullptr;
            }

        }


        bool empty() const noexcept{
            return begin_ == end_;
        }

        size_type size() const noexcept{
            return end_ - begin_;
        }

        size_type max_size() const noexcept{
            return static_cast<size_type>(-1);
        }

        void resize(size_type new_size){
           resize(new_size, value_type());
        }

        void resize(size_type new_size, const value_type& value){

            if(new_size > size()){
                fill_insert(end(), new_size - size() , value);
            }
            else{
                erase(begin_ + new_size, end());
            }


        }

        void shrink_to_fit();

        reference operator[](size_type n){
            MYSTL_DEBUG(n < size());
            return begin_[n];
        }

        const_reference operator[](size_type n) const{
            MYSTL_DEBUG(n < size());
            return begin_[n];
        }

        reference at(size_type n){
            MYSTL_DEBUG(n < size());
            return begin_[n];
        }

        const_reference at(size_type n) const{
            MYSTL_DEBUG(n < size());
            return begin_[n];
        }

        reference front(){
            MYSTL_DEBUG(empty() == false);
            return *begin_;
        }

        const_reference front() const{
            MYSTL_DEBUG(empty() == false);
            return *begin_;
        }


        reference back(){
            MYSTL_DEBUG(empty() == false);
            return *(end_ - 1);
        }

        const_reference back() const{
            MYSTL_DEBUG(empty() == false);
            return *(end_ - 1);
        }

        void assign(size_type n, const value_type& value){
            fill_assign(n,value);
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value, int>::type = 0>
        void assign(Iter first, Iter last){
            copy_assign(first,last);
        }

        void assign(std::initializer_list<value_type> ilist){
            copy_assign(ilist.begin(), ilist.end());
        }




        void push_front(const value_type& value){
            require_capacity(1,true);
            begin_--;
            *begin_ = value;
        }

        void push_back(const value_type& value){
            require_capacity(1,false);
            *end_ = value;
            end_++;
        }

        void push_front(value_type&& value){
            require_capacity(1,true);
            begin_--;
            *begin_ = std::move(value);
        }

        void push_back(value_type&& value){
            require_capacity(1,false);
            *end_ = std::move(value);
            end_++;
        }

        void pop_back(){
            MYSTL_DEBUG(size() > 0);
            --end_;
        }

        void pop_front(){
            MYSTL_DEBUG(size() > 0);
//            ++begin_;
            begin_++;
        }


       template <class ...Args>
       void emplace_front(Args&& ...args){
           push_front(value_type(std::forward<Args...>(args...)));
        }

        template <class ...Args>
        void emplace_back(Args&& ...args){
            push_back(value_type(std::forward<Args...>(args...)));
        }

        template <class ...Args>
        iterator emplace(iterator pos, Args&& ...args);



        iterator insert(iterator position, const value_type& value);
        iterator insert(iterator position, value_type&& value);
        void insert(iterator position, size_type n, const value_type& value);
        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type = 0>
        void insert(iterator position, Iter first, Iter last){
            insert_dispatch(position,first,last);
        }


        iterator begin() noexcept{
            return begin_;
        }
        const_iterator begin() const noexcept{
            return begin_;
        }
        iterator end() noexcept{
            return end_;
        }
        const_iterator end() const noexcept{
            return end_;
        }
        reverse_iterator rbegin() noexcept{
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const noexcept{
            return end();
        }
        reverse_iterator rend() noexcept{
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const noexcept{
            return begin();
        }

        const_iterator cbegin() const noexcept{
            return begin();
        }
        const_iterator cend() const noexcept{
            return end();
        }
        const_reverse_iterator crbegin() const noexcept{
            return rbegin();
        }
        const_reverse_iterator crend() const noexcept{
            return rend();
        }

        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);


        void swap(deque& rhs) noexcept{
            if(this != &rhs){
                iterator temp = std::move(rhs.begin_);
                rhs.begin_ = std::move(begin_);
                begin_ = std::move(temp);

                temp = std::move(rhs.end_);
                rhs.end_ = std::move(end_);
                end_ = std::move(temp);

                map_pointer map_temp = std::move(rhs.map_);
                rhs.map_ = std::move(map_);
                map_ = std::move(map_temp);

                size_type map_size_temp = std::move(rhs.map_size_);
                rhs.map_size_ = std::move(map_size_);
                map_size_ = std::move(map_size_temp);
            }
        }



    private:
        //帮助函数
        map_pointer create_map(size_type size);//创建一个含有size个T*的数组，返回这个数组的首地址
        void create_buffer(map_pointer nstart, map_pointer nfinsh);//为[nstart,nfinsh] 中的每一个元素都赋值一个大小为buffer_size的数组首地址，都是new出来的
        void destroy_buffer(map_pointer nstart, map_pointer nfinsh);//释放[nstart,nfinsh]中每个元素对应的数组空间，并都置为nullptr
        void map_init(size_type nelem);//deque应该可以包含eElem个元素，分配合适数量的缓冲区（最小一般不能小于8），算出来的缓冲区个数需要向上取整，并且最后还要+2，在和8取最大值。定义nstart,nfinsh都是map_pointer,实际指向的分别是应该是正好缓冲区的开始和缓冲区的结束，并对这个[nstart,nfinsh]进行new,然后赋值begin\_和end_的四个成员
        void fill_init(size_type n, const value_type& value);//调用map_init(n)进行初始化，并将对应位置都赋值为value
        template <class Iter>
        void copy_init(Iter first, Iter last);//创建缓冲区，并将对应[first,last)的内容拷贝过来
        void fill_assign(size_type n, const value_type& value);//重新分配缓冲区，并将值都变为value
        template <class Iter>
        void copy_assign(Iter first, Iter last);//重新分配缓冲区，并将值变为[first,last)
        template <class... Args>
        iterator insert_aux(iterator position, Args&& ...args);//在position前面插入一个值，如果position在前半段，就将前面的值向前移动，如果在后半段，就将后面的值向后移动，返回插入的元素的迭代器
        void fill_insert(iterator position, size_type n, const value_type& x);//在position前面插入n个x,如果postion在前半段，则前面元素前移，如果在后半段，则后面元素后移。
        template <class Iter>
        void copy_insert(iterator pos, Iter first, Iter last);//在position前面插入两个迭代器之间的值,如果postion在前半段，则前面元素前移，如果在后半段，则后面元素后移
        template <class Iter>
        void insert_dispatch(iterator pos, Iter first, Iter last);//在iterator前面插入两个迭代器之间的值，左闭右开，如果要插入的位置在前半段，就保证开头位置空间有n个空闲，如果插在后半段，九保证结尾位置空间有n个空闲空间，n是两个迭代器之间元素的个数
        void require_capacity(size_type n, bool front);//front为true表示在队头，为false表示在队尾，看看是否还有n个元素的空间，如果没有，则创建新的缓冲区来确保可以添加n个元素
        void reallocate_map_at_front(size_type need);//重新分配map,map_size的大小变为了原先的2倍和原先大小+need_buffer+DEQUE_MAP_INIT_SIZE的最大值，并且将begin\_和end\_分别执行中间的部分，并且对新的区域进行重新分配新的buffer,并将原先的buffer复制过来，删除原先的map,更新成员变量
        void reallocate_map_at_back(size_type need);//和void        reallocate_map_at_front(size_type need);差不多只不过是在后面加而已

    };

    //接口函数

    template <class T>
    void deque<T>::shrink_to_fit() {
        if(map_ == nullptr) return;
        map_pointer now = map_;
        while(now != begin_.node){
            if(*now != nullptr){
                delete[] *now;
                *now = nullptr;
            }
            ++now;
        }
        now = end_.node + 1;
        while(now != map_ + map_size_){
            if(*now != nullptr){
                delete[] *now;
                *now = nullptr;
            }
            ++now;
        }
    }

    template <class T>
    template <class ...Args>
    typename deque<T>::iterator deque<T>::emplace(iterator pos, Args &&...args) {
        return insert_aux(pos,std::forward<Args...>(args...));
    }

    template <class T>
    typename deque<T>::iterator deque<T>::insert(iterator position, const value_type &value) {
        if(position == begin_){
            push_front(value);
            return begin_;
        }
        else if(position == end_){
            push_back(value);
            return end_ - 1;
        }
        else {
            return insert_aux(position,value);
        }
    }

    template <class T>
    typename deque<T>::iterator deque<T>::insert(iterator position, value_type &&value) {
        if(position == begin_){
            emplace_front(std::move(value));
            return begin_;
        }
        else if(position == end_){
            emplace_back(std::move(value));
            return end_ - 1;
        }
        else {
            return insert_aux(position,std::move(value));
        }
    }

    template <class T>
    void deque<T>::insert(iterator position, size_type n, const value_type& value) {
        fill_insert(position,n,value);
    }

    template <class T>
    typename deque<T>::iterator deque<T>::erase(iterator position) {
        size_type elems_before = position - begin_;
        if(elems_before > (size()) / 2){//后面前移
            iterator now = position;
            while(now != end_ - 1){
                *now = *(now + 1);
                now++;
            }
            end_--;
        }
        else{//前面后移
            iterator now = position;
            while(now != begin_){
                *now = *(now - 1);
                --now;
            }
            begin_++;
        }
        return begin_ + elems_before;
    }

    template <class T>
    typename deque<T>::iterator deque<T>::erase(iterator first, iterator last){
        size_type elems_before = first - begin_;
        size_type elems_after = end_ - last;
        size_type n = last - first;
        if(elems_before > elems_after){//后面前移
            iterator now = first;
            while(now != end_ - n){
                *now = *(now + n);
                ++now;
            }
            end_-=n;
        }
        else{//前面后移
            iterator now = last - 1;
            while(now != begin_ + n - 1){
                *now = *(now - n);
                --now;
            }
            begin_ += n;
        }
        return begin_ + elems_before;
    }






    //********************////////////****************************
    //帮助函数
    template <class T>
    typename deque<T>::map_pointer deque<T>::create_map(size_type size) {
        map_pointer map = new pointer[size];
        for(int i = 0; i < size; ++i)
            map[i] = nullptr;
        return map;
    }

    template <class T>
    void deque<T>::create_buffer(map_pointer nstart, map_pointer nfinsh) {
        while(nstart <= nfinsh){
            *nstart = new value_type[buffer_size];
            nstart++;
        }
    }

    template <class T>
    void deque<T>::destroy_buffer(map_pointer nstart, map_pointer nfinsh) {
        while(nstart <= nfinsh){
            delete *nstart;
            *nstart = nullptr;
            nstart++;
        }
    }

    template <class T>
    void deque<T>::map_init(size_type nelem) {
        size_type buffer_n = nelem / buffer_size + 1;
        map_size_ = std::max(buffer_n, static_cast<size_type>(DEQUE_MAP_INIT_SIZE));
        map_ = create_map(map_size_);

        map_pointer nstart = map_ + (map_size_ - buffer_n) / 2;
        map_pointer nfinish = nstart + buffer_n - 1;

        create_buffer(nstart,nfinish);

        begin_.set_node(nstart);
        end_.set_node(nfinish);
        begin_.cur = begin_.first;
        end_.cur = end_.first + (nelem % buffer_size);
    }

    template <class T>
    void deque<T>::fill_init(size_type nelem, const value_type& value){
        map_init(nelem);
        iterator now = begin_;
        while(now < end_){
            *now = value;
            now++;
        }
    }

    template <class T>
    template <class Iter>
    void deque<T>::copy_init(Iter first, Iter last) {
        difference_type n = mystl::distance(first,last);
        map_init(n);
        iterator now = begin_;
        while(now < end_){
            *now = *first;
            now++;
            first++;
        }
    }

    template <class T>
    void deque<T>::fill_assign(size_type n, const value_type& value){
        if(n > size()){
            iterator now = begin_;
            while(now != end_){
                *now = value;
                ++now;
            }
            insert(end_,n-size(),value);
        }
        else{
            erase(begin_ + n, end_);
            iterator now = begin_;
            while(now != end_){
                *now = value;
                ++now;
            }
        }
    }

    template <class T>
    template <class Iter>
    void deque<T>::copy_assign(Iter first, Iter last) {

        iterator f1 = begin();
        iterator l1 = end();

        while(f1 != l1 && first != last){
            *f1 = *first;
            ++f1;
            ++first;
        }

        if(f1!=l1){
            erase(f1,l1);
        }
        if(first!=last){
            insert_dispatch(l1,first,last);
        }




    }


    template <class T>
    template <class... Args>
    typename deque<T>::iterator deque<T>::insert_aux(iterator position, Args &&...args) {
        size_type elems_before = position - begin_;
        if(elems_before > size() / 2){//放在后半段
            require_capacity(1,false);
            iterator now = end_;
            while(now != position){
                *now = *(now - 1);
                --now;
            }
            *now = value_type(std::forward<Args...>(args...));
            end_++;
            return now;
        }
        else{//放在前半段
            require_capacity(1,true);
            begin_--;
            iterator now = begin_;
            int temp = *now;
            while(now!=position-1){
                *now = *(now+1);
                now++;
                temp = *now;
            }
            *now = value_type(std::forward<Args...>(args...));

            return now;
        }
    }

    template <class T>
    void deque<T>::fill_insert(iterator position, size_type n, const value_type &x) {
        size_type elems_before = position - begin_;
        if(elems_before > size() / 2){//放在后面
            require_capacity(n, false);
            end_ += n;
            iterator now = end_ - 1;
            while(now != position + n - 1){
                *now = *(now - n);
                --now;
            }

            while(now!=position-1){
                *now = x;
                --now;
            }
        }
        else{//放在前面
            require_capacity(n, true);
            begin_ -= n;
            iterator now = begin_;
            while(now != position - n){
                *now = *(now + n);
                ++now;
            }

            while(now!=position){
                *now = x;
                ++now;
            }
        }
    }

    template <class T>
    template <class Iter>
    void deque<T>::copy_insert(iterator pos, Iter first, Iter last) {
        size_type n = mystl::distance(first,last);
        size_type elems_before = pos - begin_;
        if(elems_before > size() / 2) {//后移
            require_capacity(n,false);

            end_ += n;
            iterator now = end_ - 1;
            while(now != pos + n - 1){
                *now = *(now - n);
                now--;
            }
            now = pos;
            while(now != pos + n){
                *now = *first;
                ++now;
                ++first;
            }
        }
        else{//前移
            require_capacity(n, true);
            begin_ -= n;
            iterator now = begin_;
            while(now !=  pos - n){
                *now = *(now + n);
                ++now;
            }
            while(now != pos){
                *now = *first;
                ++now;
                ++first;
            }
        }
    }

    template <class T>
    template <class Iter>
    void deque<T>::insert_dispatch(iterator pos, Iter first, Iter last) {
        copy_insert(pos,first, last);
    }



    template <class T>
    void deque<T>::require_capacity(size_type n, bool front) {
        if(front && static_cast<size_type>(begin_.cur - begin_.first) < n){
            size_type need_buffer = (n - (begin_.cur - begin_.first)) / buffer_size + 1;
            if(need_buffer > begin_.node - map_){
                reallocate_map_at_front(need_buffer - (begin_.node - map_));
            }
            else{
                create_buffer(begin_.node - need_buffer, begin_.node - 1);
            }
        }
        else if(!front && static_cast<size_type>(end_.last - end_.cur - 1) < n){
            size_type need_buffer = (n - (end_.last - end_.cur - 1) ) / buffer_size + 1;
            if(need_buffer > map_size_ - (end_.node - map_ + 1)){
                reallocate_map_at_back(need_buffer - (map_size_ - (end_.node - map_ + 1)) );
            }
            create_buffer(end_.node + 1, end_.node + need_buffer);
        }
    }

    template <class T>
    void deque<T>::reallocate_map_at_front(size_type need) {
        size_type new_map_size  = std::max(map_size_ << 1, map_size_ + need + DEQUE_MAP_INIT_SIZE);

        map_pointer new_map = create_map(new_map_size);

        size_type old_buffer = end_.node - begin_.node + 1;

        map_pointer first = new_map + (new_map_size - need - old_buffer) / 2;
        map_pointer mid = first + need; //[first,mid)
        map_pointer last = mid + old_buffer;//[mid,end)
        for(auto f1 = mid,f2 = begin_.node; f1 != last; ++f1, ++f2)
            *f1 = *f2;

        create_buffer(first,mid-1);

        delete[] map_;
        map_ = new_map;
        map_size_ = new_map_size;
        begin_ = iterator(*mid + (begin_.cur - begin_.first ), mid);
        end_ = iterator(*(last - 1) + (end_.cur - end_.first),last-1);
    }

    template <class T>
    void deque<T>::reallocate_map_at_back(size_type need) {
        size_type new_map_size  = std::max(map_size_ << 1, map_size_ + need + DEQUE_MAP_INIT_SIZE);

        map_pointer new_map = create_map(new_map_size);

        size_type old_buffer = end_.node - begin_.node + 1;

        map_pointer first = new_map + (new_map_size - need - old_buffer) / 2;
        map_pointer mid = first + old_buffer; //[first,mid)
        map_pointer last = mid + need;//[mid,last)
        for(auto f1 = first,f2 = begin_.node; f1 != mid; ++f1, ++f2)
            *f1 = *f2;

        create_buffer(mid,last-1);

        delete[] map_;
//        ::operator delete[](map_);
        map_ = new_map;
        map_size_ = new_map_size;
        begin_ = iterator(*first + (begin_.cur - begin_.first ), first);
        end_ = iterator(*(mid - 1) + (end_.cur - end_.first), mid - 1);
    }




















}//namespace mystl


#endif //MYSTL_DEQUE_H
