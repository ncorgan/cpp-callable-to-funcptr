/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CALLABLE_TO_FUNCPTR_DEFINE_INTERNAL_MACROS_HPP
#define CALLABLE_TO_FUNCPTR_DEFINE_INTERNAL_MACROS_HPP

#ifndef CALLABLE_TO_FUNCPTR_HPP
#error Do not include this file directly. Include <callable_to_funcptr/callable_to_funcptr.hpp> instead.
#endif

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

#endif /* CALLABLE_TO_FUNCPTR_DEFINE_INTERNAL_MACROS_HPP */
