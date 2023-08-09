//
// Created by 孙建钊 on 2023/8/2.
//

#ifndef MYSTL_MAP_H
#define MYSTL_MAP_H


#include "functional.h"
#include "util.h"
#include "rb_tree.h"
#include "exceptdef.h"

namespace mystl{

    //参数一代表键值类型，参数二表示实值类型，参数三表示键值的比较方式
    template<class Key, class T, class Compare = mystl::less<Key>>
    class map{

    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef mystl::pair<Key, T> value_type;
        typedef Compare key_compare;

        typedef mystl::rb_tree<value_type, key_compare> base_type;

        typedef typename base_type::node_type node_type;
        typedef typename base_type::pointer pointer;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::reference reference;
        typedef typename base_type::const_reference const_reference;
        typedef typename base_type::iterator iterator;
        typedef typename base_type::reverse_iterator reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;
        typedef typename base_type::size_type size_type;
        typedef typename base_type::difference_type difference_type;


    private:

        base_type tree_;//红黑树

    public:

        map() = default;
        template <class Iter>
        map(Iter first, Iter last):tree_(){
            tree_.insert_unique(first, last);
        }
        map(std::initializer_list<value_type> ilist):tree_(){
            tree_.insert_unique(ilist.begin(),ilist.end());
        }
        map(const map& rhs):tree_(rhs.tree_){}
        map(map&& rhs) noexcept: tree_(mystl::move((rhs.tree_))){}
        map& operator=(const map& rhs){
            tree_ = rhs.tree_;
            return *this;
        }
        map& operator=(map&& rhs){
            tree_ = std::move(rhs.tree_);
            return *this;
        }
        map& operator=(std::initializer_list<value_type> ilist){
            tree_.clear();
            tree_.insert_unique(ilist.begin(),ilist.end());
            return *this;
        }
        key_compare key_comp() const {return tree_.key_comp();}

        iterator begin() noexcept{return tree_.begin();}
        const_iterator begin() const noexcept{return tree_.begin();}
        iterator end() noexcept{return tree_.end();}
        const_iterator end() const noexcept{return tree_.end();}
        reverse_iterator rbegin() noexcept{return tree_.rbegin();}
        const_reverse_iterator rbegin() const noexcept{return tree_.rbegin();}
        reverse_iterator rend() noexcept{return tree_.rend();}
        const_reverse_iterator rend() const noexcept{return tree_.rend();}
        const_iterator cbegin() const noexcept{return tree_.cbegin();}
        const_iterator cend() const noexcept{return tree_.cend();}
        const_reverse_iterator crbegin() const noexcept{return tree_.crbegin();}
        const_reverse_iterator crend() const noexcept{return tree_.crend();}

        bool empty() const noexcept{return tree_.empty();}
        size_type size() const noexcept{return tree_.size();}
        size_type max_size() const noexcept{return tree_.max_size();}

        mapped_type& at(const key_type& key){
            iterator it = tree_.find(key);
            MYSTL_DEBUG(it != end());
            return it->second;
        }

        const mapped_type& at(const key_type& key) const{
            iterator it = tree_.find(key);
            MYSTL_DEBUG(it != end());
            return it->second;
        }

        mapped_type& operator[](const key_type& key){
            iterator it = tree_.find(key);
            if(it == end()){
                it = tree_.template emplace_unique(key,T());
            }
            return it->second;
        }

        mapped_type& operator[](key_type&& key){
            iterator it = tree_.find(std::move(key));
            if(it == end()){
                auto p = tree_.template emplace_unique(key,T());
                it = p.first;
            }
            return it->second;
        }

        template <class ...Args>
        pair<iterator,bool> emplace(Args&& ...args){
            return tree_.template emplace_unique(std::forward<Args>(args)...);
        }
        template <class ...Args>
        iterator emplace_hint(iterator hint, Args&& ...args){
            return tree_.template emplace_unique_use_hint(hint,std::forward<Args>(args)...);
        }

