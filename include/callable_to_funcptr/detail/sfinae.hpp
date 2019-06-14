/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CALLABLE_TO_FUNCPTR_SFINAE_HPP
#define CALLABLE_TO_FUNCPTR_SFINAE_HPP

#include <functional>
#include <type_traits>

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

#endif /* CALLABLE_TO_FUNCPTR_SFINAE_HPP */
