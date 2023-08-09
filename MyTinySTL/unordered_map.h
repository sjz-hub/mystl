//
// Created by 孙建钊 on 2023/7/22.
//

#ifndef MYSTL_UNORDERED_MAP_H
#define MYSTL_UNORDERED_MAP_H


#include "hashtable.h"


namespace mystl{


    //Key键值类型  T 实值类型 Hash hash函数 KeyEqual键值比较方式
    template <class Key, class T, class Hash = mystl::hash<Key>, class KeyEqual=mystl::equal_to<Key>>
    class unordered_map{

        typedef hashtable<mystl::pair</*const */Key,T>, Hash, KeyEqual> base_type;//sjz
//        typedef hashtable<std::pair</*const */Key,T>, Hash, KeyEqual> base_type;//sjz
        base_type ht_;//hashtable作为底层结构

    public:
        typedef typename base_type::key_type key_type;
        typedef typename base_type::mapped_type mapped_type;
        typedef typename base_type::value_type value_type;
        typedef typename base_type::hasher hasher;
        typedef typename base_type::key_equal key_equal;

        typedef typename base_type::size_type size_type;
        typedef typename base_type::difference_type difference_type;
        typedef typename base_type::pointer pointer;
        typedef typename base_type::reference reference;
        typedef typename base_type::const_reference const_reference;

        typedef typename base_type::iterator iterator;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::local_iterator local_iterator;
        typedef typename base_type::const_local_iterator const_local_iterator;


    public:

        unordered_map():ht_(100,Hash(),KeyEqual()){}
        explicit unordered_map(size_type bucket_count, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(bucket_count,hash,equal){}
        template <class InputIterator>
        unordered_map(InputIterator first, InputIterator last, const size_type bucket_count = 100, const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual()):
        ht_(mystl::max(bucket_count, static_cast<size_type>(mystl::distance(first, last))), hash, equal){
            while(first != last){
                ht_.insert_unique(*first);
                ++first;
            }
        }
        unordered_map(std::initializer_list<value_type> ilist,const size_type bucket_count = 100, const Hash& hash = Hash(),const KeyEqual& equal = KeyEqual()) :
        ht_(mystl::max(bucket_count, static_cast<size_type>(ilist.size())), hash, equal){
            for(auto it = ilist.begin(); it != ilist.end(); ++it){
                ht_.insert_unique(*it);
            }
        }

        unordered_map(const unordered_map& rhs):ht_(rhs.ht_){}

        unordered_map(unordered_map&& rhs):ht_(std::move(rhs.ht_)){}

        unordered_map& operator=(const unordered_map& rhs){
            ht_ = rhs.ht_;
            return *this;
        }

        unordered_map& operator=(unordered_map&& rhs){
            ht_ = std::move(rhs.ht_);
            return *this;
        }

        unordered_map& operator=(std::initializer_list<value_type> ilist){
            ht_.insert_unique(ilist.begin(),ilist.end());
            return *this;
        }

        ~unordered_map() = default;

        iterator begin() noexcept{
            return ht_.begin();
        }

        const_iterator begin() const noexcept{
            return ht_.begin();
        }

        iterator end() noexcept{
            return ht_.end();
        }

        const_iterator end() const noexcept{
            return ht_.end();
        }

        const_iterator cbegin() const noexcept{
            return ht_.cbegin();
        }

        const_iterator cend() const noexcept{
            return ht_.cend();
        }

        bool empty() const noexcept{
            return ht_.empty();
        }

        size_type size() const noexcept{
            return ht_.size();
        }

        size_type max_size() const noexcept{
            return ht_.max_size();
        }

        template <class ...Args>
        pair<iterator,bool> emplace(Args&& ...args){
            return ht_.emplace_unique(std::forward<Args>(args)...);
        }

        template <class ...Args>
        iterator emplace_hint(const_iterator hint, Args&& ...args)
        { return ht_.emplace_unique_use_hint(hint, mystl::forward<Args>(args)...); }

        pair<iterator,bool> insert(const value_type& value){
            return ht_.insert_unique(value);
        }

        pair<iterator,bool> insert(value_type&& value){
            return ht_.insert_unique(std::move(value));
        }

        template <class Iter>
        void insert(Iter first, Iter last){
            ht_.insert_unique(first, last);
        }

        iterator insert(const_iterator hint, const value_type& value)
        { return ht_.insert_unique_use_hint(hint, value); }
        iterator insert(const_iterator hint, value_type&& value)
        { return ht_.emplace_unique_use_hint(hint, mystl::move(value)); }

        void erase(iterator it){
            ht_.erase(it);
        }

        void erase(iterator first, iterator last){
            ht_.erase(first,last);
        }

        size_type erase(const key_type& key){
            return ht_.erase_unique(key);
        }

        void clear(){
            ht_.clear();
        }

        void swap(unordered_map& other) noexcept{
            ht_.swap(other.ht_);
        }

        mapped_type& at(const key_type& key){
            return ht_.find(key)->second;
        }

        const mapped_type& at(const key_type& key) const{
            return ht_.find(key)->second;
        }

        mapped_type& operator[](const key_type& key) {
            return ht_.find(key)->second;
        }

        mapped_type& operator[](key_type&& key){
            return ht_.find(key)->second;
        }

        size_type count(const key_type& key) const{
            return ht_.count(key);
        }

        iterator find(const key_type& key){
            return ht_.find(key);
        }

        const_iterator find(const key_type& key) const{
            return ht_.find(key);
        }

        pair<iterator, iterator> equal_range(const key_type& key){
            return ht_.equal_range_unique(key);
        }

        pair<iterator,iterator> equal_range(const key_type& key) const{
            return ht_.equal_range_unique(key);
        }

        local_iterator begin(size_type n) noexcept{
            return ht_.begin(n);
        }

        const_local_iterator begin(size_type n) const noexcept{
            return ht_.begin(n);
        }

        const_local_iterator cbegin(size_type n) const noexcept{
            return ht_.cbegin(n);
        }

        local_iterator end(size_type n) noexcept{
            return ht_.end();
        }

        const_local_iterator end(size_type n) const noexcept{
            return ht_.end(n);
        }

        const_local_iterator cend(size_type n) const noexcept{
            return ht_.cend(n);
        }

        size_type bucket_count() const noexcept{
            return ht_.bucket_count();
        }

        size_type max_bucket_count() {
            return ht_.max_bucket_count();
        }

        size_type bucket_size(size_type n) const noexcept{
            return ht_.bucket_size(n);
        }

        size_type bucket(const key_type& key) const{
            return ht_.bucket(key);
        }

        float load_factor() const noexcept{
            return ht_.load_factor();
        }

        float max_load_factor() const noexcept{
            return ht_.max_load_factor();
        }

        void max_load_factor(float ml){
            ht_.max_load_factor(ml);
        }

        void rehash(size_type count){
            ht_.rehash(count);
        }

        void reserve(size_type count){
            return ht_.reserve(count);
        }

        hasher hash_fcn() const{
            return ht_.hash_fcn();
        }

        key_equal key_eq() const{
            return ht_.key_eq();
        }


        friend bool operator==(const unordered_map& lhs, const unordered_map& rhs) {
            return lhs.ht_ == rhs.ht_;
        }

        friend bool operator!=(const unordered_map& lhs, const unordered_map& rhs){
            return lhs.ht_ == rhs.ht_;
        }

    };


