//
// Created by 孙建钊 on 2023/7/17.
//

#ifndef MYSTL_HASHTABLE_H
#define MYSTL_HASHTABLE_H


#include <algorithm>
#include "type_traits.h"
#include "iterator.h"
#include "exceptdef.h"
#include "vector.h"
#include "functional.h"
#include "algobase.h"
#include "util.h"




namespace mystl{


    using namespace std;

    //hashtable中节点的定义，开链法解决冲突的节点
    //vector中存放的也是这样的节点
    template<class T>
    struct hashtable_node{
        hashtable_node* next;
        T value;
        hashtable_node() = default;
        hashtable_node(const T& n):next(nullptr), value(n){}

        hashtable_node(const hashtable_node& node):next(node.next),value(node.value){}
        hashtable_node(hashtable_node&& node):next(std::move(node.next)),value(std::move(node.value)){
            node.next = nullptr;
        }
        hashtable_node(T&& value):value(std::move(value)),next(nullptr){}
    };

    template <class T, bool>
    struct ht_value_traits_imp{
        typedef T key_type;
        typedef T mapped_type;
        typedef T value_type;

        template <class Ty>
        static const key_type& get_key(const Ty& value){
            return value;
        }

        template <class Ty>
        static const value_type& get_value(const Ty& value){
            return value;
        }
    };

    template <class T>
    struct ht_value_traits_imp<T, true>{
        typedef typename std::remove_cv<typename T::first_type>::type key_type;
//        typedef typename T::first_type key_type;
        typedef typename T::second_type mapped_type;
        typedef T value_type;

        template <class Ty>
        static const key_type& get_key(const Ty& value){
            return value.first;
        }

        template <class Ty>
        static const value_type& get_value(const Ty& value){
            return value;
        }
    };

    template <class T>
    struct ht_value_traits{
        static constexpr bool is_map = mystl::is_pair<T>::value;

        typedef ht_value_traits_imp<T, is_map> value_traits_type;

        typedef typename value_traits_type::key_type  key_type;
        typedef typename value_traits_type::mapped_type mapped_type;
        typedef typename value_traits_type::value_type value_type;

        template <class Ty>
        static const key_type& get_key(const Ty& value){
            return value_traits_type ::get_key(value);
        }

        template <class Ty>
        static const value_type & get_value(const Ty& value){
            return value_traits_type ::get_value(value);
        }

    };


    template <class T, class HashFun, class KeyEqual>
    class hashtable;

    template <class T, class HashFun, class KeyEqual>
    class ht_iterator;

    template <class T, class HashFun, class KeyEqual>
    class ht_const_iterator;

    template <class T>
    struct ht_local_iterator;

    template <class T>
    struct ht_const_local_iterator;


    template <class T, class Hash, class KeyEqual>
    struct ht_iterator_base: public mystl::iterator<mystl::forward_iterator_tag,T>{

        typedef mystl::hashtable<T, Hash, KeyEqual> hashtable;
        typedef ht_iterator_base<T, Hash, KeyEqual> base;
        typedef mystl::ht_iterator<T, Hash, KeyEqual> iterator;
        typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator;
        typedef hashtable_node<T>* node_ptr;
        typedef hashtable* container_ptr;
        typedef const node_ptr const_node_ptr;
        typedef const container_ptr const_contain_ptr;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        node_ptr node;//迭代器当前节点
        container_ptr ht;//对应的容器 这里加上这个主要是为了可以访问ht的内容

        ht_iterator_base() = default;
        ht_iterator_base(node_ptr n, const container_ptr t):node(n),ht(t){}
        ht_iterator_base(const base& rhs):node(rhs.node),ht(rhs.ht){}

        bool operator==(const base& rhs) const {return node == rhs.node;}
        bool operator!=(const base& rhs) const {return node != rhs.node;}
    };

    template <class T, class Hash, class KeyEqual>
    struct ht_iterator : public ht_iterator_base<T, Hash, KeyEqual>{

        typedef ht_iterator_base<T, Hash, KeyEqual> base;
        typedef typename base::hashtable hashtable;
        typedef typename base::iterator iterator;
        typedef typename base::const_iterator const_iterator;
        typedef typename base::node_ptr node_ptr;
        typedef typename base::container_ptr container_ptr;

        typedef ht_value_traits<T> value_traits;
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        using base::node;
        using base::ht;


        ht_iterator() = default;
        ht_iterator(node_ptr n, container_ptr t): ht_iterator_base<T, Hash, KeyEqual>(n,t){}
        ht_iterator(const iterator& rhs): ht_iterator_base<T, Hash, KeyEqual>(rhs.node,rhs.ht){}
        ht_iterator(const const_iterator& rhs): ht_iterator_base<T, Hash, KeyEqual>(rhs.node,rhs.ht){}

        iterator& operator=(const iterator& rhs){
            node = rhs.node;
            ht = rhs.ht;
            return *this;
        }

        iterator& operator=(const const_iterator& rhs){
            node= rhs.node;
            ht = rhs.ht;
            return this;
        }

        reference operator*() const {
            return node->value;
        }

        pointer operator->() const{
            return &(node->value);
        }

