//
// Created by 孙建钊 on 2023/7/29.
//

#ifndef MYSTL_RB_TREE_H
#define MYSTL_RB_TREE_H


//红黑树
#include "iterator.h"
#include "util.h"


namespace mystl{

    typedef bool rb_tree_color_type; // 红黑树节点颜色类型
    static constexpr rb_tree_color_type rb_tree_red = false;
    static constexpr rb_tree_color_type rb_tree_black = true;

    template <class T> struct rb_tree_node_base;
    template <class T> struct rb_tree_node;
    template <class T> struct rb_tree_iterator;
    template <class T> struct rb_tree_const_iterator;

    template <class T, bool>
    struct rb_tree_value_traits_imp{

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
    struct rb_tree_value_traits_imp<T,true>{
//        typedef typename std::remove_cv<typename T::first_type>::type key_type;
        typedef typename T::first_type key_type;
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
    struct rb_tree_value_traits{
        static constexpr bool is_map = mystl::is_pair<T>::value;

        typedef rb_tree_value_traits_imp<T, is_map> value_traits_type;

        typedef typename value_traits_type::key_type key_type;
        typedef typename value_traits_type::mapped_type mapped_type;
        typedef typename value_traits_type::value_type value_type;

        template <class Ty>
        static const key_type& get_key(const Ty& value){
            return value_traits_type::get_key(value);
        }

        template <class Ty>
        static const value_type& get_value(const Ty& value){
            return value_traits_type::get_value(value);
        }
    };

    template <class T>
    struct rb_tree_node_traits{
        typedef rb_tree_color_type color_type;
        typedef rb_tree_value_traits<T> value_traits;
        typedef typename value_traits::key_type key_type;
        typedef typename value_traits::mapped_type mapped_type;
        typedef typename value_traits::value_type value_type;

        typedef rb_tree_node_base<T>* base_ptr;
        typedef rb_tree_node<T>* node_ptr;

    };


    template <class T>
    struct rb_tree_node_base{

        typedef rb_tree_color_type color_type;
        typedef rb_tree_node_base<T>* base_ptr;
        typedef rb_tree_node<T>* node_ptr;

        base_ptr parent;//父节点
        base_ptr left;//左子节点
        base_ptr right;//右子节点
        color_type color; //节点颜色

        base_ptr get_base_ptr(){
            return &*this;
        }

        node_ptr get_node_ptr(){
            return reinterpret_cast<node_ptr>(&*this);
        }

        node_ptr& get_node_ref(){
            return reinterpret_cast<node_ptr>(&*this);
        }
    };

    template <class T>
    struct rb_tree_node : public rb_tree_node_base<T>{

        typedef rb_tree_node_base<T>* base_ptr;
        typedef rb_tree_node<T>* node_ptr;

        T value; //节点值

        rb_tree_node() = default;

        template <class ...Args>
        rb_tree_node(Args&& ...args){
            value = T(args...);
        }

        base_ptr get_base_ptr(){
            return static_cast<base_ptr>(&*this);
        }

        node_ptr get_node_ptr(){
            return &*this;
        }
    };


    template <class T>
    struct rb_tree_traits{

        typedef rb_tree_value_traits<T> value_traits;

        typedef typename value_traits::key_type key_type;
        typedef typename value_traits::mapped_type mapped_type;
        typedef typename value_traits::value_type value_type;

        typedef value_type* pointer;
        typedef value_type& reference;
        typedef const value_type* const_pointer;
        typedef const value_type& const_reference;

        typedef rb_tree_node_base<T> base_type;
        typedef rb_tree_node<T> node_type;

        typedef base_type* base_ptr;
        typedef node_type* node_ptr;
    };



    //返回以x为根节点的的树的最小值
    template<class NodePtr>
    NodePtr rb_tree_min(NodePtr x) noexcept{

//        if(x->left){
//            std::cout<<"qwe"<<std::endl;
//        }
//
//        std::cout<<x->left<<std::endl;


        if(x->left){//存在左子节点
            while(x->left){
                x = x->left;
            }
        }
        return x;
    }

    //返回以x为根节点的树的最大值
    template <class NodePtr>
    NodePtr rb_tree_max(NodePtr x) noexcept{
        if(x->right){
            while(x->right)
                x = x->right;
        }
        return x;
    }

    //判断节点node是否是左子节点
    template<class NodePtr>
    bool rb_tree_is_lchild(NodePtr node){
        return node->parent->left == node;
    }

    //判断节点是否是红色
    template <class NodePtr>
    bool rb_tree_is_red(NodePtr node){
        return node->color == rb_tree_red;
    }

    //将节点node设置为黑色
    template <class NodePtr>
    void rb_tree_set_black(NodePtr& node) noexcept{
        node->color = rb_tree_black;
    }

    //将节点node设置为红色
    template <class NodePtr>
    void rb_tree_set_red(NodePtr& node) noexcept{
        node->color = rb_tree_red;
    }