    //Key键值类型  T 实值类型 Hash hash函数 KeyEqual键值比较方式
    template <class Key, class T, class Hash = mystl::hash<Key>, class KeyEqual = mystl::equal_to<Key>>
    class unordered_multimap{
    private:
        typedef hashtable<mystl::pair</*const*/ Key, T>, Hash, KeyEqual > base_type;
        base_type ht_;

    public:
        typedef typename base_type::key_type key_type;
        typedef typename base_type::mapped_type mapped_type;
        typedef typename base_type::value_type value_type;
        typedef typename base_type::hasher hasher;
        typedef typename base_type::key_equal key_equal;

        typedef typename base_type::size_type size_type;
        typedef typename base_type::difference_type difference_type;
        typedef typename base_type::pointer pointer;
        typedef typename base_type::reference reference;
        typedef typename base_type::const_reference const_reference;

        typedef typename base_type::iterator iterator;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::local_iterator local_iterator;
        typedef typename base_type::const_local_iterator const_local_iterator;

    public:

        unordered_multimap():ht_(100,Hash(),KeyEqual()){}
        explicit unordered_multimap(size_type bucket_count, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(bucket_count,hash,equal){}
        template <class Iter>
        unordered_multimap(Iter first, Iter last, const size_type bucket_count = 100, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(mystl::max(bucket_count,static_cast<size_type>(mystl::distance(first,last))),hash,equal){
            while(first != last){
                ht_.insert_multi(*first);
                ++first;
            }
        }
        unordered_multimap(std::initializer_list<value_type> ilist, const size_type bucket_count=100, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(mystl::max(bucket_count,static_cast<size_type>(ilist.size())),hash,equal){
            for(auto it = ilist.begin(); it != ilist.end(); ++it){
                ht_.insert_multi(*it);
            }
        }
        unordered_multimap(const unordered_multimap& rhs):ht_(rhs.ht_){}
        unordered_multimap(unordered_multimap&& rhs) noexcept:ht_(std::move(rhs.ht_)){}

        unordered_multimap& operator=(const unordered_multimap& rhs){
            ht_ = rhs.ht_;
            return *this;
        }

        unordered_multimap& operator=(unordered_multimap&& rhs){
            ht_ = std::move(rhs.ht_);
            return *this;
        }

        unordered_multimap& operator=(std::initializer_list<value_type> ilist){
            ht_.insert_multi(ilist.begin(), ilist.end());
            return *this;
        }

        ~unordered_multimap() = default;



        iterator begin() noexcept{
            return ht_.begin();
        }

        const_iterator begin() const noexcept{
            ht_.begin();
        }

        iterator end() noexcept{
            return ht_.end();
        }

        const_iterator end() const noexcept{
            return ht_.end();
        }

        const_iterator cbegin() const noexcept{
            return ht_.cbegin();
        }

        const_iterator cend() const noexcept{
            return ht_.cend();
        }

        bool empty() const noexcept{
            return ht_.empty();
        }

        size_type size() const noexcept{
            return ht_.size();
        }

        size_type max_size() const noexcept{
            return ht_.max_size();
        }

        template <class ...Args>
        iterator emplace(Args&& ...args){
            return ht_.emplace_multi(std::forward<Args>(args)...);
        }

        template <class ...Args>
        iterator emplace_hint(const_iterator hint, Args&& ...args)
        { return ht_.emplace_multi_use_hint(hint, mystl::forward<Args>(args)...); }

        iterator insert(const value_type& value){
            ht_.insert_multi(value);
        }

        iterator insert(value_type&& value){
            return ht_.insert_multi(std::move(value));
        }

        iterator insert(const_iterator hint, const value_type& value)
        { return ht_.insert_multi_use_hint(hint, value); }
        iterator insert(const_iterator hint, value_type&& value)
        { return ht_.emplace_multi_use_hint(hint, mystl::move(value)); }

        template <class Iter>
        void insert(Iter first, Iter last){
            ht_.insert_multi(first,last);
        }

        void erase(iterator it){
            ht_.erase(it);
        }

        void erase(iterator first, iterator last){
            ht_.erase(first, last);
        }

        size_type erase(const key_type& key){
            return ht_.erase_multi(key);
        }

        void clear() {
            ht_.clear();
        }

        void swap(unordered_multimap& other) noexcept{
            ht_.swap(other.ht_);
        }

        size_type count(const key_type& key) const{
            return ht_.count(key);
        }

        iterator find(const key_type& key){
            return ht_.find(key);
        }

        const_iterator find(const key_type& key) const{
            return ht_.find(key);
        }

        pair<iterator, iterator> equal_range(const key_type& key){
            return ht_.equal_range_multi(key);
        }

        pair<const_iterator, const_iterator> equal_range(const key_type& key) const{
            return ht_.equal_range_multi(key);
        }

        local_iterator begin(size_type n) noexcept{
            return ht_.begin(n);
        }

        const_local_iterator begin(size_type n) const noexcept{
            return ht_.begin(n);
        }

        const_local_iterator cbegin(size_type n) const noexcept{
            return ht_.cbegin(n);
        }

        local_iterator end(size_type n) noexcept{
            return ht_.end(n);
        }

        const_local_iterator end(size_type n) const noexcept{
            return ht_.end(n);
        }

        const_local_iterator cend(size_type n) const noexcept{
            return ht_.cend(n);
        }

        size_type bucket_count() const noexcept{
            return ht_.bucket_count();
        }

        size_type max_bucket_count() const noexcept{
            return ht_.max_bucket_count();
        }

        size_type bucket_size(size_type n) const noexcept{
            return ht_.bucket_size(n);
        }

        size_type bucket(const key_type& key) const{
            return ht_.bucket(key);
        }

        float load_factor() const noexcept{
            return ht_.load_factor();
        }

        float max_load_factor() const noexcept{
            return ht_.max_load_factor();
        }

        void max_load_factor(float ml){
            ht_.max_load_factor(ml);
        }

        void rehash(size_type count){
            ht_.rehash(count);
        }

        void reserve(size_type count){
            ht_.reserve(count);
        }

        hasher hash_fcn() const{
            return ht_.hash_fcn();
        }

        key_equal key_eq() const{
            return ht_.key_eq();
        }

        friend bool operator==(const unordered_multimap& lhs, const unordered_multimap& rhs){
            return lhs.ht_ == rhs.ht_;
        }

        friend bool operator!=(const unordered_multimap& lhs, const unordered_multimap& rhs){
            return lhs.ht_ != rhs.ht_;
        }

    };






}



#endif //MYSTL_UNORDERED_MAP_H
