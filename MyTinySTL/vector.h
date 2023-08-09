//
// Created by 孙建钊 on 2023/6/22.
//

#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "iterator.h"
#include "exceptdef.h"

namespace mystl{
    template <class T>
    class vector{
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T& const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef value_type* iterator;
        typedef const value_type* const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef size_t size_type;

    private:
        //begin_==end_表示当前内容为空，end_为实际元素的后一个节点
        //cap_实际的值等于begin_+cap 也就是cap_的位置并不在真实数组中，而是在数组的结尾的下一个节点位置
        iterator begin_;//表示目前使用空间的头部
        iterator end_;//表示目前使用空间的尾部
        iterator cap_; //表示目前存储空间的尾部
    public:
        vector(){
            try_init();
        }

        explicit vector(size_type n){
            fill_init(n,T());
        }

        vector(size_type n, const value_type& value){
            fill_init(n,value);
        }

        vector(const vector& rhs){
            range_init(rhs.begin(),rhs.end());
        }

        vector(vector&& rhs)  noexcept : begin_(rhs.begin_),end_(rhs.end_),cap_(rhs.cap_){
            rhs.begin_ = nullptr;
            rhs.end_ = nullptr;
            rhs.cap_ = nullptr;
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type = 0
                >
        vector(Iter first, Iter last){
            range_init(first,last);
        }

        vector(std::initializer_list<value_type> ilst){
            range_init(ilst.begin(),ilst.end());
        }

        ~vector(){
            delete[] begin_;
            begin_ = nullptr;
            end_ = nullptr;
            cap_ = nullptr;
        }

        vector& operator=(const vector& rhs);
        vector& operator=(vector&& rhs);

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

        reverse_iterator rend() noexcept{
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const noexcept{
            return const_reverse_iterator(begin());
        }
        reverse_iterator rbegin() noexcept{
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const noexcept{
            return const_reverse_iterator(end());
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

        bool empty() const{
            return begin_ == end_;
        }

        size_type size()const{
            return (end_ - begin_);
        }

        size_type max_size(){
            return static_cast<size_type >(-1)/sizeof(T);
        }

        size_type capacity(){
            return (cap_ - begin_);
        }

        reference front(){
            MYSTL_DEBUG(begin_ != end_);
            return *begin_;
        }
        const_reference front() const{
            MYSTL_DEBUG(begin_ != end_);
            return *begin_;
        }
        reference back(){
            MYSTL_DEBUG(begin_ != end_);
            return *(end_ - 1);
        }
        const_reference back() const{
            MYSTL_DEBUG(begin_ != end_);
            return *(end_ - 1);
        }

        pointer data(){
            MYSTL_DEBUG(begin_ != end_);
            return begin_;
        }

        const_pointer data() const noexcept{
            MYSTL_DEBUG(begin_ != end_);
            return begin_;
        }

        reference operator[](size_type n){
            MYSTL_DEBUG(n < size());
            return *(begin_+n);
        }

        const_reference operator[](size_type n) const{
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }

        reference at(size_type n){
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }
        const_reference at(size_type n) const{
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }

        void reserve(size_type n);//当原容量大小小于n时会重新分配，新的容量应该大于等于n





        void push_back(const value_type& value);
        void pop_back();

        void pop_front(){
            erase(begin());
        }

        void push_back(value_type&& value){
            emplace(end(),std::move(value));
        }



        void swap(vector<T>& rhs) noexcept{
            if(this != &rhs){
                mystl::swap(rhs.begin_,begin_);
                mystl::swap(rhs.end_,end_);
                mystl::swap(rhs.cap_,cap_);
            }
        }

        void assign(size_type n, const value_type& value){//重新分配空间，包含n个内容为value的元素
            fill_assign(n,value);
        }

        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type=0>
        void assign(Iter first, Iter last){
            copy_assign(first,last);
        }

        void assign(std::initializer_list<value_type> il){
            copy_assign(il.begin(),il.end());
        }

        void clear();
        iterator insert(const_iterator pos, const value_type& value);//在pos前面插入一个元素，并返回这个元素的迭代器
        iterator insert(const_iterator pos, size_type n, const value_type& value);//在pos前面插入n个元素，并返回这n个元素中第一个元素的迭代器
        template <class Iter, typename std::enable_if<
                mystl::is_input_iterator<Iter>::value,int>::type = 0>
        void insert(const_iterator pos, Iter first, Iter last){
            copy_insert(const_cast<iterator>(pos),first,last);
        }
        iterator insert(const_iterator pos, value_type&& value){
            return emplace(pos,std::move(value));
        }

        void shrink_to_fit(){//放弃多余的容量，使得容器容量变为和size一样的大小
            reinsert(size());
        }

        template<class ...Args>
        iterator emplace(const_iterator pos, Args&& ...args);//如果pos是尾迭代器，直接在这个位置构造元素，并且尾迭代器后移；在pos位置构造元素，原先的元素和后面的都统一后移
        template <class ...Args>
        iterator emplace_back(Args&& ...args){
            return emplace(end(),std::forward<Args...>(args...));
        }

        void resize(size_type new_size){resize(new_size, value_type());}
        void resize(size_type new_size, const value_type& value);//重置容器大小，如果size大于new_size删除对应的元素即可，如果小于new_size,则在后面使用value进行填充

        iterator erase(const_iterator pos);//删除指定位置的元素，使得后面的元素前移一位，并且返回删除位置的迭代器
        iterator erase(const_iterator first, const_iterator last);//删除指定范围的元素，并返回删除元素后的first的迭代器

        void reverse();//容器元素反转

    public:
        //帮助函数
        void try_init() noexcept;//尝试初始化一个16个元素的空间
        void init_space(size_type size, size_type cap);//分配cap大小空间，然后将end_调整到size的位置
        void fill_init(size_type n, const value_type& value);//初始化包含n个value的的vector,如果小于16就开辟16个元素空间，如果大于16，则开辟n个空间，然后将前n个元素进行初始化
        template <class Iter>
        void range_init(Iter first, Iter last);//这个和fill_init(size_type n, const value_type& value)函数一样，只不过每个元素的值为两个迭代器之间的值
        size_type get_new_cap(size_type add_size);//根据给出的增加的大小返回一个新的容器的大小；如果old_size*1.5>max_size() 此时返回的是 old_size+add_size（+16）如果加上16之后没有超过max_size()的话；如果old_size * 1.5< max_size() 此时会根据old_size是否为0，如果为0则返回16 和add_size的最大值，如果不为0，则返回1.5old_size和old_size+add_size的最大值
        void fill_assign(size_type n, const value_type& value);//对vector进行重新分配，如果n>capacity()则重新分配一个新空间的为n,然后为其进行赋值为value;如果只是大于size()则只需要将其前n个进行填充替换即可;如果n<size()，则填充前n个后面的则删除
        iterator fill_insert(iterator pos, size_type n, const value_type& value);//就是在pos前面插入n个value,返回插入元素的迭代器
        template <class Iter>
        void copy_insert(iterator pos, Iter first, Iter last);//和fill_insert(iterator pos, size_type n, const value_type& value)类似，只是使用两个迭代器之间的值进行赋值[first,last)
        void reinsert(size_type new_size);//申请vector大小为size,然后将原先的内容move过来新的数组空间
        template <class Iter>
        void copy_assign(Iter first, Iter last);
    };

    template <class T>
    vector<T>& vector<T>::operator=(const vector <T> &rhs) {
        int len = rhs.size();
        if(len > capacity()){
            //此时不需要重新分配内存
            vector<T> temp(rhs.begin(),rhs.end());
            swap(temp);
        }
        else{//此时的内存空间足够，不需要重新申请
            clear();
            for(auto it = rhs.begin(); it != rhs.end(); ++it){
                *end_ = *it;
                end_++;
            }
        }
        return *this;
    }


    template <class T>
    vector<T>& vector<T>::operator=(vector<T> &&rhs) {
        delete[] begin_;
        begin_ = rhs.begin_;
        end_ = rhs.end_;
        cap_ = rhs.cap_;
        rhs.begin_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
        return *this;
    }

    template <class T>
    void vector<T>::reserve(size_type n) {
        if(n > capacity()){
            size_type new_size = get_new_cap(n - capacity());
            iterator temp = new T[new_size];
            size_type size_ = size();
            for(int i = 0; i < size_; ++i){
                temp[i] = std::move(*(begin_ +  i));
            }
            clear();
//            delete[] begin_;
            ::operator delete[](begin_);
            begin_ = temp;
            end_ = begin_ + size_;
            cap_ = begin_ + new_size;

        }
    }

    template <class T>
    void vector<T>::push_back(const value_type& value){
        if(end_ != cap_){//此时可以继续插入
            *end_ = value;
            end_++;
        }
        else{//此时不能继续插入，需要重新分配空间
            reserve(capacity() + 1);
            *end_ = value;
            end_++;
        }
    }

    template <class T>
    void vector<T>::pop_back() {
        MYSTL_DEBUG(begin_ != end_);
        end_--;
        end_->~T();
    }

    template <class T>
    void vector<T>::clear(){
        iterator now = begin_;
        while(now != end_){
            now->~T();//调用析构函数进行释放
            now++;
        }
        end_ = begin_;
    }




    template <class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value){
        if(end_ == cap_){
            reserve(size() + 1);
        }
        iterator dpos = end_;
        end_++;
        while(dpos != pos){
            *dpos = *(dpos-1);
            dpos-=1;
        }
        *dpos = value;
        return dpos;
    }

    template <class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, size_type n, const value_type& value){
        return fill_insert(const_cast<iterator>(pos),n,value);
    }

    template <class T>
    template <class ...Args>
    typename vector<T>::iterator vector<T>::emplace(const_iterator pos, Args&& ...args){
        if(end_ == cap_){
            size_type  pos_n = pos - begin_;
            reserve(size()+1);
            pos = begin_ + pos_n;
        }
        iterator dpos = end_;
        end_++;
        while(dpos!=pos){
            *dpos = *(dpos-1);
            dpos--;
        }
        *dpos = value_type(std::forward<Args...>(args...));
        return dpos;
    }

    template <class T>
    void vector<T>::resize(size_type new_size, const value_type& value){
        if(size() > new_size){
            erase(begin_+new_size,end_);
        }
        else{
            insert(end_,new_size - size(), value);
        }
    }

    template <class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
        iterator dpos = begin_ + (pos - begin_);
        end_ -= 1;
        while(dpos != end_){
            *dpos = *(dpos+1);
            dpos+=1;
        }
        return begin_+(pos - begin_);
    }