        iterator& operator++(){
            if(node->next != nullptr){
                node = node->next;
            }
            else{//需要看vector的下面的节点的内容
                int index = ht->hash(value_traits::get_key(node->value));
                node = node->next;
                while(!node && ++index< ht->buckets_size_){
                    node = ht->buckets_[index];
                }
            }
            return *this;
        }

        iterator operator++(int){
            iterator temp(*this);
            ++*this;
            return temp;
        }
    };


    template <class T, class Hash, class KeyEqual>
    struct ht_const_iterator : public ht_iterator_base<T, Hash, KeyEqual>{

        typedef ht_iterator_base<T, Hash, KeyEqual> base;
        typedef typename base::hashtable hashtable;
        typedef typename base::iterator iterator;
        typedef typename base::const_iterator const_iterator;
        typedef typename base::node_ptr node_ptr;
        typedef typename base::container_ptr container_ptr;

        typedef ht_value_traits<T> value_traits;
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        using base::node;
        using base::ht;


        ht_const_iterator() = default;
        ht_const_iterator(node_ptr n, const container_ptr t): ht_iterator_base<T, Hash, KeyEqual>(n,t){}
        ht_const_iterator(const iterator& rhs): ht_iterator_base<T, Hash, KeyEqual>(rhs.node,rhs.ht){}
        ht_const_iterator(const const_iterator& rhs): ht_iterator_base<T, Hash, KeyEqual>(rhs.node,rhs.ht){}
        const_iterator& operator=(const iterator& rhs){
            node = rhs.node;
            ht = rhs.ht;
            return *this;
        }
        const_iterator& operator=(const const_iterator& rhs){
            node = rhs.node;
            ht = rhs.ht;
            return *this;
        }

        reference operator*() const{
            return node->value;
        }
        pointer operator->(){
            return &(node->value);
        }

        const_iterator& operator++(){
            if(node->next != nullptr){
                node = node->next;
            }
            else{//需要看vector的下面的节点的内容
                int index = ht->hash(value_traits::get_key(node->value));
                node = node->next;
                while(!node && ++index<ht->buckets_size_){
                    node = ht->buckets_[index];
                }
            }
            return *this;
        }

        const_iterator operator++(int){
            iterator temp(*this);
            ++*this;
            return temp;
        }
    };

    template <class T>
    struct ht_local_iterator: public mystl::iterator<mystl::forward_iterator_tag, T>{

        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef hashtable_node<T>* node_ptr;

        typedef ht_local_iterator<T> self;
        typedef ht_local_iterator<T> local_iterator;
        typedef ht_const_local_iterator<T> const_local_iterator;

        node_ptr node;

        ht_local_iterator(node_ptr n):node(n){}
        ht_local_iterator(const local_iterator& rhs):node(rhs.node){}
        ht_local_iterator(const const_local_iterator& rhs):node(rhs.node){}

        reference operator*() const{
            return node->value;
        }

        pointer operator->() const{
            return &(node->value);
        }

        self& operator++(){
            MYSTL_DEBUG(node != nullptr);
            node = node->next;
            return *this;
        }

        self operator++(int){
            self tmp(*this);
            ++*this;
            return tmp;
        }

        bool operator==(const self& other) const{
            return node == other.node;
        }

        bool operator!=(const self& other) const{
            return node != other.node;
        }

    };


    template <class T>
    struct ht_const_local_iterator: public mystl::iterator<mystl::forward_iterator_tag, T>{

        typedef T                          value_type;
        typedef const value_type*          pointer;
        typedef const value_type&          reference;
        typedef size_t                     size_type;
        typedef ptrdiff_t                  difference_type;
        typedef const hashtable_node<T>*   node_ptr;

        typedef ht_const_local_iterator<T> self;
        typedef ht_local_iterator<T>       local_iterator;
        typedef ht_const_local_iterator<T> const_local_iterator;

        node_ptr node;

        ht_const_local_iterator(node_ptr n):node(n){}
        ht_const_local_iterator(const local_iterator& rhs):node(rhs.node){}
        ht_const_local_iterator(const const_local_iterator& rhs):node(rhs.node){}

        reference operator*() const{
            return node->value;
        }

        pointer operator->() const{
            return &(node->value);
        }

        self& operator++(){
            MYSTL_DEBUG(node != nullptr);
            node = node->next;
            return *this;
        }

        self operator++(int){
            self temp(*this);
            ++*this;
            return temp;
        }

        bool operator==(const self& other) const{
            return node == other.node;
        }

