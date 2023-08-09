//
// Created by 孙建钊 on 2023/8/3.
//

#ifndef MYSTL_SET_H
#define MYSTL_SET_H


#include "functional.h"
#include "rb_tree.h"

namespace mystl{

    //参数1 键值类型  参数2 简直不i交房时
    template<class Key, class Compare = mystl::less<Key>>
    class set{

    public:
        typedef Key key_type;
        typedef Key value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;

        typedef mystl::rb_tree<value_type, Compare> base_type;

        typedef typename base_type::node_type node_type;
        typedef typename base_type::node_ptr node_ptr;
        typedef typename base_type::pointer pointer;
        typedef typename base_type::const_pointer const_pointer;
        typedef typename base_type::reference reference;
        typedef typename base_type::const_reference const_reference;
        typedef typename base_type::iterator iterator;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::reverse_iterator reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;
        typedef typename base_type::size_type size_type;
        typedef typename base_type::difference_type difference_type;



    private:
        base_type tree_;



    public:

        set() = default;
        template <class Iter>
        set(Iter first, Iter last):tree_(){
            tree_.insert_unique(first,last);
        }
        set(std::initializer_list<value_type> ilist):tree_(){
            tree_.insert_unique(ilist.begin(),ilist.end());
        }
        set(const set& rhs):tree_(rhs.tree_){}
        set(set&& rhs):tree_(std::move(rhs.tree_)){
//            rhs.tree_.reset();
        }

        set& operator=(const set& rhs){
            tree_ = rhs.tree_;
            return *this;
        }
        set& operator=(set&& rhs){
            tree_ = std::move(rhs.tree_);
//            rhs.tree_.reset();
            return *this;
        }
        set& operator=(std::initializer_list<value_type> ilist){
            tree_.clear();
            tree_.insert_unique(ilist.begin(),ilist.end());
            return *this;
        }

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
        iterator insert(iterator hint, const value_type& value){
            return tree_.insert_unique(hint,value);
        }
        iterator insert(iterator hint, value_type&& value){
            return tree_.insert_unique(hint,std::move(value));
        }
        template <class Iter>
        void insert(Iter first, Iter last){
            tree_.insert_unique(first,last);
        }
        void erase(iterator position){
            tree_.erase(position);
        }
        size_type erase(const key_type& key){
            return tree_.erase_unique(key);
        }
        void erase(iterator first, iterator last){
            tree_.erase(first,last);
        }
        void clear(){
            tree_.clear();
        }

        iterator find(const key_type& key){
            return tree_.find(key);
        }
        const_iterator find(const key_type& value) const{
            return tree_.find(std::move(value));
        }

        size_type count(const key_type& key) const {
            return tree_.count_unique(key);
        }
        iterator lower_bound(const key_type& key){
            return tree_.lower_bound(key);
        }
        const_iterator lower_bound(const key_type& key) const{
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
        pair<const_iterator,const_iterator> equal_range(const key_type& key) const{
            return tree_.equal_range_unique(key);
        }
        void swap(set& rhs) noexcept{
            tree_.swap(rhs.tree_);
        }
    public:
        friend bool operator==(const set& lhs, const set& rhs){
            return lhs.tree_ == rhs.tree_;
        }
        friend bool operator!=(const set& lhs, const set& rhs){
            return lhs.tree_ != rhs.tree_;
        }
    };


