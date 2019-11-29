/*
 * Copyright (c) 2013, 2016 Stephan Hohe
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#if !defined(CALLABLE_FUNCTION_HPP_INCLUDED)
#define CALLABLE_FUNCTION_HPP_INCLUDED

#ifndef CALLABLE_TO_FUNCPTR_HPP
#error Do not include this file directly. Include <callable_to_funcptr/callable_to_funcptr.hpp> instead.
#endif

#include "../check_header.hpp"

#include "helpers.hpp"
#include <cstddef>

BEGIN_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

namespace detail {

namespace {

/** Define traits for a function type */
template<typename Fun>
struct function_traits;

template<typename Ret, typename... Args>
struct function_traits<Ret (Args...)> {
	typedef Ret function_type(Args...);
	typedef Ret return_type;
	static constexpr std::size_t argc = types_count<Args...>::value;

	template<std::size_t N>
	using argument_type = typename types_n<N, Args...>::type;
};

template<typename Ret, typename... Args>
const std::size_t function_traits<Ret (Args...)>::argc;

} // namespace

} // namespace detail


template<typename Func>
struct function_traits : detail::function_traits<detail::remove_cvref_t<Func>> {
};

template<typename Func>
struct function_traits<Func*> : detail::function_traits<detail::remove_cvref_t<Func>> {
};

END_CALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE

#endif // CALLABLE_FUNCTION_HPP_INCLUDED