        bool operator!=(const self& other) const{
            return node != other.node;
        }
    };




// bucket 使用的大小

#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
#define SYSTEM_64 1
#else
#define SYSTEM_32 1
#endif

#ifdef SYSTEM_64

#define PRIME_NUM 99

// 1. start with p = 101
// 2. p = next_prime(p * 1.7)
// 3. if p < (2 << 63), go to step 2, otherwise, go to step 4
// 4. end with p = prev_prime(2 << 63 - 1)
    static constexpr size_t ht_prime_list[] = {
            101ull, 173ull, 263ull, 397ull, 599ull, 907ull, 1361ull, 2053ull, 3083ull,
            4637ull, 6959ull, 10453ull, 15683ull, 23531ull, 35311ull, 52967ull, 79451ull,
            119179ull, 178781ull, 268189ull, 402299ull, 603457ull, 905189ull, 1357787ull,
            2036687ull, 3055043ull, 4582577ull, 6873871ull, 10310819ull, 15466229ull,
            23199347ull, 34799021ull, 52198537ull, 78297827ull, 117446801ull, 176170229ull,
            264255353ull, 396383041ull, 594574583ull, 891861923ull, 1337792887ull,
            2006689337ull, 3010034021ull, 4515051137ull, 6772576709ull, 10158865069ull,
            15238297621ull, 22857446471ull, 34286169707ull, 51429254599ull, 77143881917ull,
            115715822899ull, 173573734363ull, 260360601547ull, 390540902329ull,
            585811353559ull, 878717030339ull, 1318075545511ull, 1977113318311ull,
            2965669977497ull, 4448504966249ull, 6672757449409ull, 10009136174239ull,
            15013704261371ull, 22520556392057ull, 33780834588157ull, 50671251882247ull,
            76006877823377ull, 114010316735089ull, 171015475102649ull, 256523212653977ull,
            384784818980971ull, 577177228471507ull, 865765842707309ull, 1298648764060979ull,
            1947973146091477ull, 2921959719137273ull, 4382939578705967ull, 6574409368058969ull,
            9861614052088471ull, 14792421078132871ull, 22188631617199337ull, 33282947425799017ull,
            49924421138698549ull, 74886631708047827ull, 112329947562071807ull, 168494921343107851ull,
            252742382014661767ull, 379113573021992729ull, 568670359532989111ull, 853005539299483657ull,
            1279508308949225477ull, 1919262463423838231ull, 2878893695135757317ull, 4318340542703636011ull,
            6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull
    };

#else

    #define PRIME_NUM 44

// 1. start with p = 101
// 2. p = next_prime(p * 1.7)
// 3. if p < (2 << 31), go to step 2, otherwise, go to step 4
// 4. end with p = prev_prime(2 << 31 - 1)
static constexpr size_t ht_prime_list[] = {
  101u, 173u, 263u, 397u, 599u, 907u, 1361u, 2053u, 3083u, 4637u, 6959u,
  10453u, 15683u, 23531u, 35311u, 52967u, 79451u, 119179u, 178781u, 268189u,
  402299u, 603457u, 905189u, 1357787u, 2036687u, 3055043u, 4582577u, 6873871u,
  10310819u, 15466229u, 23199347u, 34799021u, 52198537u, 78297827u, 117446801u,
  176170229u, 264255353u, 396383041u, 594574583u, 891861923u, 1337792887u,
  2006689337u, 3010034021u, 4294967291u,
};

#endif


    // 找出最接近并大于等于 n 的那个质数
    inline size_t ht_next_prime(size_t n) {
        size_t l = 0;
        size_t r = PRIME_NUM;

        while(l < r) {
            size_t mid = (l + r) >> 1;

            if(ht_prime_list[mid] > n) {
                r = mid;
            }
            else {
                l = mid + 1;
            }
        }
        return ht_prime_list[r];
    }


    template <class T, class Hash, class KeyEqual>
    class hashtable {

        friend struct mystl::ht_iterator<T, Hash, KeyEqual>;
        friend struct mystl::ht_const_iterator<T, Hash, KeyEqual>;

    public:
        typedef ht_value_traits<T> value_traits;
        typedef typename value_traits ::key_type key_type;
        typedef typename value_traits ::mapped_type mapped_type;
        typedef typename value_traits ::value_type value_type;
        typedef Hash hasher;
        typedef KeyEqual key_equal;

        typedef hashtable_node<T> node_type;
        typedef node_type* node_ptr;
        typedef mystl::vector<node_ptr> bucket_type;

        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef ht_iterator<T, Hash, KeyEqual> iterator;
        typedef ht_const_iterator<T, Hash, KeyEqual> const_iterator;
        typedef ht_local_iterator<T> local_iterator;
        typedef ht_const_local_iterator<T> const_local_iterator;


    private:
        bucket_type buckets_;//一个vector
        size_type buckets_size_;
        size_type size_;
        float mlf_;//负载因子
        hasher hash_;
        key_equal equal_;


    private:
        bool is_equal(const key_type& key1, const key_type& key2) {
            return equal_(key1, key2);
        }
        bool is_equal(const key_type& key1, const key_type& key2) const {
            return equal_(key1, key2);
        }
        const_iterator M_cit(node_ptr node) const noexcept {
            return const_iterator(node, this);
        }
        iterator M_begin() noexcept {
            int index = 0;
            while(index < buckets_size_ && buckets_[index] == nullptr)
                ++index;
            if(index >= buckets_size_) return iterator(nullptr,this);
            else return iterator(buckets_[index], this);
        }

        const_iterator M_begin() const noexcept {
            int index = 0;
            while(buckets_[index] == nullptr)
                ++index;
            return const_iterator(buckets_[index], this);
        }

        void init(size_type n) {
            const auto bucket_nums = next_size(n);
            buckets_.reserve(bucket_nums);
            buckets_.assign(bucket_nums, nullptr);
            buckets_size_ = bucket_nums;
        }

