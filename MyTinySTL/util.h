//
// Created by 孙建钊 on 2023/6/13.
//

#ifndef MYSTL_UTIL_H
#define MYSTL_UTIL_H

// 这个文件包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等
#include <cstddef>
#include "type_traits.h"
#include "iterator.h"


namespace mystl{

// move

    template <class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

// forward

    template <class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept
    {
        return static_cast<T&&>(arg);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
    {
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T&&>(arg);
    }

// swap

    template <class Tp>
    void swap(Tp& lhs, Tp& rhs)
    {
        auto tmp(mystl::move(lhs));
        lhs = mystl::move(rhs);
        rhs = mystl::move(tmp);
    }

    template <class ForwardIter1, class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
    {
        for (; first1 != last1; ++first1, (void) ++first2)
            mystl::swap(*first1, *first2);
        return first2;
    }

    template <class Tp, size_t N>
    void swap(Tp(&a)[N], Tp(&b)[N])
    {
        mystl::swap_range(a, a + N, b);
    }


    template <class Ty1, class Ty2>
    struct pair  {

            typedef Ty1 first_type;
            typedef Ty2 second_type;

            first_type first;
            second_type second;

        public:
            constexpr pair():first(first_type()),second(second_type()){}
            constexpr pair(const first_type& a, const second_type& b):first(a),second(b){}

        template <class U1 = Ty1, class U2 = Ty2,
                typename std::enable_if<
                        std::is_copy_constructible<U1>::value &&
                        std::is_copy_constructible<U2>::value &&
                        std::is_convertible<const U1&, Ty1>::value &&
                        std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
        constexpr pair(const Ty1& a, const Ty2& b)
                : first(a), second(b)
        {
        }

        template <class U1 = Ty1, class U2 = Ty2,
                typename std::enable_if<
                        std::is_copy_constructible<U1>::value &&
                        std::is_copy_constructible<U2>::value &&
                        (!std::is_convertible<const U1&, Ty1>::value ||
                         !std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
        explicit constexpr pair(const Ty1& a, const Ty2& b)
                : first(a), second(b)
        {
        }

        // implicit constructiable for other type
        template <class Other1, class Other2,
                typename std::enable_if<
                        std::is_constructible<Ty1, Other1>::value &&
                        std::is_constructible<Ty2, Other2>::value &&
                        std::is_convertible<Other1&&, Ty1>::value &&
                        std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
        constexpr pair(Other1&& a, Other2&& b)
                : first(mystl::forward<Other1>(a)),
                  second(mystl::forward<Other2>(b))
        {
        }

        // explicit constructiable for other type
        template <class Other1, class Other2,
                typename std::enable_if<
                        std::is_constructible<Ty1, Other1>::value &&
                        std::is_constructible<Ty2, Other2>::value &&
                        (!std::is_convertible<Other1, Ty1>::value ||
                         !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
        explicit constexpr pair(Other1&& a, Other2&& b)
                : first(mystl::forward<Other1>(a)),
                  second(mystl::forward<Other2>(b))
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
                typename std::enable_if<
                        std::is_constructible<Ty1, const Other1&>::value &&
                        std::is_constructible<Ty2, const Other2&>::value &&
                        std::is_convertible<const Other1&, Ty1>::value &&
                        std::is_convertible<const Other2&, Ty2>::value, int>::type = 0>
        constexpr pair(const pair<Other1, Other2>& other)
                : first(other.first),
                  second(other.second)
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
                typename std::enable_if<
                        std::is_constructible<Ty1, const Other1&>::value &&
                        std::is_constructible<Ty2, const Other2&>::value &&
                        (!std::is_convertible<const Other1&, Ty1>::value ||
                         !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
        explicit constexpr pair(const pair<Other1, Other2>& other)
                : first(other.first),
                  second(other.second)
        {
        }

        // implicit constructiable for other pair
        template <class Other1, class Other2,
                typename std::enable_if<
                        std::is_constructible<Ty1, Other1>::value &&
                        std::is_constructible<Ty2, Other2>::value &&
                        std::is_convertible<Other1, Ty1>::value &&
                        std::is_convertible<Other2, Ty2>::value, int>::type = 0>
        constexpr pair(pair<Other1, Other2>&& other)
                : first(mystl::forward<Other1>(other.first)),
                  second(mystl::forward<Other2>(other.second))
        {
        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,
                typename std::enable_if<
                        std::is_constructible<Ty1, Other1>::value &&
                        std::is_constructible<Ty2, Other2>::value &&
                        (!std::is_convertible<Other1, Ty1>::value ||
                         !std::is_convertible<Other2, Ty2>::value), int>::type = 0>
        explicit constexpr pair(pair<Other1, Other2>&& other)
                : first(mystl::forward<Other1>(other.first)),
                  second(mystl::forward<Other2>(other.second))
        {
        }


            pair(const pair& rhs):first(rhs.first),second(rhs.second){}
            pair(const pair&& rhs):first(std::move(rhs.first)),second(std::move(rhs.second)){

            }
            //constexpr pair(first_type&& a, second_type&& b):first(std::move(a)),second(std::move(b)){}

            pair& operator=(const pair& rhs){
                first = rhs.first;
                second = rhs.second;
                return *this;
            }

            pair& operator=(pair&& rhs){
                first = std::move(rhs.first);
                second = std::move(rhs.second);
                return *this;
            }

            ~pair() = default;


        };

//    template <class Ty1, class Ty2>
//    pair<Ty1,Ty2> make_pair(Ty1&& first, Ty2&& second){
//        return pair<Ty1,Ty2>(std::forward<Ty1>(first),std::forward<Ty2>(second));
//    }


    template <class Ty1, class Ty2>
    pair<Ty1,Ty2> make_pair(Ty1&& first, Ty2&& second){
        return pair<Ty1,Ty2>(std::forward<Ty1>(first),std::forward<Ty2>(second));
    }











}


#endif //MYSTL_UTIL_H