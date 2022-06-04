/*
 * Copyright (c) 2019,2022 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "hash.hpp"
#include "nameof.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <type_traits>

namespace nc {

namespace detail
{
    template <size_t _UniqueId, typename T>
    constexpr hash_t template_hash()
    {
        constexpr auto type_name = NAMEOF_TYPE(T);
        constexpr auto hash = fnv1a_hash(type_name.size(), &type_name[0]);

        return (hash ^ _UniqueId);
    }

    constexpr hash_t hash_location(const char* file, hash_t line)
    {
        return fnv1a_hash(strlen(file), file)
               ^ line
               ^ (line << 16)
               ^ (line << 32)
               ^ (line << 48);
    }

    template <hash_t _Hash, typename _Res, typename... _Args>
    struct funcptr_helper
    {
        public:

            using return_type = _Res;
            using function_type = std::function<_Res(_Args...)>;

            static inline void
            bind(const function_type& f)
            {
                instance()._fcn = f;
            }

            static inline void
            bind(function_type&& f)
            {
                instance()._fcn.swap(f);
            }

            static inline return_type
            invoke(_Args... args)
            {
                assert(instance()._fcn);
                return instance()._fcn(args...);
            }

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

    template <hash_t hash, typename _Res, typename... _Args>
    auto stdfunction_to_funcptr(std::function<_Res(_Args...)>&& f)
    {
        assert(f);
        detail::funcptr_helper<hash, _Res, _Args...>::bind(std::move(f));
        return detail::funcptr_helper<hash, _Res, _Args...>::ptr();
    }
}

}