        size_type hash(const key_type& key, size_type n) const{
            return hash_(key) % n;
        }
        size_type hash(const key_type& key) const{
            return hash_(key) % buckets_size_;
        }

        size_type next_size(size_type n) const {
            return ht_next_prime(n);
        }

        template <class ...Args>
        node_ptr create_node(Args&& ...args) {

            node_ptr new_node = new node_type(T(std::move(args)...));

//            node_ptr  new_node(std::move(args...));
            return new_node;
        }

        void destroy_node(node_ptr n){//销毁一个节点
            delete n;
        }

        void copy_init(const hashtable& ht);//进行整个赋值

        void rehash_if_need(size_type n);//判断如果当前元素的个数加上n大于bucket_size\_*mlf\_，则重新分配vector,并重新进行hash,重新分配的vector的大小要大于等于size_+n


        void replace_bucket(size_type bucket_count);//重新生成了一个bucket_count大小的vector,并且将原先的元素重新hash之后拷贝了过来，然后让新的vector和原先的vector交换即可，因为原先的vector交换后，等函数的声明周期结束，该变量自动释放了
        pair<iterator,bool> insert_node_unique(node_ptr np);//向hash表中插入一个节点，不允许该节点和已有的节点重复，并返回插入节点（或重复节点）的iterator
        iterator insert_node_multi(node_ptr np);//向hash表中插入一个节点，允许该节点和已有的节点重复，并返回插入节点的iterator
        template <class Iter>
        void copy_insert_multi(Iter first, Iter last);//同时将两个迭代器之间的值插入到hash表中；首先查看是否需要扩充vector（也就是是否需要重新rehash）;<br />上述问题解决之后，则将两个迭代器之间的值插入到hash表中，允许键值重复
        template <class Iter>
        void copy_insert_unique(Iter first, Iter last);//同时将两个迭代器之间的值插入到hash表中；首先查看是否需要扩充vector（也就是是否需要重新rehash）;<br />上述问题解决之后，则将两个迭代器之间的值插入到hash表中，不允许键值重复
        void erase_bucket(size_type n, node_ptr first, node_ptr last);//在第 n 个 bucket 内，删除 [first, last) 的节点
        void erase_bucket(size_t n, node_ptr last);//在第 n 个 bucket 内，删除 [buckets_[n], last) 的节点
        bool equal_to_multi(const hashtable& other);//判断两个hashtable是否相等，也就是存储的元素是否相同，需要考虑重复值
        bool equal_to_unique(const hashtable& other);//判断两个hashtable是否相等，也就是存储的元素是否相同，不需要考虑重复值

    public:


        template <class Iter, typename std::enable_if<   mystl::is_input_iterator<Iter>::value, int>::type = 0>
        hashtable(Iter first, Iter last, size_type bucket_count=100, const Hash& hash = Hash(), const KeyEqual& equal = KeyEqual()):buckets_size_(bucket_count),buckets_(std::max(bucket_count,static_cast<size_type>(mystl::distance(first,last)))),mlf_(1.0),hash_(hash),equal_(equal){
            copy_insert_multi(first,last);
        }


        explicit hashtable(size_type bucket_count, const Hash& hash=Hash(), const KeyEqual& equal=KeyEqual()):mlf_(1.0),size_(0),hash_(hash),equal_(equal){
            init(bucket_count);
        }

        hashtable(const hashtable& rhs):hash_(rhs.hash_),equal_(rhs.equal_){
            copy_init(rhs);
        }
        hashtable(hashtable&& rhs) noexcept:buckets_(std::move(rhs.buckets_)),buckets_size_(std::move(rhs.buckets_size_)),size_(std::move(rhs.size_)),mlf_(std::move(rhs.mlf_)),hash_(std::move(rhs.hash_)),equal_(std::move(equal_)){
            rhs.buckets_size_ = 0;
            rhs.size_ = 0;
            rhs.mlf_ = 0;
        }

        hashtable& operator=(const hashtable& rhs){
            hashtable tmp(rhs);
            swap(tmp);
            return *this;
        }

        hashtable& operator=(hashtable&& rhs) noexcept{
            hashtable tmp(std::move(rhs));
            swap(tmp);
            return *this;
        }

        ~hashtable(){
            clear();
        }

        template <class ...Args>
        iterator emplace_multi(Args&& ...args){
            node_ptr new_node = create_node(std::move(args)...);
            return insert_node_multi(new_node);
        }

        template <class ...Args>
        pair<iterator,bool> emplace_unique(Args&& ...args){
            node_ptr new_node = create_node(std::forward<Args>(args)...);
            return insert_node_unique(new_node);
        }

        template <class ...Args>
        iterator emplace_multi_use_hint(const_iterator hint, Args&& ...args) {
            return emplace_multi(mystl::forward<Args>(args)...);
        }
        template <class ...Args>
        iterator emplace_unique_use_hint(const_iterator /*hint*/, Args&& ...args)
        { return emplace_unique(mystl::forward<Args>(args)...).first; }


        iterator insert_multi_noresize(const value_type& value){//插入一个新节点，允许重复，返回插入节点的iterator
            node_ptr new_node = create_node(value);
            return insert_node_multi(new_node);
        }

