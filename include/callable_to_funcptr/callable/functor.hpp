/*
 * Copyright (c) 2013, 2016 Stephan Hohe
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#if !defined(CALLABLE_FUNCTOR_HPP_INCLUDED)
#define CALLABLE_FUNCTOR_HPP_INCLUDED

#ifndef CALLABLE_TO_FUNCPTR_HPP
#error Do not include this file directly!
#endif

#if !defined(BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif
#if !defined(END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE)
#error Internal macro END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE not defined!
#endif

#include "helpers.hpp"
#include "function.hpp"
#include "member_function.hpp"

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

namespace detail {

template<typename Class>
using call_operator_traits = member_function_traits<decltype(&Class::operator())>;

// classes with operator()
template<typename Class>
struct functor_traits : function_traits<typename call_operator_traits<Class>::function_type> {
	typedef call_operator_traits<Class> call_operator; 
};

} // namespace detail


template<typename Class>
struct functor_traits : detail::functor_traits<detail::remove_cvref_t<Class>> {
};

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif // CALLABLE_FUNCTOR_HPP_INCLUDED