    template <class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last) {
        size_type delete_num = mystl::distance(first, last);
        iterator pos = begin_ + (first - begin_);
        end_ -= delete_num;
        while(pos != end_){
            *pos = *(pos + delete_num);
            pos+=1;
        }
        return begin_ + (first - begin_);
    }

    template <class T>
    void vector<T>::reverse() {
        iterator first = begin();
        iterator last = end() - 1;
        T temp;
        while(first != last && first != last - 1){
            temp = std::move(*first);
            *first = std::move(*last);
            *last = std::move(temp);
            ++first;
            --last;
        }

    }




    //帮助函数********************************

    //初始化一个含有16个元素的空间
    template <class T>
    void vector<T>::try_init() noexcept {
        begin_ = new T[16];
        end_ = begin_;//此时空间为空
        cap_ = end_ + 16;
    }

    template <class T>
    void vector<T>::init_space(size_type size, size_type cap) {
        delete[] begin_;
        begin_ = new T[cap];
        end_ = begin_ + size;
        cap_ = begin_ + cap;
    }

    template <class T>
    void vector<T>::fill_init(size_type n, const value_type &value) {
        if(n < 16) {
            try_init();
        }
        else {
            begin_ = new T[n];
            end_ = begin_;
            cap_ = begin_ + n;
        }
        while(n){
            *end_ = value;
            end_++;
            n--;
        }
    }