        pair<iterator,bool> insert_unique_noresize(const value_type& value){
            node_ptr new_node = create_node(value);
            return insert_node_unique(new_node);
        }

        iterator insert_multi(const value_type& value){
            rehash_if_need(1);
            return insert_multi_noresize(value);
        }

        iterator insert_multi(const value_type&& value){
            rehash_if_need(1);
            return insert_multi_noresize(std::move(value));
        }

        pair<iterator,bool> insert_unique(const value_type& value){
            rehash_if_need(1);
            return insert_unique_noresize(value);
        }

        pair<iterator,bool> insert_unique(value_type&& value){
            rehash_if_need(1);
            return insert_unique_noresize(std::move(value));
        }

        iterator insert_multi_use_hint(const_iterator /*hint*/, const value_type& value)
        { return insert_multi(value); }
        iterator insert_multi_use_hint(const_iterator /*hint*/, value_type&& value)
        { return emplace_multi(mystl::move(value)); }

        iterator insert_unique_use_hint(const_iterator /*hint*/, const value_type& value)
        { return insert_unique(value).first; }
        iterator insert_unique_use_hint(const_iterator /*hint*/, value_type&& value)
        { return emplace_unique(mystl::move(value)).first; }

        template <class Iter>
        void insert_multi(Iter first, Iter last);//插入两个迭代器之间的值，允许重复

        template <class Iter>
        void insert_unique(Iter first, Iter last);

        void erase(const_iterator position);//删除迭代器所指的节点
        void erase(const_iterator first, const_iterator last);//删除[first, last)内的节点

        size_type erase_multi(const key_type& key);//删除键值为 key 的节点，返回删除的个数
        size_type erase_unique(const key_type& key);

        void clear();//清空hashtable,释放所有节点
        size_type count(const key_type& key) const;//查找键值为key出现的次数



        iterator begin() noexcept {
            return M_begin();
        }

        const_iterator begin() const noexcept{
            return M_begin();
        }

        iterator end() noexcept {
            return iterator(nullptr, this);
        }
        const_iterator end() const noexcept {
            return const_iterator(nullptr, const_cast<hashtable*>(this));
        }
        const_iterator cbegin() const noexcept {
            return begin();
        }
        const_iterator cend() const noexcept {
            return end();
        }
        bool empty() const noexcept{
            return size_ == 0;
        }
        size_type size() const noexcept{
            return size_;
        }
        size_type max_size() const noexcept{
            return static_cast<size_type>(-1);
        }

        float max_load_factor() const noexcept{
            return mlf_;
        }
        void max_load_factor(float ml){
            mlf_ = ml;
        }


        void rehash(size_type count);//按照传入的count找到大于等于count的最小的质数n为vector的长度，然后重新创建vector并重新hash内容；前提是n大于bucket\_size\_或者((float)size_ / (float)n < max_load_factor() - 0.25f &&     (float)n < (float)bucket_size_ * 0.75)





        pair<iterator,iterator> equal_range_multi(const key_type& key);//返回一个pair,两个元素都是迭代器，分别表示键值等于key的区间的开始和结束，都是左闭右开，如果当前链表结尾都是相等，则需要看下一个bucket,直到找到下一个不为空的bucket,否则就是使用end();如果没有这个值，则直接返回两个end() end()
        pair<const_iterator,const_iterator> equal_range_multi(const key_type& key) const;//返回一个pair,两个元素都是迭代器，分别表示键值等于key的区间的开始和结束，都是左闭右开，如果当前链表结尾都是相等，则需要看下一个bucket,直到找到下一个不为空的bucket,否则就是使用end();如果没有这个值，则直接返回两个end() end()

        pair<iterator,iterator> equal_range_unique(const key_type& key);
        pair<const_iterator,const_iterator> equal_range_unique(const key_type& key) const ;


        local_iterator begin(size_type n) noexcept{
            return local_iterator(buckets_[n]);
        }
        const_local_iterator begin(size_type n) const noexcept{
            return const_local_iterator(buckets_[n]);
        }
        const_local_iterator cbegin(size_type n) const noexcept{
            return const_local_iterator(buckets_[n]);
        }

        local_iterator end(size_type n) noexcept{
            return local_iterator(nullptr);
        }

        const_local_iterator end(size_type n) const noexcept{
            return const_local_iterator(nullptr);
        }
        const_local_iterator cend(size_type n) const noexcept{
            return const_local_iterator(nullptr);
        }

        size_type bucket_count() const noexcept{
            return buckets_size_;
        }

        size_type max_bucket_count() const noexcept{
            return ht_prime_list[PRIME_NUM - 1];
        }

        size_type bucket_size(size_type n) const noexcept;
        size_type bucket(const key_type& key) const;//返回对key的hash值

        float load_factor() const noexcept{
            return buckets_size_ == 0 ? 0.0f : static_cast<float>(size_) / buckets_size_;
        }

        void reserve(size_type count);//重新分派hash表，使得其刚好满足可以存储count个元素，并且刚好不超过mlf_


        iterator find(const key_type& key);//查找键值为 key 的节点，返回其迭代器，如果没有，则返回一个内部元素为空的迭代器
        const_iterator find(const key_type& key) const;


