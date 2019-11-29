/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CALLABLE_TO_FUNCPTR_HPP
#define CALLABLE_TO_FUNCPTR_HPP

#include "detail/define_internal_macros.hpp"

#include "detail/callable/callable.hpp"
#include "detail/funcptr_helper.hpp"

#include <cassert>
#include <functional>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

template <size_t _UniqueId, typename _Callable>
auto callable_to_funcptr(_Callable f)
{
    auto func = to_stdfunction(f);
    assert(func);

    return detail::stdfunction_to_funcptr<detail::template_hash<_UniqueId, _Callable>()>(std::move(func));
}

template <size_t _UniqueId, typename _Callable, typename _Res, typename... _Args>
auto callable_to_funcptr(_Callable f)
{
    std::function<_Res(_Args...)> func(f);
    assert(func);

    return detail::stdfunction_to_funcptr<detail::template_hash<_UniqueId, _Callable>()>(std::move(func));
}

#define CALLABLE_TO_FUNCPTR(f) \
    CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE::callable_to_funcptr< \
        CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE::detail::hash_location(__FILE__, __LINE__), \
        decltype(f)>(f)

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif /* CALLABLE_TO_FUNCPTR_HPP */
