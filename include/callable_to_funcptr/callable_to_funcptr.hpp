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

#include "callable/callable.hpp"

#include "detail/funcptr_helper.hpp"
#include "detail/sfinae.hpp"

#include <functional>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

template <size_t _UniqueId, typename _Res, typename... _Args>
auto callable_to_funcptr(const std::function<_Res(_Args...)>& f)
{
    detail::funcptr_helper<_UniqueId, _Res, _Args...>::bind(f);
    return detail::funcptr_helper<_UniqueId, _Res, _Args...>::ptr();
}

template <size_t _UniqueId, typename _Callable>
auto callable_to_funcptr(_Callable f)
{
    return callable_to_funcptr<_UniqueId>(to_stdfunction(f));
}

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif /* CALLABLE_TO_FUNCPTR_HPP */