        key_equal key_eq() const{
            return equal_;
        }
        hasher hash_fcn() const {
            return hash_;
        }

        void swap(hashtable& rhs) noexcept{

            buckets_.swap(rhs.buckets_);
            sjz_swap(buckets_size_,rhs.buckets_size_);
            sjz_swap(size_,rhs.size_);
            sjz_swap(mlf_,rhs.mlf_);
            sjz_swap(hash_,rhs.hash_);
            sjz_swap(equal_,rhs.equal_);

        }

        template <class Ty>
        void sjz_swap(Ty& lhs, Ty& rhs){
            Ty tmp(std::move(lhs));
            lhs = std::move(rhs);
            rhs = std::move(tmp);
        }









    };

    /*****************************private**********************************/

    template <class T, class Hash, class KeyEqual>
    void hashtable<T,Hash,KeyEqual>::copy_init(const hashtable& ht) {
        buckets_.assign(ht.buckets_size_, nullptr);
        buckets_size_ = ht.buckets_size_;
        size_ = ht.size_;
        mlf_ = ht.mlf_;
        hash_ = ht.hash_;
        equal_ = ht.equal_;

        for(int i = 0; i < ht.buckets_size_; ++i) {
            if(ht.buckets_[i] == nullptr) continue;
            node_ptr now = ht.buckets_[i];
            node_ptr new_node = create_node(now->value);//node_ptr(now->value);
            now = now->next;
            node_ptr pre = new_node;
            buckets_[i] = new_node;
            while(now != nullptr){
                new_node = create_node(now->value);//node_ptr(now->value);
                pre->next = new_node;
                pre = pre->next;
                now = now->next;
            }
        }
    }


    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::rehash_if_need(size_type n) {
        if(static_cast<size_type>(n+size_) > static_cast<float>(buckets_size_) * max_load_factor()){
            rehash(n+size_);
        }
    }


    template<class T, class Hash, class KeyEqual>
    void hashtable<T,Hash,KeyEqual>::replace_bucket(size_type bucket_count) {

        vector<node_ptr> tmp(bucket_count, nullptr);

        for(auto it = begin(); it != end(); ++it) {
            node_ptr new_node = create_node(*it);

            size_type key_map = hash(value_traits::get_key(*it));

            if(tmp[key_map] == nullptr) {
                tmp[key_map] = new_node;
            }
            else {
                node_ptr now = tmp[key_map];
                bool is_have = false;
                while(now){
                    if(is_equal(value_traits::get_key(now->value),value_traits::get_key(new_node->value))){
                        is_have = true;
                        break;
                    }
                    now = now->next;
                }
                if(!is_have){
                    new_node->next = tmp[key_map];
                    tmp[key_map] = new_node;
                }
            }
        }
        buckets_size_ = tmp.size();
        buckets_.swap(tmp);

    }

    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T, Hash, KeyEqual>::iterator,bool> hashtable<T, Hash, KeyEqual>::insert_node_unique(node_ptr np) {
        size_type key_map = hash(value_traits::get_key(np->value));
        if(buckets_[key_map] == nullptr) {
            buckets_[key_map] = np;
            size_++;
            return pair<iterator,bool>(iterator(buckets_[key_map], this),true);
        }
        else {
            node_ptr head = buckets_[key_map];
            bool is_have = false;
            while(head){
                if(is_equal(value_traits::get_key(head->value), value_traits::get_key(np->value))) {
                    is_have = true;
                    break;
                }
                head = head->next;
            }
            if(is_have){
                return pair<iterator,bool>(iterator(head, this),false);
            }
            else{
                np->next = buckets_[key_map];
                buckets_[key_map] = np;
                size_++;
                return pair<iterator,bool>(iterator(buckets_[key_map], this),true);
            }
        }
    }

    template <class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::iterator hashtable<T,Hash,KeyEqual>::insert_node_multi(node_ptr np) {
        size_type key_map = hash(value_traits::get_key(np->value));
        if(buckets_[key_map] == nullptr) {
            buckets_[key_map] = np;
            size_++;
            return iterator(buckets_[key_map], this);
        }
        else {
            node_ptr head = buckets_[key_map];
            bool is_have = false;
            while(head){
                if(is_equal(value_traits::get_key(head->value), value_traits::get_key(np->value))) {
                    is_have = true;
                    break;
                }
                head = head->next;
            }
            if(is_have){
                np->next = head->next;
                head->next = np;
                size_++;
                return iterator(np, this);
            }
            else{
                np->next = buckets_[key_map];
                buckets_[key_map] = np;
                size_++;
                return iterator(buckets_[key_map], this);
            }
        }
    }


    template <class T, class Hash, class KeyEqual>
    template <class Iter>
    void hashtable<T,Hash,KeyEqual>::copy_insert_multi(Iter first, Iter last){
        auto n = mystl::distance(first,last);
        rehash_if_need(n);
        while(first != last){
            node_ptr new_node = create_node(*first);
            insert_node_multi(new_node);
        }
    }

    template <class T, class Hash, class KeyEqual>
    template <class Iter>
    void hashtable<T,Hash,KeyEqual>::copy_insert_unique(Iter first, Iter last) {
        auto n = mystl::distance(first,last);
        rehash_if_need(n);
        while(first != last){
            node_ptr new_node = create_node(*first);
            insert_node_unique(new_node);
        }
    }

