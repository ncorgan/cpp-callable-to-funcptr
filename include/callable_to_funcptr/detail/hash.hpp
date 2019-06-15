/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CALLABLE_TO_FUNCPTR_HASH_HPP
#define CALLABLE_TO_FUNCPTR_HASH_HPP

#ifndef CALLABLE_TO_FUNCPTR_HPP
#error Do not include this file directly!
#endif

#if !defined(BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif
#if !defined(END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif

#include <cstdint>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

namespace detail
{
    using hash_t = std::uint64_t;

    // See http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
    constexpr hash_t fnv_basis = 14695981039346656037ull;
    constexpr hash_t fnv_prime = 1099511628211ull;

    // FNV-1a 64 bit hash
    constexpr hash_t fnv1a_hash(std::size_t n, const char *str, hash_t hash = fnv_basis)
    {
        return n > 0 ? fnv1a_hash(n - 1, str + 1, (hash ^ *str) * fnv_prime) : hash;
    }

    // See https://github.com/Manu343726/ctti/blob/master/include/ctti/detail/hash.hpp
    template<std::size_t N>
    constexpr hash_t fnv1a_hash(const char (&array)[N])
    {
        return fnv1a_hash(N - 1, &array[0]);
    }
}

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif /* CALLABLE_TO_FUNCPTR_HASH_HPP */