    template <class T>
    template <class Iter>
    void vector<T>::range_init(Iter first, Iter last) {
        size_type n = mystl::distance(first,last);
        if(n < 16) {
            try_init();
        }
        else {
            begin_ = new T[n];
            end_ = begin_;
            cap_ = begin_ + n;
        }
        while(n){
            *end_ = *first;
            ++first;
            end_++;
            n--;
        }
    }

    template <class T>
    typename vector<T>::size_type vector<T>::get_new_cap(size_type add_size) {
        size_type old_size = capacity();
        if(old_size + old_size / 2 > max_size()){
            return old_size + add_size + 16 > max_size() ? old_size + add_size : old_size + add_size + 16;
        }
        else{
            return old_size == 0 ? std::max(add_size,static_cast<size_type>(16)) :
            std::max(old_size + old_size / 2 , old_size + add_size);
        }
    }

    template <class T>
    void vector<T>::fill_assign(size_type n, const value_type &value) {
        if(capacity() >= n){
            clear();
            while(n--){
                *end_ = value;
                end_++;
            }
        }
        else{
            //重新分配空间
            clear();
            size_type new_size = get_new_cap(n - capacity());
            iterator new_begin = new T[new_size];
            for(int i = 0; i < n; ++i){
                new_begin[i] = T(value);
            }
            delete[] begin_;
            begin_ = new_begin;
            end_ = begin_ + n;
            cap_ = begin_ + new_size;
        }
    }

