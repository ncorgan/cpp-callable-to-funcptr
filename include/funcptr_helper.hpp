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

namespace detail
{

    // TODO: support enum unique IDs and other literals
    // TODO: thread-local storage to avoid collision
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
                static funcptr_helper _instance;
                return _instance;
            };

            funcptr_helper() {}

            function_type _fcn;
    };
}

template<typename T>
std::function<typename std::enable_if<std::is_function<T>::value, T>::type>
make_function(T *t)
{
    return {t};
}

template <size_t _UniqueId, typename _Res, typename... _ArgTypes>
typename detail::funcptr_helper<_UniqueId, _Res, _ArgTypes...>::pointer_type
get_funcptr(const std::function<_Res(_ArgTypes...)>& f)
{
    detail::funcptr_helper<_UniqueId, _Res, _ArgTypes...>::bind(f);
    return detail::funcptr_helper<_UniqueId, _Res, _ArgTypes...>::ptr();
}

#endif /* FUNCPTR_HELPER_HPP */