        pair<iterator,bool> insert(const value_type& value){
            return tree_.insert_unique(value);
        }
        pair<iterator,bool> insert(value_type&& value){
            return tree_.insert_unique(std::move(value));
        }
        iterator insert(iterator hint, value_type& value){
            return tree_.insert_unique_use_hint(hint,value);
        }
        iterator insert(iterator hint, value_type&& value){
            return tree_.emplace_multi_use_hint(hint,std::move(value));
        }
        template <class Iter>
        void insert(Iter first, Iter last){
            tree_.insert_unique(first, last);
        }
        void erase(iterator position){
            tree_.erase(position);
        }
        size_type erase(const key_type& key){
            return tree_.erase_unique(key);
        }
        void erase(iterator first, iterator last){
            tree_.erase(first, last);
        }
        void clear(){
            tree_.clear();
        }
        iterator find(const key_type& key){
            return tree_.find(key);
        }
        const_iterator find(const key_type& key) const{
            return tree_.find(key);
        }
        size_type count(const key_type& key) const{
            return tree_.count_unique(key);
        }
        iterator lower_bound(const key_type& key){
            return tree_.lower_bound(key);
        }
        const_iterator lower_bound(const key_type& key) const {
            return tree_.lower_bound(key);
        }
        iterator upper_bound(const key_type& key){
            return tree_.upper_bound(key);
        }
        const_iterator upper_bound(const key_type& key) const{
            return tree_.upper_bound(key);
        }
        pair<iterator,iterator> equal_range(const key_type& key){
            return tree_.equal_range_unique(key);
        }
        pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
            return tree_.equal_range_unique(key);
        }
        void swap(map& rhs) noexcept{
            tree_.swap(rhs.tree_);
        }