    template <class Key, class Compare> bool operator==(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs){
        return lhs.tree_ == rhs.tree_;
    }
    template <class Key, class Compare> bool operator<(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs){
        return lhs.tree_ < rhs.tree_;
    }
    template <class Key, class Compare> bool operator!=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs){
        return lhs.tree_ != rhs.tree_;
    }
    template <class Key, class Compare> bool operator>(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs){
        return lhs.tree_ > rhs.tree_;
    }
    template <class Key, class Compare> bool operator<=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs){
        return lhs.tree_ <= rhs.tree_;
    }
    template <class Key, class Compare> bool operator>=(const set<Key, Compare>& lhs, const set<Key, Compare>& rhs){
        return lhs.tree_ >= rhs.tree_;
    }
    template <class Key, class Compare> void swap(set<Key, Compare>& lhs, set<Key, Compare>& rhs) noexcept{
        lhs.swap(rhs);
    }


    //参数1 键值类型 参数2 键值比较方式
    template <class Key, class Compare = mystl::less<Key>>
    class multiset{
        public:

            typedef Key key_type;
            typedef Key value_type;
            typedef Compare key_compare;
            typedef Compare value_compare;

            typedef mystl::rb_tree<value_type, key_compare> base_type;

            typedef typename base_type::node_type node_type;
            typedef typename base_type::node_ptr node_ptr;
            typedef typename base_type::pointer pointer;
            typedef typename base_type::const_pointer const_pointer;
            typedef typename base_type::reference reference;
            typedef typename base_type::const_reference const_reference;
            typedef typename base_type::iterator iterator;
            typedef typename base_type::const_iterator const_iterator;
            typedef typename base_type::reverse_iterator reverse_iterator;
            typedef typename base_type::const_reverse_iterator const_reverse_iterator;
            typedef typename base_type::size_type size_type;
            typedef typename base_type::difference_type difference_type;

        private:
            base_type tree_;//红黑树结构

        public:
            multiset() = default;
            template <class Iter>
            multiset(Iter first, Iter last):tree_(){
                tree_.insert_multi(first,last);
            }
            multiset(std::initializer_list<value_type> ilist):tree_(){
                tree_.insert_multi(ilist.begin(),ilist.end());
            }
            multiset(const multiset& rhs):tree_(rhs.tree_){}
            multiset(multiset&& rhs):tree_(std::move(rhs.tree_)){
//                rhs.tree_.reset();
            }

            multiset& operator=(const multiset& rhs){
                tree_ = rhs.tree_;
                return *this;
            }
            multiset& operator=(multiset&& rhs){
                tree_ = std::move(rhs.tree_);
//                rhs.tree_.reset();
                return *this;
            }


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
                return tree_.template emplace_multi_use_hint(hint, std::forward<Args>(args)...);
            }

            iterator insert(const value_type& value){
                return tree_.insert_multi(value);
            }
            iterator insert(value_type&& value){
                return tree_.insert_multi(std::move(value));
            }
            iterator insert(iterator hint, const value_type& value){
                return tree_.insert_multi(hint, value);
            }
            iterator insert(iterator hint, value_type&& value){
                return tree_.insert_multi(hint, std::move(value));
            }
            template <class Iter>
            void insert(Iter first, Iter last){
                tree_.insert_multi(first,last);
            }

            void erase(iterator position){
                tree_.erase(position);
            }
            size_type erase(const key_type& key){
                return tree_.erase_multi(key);
            }
            void erase(iterator first, iterator last){
                tree_.erase(first,last);
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
            size_type count(const key_type& key) const {return tree_.count_multi(key);}

            iterator lower_bound(const key_type& key){
                return tree_.lower_bound(key);
            }
            const_iterator lower_bound(const key_type& key) const{
                return tree_.lower_bound(key);
            }
            iterator upper_bound(const key_type& key){
                return tree_.upper_bound(key);
            }
            const_iterator upper_bound(const key_type& key) const{
                return tree_.upper_bound(key);
            }

            pair<iterator,iterator> equal_range(const key_type& key){
                return tree_.equal_range_multi(key);
            }
            pair<iterator,iterator> equal_range(const key_type& key) const{
                return tree_.equal_range_multi(key);
            }

            void swap(multiset& rhs) noexcept{
                tree_.swap(rhs.tree_);
            }

        public:
            friend bool operator==(const multiset& lhs, const multiset& rhs ){
                return lhs.tree_ == rhs.tree_;
            }
            friend bool operator!=(const multiset& lhs, const multiset& rhs ){
                return lhs.tree_ != rhs.tree_;
            }

        };



    template <class Key, class Compare> bool operator==(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs){
        return lhs.tree_ == rhs.tree_;
    }
    template <class Key, class Compare> bool operator<(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs){
        return lhs.tree_ < rhs.tree_;
    }
    template <class Key, class Compare> bool operator!=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs){
        return lhs.tree_ != rhs.tree_;
    }
    template <class Key, class Compare> bool operator>(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs){
        return lhs.tree_ > rhs.tree_;
    }
    template <class Key, class Compare> bool operator<=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs){
        return lhs.tree_ <= rhs.tree_;
    }
    template <class Key, class Compare> bool operator>=(const multiset<Key, Compare>& lhs, const multiset<Key, Compare>& rhs){
        return lhs.tree_ >= rhs.tree_;
    }
    template <class Key, class Compare> void swap(multiset<Key, Compare>& lhs, multiset<Key, Compare>& rhs) noexcept{
        lhs.swap(rhs);
    }



}




#endif //MYSTL_SET_H