    //返回node的下一个节点，数值上的下一个节点
    template<class NodePtr>
    NodePtr rb_tree_next(NodePtr node) noexcept{

        if(node->right){//存在右子节点，那么就返回右子节点子树的最小值
            return rb_tree_min(node->right);
        }
        else{
            //此时只能向上寻找
            NodePtr parent = node->parent;

            if(parent->parent == node){
                return parent;//返回header_
            }

            while(parent->right == node){
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    //左旋
    //x为左旋点，root为根节点
    /*---------------------------------------*\
    |       p                         p       |
    |      / \                       / \      |
    |     x   d    rotate left      y   d     |
    |    / \       ===========>    / \        |
    |   a   y                     x   c       |
    |      / \                   / \          |
    |     b   c                 a   b         |
    \*---------------------------------------*/
    template<class NodePtr>
    void rb_tree_rotate_left(NodePtr x, NodePtr& root) noexcept{
        NodePtr y = x->right;
        NodePtr p = x->parent;

        //x和b连接
        x->right = y->left;
        if(y->left) y->left->parent = x;
        //y和p连接
        y->parent = p;
        if(x == root){//如果x是根节点，需要更新根节点的内容，同时更新根节点和header_的父节点，并且不会跟新p的左右子节点
            root = y;
            y->parent = x->parent;
            x->parent->parent = y;
        }
        else if(rb_tree_is_lchild(x))  p->left = y;
        else p->right = y;
        //x和y连接
        y->left = x;
        x->parent = y;

    }
    /*----------------------------------------*\
    |     p                         p          |
    |    / \                       / \         |
    |   d   x      rotate right   d   y        |
    |      / \     ===========>      / \       |
    |     y   a                     b   x      |
    |    / \                           / \     |
    |   b   c                         c   a    |
    \*----------------------------------------*/
    // 右旋，参数一为右旋点，参数二为根节点
    template<class NodePtr>
    void rb_tree_rotate_right(NodePtr x, NodePtr& root) noexcept{
        NodePtr y = x->left;
        NodePtr p = x->parent;

        //x和c相连
        x->left = y->right;
        if(y->right) y->right->parent = x;

        //y和p相连
        y->parent = p;
        if(x == root){
            root = y;
            root->parent = p;
            p->parent = y;
        }
        else if(rb_tree_is_lchild(x)){
            p->left = y;
        }
        else p->right = y;

        //x和y相连
        x->parent = y;
        y->right = x;
    }

    /*
        + case1 : 新增节点是根节点，令新增节点为黑
        + case2 :新增节点的父节点为黑，没有破坏平衡，直接返回
        + case3 :父节点和叔叔节点都为红，令父节点和叔叔节点为黑，祖父节点为红，然后令祖父节点为当前节点，继续处理
        + case4 :父节点为红，叔叔节点为黑或nullptr,父节点为左（右）孩子，当前节点为左（右）孩子，让父节点变为黑色，祖父节点变为红色，然后以父节点为当前节点，进行右（左）旋，不需要递归向上处理
        + case5 :父节点为红，叔叔节点为黑或nullptr,父节点为左（右孩子），当前节点为右（左）孩子，插入节点变为黑色，grand变为红色，parent左（右）旋，grand右（左）选，不需要递归向上处理
     * */
    //插入节点后重新平衡，也就是这个已经插入了，方法是为了重新平衡
    //参数一是插入节点，参数二是根节点
//    template <class NodePtr>
//    void rb_tree_insert_rebalance(NodePtr x, NodePtr& root) noexcept{
//
//        rb_tree_set_red(x);//设置为红色
//
//        if(x == root){// case 1
//            rb_tree_set_black(x);
//            return ;
//        }
//
//        while(x!=root){
//
//            if(!rb_tree_is_red(x->parent)){// case 2
//                return ;
//            }
//            if(rb_tree_is_red(x->parent->parent->left) && rb_tree_is_red(x->parent->parent->right)){ //case 3
//                rb_tree_set_red(x->parent->parent);
//                rb_tree_set_black(x->parent->parent->left);
//                rb_tree_set_black(x->parent->parent->right);
//                x = x->parent->parent;
//                continue;
//            }
//            //此时父节点一定为红,叔叔节点为黑或者nullptr
//            if(rb_tree_is_lchild(x)){//x是父节点的左节点
//                if(rb_tree_is_lchild(x->parent)){//父亲节点为左节点
//                    //case 4
//                    rb_tree_set_black(x->parent);
//                    rb_tree_set_red(x->parent->parent);
//                    x = x->parent;
//                    rb_tree_rotate_right(x,root);
//                    break;
//                }
//                else{//父亲节点为右节点
//                    //case 5
//                    rb_tree_set_black(x);
//                    rb_tree_set_red(x->parent->parent);
//                    rb_tree_rotate_left(x->parent,root);
//                    rb_tree_rotate_right(x->parent->parent,root);
//                    break;
//                }
//            }
//            else{//x是父节点的右节点
//                if(rb_tree_is_lchild(x->parent)){//父亲节点为左节点
//                    //case 5
//                    rb_tree_set_black(x);
//                    rb_tree_set_red(x->parent->parent);
//                    rb_tree_rotate_right(x->parent,root);
//                    rb_tree_rotate_left(x->parent->parent,root);
//                    break;
//                }
//                else{//父亲节点为右节点
//                    //case 4
//                    rb_tree_set_black(x->parent);
//                    rb_tree_set_red(x->parent->parent);
//                    x = x->parent;
//                    rb_tree_rotate_left(x,root);
//                    break;
//                }
//            }
//        }
//    }


    template <class NodePtr>
    void rb_tree_insert_rebalance(NodePtr x, NodePtr& root) noexcept
    {
        rb_tree_set_red(x);  // 新增节点为红色
        while (x != root && rb_tree_is_red(x->parent))
        {
            if (rb_tree_is_lchild(x->parent))
            { // 如果父节点是左子节点
                auto uncle = x->parent->parent->right;
                if (uncle != nullptr && rb_tree_is_red(uncle))
                { // case 3: 父节点和叔叔节点都为红
                    rb_tree_set_black(x->parent);
                    rb_tree_set_black(uncle);
                    x = x->parent->parent;
                    rb_tree_set_red(x);
                }
                else
                { // 无叔叔节点或叔叔节点为黑
                    if (!rb_tree_is_lchild(x))
                    { // case 4: 当前节点 x 为右子节点
                        x = x->parent;
                        rb_tree_rotate_left(x, root);
                    }
                    // 都转换成 case 5： 当前节点为左子节点
                    rb_tree_set_black(x->parent);
                    rb_tree_set_red(x->parent->parent);
                    rb_tree_rotate_right(x->parent->parent, root);
                    break;
                }
            }
            else  // 如果父节点是右子节点，对称处理
            {
                auto uncle = x->parent->parent->left;
                if (uncle != nullptr && rb_tree_is_red(uncle))
                { // case 3: 父节点和叔叔节点都为红
                    rb_tree_set_black(x->parent);
                    rb_tree_set_black(uncle);
                    x = x->parent->parent;
                    rb_tree_set_red(x);
                    // 此时祖父节点为红，可能会破坏红黑树的性质，令当前节点为祖父节点，继续处理
                }
                else
                { // 无叔叔节点或叔叔节点为黑
                    if (rb_tree_is_lchild(x))
                    { // case 4: 当前节点 x 为左子节点
                        x = x->parent;
                        rb_tree_rotate_right(x, root);
                    }
                    // 都转换成 case 5： 当前节点为左子节点
                    rb_tree_set_black(x->parent);
                    rb_tree_set_red(x->parent->parent);
                    rb_tree_rotate_left(x->parent->parent, root);
                    break;
                }
            }
        }
        rb_tree_set_black(root);  // 根节点永远为黑
    }


    //交换两个节点，交换两个节点的位置
    //暂时没考虑两个节点有父子关系连接的情况,并且x y 都不是root节点
    template<class NodePtr>
    void rb_tree_swap_node(NodePtr x, NodePtr y){

        NodePtr xp = x->parent;
        NodePtr xl = x->left;
        NodePtr xr = x->right;

        NodePtr yp = y->parent;
        NodePtr yl = y->left;
        NodePtr yr = y->right;


        //修改xp
        if(rb_tree_is_lchild(x)) xp->left = y;
        else xp->right = y;
        //修改yp
        if(rb_tree_is_lchild(y)) yp->left = x;
        else yp->right = x;

        //修改x
        x->parent = yp;
        x->left = yl;
        x->right = yr;

        //修改y
        y->parent = xp;
        y->left = xl;
        y->right = xr;

        //修改xl
        if(xl) xl->parent = y;
        //修改xr
        if(xr) xr->parent = y;

        //修改yl
        if(yl) yl->parent = x;
        //修改yr
        if(yr) yr->parent = x;

    }
//
//    template<class NodePtr>
//    NodePtr rb_tree_erase_rebalance1(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost){
//
//        //y表示可能的替换节点
//        auto y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_min(z->right);
//        //x是y的独子节点或者nullptr
//        auto x = y->left != nullptr ? y->left : y->right;
//        NodePtr xp = nullptr;//x的父节点
//
//
//        if(y!=z){
//            //此时说明z是有两个子节点的，所以使用其右子树的最大值来代替z
//            //将y顶替z的位置，然后将x放到z的位置，然后让y指向z表明要删除的节点
//
//            //让y和zl相连
//            y->left = z->left;
//            z->left->parent = y;
//
//            if(z->right != y){//y是z的右孩子
//                //连接y和zr
//                y->right = z->right;
//                z->right->parent = y;
//                //连接x和yp
//                y->parent->left = x;
//                if(x) x->parent = y->parent;
//
//                xp = y->parent;
//            }
//            else{
//                xp = y;
//            }
//
//            //连接x和zp
//            if(z==root) root = y;
//            else if(rb_tree_is_lchild(z)) z->parent->left = y;
//            else z->parent->right = y;
//            y->parent = z->parent;
//
//            //交换y和z的颜色
//            bool y_is_red = rb_tree_is_red(y);
//            if(rb_tree_red(z)) rb_tree_set_red(y);
//            else rb_tree_set_black(y);
//            if(y_is_red) rb_tree_set_red(z);
//            else rb_tree_set_black(z);
//
//            y = z;
//
//        }
//        else{
//            //此时z最多只有一个子节点
//            //连接x和zp
//            if(z==root) root = x;
//            else if(rb_tree_is_lchild(z)) z->parent->left = x;
//            else z->parent->right = x;
//            if(x) x->parent = z->parent;
//
//            xp = z->parent;
//
//            if(z == leftmost) leftmost = x == nullptr ? xp : rb_tree_min(z->right);
//            if(z == rightmost) rightmost = x == nullptr ? xp : rb_tree_max(z->left);
//
//        }
//
//        //此时y指向要删除的节点，x为替代节点，从x节点开始调整，此时已经满足AVL,只是颜色可能不对，需要调整颜色
//
//        //如果要删除的节点为红色，树的性质没有被破坏，否则按照以下情况进行调整(x为左子节点为例)
//        //x是当前节点 x是左(右)子节点
//        //case 1: 当前节点的兄弟节点为红色
//        //case 2: 当前节点的兄弟节点为黑色，且兄弟节点的两个子节点都为黑色或nil
//        //case 3:
//
//
//
//
//
//
//
//    }


    //参数1 要删除的节点
    //参数2 根节点
    //参数3 最小节点
    //参数4 最大节点
    //case1 无子节点时，删除节点可能为红色或者黑色
    //  case1.1 如果为红色，直接删除即可，不会影响黑色节点的数量，退出即可
    //  case1.2 如果为黑色，则需要进行删除平衡操作
    //      case1.2.1 如果这个黑色节点为根节点，说明这棵树只有一个节点，直接删除即可
    //      case1.2.2 删除节点的兄弟节点为黑色
    //          case1.2.2.1 兄弟节点至少有一个红色子节点；此时兄弟节点不会有孙子节点
    //              case1.2.2.1.1 兄弟节点只有一个红右子节点  此时让这个右子节点放到删除节点的父节点上（并且变为原先删除节点父节点的颜色），然后让父节点放到删除节点的位置（并且变为删除节点的颜色，也就是黑色）
    //              case1.2.2.1.2 兄弟节点只有一个红左子节点 此时让删除节点的父节点放到删除节点的位置并变为黑色；让删除节点的兄弟接待那放到父亲节点的位置，并变为父亲节点的颜色；让删除节点的兄弟节点的左子节点放到删除节点的兄弟节点位置，变为黑色
    //              case1.2.2.1.3 兄弟节点有两个红色子节点  删除节点是父节点的右（左）节点；让删除节点的父节点放到删除节点的位置并变为黑色；让删除节点的兄弟节点放到删除节点的父节点位置，病变为删除节点父节点的颜色；让删除节点的兄弟节点的左（右）节点放到删除节点的兄弟节点上并变为黑色；
    //                              让删除节点的兄弟节点的右（左）节点放到原删除节点（先删除节点的父节点）的左（右）节点上
    //          case1.2.2.2 兄弟节点没有红色节点，也就是兄弟节点没有子节点
    //              case1.2.2.2.1 父节点为红色 直接让父节点变为黑色，兄弟节点变为红色即可
    //              case1.2.2.2.2 父节点为黑色 将兄弟节点变为红色，然后递归向上处理  不过此时递归怎么递归很不理解
    //      case1.2.3 删除节点的兄弟节点为红色 删除节点是右（左）节点，删除节点的父节点放到删除节点的位置并变为黑色，删除节点的兄弟节点放到删除节点父节点位置并变为父节点颜色，删除节点的兄弟节点的右（左）节点放到原删除节点（现删除节点的父节点）的左（右）子节点并变为红色
    //case2 只有一个子节点时，删除节点只能是黑色，其子节点为红色，否则无法满足红黑树的性质；此时删除节点的子节点连接到父节点，且将子节点颜色涂黑，保证黑色数量
    //case3 有两个子节点时，与二叉搜索树一样，使用后继节点作为替换的删除节点，情形转换为case1或者case2进行处理；后继节点可以是左子树的最大值或者右子树的最小值；如果是右子树的最小值，这个最小值可能是叶子节点，也可能只有一个有孩子的节点
//    template<class NodePtr>
//    NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost){
//
//        NodePtr y;//只想实际要删除的节点
//
//        if(z->left == nullptr && z->right == nullptr){ // case1
//
//            if(rb_tree_is_red(z)){// case1.1
//                return z;//返回要删除的节点，出去后直接删除这个节点即可
//            }
//            else{//case1.2
//                if(z == root) {
//                    //case1.2.1
//                    leftmost = root->parent;
//                    rightmost = root->parent;
//                    root = nullptr;
//                    return z;
//                }
//                NodePtr uncle;
//                if(rb_tree_is_lchild(z)) uncle = z->parent->right;
//                else uncle = z->parent->left;
//
//                if(uncle->left){
//                    if(uncle->right){
//                        //兄弟节点有左右节点
//
//                        if(!rb_tree_is_red(uncle)){
//                            //case1.2.2.1.3 兄弟节点为黑色，并且有两个红色子节点 删除节点是黑色节点
//                            //变色
//                            //删除节点的兄弟节点变为删除节点父节点的颜色
//                            if(z->parent->color == rb_tree_red) rb_tree_set_red(uncle);
//                            else rb_tree_set_black(uncle);
//                            rb_tree_set_black(z->parent);//删除节点的父节点变为黑色
//
//                            if(rb_tree_is_lchild(z)){//删除节点是左节点
//                                rb_tree_set_black(uncle->right);//删除节点的兄弟节点的右节点变为黑色
//                                rb_tree_rotate_left(z->parent);//删除节点的父节点左旋
//                            }
//                            else{
//                                rb_tree_set_black(uncle->left);//删除节点的兄弟节点的左节点变为黑色
//                                rb_tree_rotate_right(z->parent);//删除节点的父节点右旋
//                            }
//
//                            return z;
//
//                        }
//                        else{
//                            //case1.2.3 删除节点为黑色无子节点，删除节点的兄弟节点为红色
//                            if(z->parent == rb_tree_red) rb_tree_set_red(uncle);
//                            else rb_tree_set_black(uncle);
//                            rb_tree_set_black(z->parent);//父节点变为黑色
//                            if(rb_tree_is_lchild(z)){
//                                rb_tree_set_red(uncle->left);//兄弟节点的左节点变为红色
//                                rb_tree_rotate_left(z->parent);//删除节点的父节点左旋
//                            }
//                            else{
//                                rb_tree_set_red(uncle->right);//兄弟节点的右节点变为红色
//                                rb_tree_rotate_right(z->parent);//删除节点的父节点右旋
//
//                            }
//                        }
//
//                    }
//                    else{
//                        //兄弟节点只有左节点
//
//                        if(!rb_tree_is_red(uncle)){
//                            //case1.2.2.1.2 兄弟节点为黑色，并且只有一个红色左节点
//                            //删除节点的兄弟节点变为删除节点父节点的颜色
//                            if(z->parnet->color == rb_tree_red) rb_tree_set_red(uncle);
//                            else rb_tree_set_black(uncle);
//                            rb_tree_set_black(uncle->left);//兄弟节点的左节点变为黑色
//                            rb_tree_set_black(z->parent);//删除节点的父节点变为黑色
//
//                            if(rb_tree_is_lchild(z)){
//                                uncle->right = uncle->left;
//                                uncle->left = nullptr;
//                                rb_tree_rotate_left(z->parent);
//                            }
//                            else{
//                                rb_tree_rotate_right(z->parent);
//                            }
//                            return z;
//                        }
//
//                    }
//                }
//                else{
//                    if(uncle->right){
//                        //兄弟节点只有右节点
//
//                        if(!rb_tree_is_red(uncle)){
//                            //case1.2.2.1.1  兄弟节点为黑色，并且只有一个红色右节点
//                            NodePtr uncleR = uncle->right;
//                            NodePtr zp = z->parent;
//
//                            //改变uncleR的颜色，变为和zp一样
//                            if(zp->color == rb_tree_red) rb_tree_set_red(uncleR);
//                            else rb_tree_set_black(uncleR);
//                            //将zp变为黑色，变为和z一样
//                            rb_tree_set_black(zp);
//
//                            //将uncleR放到删除节点的父节点上
//
//                            //改变zp->parent
//                            if(rb_tree_is_lchild(zp)) zp->parent->left = uncleR;
//                            else zp->parent->right = uncleR;
//
//                            //改变uncleR的值
//                            uncleR->parent = zp->parent;
//                            if(rb_tree_is_lchild(z)){
//                                uncleR->left = zp;
//                                uncleR->right = uncle;
//                            }
//                            else{
//                                uncleR->left = uncle;
//                                uncleR->right = zp;
//                            }
//
//                            //改变uncle的值
//                            uncle->parent = uncleR;
//                            uncle->right = nullptr;
//
//                            //改变zp的值
//                            zp->left = nullptr;
//                            zp->parent = uncleR;
//
//                            return z;
//
//                        }
//
//                    }
//                    else{
//                        //兄弟节点没有子节点
//                        //此时删除节点没有子节点，删除节点为黑色，兄弟节点没有子节点，
//                        if(!rb_tree_is_red(uncle)){
//                            //此时删除节点没有子节点，删除节点为黑色，兄弟节点没有子节点，兄弟节点为黑色
//                            if(rb_tree_is_red(z->parent)){
//                                //此时删除节点没有子节点，删除节点为黑色，兄弟节点没有子节点，兄弟节点为黑色,父节点为红色
//                                //case1.2.2.2.1
//                                rb_tree_set_black(z->parent);//父节点变为黑色
//                                rb_tree_set_red(uncle);//兄弟节点变为红色
//                                return z;
//                            }
//                        }
//
//
//                    }
//                }
//
//
//
//
//
//
//            }
//        }
//        else if(z->left && z->right){//case3
//
//            NodePtr x = rb_tree_min(z->right);//x是要替换的节点
//
//            //x和z进行调换
//            NodePtr xr = x->right;
//            NodePtr xp = x->parent;
//
//            x->right = z->right;
//            x->left = z->left;
//            x->parent = z->parent;
//            if(rb_tree_is_lchild(z)) z->parent->left = x;
//            else z->parent->right = x;
//            z->left->parent = x;
//            z->right->parent = x;
//
//            z->left = nullptr;
//            z->right = xr;
//            z->parent = xp;
//
//            xp->left = z;
//            if(xr) xr->parent = z;
//
//        }
//        else{//case2
//            if(z->left){
//                rb_tree_set_black(z->left);
//                z->left->parent = z->parent;
//                z->parent->left = z->left;
//            }
//            else{
//                rb_tree_set_black(z->right);
//                z->right->parent = z->parent;
//                z->parent->right = z->right;
//            }
//            return z;
//        }
//
//
//
//    }



    template <class NodePtr>
    NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
    {
        // y 是可能的替换节点，指向最终要删除的节点
        auto y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_next(z);
        // x 是 y 的一个独子节点或 NIL 节点
        auto x = y->left != nullptr ? y->left : y->right;
        // xp 为 x 的父节点
        NodePtr xp = nullptr;

        // y != z 说明 z 有两个非空子节点，此时 y 指向 z 右子树的最左节点，x 指向 y 的右子节点。
        // 用 y 顶替 z 的位置，用 x 顶替 y 的位置，最后用 y 指向 z
        if (y != z)
        {
            z->left->parent = y;
            y->left = z->left;

            // 如果 y 不是 z 的右子节点，那么 z 的右子节点一定有左孩子
            if (y != z->right)
            { // x 替换 y 的位置
                xp = y->parent;
                if (x != nullptr)
                    x->parent = y->parent;

                y->parent->left = x;
                y->right = z->right;
                z->right->parent = y;
            }
            else
            {
                xp = y;
            }

            // 连接 y 与 z 的父节点
            if (root == z)
                root = y;
            else if (rb_tree_is_lchild(z))
                z->parent->left = y;
            else
                z->parent->right = y;
            y->parent = z->parent;
//            mystl::swap(y->color, z->color);

            bool y_is_red = rb_tree_is_red(y);
            if(rb_tree_is_red(z)) rb_tree_set_red(y);
            else rb_tree_set_black(y);
            if(y_is_red) rb_tree_set_red(z);
            else rb_tree_set_black(z);


            y = z;
        }
            // y == z 说明 z 至多只有一个孩子
        else
        {
            xp = y->parent;
            if (x)
                x->parent = y->parent;

            // 连接 x 与 z 的父节点
            if (root == z)
                root = x;
            else if (rb_tree_is_lchild(z))
                z->parent->left = x;
            else
                z->parent->right = x;

            // 此时 z 有可能是最左节点或最右节点，更新数据
            if (leftmost == z)
                leftmost = x == nullptr ? xp : rb_tree_min(x);
            if (rightmost == z)
                rightmost = x == nullptr ? xp : rb_tree_max(x);
        }

        // 此时，y 指向要删除的节点，x 为替代节点，从 x 节点开始调整。
        // 如果删除的节点为红色，树的性质没有被破坏，否则按照以下情况调整（x 为左子节点为例）：
        // case 1: 兄弟节点为红色，令父节点为红，兄弟节点为黑，进行左（右）旋，继续处理
        // case 2: 兄弟节点为黑色，且两个子节点都为黑色或 NIL，令兄弟节点为红，父节点成为当前节点，继续处理
        // case 3: 兄弟节点为黑色，左子节点为红色或 NIL，右子节点为黑色或 NIL，
        //         令兄弟节点为红，兄弟节点的左子节点为黑，以兄弟节点为支点右（左）旋，继续处理
        // case 4: 兄弟节点为黑色，右子节点为红色，令兄弟节点为父节点的颜色，父节点为黑色，兄弟节点的右子节点
        //         为黑色，以父节点为支点左（右）旋，树的性质调整完成，算法结束
        if (!rb_tree_is_red(y))
        { // x 为黑色时，调整，否则直接将 x 变为黑色即可
            while (x != root && (x == nullptr || !rb_tree_is_red(x)))
            {
                if (x == xp->left)
                { // 如果 x 为左子节点
                    auto brother = xp->right;
                    if (rb_tree_is_red(brother))
                    { // case 1
                        rb_tree_set_black(brother);
                        rb_tree_set_red(xp);
                        rb_tree_rotate_left(xp, root);
                        brother = xp->right;
                    }
                    // case 1 转为为了 case 2、3、4 中的一种
                    if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
                        (brother->right == nullptr || !rb_tree_is_red(brother->right)))
                    { // case 2
                        rb_tree_set_red(brother);
                        x = xp;
                        xp = xp->parent;
                    }
                    else
                    {
                        if (brother->right == nullptr || !rb_tree_is_red(brother->right))
                        { // case 3
                            if (brother->left != nullptr)
                                rb_tree_set_black(brother->left);
                            rb_tree_set_red(brother);
                            rb_tree_rotate_right(brother, root);
                            brother = xp->right;
                        }
                        // 转为 case 4
                        brother->color = xp->color;
                        rb_tree_set_black(xp);
                        if (brother->right != nullptr)
                            rb_tree_set_black(brother->right);
                        rb_tree_rotate_left(xp, root);
                        break;
                    }
                }
                else  // x 为右子节点，对称处理
                {
                    auto brother = xp->left;
                    if (rb_tree_is_red(brother))
                    { // case 1
                        rb_tree_set_black(brother);
                        rb_tree_set_red(xp);
                        rb_tree_rotate_right(xp, root);
                        brother = xp->left;
                    }
                    if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
                        (brother->right == nullptr || !rb_tree_is_red(brother->right)))
                    { // case 2
                        rb_tree_set_red(brother);
                        x = xp;
                        xp = xp->parent;
                    }
                    else
                    {
                        if (brother->left == nullptr || !rb_tree_is_red(brother->left))
                        { // case 3
                            if (brother->right != nullptr)
                                rb_tree_set_black(brother->right);
                            rb_tree_set_red(brother);
                            rb_tree_rotate_left(brother, root);
                            brother = xp->left;
                        }
                        // 转为 case 4
                        brother->color = xp->color;
                        rb_tree_set_black(xp);
                        if (brother->left != nullptr)
                            rb_tree_set_black(brother->left);
                        rb_tree_rotate_right(xp, root);
                        break;
                    }
                }
            }
            if (x != nullptr)
                rb_tree_set_black(x);
        }
        return y;
    }







    template <class T>
    struct rb_tree_iterator_base : public mystl::iterator<mystl::bidirectional_iterator_tag,T>{

        typedef typename rb_tree_traits<T>::base_ptr base_ptr;

        base_ptr node; //指向节点本身

        rb_tree_iterator_base():node(nullptr){}

        //迭代器前进一位
        void inc(){
            node = rb_tree_next(node);
        }

        //迭代器后退一位
        void dec(){

            if(node->parent->parent == node && rb_tree_is_red(node)){
               node = node->right;
               return ;
            }

            if(node->left){
                node = rb_tree_max(node->left);
            }
            else{
                //没有左子节点，只能向上去寻找
                base_ptr parent = node->parent;
                while(parent->left == node){
                    node = parent;
                    parent = node->parent;
                }
                node = parent;
            }
        }

        bool operator==(const rb_tree_iterator_base& rhs){
            return node == rhs.node;
        }

        bool operator!=(const rb_tree_iterator_base& rhs){
            return node != rhs.node;
        }
    };

    template <class T>
    struct rb_tree_iterator: public rb_tree_iterator_base<T>{

        typedef rb_tree_traits<T> tree_traits;

        typedef typename tree_traits::value_type value_type;
        typedef typename tree_traits::mapped_type mapped_type;
        typedef typename tree_traits::key_type key_type;
        typedef typename tree_traits::pointer pointer;
        typedef typename tree_traits::const_reference const_reference;
        typedef typename tree_traits::reference reference;

        typedef typename tree_traits::base_ptr base_ptr;
        typedef typename tree_traits::node_ptr node_ptr;

        typedef rb_tree_iterator<T> iterator;
        typedef rb_tree_const_iterator<T> const_iterator;
        typedef iterator self;

        using rb_tree_iterator_base<T>::node;
        rb_tree_iterator(){
            node = nullptr;
        }
        rb_tree_iterator(base_ptr x){
            node = x;
        }
        rb_tree_iterator(node_ptr x){
            node = x;
        }
        rb_tree_iterator(const iterator& rhs){
            node = rhs.node;
        }
        rb_tree_iterator(const const_iterator& rhs){
            node = rhs.node;
        }

        reference operator*() const{
            return node->get_node_ptr()->value;
        }

        pointer operator->() const{
            return &(operator*());
        }

        self& operator++(){
            this->inc();
            return *this;
        }

        self operator++(int){
            iterator tmp(*this);
            this->inc();
            return tmp;
        }

        self& operator--(){
            this->dec();
            return *this;
        }

        self operator--(int){
            iterator tmp(*this);
            this->dec();
            return tmp;
        }
    };

    template <class T>
    struct rb_tree_const_iterator : public rb_tree_iterator_base<T>{

        typedef rb_tree_traits<T> tree_traits;

        typedef typename tree_traits::value_type value_type;
        typedef typename tree_traits::mapped_type mapped_type;
        typedef typename tree_traits::key_type key_type;
        typedef typename tree_traits::pointer pointer;
        typedef typename tree_traits::const_reference const_reference;
        typedef typename tree_traits::reference reference;

        typedef typename tree_traits::base_ptr base_ptr;
        typedef typename tree_traits::node_ptr node_ptr;

        typedef rb_tree_iterator<T> iterator;
        typedef rb_tree_const_iterator<T> const_iterator;
        typedef const_iterator self;

        using rb_tree_iterator_base<T>::node;

        rb_tree_const_iterator(){}
        rb_tree_const_iterator(base_ptr x){
            node = x;
        }
        rb_tree_const_iterator(node_ptr x){
            node = x;
        }

        rb_tree_const_iterator(const iterator& rhs){
            node = rhs.node;
        }

        rb_tree_const_iterator(const const_iterator& rhs){
            node = rhs.node;
        }

        reference operator*() const{
            return node->get_node_ptr()->value;
        }

        pointer operator->() const{
            return &(operator*());
        }

        self& operator++(){
            this->inc();
            return *this;
        }

        self operator++(int){
            const_iterator tmp(*this);
            this->inc();
            return tmp;
        }

        self& operator--(){
            this->dec();
            return *this;
        }

        self operator--(int){
            const_iterator tmp(*this);
            this->dec();
            return tmp;
        }

    };


    //参数1 数据类型 参数2 键值比较类型
    template <class T, class Compare>
    class rb_tree{

    public:

        typedef rb_tree_traits<T> tree_traits;
        typedef rb_tree_value_traits<T> value_traits;

        typedef typename tree_traits::base_type base_type;
        typedef typename tree_traits::base_ptr base_ptr;
        typedef typename tree_traits::node_type node_type;
        typedef typename tree_traits::node_ptr node_ptr;
        typedef typename tree_traits::key_type key_type;
        typedef typename tree_traits::mapped_type mapped_type;
        typedef typename tree_traits::value_type value_type;

        typedef Compare key_compare;

        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef rb_tree_iterator<T> iterator;
        typedef rb_tree_const_iterator<T> const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

        key_compare key_comp() {return key_comp_;}

    private:
        base_ptr header_;//与根节点互为对方的父节点 ，左子节点为最小节点；右子节点为最大节点
        size_type node_count_;//节点数
        key_compare key_comp_;//节点键值比较的准则

    private:
        base_ptr& root() const{ return header_->parent;}
        base_ptr& leftmost() const{ return header_->left;}
        base_ptr& rightmost() const {return header_->right;}

    public:

        rb_tree(){
            rb_tree_init();
        }

        rb_tree(const rb_tree& rhs);

        rb_tree(rb_tree&& rhs) noexcept;

        rb_tree& operator=(const rb_tree& rhs);

        rb_tree& operator=(rb_tree&& rhs);

        ~rb_tree(){
            clear();
        }

        iterator begin() noexcept{
            return leftmost();
        }
        const_iterator begin() const noexcept{
            return leftmost();
        }
        iterator end() noexcept{
            return header_;
        }
        const_iterator end() const noexcept{
            return header_;
        }

        reverse_iterator rbegin() noexcept{
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const noexcept{
            return const_reverse_iterator(end());
        }
        reverse_iterator rend() noexcept{
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const noexcept{
            return const_reverse_iterator(begin());
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

        bool empty() const noexcept{return node_count_ == 0;}
        size_type size() const noexcept {return node_count_;}
        size_type max_size() const noexcept{return static_cast<size_type>(-1);}

        template <class ...Args>
        iterator emplace_multi(Args&& ...args);

        template <class ...Args>
        mystl::pair<iterator,bool> emplace_unique(Args&& ...args);//构造插入一个节点，返回插入节点的迭代器以及是否插入成功

        template <class ...Args>
        iterator emplace_multi_use_hint(iterator hint, Args&& ...args);//就地插入元素，键值允许重复，当 hint 位置与插入位置接近时，插入操作的时间复杂度可以降低<br />当红黑树没有元素的时候，直接插入到header_下面即可<br />当hint为begin()且插入值小于hint值的时候，直接插入到hint左边下面即可<br />当hint为end()且插入的值大于hint的值的时候，直接插入到hint的右边即可<br />剩下的情况需要插入到具体的位置，自己寻找位置了

        template <class ...Args>
        iterator emplace_unique_use_hint(iterator hint, Args&& ...args);//就地插入元素，键值不允许重复，当 hint 位置与插入位置接近时，插入操作的时间复杂度可以降低<br />需要考虑的情况和multi一样

        iterator insert_multi(const value_type& value);//插入到指定位置，返回插入节点的迭代器
        iterator insert_multi(value_type&& value){
            return emplace_multi(std::move(value));
        }

        iterator insert_multi(iterator hint, const value_type& value){
            return emplace_multi_use_hint(hint,value);
        }

        iterator insert_multi(iterator hint, value_type&& value){
            return emplace_multi_use_hint(hint, std::move(value));
        }

        template <class Iter>
        void insert_multi(Iter first, Iter last);

        mystl::pair<iterator,bool> insert_unique(const value_type& value);

        mystl::pair<iterator,bool> omsert_unique(value_type&& value){
            return emplace_unique(std::move(value));
        }

        iterator insert_unique(iterator hint, const value_type& value){
            return emplace_unique_use_hint(hint,value);
        }

        iterator insert_unique(iterator hint, value_type&& value){
            return emplace_unique_use_hint(hint,std::move(value));
        }

        template <class Iter>
        void insert_unique(Iter first, Iter last);

        iterator erase(iterator hint);

        size_type erase_multi(const key_type& key);
        size_type erase_unique(const key_type& key);




        void erase(iterator first, iterator last);
        void clear();
        iterator find(const key_type& key);
        const_iterator find(const key_type& key) const;

        size_type count_multi(const key_type& key) const{
            auto p = equal_range_multi(key);
            return static_cast<size_type>(mystl::distance(p.first,p.second));
        }

        size_type count_unique(const key_type& key) const{
            return find(key) != end() ? 1 : 0;
        }

        iterator lower_bound(const key_type& key);//返回键值不小于key的第一个位置的迭代器
        const_iterator lower_bound(const key_type& key) const;

        iterator upper_bound(const key_type& key);//返回键值不小于 key 的最后一个位置的迭代器
        const_iterator upper_bound(const key_type& key) const;

        mystl::pair<iterator,iterator> equal_range_multi(const key_type& key){//返回查找key的第一个位置和最后一个位置，都是闭区间
            return mystl::pair<iterator,iterator>(lower_bound(key), upper_bound(key));
        }
        mystl::pair<const_iterator, const_iterator> equal_range_multi(const key_type& key) const{
            return mystl::pair<const_iterator,const_iterator>(lower_bound(key), upper_bound(key));
        }

        mystl::pair<iterator,iterator> equal_range_unique(const key_type& key){
            iterator it = find(key);
            iterator next = it;
            return it == end() ? mystl::pair<iterator,iterator>(end(),end()) : mystl::pair<iterator,iterator>(it,++next);
        }

        mystl::pair<const_iterator,const_iterator> equal_range_unique(const key_type& key) const{
            const_iterator it = find(key);
            auto next = it;
            return it == end() ? mystl::make_pair(end(),end()) : mystl::make_pair(it,++next);
        }

        void swap(rb_tree& rhs) noexcept;











    private:
        template <class...Args>
        node_ptr create_node(Args&&...args);//创建一个节点，其左右节点和父亲都设置为nullptr,将其value设置初始化,返回一个node_ptr，color默认为红色
        node_ptr clone_node(base_ptr x);//克隆一个节点的值和颜色，其父亲和左右子节点都为nullptr
        void destroy_node(node_ptr p);//销毁一个节点，首先是调用value的析构函数，然后释放这个指针节点
        void rb_tree_init();//创建header_，并将其颜色设置为红色，将root设置为空，header\_的左子节点和右子接待那设置为header\_,node_count设置为0
        void reset();//header_设置为nullptr,node\_count\_设置为0
        mystl::pair<base_ptr,bool> get_insert_multi_pos(const key_type& key);//找到key应该存放的位置，返回一个pair,其first为要插入位置的父节点，second为true,表示是否应该插入父节点的左边
        mystl::pair<mystl::pair<base_ptr,bool>,bool> get_insert_unique_pos(const key_type& key);//找到key应该存放的位置，返回一个pair,其first也是一个pair,first.first表示要插入节点的父节点，first.second表示是否要插入到左子节点，second表示插入节点是否没有重复，true表示没有重复，false表示重复
        iterator insert_value_at(base_ptr x, const value_type& key, bool add_to_left);//x插入节点的父节点，value要插入的值，add_to_left是否插入到左子节点<br />插入情况要考虑的情况和下面的函数差不多，返回插入节点的迭代器
        iterator insert_node_at(base_ptr x, node_ptr node, bool add_to_left);//x插入节点的父节点，node是要插入的节点，add_to_left表示是否要插入到父节点的左子节点<br />将node的父节点设置为x<br />如果x是header_说明此时插入的节点是root,也就是插入之前一个节点都没有，此时直接插入到header\_的下面即可<br />如果不是header\_就根据add_to_left确定其插入到左节点还是右节点<br />在插入后，需要同时更新最小节点和最大节点<br />返回插入节点的迭代器
        iterator insert_multi_use_hint(iterator hint, key_type key, node_ptr node);
        iterator insert_unique_use_hint(iterator hint, key_type key, node_ptr node);
        base_ptr copy_from(base_ptr x, base_ptr p);//递归复制以x为根节点的子树，p是x的parent,返回的是复制后的x的base_ptr
        void erase_since(base_ptr x);//删除x为根节点的子树
    };

    /**************************************public*******************************************/

    template <class T, class Compare>
    rb_tree<T,Compare>::rb_tree(const rb_tree& rhs) {
        rb_tree_init();
        if(rhs.node_count_ != 0){
            root() = copy_from(rhs.root(),header_);
            leftmost() = rb_tree_min(root());
            rightmost() = rb_tree_max(root());
        }
        node_count_ = rhs.node_count_;
        key_comp_ = rhs.key_comp_;
    }

    template <class T, class Compare>
    rb_tree<T,Compare>::rb_tree(rb_tree&& rhs) noexcept:header_(std::move(rhs.header_)), node_count_(std::move(rhs.node_count_)),key_comp_(rhs.key_comp_){
        rhs.reset();
    }

    template <class T, class Compare>
    rb_tree<T,Compare>& rb_tree<T,Compare>::operator=(const rb_tree &rhs) {
        if (this != &rhs)
        {
            clear();

            if (rhs.node_count_ != 0)
            {
                root() = copy_from(rhs.root(), header_);
                leftmost() = rb_tree_min(root());
                rightmost() = rb_tree_max(root());
            }

            node_count_ = rhs.node_count_;
            key_comp_ = rhs.key_comp_;
        }
        return *this;
    }

    template <class T, class Compare>
    rb_tree<T,Compare>& rb_tree<T,Compare>::operator=(rb_tree<T, Compare> &&rhs) {
        clear();
        header_ = mystl::move(rhs.header_);
        node_count_ = rhs.node_count_;
        key_comp_ = rhs.key_comp_;
        rhs.reset();
        return *this;
    }

    template <class T, class Compare>
    template <class ...Args>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::emplace_multi(Args &&...args) {
        node_ptr new_node = create_node(std::forward<Args>(args)...);
        auto p = get_insert_multi_pos(value_traits::get_key(new_node->value));
        return insert_node_at(p.first,new_node,p.second);
    }

    template <class T, class Compare>
    template <class ...Args>
    mystl::pair<typename rb_tree<T,Compare>::iterator,bool> rb_tree<T,Compare>::emplace_unique(Args &&...args) {
        node_ptr new_node = create_node(std::forward<Args>(args)...);
        auto pp = get_insert_unique_pos(value_traits::get_key(new_node->value));
        if(pp.second){
            //表明此时重复
            return mystl::pair<iterator,bool>(iterator(),false);
        }
        auto it = insert_node_at(pp.first.first,new_node,pp.first.second);
        return mystl::pair<iterator,bool>(it,true);
    }

    template <class T, class Compare>
    template <class ...Args>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::emplace_multi_use_hint(iterator hint, Args &&...args) {
        node_ptr new_node = create_node(std::forward<Args>(args)...);

        if(root() == nullptr){
            root() = new_node;
            new_node->parent = header_;
            node_count_++;
            leftmost() = new_node;
            rightmost() = new_node;
            return iterator(new_node);
        }

        if(hint == begin() && key_comp_(value_traits::get_key(new_node->value), value_traits::get_key(*hint))){
            hint.node->left = new_node;
            new_node->parent = hint.node;
            leftmost() = new_node;
            node_count_++;
            return iterator(new_node);
        }

        if(hint == end() && key_comp_(value_traits::get_key(new_node->value),value_traits::get_key(rightmost()->get_node_ptr()->value)) ){
            rightmost()->right = new_node;
            new_node->parent = rightmost();
            node_count_++;
            rightmost() = new_node;
            return iterator(new_node);
        }

        auto p = get_insert_multi_pos(value_traits::get_key(new_node->value));
        return insert_node_at(p.first,new_node,p.second);
    }

    template <class T, class Compare>
    template <class ...Args>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::emplace_unique_use_hint(iterator hint, Args &&...args) {

        node_ptr new_node = create_node(std::forward<Args>(args)...);

        if(root() == nullptr){
            root() = new_node;
            leftmost() = new_node ;
            rightmost() = new_node;
            node_count_++;
            return iterator(new_node);
        }

        if(hint == begin() && key_comp_(value_traits::get_key(new_node->value),value_traits::get_key(*begin()))){
            leftmost()->left = new_node;
            new_node->parent = leftmost();
            node_count_++;
            leftmost() = new_node;
            return iterator(new_node);
        }

        if(hint == end() && key_comp_(value_traits::get_key(rightmost()->get_node_ptr()->value),value_traits::get_key(new_node->value))){
            rightmost()->right = new_node;
            new_node->parent = rightmost();
            node_count_++;
            rightmost() = new_node;
            return iterator(new_node);
        }

        auto pp = get_insert_unique_pos(value_traits::get_key(new_node->value));
        if(pp.second){//此时不需要连接，因为重复
            return iterator(new_node);
        }

        return insert_node_at(pp.first.first,new_node,pp.first.second);


    }


    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::insert_multi(const value_type &value) {
        node_ptr new_node = create_node(value);
        auto p = get_insert_multi_pos(value_traits::get_key(value));
        return insert_node_at(p.first,new_node,p.second);
    }

    template <class T, class Compare>
    template <class Iter>
    void rb_tree<T,Compare>::insert_multi(Iter first, Iter last) {
        while(first!=last){
            insert_multi(*first);
            ++first;
        }
    }

    template <class T, class Compare>
    mystl::pair<typename rb_tree<T,Compare>::iterator, bool> rb_tree<T,Compare>::insert_unique(const value_type &value) {

        node_ptr new_node = create_node(value);
        auto pp = get_insert_unique_pos(value_traits::get_key(new_node->value));
        if(pp.second){
            //此时重复
            return make_pair(iterator(),false);
        }
        return make_pair(insert_node_at(pp.first.first,new_node,pp.first.second),true);
    }

    template <class T, class Compare>
    template <class Iter>
    void rb_tree<T,Compare>::insert_unique(Iter first, Iter last){
        while(first!=last){
            insert_unique(*first);
            ++first;
        }
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::erase(iterator hint) {
        node_ptr next_node = rb_tree_next(hint.node->get_base_ptr())->get_node_ptr();
        node_ptr old_node = rb_tree_erase_rebalance(hint.node, root(), leftmost(), rightmost())->get_node_ptr();
        destroy_node(old_node);
        node_count_--;
        return iterator(next_node);
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::size_type rb_tree<T,Compare>::erase_multi(const key_type &key) {
        auto p = equal_range_multi(key);
        size_type n = mystl::distance(p.first, p.second);
        erase(p.first, p.second);
        return n;
    }

    template <class T, class Compare>
    typename rb_tree<T, Compare>::size_type rb_tree<T,Compare>::erase_unique(const key_type &key) {
        auto it = find(key);
        if(it != end()){
            erase(it);
            return 1;
        }
        return 0;
    }

    template <class T, class Compare>
    void rb_tree<T,Compare>::erase(iterator first, iterator last) {

        if(first == begin() && last == end()){
            clear();
            return ;
        }

        while(first != last){
            first = erase(first);
        }
    }

    template <class T, class Compare>
    void rb_tree<T,Compare>::clear() {
        if(node_count_!=0){
            erase_since(root());
            leftmost() = header_;
            rightmost() = header_;
            root() = nullptr;
            node_count_ = 0;
        }

    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::find(const key_type &key) {

        auto y = header_;  // 最后一个不小于 key 的节点
        auto x = root();
        while (x != nullptr)
        {
            if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
            { // key 小于等于 x 键值，向左走
                y = x, x = x->left;
            }
            else
            { // key 大于 x 键值，向右走
                x = x->right;
            }
        }
        iterator j = iterator(y);
        return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::const_iterator rb_tree<T,Compare>::find(const key_type &key) const {

        auto y = header_;  // 最后一个不小于 key 的节点
        auto x = root();
        while (x != nullptr)
        {
            if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
            { // key 小于等于 x 键值，向左走
                y = x, x = x->left;
            }
            else
            { // key 大于 x 键值，向右走
                x = x->right;
            }
        }
        const_iterator j = const_iterator(y);
        return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::lower_bound(const key_type &key) {
        //返回键值不小于key的第一个位置的迭代器

        auto y = header_;
        auto x = root();

        while(x){

            if(!key_comp_(value_traits::get_key(x->get_node_ptr()->value),key)){
                //x>=key
                y = x;
                x = x->left;
            }
            else{
                x = x->right;
            }
        }

        return iterator(y);
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::const_iterator rb_tree<T,Compare>::lower_bound(const key_type &key) const{

        auto y = header_;
        auto x = root();

        while(x){

            if(!key_comp_(value_traits::get_key(x->get_node_ptr()->value),key)){
                //x>=key
                y = x;
                x = x->left;
            }
            else{
                x = x->right;
            }
        }

        return const_iterator(y);

    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::upper_bound(const key_type &key) {
        auto y = header_;
        auto x = root();
        while (x != nullptr)
        {
            if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value)))
            { // key < x
                y = x, x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        return iterator(y);
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::const_iterator rb_tree<T,Compare>::upper_bound(const key_type &key) const{

        auto y = header_;
        auto x = root();
        while (x != nullptr)
        {
            if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value)))
            { // key < x
                y = x, x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        return const_iterator(y);

    }










    /***************************************帮助函数******************************************/
    template <class T, class Compare>
    template <class ...Args>
    typename rb_tree<T,Compare>::node_ptr rb_tree<T,Compare>::create_node(Args&& ...args){
        node_ptr new_node = new node_type(args...);
        new_node->parent = nullptr;
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->color = rb_tree_red;
        return new_node;
    }

    template <class T, class Compare>
    typename rb_tree<T, Compare>::node_ptr rb_tree<T, Compare>::clone_node(base_ptr x) {
        node_ptr new_node = create_node(x->get_node_ptr()->value);
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->parent = nullptr;
        new_node->color = x->color;
        return new_node;
    }

    template <class T, class Compare>
    void rb_tree<T, Compare>::destroy_node(node_ptr p) {
        delete p;
    }

    template <class T, class Compare>
    void rb_tree<T, Compare>::rb_tree_init() {
        header_ = new node_type();
        header_->color = rb_tree_red;
        root() = nullptr;
        leftmost() = nullptr;
        rightmost() = nullptr;
        node_count_ = 0;
    }

    template <class T, class Compare>
    void rb_tree<T,Compare>::reset() {
        header_ = nullptr;
        node_count_ = 0;
    }

    template <class T, class Compare>
    mystl::pair<typename rb_tree<T,Compare>::base_ptr, bool> rb_tree<T,Compare>::get_insert_multi_pos(const key_type &key) {
        auto parent = header_;
        auto now = root();
        bool add_is_left = true;
        while(now){
            parent = now;
            if(key_comp_(key,value_traits::get_key(now->get_node_ptr()->value))) {
                now = now->left;
                add_is_left = true;
            }
            else {
                now = now->right;
                add_is_left = false;
            }

        }
        return mystl::pair<base_ptr,bool>(parent,add_is_left);
    }

    template <class T, class Compare>
    mystl::pair<mystl::pair<typename rb_tree<T,Compare>::base_ptr ,bool>,bool>  rb_tree<T,Compare>::get_insert_unique_pos(const key_type &key) {

        auto parent = header_;
        auto now = root();
        bool add_is_left = true;
        bool is_repeat = false;
        while(now){
            parent = now;
            if(key == value_traits::get_key(now->get_node_ptr()->value)){
                is_repeat = true;
            }
            if(key_comp_(key,value_traits::get_key(now->get_node_ptr()->value))) {
                now = now->left;
                add_is_left = true;
            }
            else {
                now = now->right;
                add_is_left = false;
            }

        }
        return mystl::pair<mystl::pair<base_ptr,bool>,bool>(mystl::pair<base_ptr ,bool>(parent,add_is_left),is_repeat);
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::insert_value_at(base_ptr x,const value_type &key,bool add_to_left) {

        if(x == header_){
            root() = create_node(key);
            root()->parent = header_;
            leftmost() = root();
            rightmost() = root();
            node_count_+=1;
            return iterator(root());
        }

        node_ptr new_node = create_node(key);
        new_node->parent = x;
        if(add_to_left){
            x->left = new_node;
            if(leftmost() == x) leftmost() = new_node;
        }
        else{
            x->right = new_node;
            if(rightmost() == x) rightmost() = new_node;
        }
        node_count_++;
        rb_tree_insert_rebalance(new_node,root());
        return iterator(new_node);
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::insert_node_at(base_ptr x, node_ptr node, bool add_to_left) {

        node_count_++;
        node->parent = x;

        if(header_ == x){
            root() = node;
            leftmost() = node;
            node->parent = header_;
            rightmost() = node;
            return iterator(node);
        }

        if(add_to_left){
            x->left = node;
            if(leftmost() == x) leftmost() = node;
        }
        else{
            x->right = node;
            if(rightmost() == x) rightmost() = node;
        }

        rb_tree_insert_rebalance(node->get_base_ptr(),root());
        return iterator(node);
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::insert_multi_use_hint(iterator hint, key_type key, node_ptr node) {


        auto before = hint;
        before--;
        auto bnp = before.node;
        auto np = hint.node;
        if(!key_comp_(key,value_traits::get_key(*hint)) && !key_comp_(value_traits::get_key(*hint),key)){
            //before<=node<=hint
            if(bnp->right == nullptr)
                return insert_node_at(bnp,node,false);
            else if(np->left == nullptr)
                return insert_node_at(np,node,true);
        }

        auto pos = get_insert_multi_pos(key);
        return insert_node_at(pos.first,node,pos.second);
    }

    template <class T, class Compare>
    typename rb_tree<T,Compare>::iterator rb_tree<T,Compare>::insert_unique_use_hint(iterator hint, key_type key, node_ptr node) {
        // 在 hint 附近寻找可插入的位置
        auto np = hint.node;
        auto before = hint;
        --before;
        auto bnp = before.node;
        if (key_comp_(value_traits::get_key(*before), key) &&
            key_comp_(key, value_traits::get_key(*hint)))
        { // before < node < hint
            if (bnp->right == nullptr)
            {
                return insert_node_at(bnp, node, false);
            }
            else if (np->left == nullptr)
            {
                return insert_node_at(np, node, true);
            }
        }
        auto pos = get_insert_unique_pos(key);
        if (!pos.second)
        {
            destroy_node(node);
            return pos.first.first;
        }
        return insert_node_at(pos.first.first, node, pos.first.second);
    }



    template <class T, class Compare>
    typename rb_tree<T,Compare>::base_ptr rb_tree<T,Compare>::copy_from(base_ptr x, base_ptr p) {
        if(x==nullptr) return x;
        node_ptr now = clone_node(x);
        now->parent = p;
        if(x->right){
            now->right = copy_from(x->right,now);

        }
        if(x->left){
            now->left = copy_from(x->left,now);

        }
        return now;
    }

    template <class T, class Compare>
    void rb_tree<T,Compare>::erase_since(base_ptr x) {

        if(x == nullptr) return ;

        if(x->parent == header_) header_->parent = nullptr;
        else if(rb_tree_is_lchild(x)) x->parent->left = nullptr;
        else x->parent->right = nullptr;

        if(x->left) erase_since(x->left);
        if(x->right) erase_since(x->right);

        destroy_node(x->get_node_ptr());
    }

    template <class T, class Compare>
    bool operator==(const rb_tree<T,Compare>& lhs, const rb_tree<T,Compare>& rhs){
        if(lhs.size() != rhs.size()) return false;

        auto f1 = lhs.begin();
        auto f2 = rhs.begin();
        auto l1 = lhs.end();
        auto l2 = rhs.end();

        while(f1!=l1){
            if(*f1 != *f2) return false;
            ++f1;
            ++f2;
        }
        return true;
    }

    template <class T, class Compare>
    bool operator<(const rb_tree<T,Compare>& lhs, const rb_tree<T,Compare>& rhs){

        auto f1 = lhs.begin();
        auto f2 = rhs.begin();
        auto l1 = lhs.end();
        auto l2 = rhs.end();

        while(f1!=l1 && f2!=l2){
            if(*f1>=*f2) return false;
            ++f1;
            ++f2;
        }

        if(f2!=l2) return true;
        if(f1!=l1) return false;
        if(f1==l1 && f2==l2) return false;
    }

    template <class T, class Compare>
    bool operator!=(const rb_tree<T,Compare>& lhs, const rb_tree<T,Compare>& rhs){
        return  !(lhs == rhs);
    }

    template <class T, class Compare>
    bool operator>(const rb_tree<T,Compare>& lhs, const rb_tree<T,Compare>& rhs){
        return rhs<lhs;
    }

    template <class T, class Compare>
    bool operator<=(const rb_tree<T,Compare>& lhs, const rb_tree<T,Compare>& rhs){
        return !(lhs > rhs);
    }

    template <class T, class Compare>
    bool operator>=(const rb_tree<T,Compare>& lhs, const rb_tree<T,Compare>& rhs){
        return rhs<lhs;
    }



    template <class Ty>
    void my_swap(Ty& t1, Ty& t2){
        Ty temp(std::move(t1));
        t1 = std::move(t2);
        t2 = std::move(temp);
    }

    template <class T, class Compare>
    void rb_tree<T,Compare>::swap(rb_tree<T, Compare> &rhs) noexcept {
        if(this != &rhs){
            my_swap(header_, rhs.header_);
            my_swap(node_count_, rhs.node_count_);
            my_swap(key_comp_, rhs.key_comp_);
        }
    }












































}


#endif //MYSTL_RB_TREE_H
