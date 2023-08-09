### stack

代码仓库：`https://github.com/sjz-hub/mystl`

+ stack是一种容器适配器，提供了一种简化和封装底层容器操作的方式，以便使用特定的数据结构和功能
+ stack本身并没有什么需要特别编写的地方，主要实现了构造函数、top() pop() push()等几个接口，并且每个接口都调用的具体底层结构的方法接口



```c++
template <class T, class Container = mystl::deque<T>>
class stack{
    
}
```

+ stack的成员是一个`Container c_` 用底层容器表现stack







stack的public方法

| 方法名                                                       | 含义                                     |
| ------------------------------------------------------------ | ---------------------------------------- |
| stack() = default;                                           | 默认构造函数                             |
| explicit stack(size_type n)                                  | 构造函数，构造包含n个默认值              |
| stack(size_type n, const value_type& value)                  | 构造函数，构造包含n个指定值              |
| template <class IIter> stack(IIter first, IIter last)        | 构造函数，使用两个迭代器之间的值进行构造 |
| stack(std::initializer_list<T> ilist)                        | 使用列表的方式构造                       |
| stack(const Container& c)                                    | 使用本身容器对象的方式构造               |
| stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value) | 移动构造函数                             |
| stack(const stack& rhs)                                      | 拷贝构造函数                             |
| stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) | 移动构造函数                             |
| stack& operator=(const stack& rhs)                           | 拷贝赋值运算符                           |
| stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value) | 移动赋值运算符                           |
| stack& operator=(std::initializer_list<T> ilist)             | 赋值运算符                               |
| ~stack() = default;                                          | 析构函数                                 |
| reference       top()                                        | 返回栈顶元素                             |
| const_reference top() const                                  | 返回栈顶元素                             |
| bool      empty() const noexcept                             | 判断是否为空                             |
| size_type size()  const noexcept                             | 返回容器大小                             |
| template <class... Args> void emplace(Args&& ...args)        | 在栈顶构造一个元素                       |
| void push(const value_type& value)                           | 向栈顶添加一个元素                       |
| void push(value_type&& value)                                | 向栈顶添加一个元素                       |
| void pop()                                                   | 弹出栈顶元素                             |
| void clear()                                                 | 清空                                     |
| void swap(stack& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_))) | 两个stack交换                            |
| friend bool operator==(const stack& lhs, const stack& rhs)   |                                          |
| friend bool operator< (const stack& lhs, const stack& rhs)   |                                          |

重载操作符

| 方法名                                                       | 含义 |
| ------------------------------------------------------------ | ---- |
| template <class T, class Container> bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) |      |
| template <class T, class Container> bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) |      |
| template <class T, class Container> bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) |      |
| template <class T, class Container> bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) |      |
| template <class T, class Container> bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) |      |
| template <class T, class Container> bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) |      |

代码仓库：`https://github.com/sjz-hub/mystl`