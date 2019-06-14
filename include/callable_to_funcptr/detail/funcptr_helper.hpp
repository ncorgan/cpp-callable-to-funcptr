/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CALLABLE_TO_FUNCPTR_FUNCPTR_HELPER_HPP
#define CALLABLE_TO_FUNCPTR_FUNCPTR_HELPER_HPP

#ifndef CALLABLE_TO_FUNCPTR_HPP
#error Do not include this file directly!
#endif

#if !defined(BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif
#if !defined(END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif

#include "../callable/callable.hpp"
#include "sfinae.hpp"

#include <cstdlib>
#include <functional>
#include <type_traits>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

namespace detail
{
    // TODO: support enum unique IDs and other literals
    template <size_t _UniqueId, typename _Callable>
    struct funcptr_helper
    {
        public:

            using return_type = typename callable_traits<_Callable>::return_type;
            using function_type = std::function<typename callable_traits<_Callable>::function_type>;

            static inline void
            bind(const _Callable& f)
            {
                instance()._fcn = to_stdfunction(f);
            }

            template <typename... _ArgTypes>
            static inline return_type
            invoke(_ArgTypes... args)
            {
                return instance()._fcn(args...);
            }

            //using pointer_type = decltype(&funcptr_helper::invoke);
            using pointer_type = decltype(&funcptr_helper::invoke);
            static pointer_type
            ptr()
            {
                return &invoke;
            }

        private:

            static funcptr_helper& instance()
            {
                static thread_local funcptr_helper _instance;
                return _instance;
            };

            funcptr_helper() {}

            function_type _fcn;
    };
}

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif /* CALLABLE_TO_FUNCPTR_FUNCPTR_HELPER_HPP */