    template <class T, class Hash, class KeyEqual>
    void hashtable<T,Hash,KeyEqual>::erase_bucket(size_type n, node_ptr first, node_ptr last) {

        node_ptr temp = new node_type(value_type());
        temp->next = buckets_[n];
        node_ptr now = temp;
        while(now){
            if(now->next == first) break;
        }

        while(now->next != last){
            node_ptr de = now->next;
            now->next = now->next->next;
            destroy_node(de);
            --size_;
            now = now->next;
        }
    }

    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::erase_bucket(size_t n, node_ptr last) {
        node_ptr temp = buckets_[n];
        buckets_[n] = last;
        while(temp != last){
            node_ptr de = temp;
            temp = temp->next;
            destroy_node(de);
            size_--;
        }
    }

    template <class T, class Hash, class KeyEqual>
    bool hashtable<T,Hash,KeyEqual>::equal_to_multi(const hashtable& other) {
        if(size() != other.size()) return false;
        iterator first = begin();
        while(first != end()){
            pair<iterator,iterator> it1 = equal_range_multi(*first);
            pair<iterator,iterator> it2 = other.equal_range_multi(*first);
            if(mystl::distance(it1.first,it2.last) != mystl::distance(it2.first,it2.last)) return false;
            first = it1.last;
        }
        return true;
    }

    template <class T, class Hash, class KeyEqual>
    bool hashtable<T, Hash, KeyEqual>::equal_to_unique(const hashtable& other) {
        if(size() != other.size()) return false;
        iterator first;
        for(;first!=end(); ++first){
            if(other.find(*first)==end()) return false;
        }
        return true;
    }
















    /******************************public***********************************/



    template <class T, class Hash, class KeyEqual>
    template <class Iter>
    void hashtable<T, Hash, KeyEqual>::insert_multi(Iter first, Iter last){
        while(first != last){
            insert_multi(*first);
            ++first;
        }
    }

    template <class T, class Hash, class KeyEqual>
    template <class Iter>
    void hashtable<T,Hash,KeyEqual>::insert_unique(Iter first, Iter last) {
        while(first != last){
            insert_unique(*first);
            ++first;
        }
    }

    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::erase(const_iterator position) {
        size_type key_map = hash(value_traits::get_key(*position));
        node_ptr p = position.node;
        node_ptr cur = buckets_[key_map];
        if(cur == p){//链表第一个就是
            buckets_[key_map] = buckets_[key_map]->next;
            destroy_node(cur);
            size_--;
        }
        else{
            while(cur->next){
                if(cur->next == p){
                    node_ptr de = cur->next;
                    cur->next = cur->next->next;
                    destroy_node(de);
                    size_--;
                    break;
                }
                cur = cur->next;
            }
        }
    }


    template <class T, class Hash, class KeyEqual>
    void hashtable<T, Hash, KeyEqual>::erase(const_iterator first,const_iterator last) {
        size_type first_bucket = hash(value_traits::get_key(*first));
        size_type last_bucket = last.node == nullptr ? buckets_size_ - 1 : hash(value_traits::get_key(*last));
        if(first_bucket == last_bucket){
            erase_bucket(first_bucket,first.node,last.node);
            return ;
        }
        while(first_bucket < last_bucket){
            if(buckets_[first_bucket] != nullptr)
                erase_bucket(first_bucket, nullptr);
            ++first_bucket;
        }

        if(buckets_[first_bucket] != nullptr)
            erase_bucket(first_bucket,last.node);
    }


    template<class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::size_type hashtable<T,Hash,KeyEqual>::erase_multi(const key_type &key) {
        pair<iterator,iterator> p = equal_range_multi(key);
        size_type key_map = hash(key);
        size_type n = mystl::distance(p.first,p.second);
        erase_bucket(key_map,p.first.node,p.second.node);
        return n;
    }

    template <class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::size_type hashtable<T,Hash,KeyEqual>::erase_unique(const key_type &key) {
        iterator it = find(key);
        if(it != end()){
            erase(it);
            return 1;
        }
        else return 0;
    }

    template <class T, class Hash, class KeyEqual>
    void hashtable<T,Hash,KeyEqual>::clear() {
        if(size() != 0){
            for(int i = 0; i <buckets_size_; ++i){
                erase_bucket(i, nullptr);
            }
            size_ = 0;
        }
    }

    template <class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::size_type hashtable<T,Hash,KeyEqual>::count(const key_type &key) const {
        pair<const_iterator,const_iterator> p = equal_range_multi(key);
        return mystl::distance(p.first,p.second);
    }