    public:
        friend bool operator==(const map& lhs, const map& rhs){
            return lhs.tree_ == rhs.tree_;
        }
        friend bool operator!=(const map& lhs, const map& rhs){
            return lhs.tree_ != rhs.tree_;
        }

    };

    template <class Key, class T, class Compare> bool operator==(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs){
        return lhs.tree_ == rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator<(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs){
        return lhs.tree_ < rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator!=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs){
        return lhs.tree_!=rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator>(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs){
        return lhs.tree_ > rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator<=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs){
        return lhs.tree_ <= rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator>=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs){
        return lhs.tree_ >= rhs.tree_;
    }
    template <class Key, class T, class Compare> void swap(map<Key, T, Compare>& lhs, map<Key, T, Compare>& rhs) noexcept{
        lhs.swap(rhs);
    }



    //参数1 键值类型， 参数2 实值类型， 参数3 键值比较方式
    template <class Key, class T, class Compare = mystl::less<Key>>
    class multimap{

        public:
            typedef Key key_type;
            typedef T mapped_type;
            typedef mystl::pair<Key, T> value_type;
            typedef Compare key_compare;

            typedef mystl::rb_tree<value_type, key_compare> base_type;

        typedef typename base_type::node_type node_type;
        typedef typename base_type::pointer pointer;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::reference reference;
        typedef typename base_type::const_reference const_reference;
        typedef typename base_type::iterator iterator;
        typedef typename base_type::reverse_iterator reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;
        typedef typename base_type::size_type size_type;
        typedef typename base_type::difference_type difference_type;


        private:
            base_type tree_;//红黑树结构

        public:
            multimap() = default;
            template <class Iter>
            multimap(Iter first, Iter last):tree_(){
                tree_.insert_multi(first, last);
            }
            multimap(std::initializer_list<value_type> ilist):tree_(){
                tree_.insert_multi(ilist.begin(), ilist.end());
            }
            multimap(const multimap& rhs):tree_(rhs.tree_){}
            multimap(multimap&& rhs) noexcept:tree_(std::move(rhs.tree_)){}
            multimap& operator=(const multimap& rhs){
                tree_ = rhs.tree_;
                return *this;
            }
            multimap& operator=(multimap&& rhs){
                tree_ = std::move(rhs.tree_);
                return *this;
            }
            multimap& operator=(std::initializer_list<value_type> ilist){
                tree_.clear();
                tree_.insert_multi(ilist.begin(),ilist.end());
                return *this;
            }
            key_compare key_comp() const {return tree_.key_comp();}

        iterator begin() noexcept{return tree_.begin();}
        const_iterator begin() const noexcept{return tree_.begin();}
        iterator end() noexcept{return tree_.end();}
        const_iterator end() const noexcept{return tree_.end();}
        reverse_iterator rbegin() noexcept{return tree_.rbegin();}
        const_reverse_iterator rbegin() const noexcept{return tree_.rbegin();}
        reverse_iterator rend() noexcept{return tree_.rend();}
        const_reverse_iterator rend() const noexcept{return tree_.rend();}
        const_iterator cbegin() const noexcept{return tree_.cbegin();}
        const_iterator cend() const noexcept{return tree_.cend();}
        const_reverse_iterator crbegin() const noexcept{return tree_.crbegin();}
        const_reverse_iterator crend() const noexcept{return tree_.crend();}

        bool empty() const noexcept{return tree_.empty();}
        size_type size() const noexcept{return tree_.size();}
        size_type max_size() const noexcept{return tree_.max_size();}

        template <class ...Args>
        iterator emplace(Args&& ...args){
            return tree_.template emplace_multi(std::forward<Args>(args)...);
        }
        template <class ...Args>
        iterator emplace_hint(iterator hint, Args&& ...args){
            return tree_.template emplace_multi_use_hint(hint,std::forward<Args>(args)...);
        }
        iterator insert(const value_type& value){
            return tree_.insert_multi(value);
        }
        iterator insert(value_type&& value){
            return tree_.insert_multi(std::move(value));
        }
        iterator insert(iterator hint, const value_type& value){
            return tree_.emplace_multi_use_hint(hint, value);
        }
        iterator insert(iterator hint, value_type&& value){
            return tree_.emplace_multi_use_hint(hint, std::move(value));
        }
        template <class Iter>
        void insert(Iter first, Iter last){
            tree_.template insert_multi(first, last);
        }
        void erase(iterator position){
            tree_.erase(position);
        }
        size_type erase(const key_type& key){
            return tree_.erase_multi(key);
        }
        void erase(iterator first, iterator last){
            return tree_.erase(first, last);
        }
        void clear(){
            tree_.clear();
        }
        iterator find(const key_type& key){
            return tree_.find(key);
        }
        const_iterator find(const key_type& key) const {
            tree_.find(key);
        }
        size_type count(const key_type& key) const{
            return tree_.count_multi(key);
        }
        iterator lower_bound(const key_type& key){
            return tree_.lower_bound(key);
        }
        const_iterator lower_bound(const key_type& key) const{
            return lower_bound(key);
        }
        iterator upper_bound(const key_type& key){
            return tree_.upper_bound(key);
        }
        const_iterator upper_bound(const key_type& key) const{
            return tree_.upper_bound(key);
        }
        pair<iterator,iterator> equal_range(const key_type& key) {
            return tree_.equal_range_multi(key);
        }
        pair<const_iterator, const_iterator> equal_range(const key_type& key) const{
            return tree_.equal_range_multi(key);
        }
        void swap(multimap& rhs){
            tree_.swap(rhs.tree_);
        }

        public:
            friend bool operator==(const multimap& lhs, const multimap& rhs){
                return lhs.tree_ == rhs.tree_;
            }
        friend bool operator!=(const multimap& lhs, const multimap& rhs){
            return lhs.tree_ != rhs.tree_;
        }

        };

    template <class Key, class T, class Compare> bool operator==(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs){
        return lhs.tree_ == rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator<(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs){
        return lhs.tree_ < rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator!=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs){
        return lhs.tree_!=rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator>(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs){
        return lhs.tree_ > rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator<=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs){
        return lhs.tree_ <= rhs.tree_;
    }
    template <class Key, class T, class Compare> bool operator>=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs){
        return lhs.tree_ >= rhs.tree_;
    }
    template <class Key, class T, class Compare> void swap(multimap<Key, T, Compare>& lhs, multimap<Key, T, Compare>& rhs) noexcept{
        lhs.tree_.swap(rhs.tree_);
    }





}

#endif //MYSTL_MAP_H
