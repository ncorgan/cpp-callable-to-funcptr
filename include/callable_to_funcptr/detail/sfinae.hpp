/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CALLABLE_TO_FUNCPTR_SFINAE_HPP
#define CALLABLE_TO_FUNCPTR_SFINAE_HPP

#ifndef CALLABLE_TO_FUNCPTR_HPP
#error Do not include this file directly!
#endif

#if !defined(BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif
#if !defined(END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif

#include <functional>
#include <type_traits>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

namespace detail
{
    // https://stackoverflow.com/a/42481160
    template <typename T>
    class has_parentheses
    {
        private:
            typedef char one;
            typedef long two;

            template <typename C> static one test(decltype(&C::operator()));
            template <typename C> static two test(...);

        public:
            enum { value = (sizeof(test<T>(0)) == sizeof(char)) };
    };

    // Function pointers
    template <typename T>
    using enable_function_if_functional =
        typename std::enable_if<
                     std::is_function<T>::value,
                     std::function<T>
                 >::type;
}

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif /* CALLABLE_TO_FUNCPTR_SFINAE_HPP */
