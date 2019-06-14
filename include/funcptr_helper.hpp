/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef FUNCPTR_HELPER_HPP
#define FUNCPTR_HELPER_HPP

#include <cstdlib>
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

    template <typename T> enable_function_if_functional<T>
    to_func(T* t)
    {
        return {t};
    }

    // TODO: support enum unique IDs and other literals
    template <size_t _UniqueId, typename _Res, typename... _ArgTypes>
    struct funcptr_helper
    {
        public:

            using function_type = std::function<_Res(_ArgTypes...)>;

            static inline void bind(const function_type& f)
            {
                instance()._fcn = f;
            }

            static inline void bind(function_type&& f)
            {
                instance().fcn.swap(f);
            }

            static inline _Res invoke(_ArgTypes... args)
            {
                return instance()._fcn(args...);
            }

            using pointer_type = decltype(&funcptr_helper::invoke);
            static pointer_type ptr()
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

template <size_t _UniqueId, typename _Res, typename... _ArgTypes>
auto get_funcptr(const std::function<_Res(_ArgTypes...)>& f)
{
    detail::funcptr_helper<_UniqueId, _Res, _ArgTypes...>::bind(f);
    return detail::funcptr_helper<_UniqueId, _Res, _ArgTypes...>::ptr();
}

template <size_t _UniqueId, typename _Functor>
auto get_funcptr(_Functor f)
{
    return get_funcptr<_UniqueId>(detail::to_func(f));
}

#endif /* FUNCPTR_HELPER_HPP */
