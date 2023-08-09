//
// Created by 孙建钊 on 2023/7/24.
//

#ifndef MYSTL_UNORDERED_SET_H
#define MYSTL_UNORDERED_SET_H

#include "hashtable.h"

namespace mystl{


    template <class Key, class Hash = mystl::hash<Key>, class KeyEqual = mystl::equal_to<Key>>
    class unordered_set{
    private:
        typedef hashtable<Key, Hash, KeyEqual> base_type;
        base_type ht_;

    public:

        typedef typename base_type::key_type key_type;
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
        unordered_set():ht_(100,Hash(), KeyEqual()){}
        explicit unordered_set(size_type bucket_count, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(bucket_count,hash,equal){}
        template <class Iter>
        unordered_set(Iter first, Iter last, const size_type bucket_count=100, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(mystl::max(bucket_count,static_cast<size_type>(mystl::distance(first,last))),hash,equal){
            while(first != last){
                ht_.insert_unique(*first);
                ++first;
            }
        }
        unordered_set(std::initializer_list<value_type> ilist, const size_type bucket_count=100, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(mystl::max(bucket_count,static_cast<size_type>(ilist.size())),hash,equal){
            for(auto it = ilist.begin(); it != ilist.end(); ++it){
                ht_.insert_unique(*it);
            }
        }
        unordered_set(const unordered_set& rhs):ht_(rhs.ht_){}
        unordered_set(unordered_set&& rhs)noexcept:ht_(std::move(rhs.ht_)){}
        unordered_set& operator=(const unordered_set& rhs){
            ht_ = rhs.ht_;
            return *this;
        }
        unordered_set& operator=(unordered_set&& rhs){
            ht_ = std::move(rhs.ht_);
            return *this;
        }
        unordered_set& operator=(std::initializer_list<value_type> ilist){
            ht_.clear();
            ht_.insert_unique(ilist.begin(),ilist.end());
            return *this;
        }

        ~unordered_set() = default;


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
        iterator emplace_hint(const_iterator hint,Args&& ...args){
            return ht_.template emplace_unique_use_hint(hint,std::forward<Args>(args)...);
        }
        pair<iterator,bool> insert(const value_type& value){
            return ht_.insert_unique(value);
        }
        pair<iterator,bool> insert(value_type&& value){
            return ht_.insert_unique(std::move(value));
        }
        iterator insert(const_iterator hint, const value_type& value){
            return ht_.insert_unique_use_hint(hint, value);
        }
        iterator insert(const_iterator hint, value_type&& value){
            return ht_.insert_unique_use_hint(hint,std::move(value));
        }
        template <class Iter>
        void insert(Iter first, Iter last){
            ht_.template insert_unique(first, last);
        }
        void erase(iterator it){
            ht_.erase(it);
        }
        void erase(iterator first, iterator last){
            ht_.erase(first, last);
        }
        size_type erase(const key_type& key){
            return ht_.erase_unique(key);
        }
        void clear(){
            ht_.clear();
        }
        void swap(unordered_set& other) noexcept{
            ht_.swap(other.ht_);
        }
        size_type count(const key_type& key) const {
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
        pair<const_iterator, const_iterator> equal_range(const key_type& key) const{
            return ht_.equal_range_unique(key);
        }
        local_iterator begin(size_type n) noexcept{
            return ht_.begin(n);
        }
        const_local_iterator begin(size_type n) const noexcept{
            return ht_.begin(n);
        }
        const_local_iterator cbegin(size_type n) const noexcept{
            return ht_.begin(n);
        }
        local_iterator end(size_type n) noexcept{
            return ht_.end(n);
        }
        const_local_iterator end(size_type n) const noexcept{
            return ht_.end(n);
        }
        const_local_iterator cend(size_type n) const noexcept{
            return ht_.end(n);
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
        void max_load_factor(float ml) noexcept{
            ht_.max_load_factor(ml);
        }
        void rehash(size_type count){
            ht_.rehash(count);
        }
        void reserve(size_type count){
            ht_.reserve(count);
        }
        hasher hash_fcn() const {
            return ht_.hash_fcn();
        }
        key_equal key_eq() const{
            return ht_.key_eq();
        }

        friend bool operator==(const unordered_set& lhs, const unordered_set& rhs){
            return lhs.ht_ == rhs.ht_;
        }
        friend bool operator!=(const unordered_set& lhs, const unordered_set& rhs){
            return lhs.ht_ != rhs.ht_;
        }

    };

    template <class Key, class Hash, class KeyEqual>
    bool operator==(const unordered_set<Key, Hash, KeyEqual>& lhs, const unordered_set<Key, Hash, KeyEqual>& rhs){
        return lhs == rhs;
    }

    template <class Key, class Hash, class KeyEqual>
    bool operator!=(const unordered_set<Key, Hash, KeyEqual>& lhs, const unordered_set<Key, Hash, KeyEqual>& rhs){
        return lhs != rhs;
    }

    template <class Key, class Hash, class KeyEqual>
    void swap(const unordered_set<Key, Hash, KeyEqual>& lhs, const unordered_set<Key, Hash, KeyEqual>& rhs){
        lhs.ht_.swap(rhs.ht_);
    }




    template <class Key, class Hash = mystl::hash<Key>, class KeyEqual=mystl::equal_to<Key>>
    class unordered_multiset{
        private:
            typedef hashtable<Key, Hash, KeyEqual> base_type;
            base_type ht_;

        public:
            typedef typename base_type::key_type key_type;
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
            unordered_multiset():ht_(100,Hash(),KeyEqual()){}
            explicit unordered_multiset(size_type bucket_count, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(bucket_count,hash,equal){}
            template <class Iter>
            unordered_multiset(Iter first, Iter last, const size_type bucket_count=100, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(mystl::max(bucket_count,static_cast<size_type>(mystl::distance(first,last))),hash,equal){
                while(first!=last){
                    ht_.insert_multi(*first);
                    ++first;
                }
            }
            unordered_multiset(std::initializer_list<value_type> ilist, const size_type bucket_count=100, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):ht_(mystl::max(bucket_count,static_cast<size_type>(mystl::distance(ilist.begin(),ilist.end()))),hash,equal){
                for(auto it = ilist.begin(); it != ilist.end(); ++it){
                    ht_.insert_multi(*it);
                }
            }

            unordered_multiset(const unordered_multiset& rhs):ht_(rhs.ht_){}
            unordered_multiset(unordered_multiset&& rhs):ht_(std::move(rhs.ht_)){}

            unordered_multiset& operator=(const unordered_multiset& rhs){
                ht_ = rhs.ht_;
                return *this;
            }

            unordered_multiset& operator=(unordered_multiset&& rhs){
                ht_ = std::move(rhs.ht_);
                return *this;
            }

            unordered_multiset& operator=(std::initializer_list<value_type> ilist){
                ht_.clear();
                for(auto it = ilist.begin(); it != ilist.end(); ++it){
                    ht_.insert_multi(*it);
                }
                return *this;
            }

            ~unordered_multiset() = default;

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
            iterator emplace(Args&& ...args){
                return ht_.emplace_multi(std::move(args)...);
            }

            template <class ...Args>
            iterator emplace_hint(const_iterator hint, Args&& ...args){
                return ht_.template emplace_multi_use_hint(hint,std::move(args)...);
            }

            iterator insert(const value_type& value){
                return ht_.insert_multi(value);
            }

            iterator insert(value_type&& value){
                return ht_.insert_multi(std::move(value));
            }

            iterator insert(const_iterator hint, const value_type& value){
                return ht_.insert_multi_use_hint(hint,value);
            }

            iterator insert(const_iterator hint, value_type&& value){
                return ht_.insert_multi_use_hint(hint, std::move(value));
            }

            template <class Iter>
            void insert(Iter first, Iter last){
                ht_.template insert_multi(first, last);
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

            void clear(){
                ht_.clear();
            }

            void swap(unordered_multiset& other) noexcept{
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

            pair<iterator,iterator> equal_range(const key_type& key){
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

            size_type bucket_count() const {
                return ht_.bucket_count();
            }

            size_type max_bucket_count() {
                return ht_.max_bucket_count();
            }

            size_type bucket_size(size_type n){
                return ht_.bucket_size(n);
            }

            size_type bucket(const key_type& key){
                return ht_.bucket(key);
            }

            float load_factor(){
                return ht_.load_factor();
            }

            float max_load_factor(){
                return ht_.max_load_factor();
            }

            void max_load_factor(float ml) {
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

            friend bool operator==(const unordered_multiset& lhs, const unordered_multiset& rhs){
                return lhs.ht_ == rhs.ht_;
            }

            friend bool operator!=(const unordered_multiset& lhs, const unordered_multiset& rhs){
                return lhs.ht_ != rhs.ht_;
            }

        };


        template <class Key, class Hash, class KeyEqual>
        bool operator==(const unordered_multiset<Key,Hash,KeyEqual>& lhs,const unordered_multiset<Key,Hash,KeyEqual>& rhs){
            return lhs == rhs;
        }

        template <class Key, class Hash, class KeyEqual>
        bool operator!=(const unordered_multiset<Key,Hash,KeyEqual>& lhs,const unordered_multiset<Key,Hash,KeyEqual>& rhs){
            return lhs != rhs;
        }

        template <class Key, class Hash, class KeyEqual>
        void swap(const unordered_multiset<Key,Hash,KeyEqual>& lhs,const unordered_multiset<Key,Hash,KeyEqual>& rhs){
            return lhs.ht_.swap(rhs.ht_);
        }
}


#endif //MYSTL_UNORDERED_SET_H
