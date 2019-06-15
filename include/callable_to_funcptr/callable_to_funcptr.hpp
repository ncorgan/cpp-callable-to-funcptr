/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CALLABLE_TO_FUNCPTR_HPP
#define CALLABLE_TO_FUNCPTR_HPP

// Allow the caller to put this in their own namespace.
#ifdef CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#define BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE \
    namespace CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE {
#define END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE \
    }

#else

#define BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE
#define END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif

#include "detail/callable/callable.hpp"
#include "detail/funcptr_helper.hpp"

#include <functional>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

namespace detail
{
    template <hash_t hash, typename _Res, typename... _Args>
    auto callable_to_funcptr(std::function<_Res(_Args...)>&& f)
    {
        detail::funcptr_helper<hash, _Res, _Args...>::bind(std::move(f));
        return detail::funcptr_helper<hash, _Res, _Args...>::ptr();
    }
}

template <size_t _UniqueId, typename _Callable>
auto callable_to_funcptr(_Callable f)
{
    auto func = to_stdfunction(f);

    return detail::callable_to_funcptr<detail::template_hash<_UniqueId, _Callable>()>(std::move(func));
}

template <size_t _UniqueId, typename _Callable, typename _Res, typename... _Args>
auto callable_to_funcptr(_Callable f)
{
    std::function<_Res(_Args...)> func(f);

    return detail::callable_to_funcptr<detail::template_hash<_UniqueId, _Callable>()>(std::move(func));
}

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif /* CALLABLE_TO_FUNCPTR_HPP */