    template <class T, class Hash, class KeyEqual>
    void hashtable<T,Hash,KeyEqual>::rehash(size_type count) {
        size_type n = next_size(count);
        if(n > buckets_size_) {
            replace_bucket(n);
        }
        else if((float)size_ / (float)n < max_load_factor() - 0.25f && (float)n < (float)buckets_size_ * 0.75){
            replace_bucket(n);
        }
    }


    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T,Hash,KeyEqual>::iterator,typename hashtable<T,Hash,KeyEqual>::iterator>  hashtable<T,Hash,KeyEqual>::equal_range_multi(const key_type &key) {
        size_type key_map = hash(key);
        node_ptr first = buckets_[key_map];


        iterator f,l;

        while(first!= nullptr && !is_equal(value_traits::get_key(first->value), key)){
            first = first->next;

        }

        if(first == nullptr){
            return pair<iterator,iterator>(end(),end());//此时没有找到
        }


        //此时first就变为了真正的开始的first
        f = iterator(first,this);

        node_ptr last = first;

        while(is_equal(value_traits::get_key(last->value),key)){
            if(last->next != nullptr){
                last = last->next;
            }
            else break;
        }
        if(last->next == nullptr){
            bool is_have = false;
            size_type now_index = (key_map + 1) % buckets_size_;
            while(now_index != key_map){
                if(buckets_[now_index] != nullptr){
                    is_have = true;
                    break;
                }
                now_index++;
                now_index%=buckets_size_;
            }

            if(is_have) {
                last = buckets_[now_index];
                l = iterator(last,this);
            }
            else {
                l = end();
            }

        }
        else {
            //此时last变为了真的last
            l = iterator(last, nullptr);
        }
        return pair<iterator,iterator>(f,l);
    }


    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T,Hash,KeyEqual>::const_iterator,typename hashtable<T,Hash,KeyEqual>::const_iterator>  hashtable<T,Hash,KeyEqual>::equal_range_multi(const key_type &key) const{
        size_type key_map = hash(key);
        node_ptr first = buckets_[key_map];


        const_iterator f,l;

        while(!is_equal(value_traits::get_key(first->value), key)){
            first = first->next;
            if(first == nullptr) break;
        }

        if(first == nullptr){
            return pair<const_iterator,const_iterator>(end(),end());//此时没有找到
        }


        //此时first就变为了真正的开始的first
        f = const_iterator(first,const_cast<hashtable*>(this));

        node_ptr last = first;

        while(is_equal(value_traits::get_key(last->value),key)){
            if(last->next != nullptr){
                last = last->next;
            }
            else break;
        }
        if(last->next == nullptr){
            bool is_have = false;
            size_type now_index = (key_map + 1) % buckets_size_;
            while(now_index != key_map){
                if(buckets_[now_index] != nullptr){
                    is_have = true;
                    break;
                }
                now_index++;
                now_index%=buckets_size_;
            }

            if(is_have) {
                last = buckets_[now_index];
                l = const_iterator(last,const_cast<hashtable*>(this));
            }
            else {
                l = end();
            }

        }
        else {
            //此时last变为了真的last
            l = const_iterator(last, nullptr);
        }
        return pair<const_iterator,const_iterator>(f,l);
    }



    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T,Hash,KeyEqual>::iterator, typename hashtable<T,Hash,KeyEqual>::iterator> hashtable<T,Hash,KeyEqual>::equal_range_unique(
            const key_type &key) {
        iterator l = find(key);
        if(l == end()){
            return pair<iterator,iterator>(end(),end());
        }
        else{
            iterator f = l++;
            return pair<iterator,iterator>(f,l);
        }
    }

    template <class T, class Hash, class KeyEqual>
    pair<typename hashtable<T,Hash,KeyEqual>::const_iterator,typename hashtable<T,Hash,KeyEqual>::const_iterator> hashtable<T,Hash,KeyEqual>::equal_range_unique(const key_type &key) const{
        const_iterator l = find(value_traits::get_key(key));
        if(l == end()){
            return pair<const_iterator,const_iterator>(end(),end());
        }
        else{
            const_iterator f = l++;
            return pair<const_iterator,const_iterator>(f,l);
        }
    }


    template <class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::size_type hashtable<T,Hash,KeyEqual>::bucket_size(size_type n) const noexcept {
        size_type count = 0;
        if(buckets_[n] == nullptr){
            return 0;
        }
        else{
            node_ptr now = buckets_[n];
            while(now){
                ++count;
                now = now->next;
            }
            return count;
        }
    }

    template <class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::size_type hashtable<T,Hash,KeyEqual>::bucket(const key_type &key) const {
        return hash(key);
    }

    template <class T, class Hash, class KeyEqual>
    void hashtable<T,Hash,KeyEqual>::reserve(size_type count) {
        rehash(static_cast<size_type>((float) count / max_load_factor()+0.5f));
    }


    template <class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::iterator hashtable<T,Hash,KeyEqual>::find(const key_type& key){
//        size_type key_map = hash(value_traits::get_key(key));
        size_type key_map = hash(key);
        node_ptr first = buckets_[key_map];
        for(;first && !is_equal(key,value_traits::get_key(first->value));first= first->next){}
        return iterator(first,this);
    }

    template<class T, class Hash, class KeyEqual>
    typename hashtable<T,Hash,KeyEqual>::const_iterator hashtable<T,Hash,KeyEqual>::find(const key_type &key) const{
        size_type key_map = hash(value_traits::get_key(key));
        node_ptr first = buckets_[key_map];
        for(;first && !is_equal(value_traits::get_key(key),value_traits::get_key(first->value));first= first->next){}
        return const_iterator(first,this);
    }














}



#endif //MYSTL_HASHTABLE_H



