    template <class T>
    typename vector<T>::iterator  vector<T>::fill_insert(iterator pos, size_type n, const value_type& value){
        if(n == 0) return pos;
        if(size() + n > capacity()){
            size_type pos_n = pos - begin_;
            reserve(size() + n);
            pos = begin_ + pos_n;
        }

        end_ += n;

        iterator dpos = end_ - 1;

        while(dpos != pos + n - 1){
            *dpos = *(dpos - n);
            dpos--;
        }
        while(n--){
            *dpos = value;
            dpos--;
        }
        return dpos + 1;

    }

    template <class T>
    template <class Iter>
    void vector<T>::copy_insert(iterator pos, Iter first, Iter last) {
        size_type n = mystl::distance(first,last);
        if(n == 0) return ;

        if(size() + n > capacity()){
            reserve(size() + n);
        }

        end_ += n;
        iterator dpos = end_ - 1;
        while(dpos != pos + n - 1){
            *dpos = *(dpos - n);
            dpos--;
        }
        while(n--){
            *pos = *first;
            ++pos;
            ++first;
        }
        return ;
    }

    template <class T>
    void vector<T>::reinsert(size_type new_size) {
        iterator new_begin = new T[new_size];
        size_type size_ = size();
        for(int i = 0 ; i < size_; ++i){
            new_begin[i] = std::move(begin_[i]);
        }
        ::operator delete[](begin_);
        begin_ = new_begin;
        end_ = begin_ + size_;
        cap_ = begin_ + new_size;
    }

    template <class T>
    template <class Iter>
    void vector<T>::copy_assign(Iter first, Iter last) {
        size_type n = mystl::distance(first,last);
        if(n > capacity()){
            reserve(n);
        }
        iterator pos = begin_;
        while(first != last){
            *pos = *first;
            ++first;
            ++pos;
        }
        end_ = begin_ + n;
    }












}//namespace mystl






#endif //MYSTL_VECTOR_H
