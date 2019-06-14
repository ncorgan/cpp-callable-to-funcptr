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

#include <callable_to_funcptr/detail/funcptr_helper.hpp>
#include <callable_to_funcptr/detail/sfinae.hpp>

#include <functional>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

template <size_t _UniqueId, typename _Res, typename... _ArgTypes>
auto callable_to_funcptr(const std::function<_Res(_ArgTypes...)>& f)
{
    detail::funcptr_helper<_UniqueId, _Res, _ArgTypes...>::bind(f);
    return detail::funcptr_helper<_UniqueId, _Res, _ArgTypes...>::ptr();
}

template <size_t _UniqueId, typename _Functor>
auto callable_to_funcptr(_Functor f)
{
    return callable_to_funcptr<_UniqueId>(detail::to_func(f));
}

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif /* CALLABLE_TO_FUNCPTR_HPP */
