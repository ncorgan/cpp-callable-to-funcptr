/*
 * Copyright (c) 2019,2022 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include "config.h"

// Forward-declare opaque struct
struct testlib_struct;
typedef struct testlib_struct* testlib_handle;

typedef void (*testlib_simple_callback)(void);
typedef int (*testlib_complex_callback)(const char*, double, void*);

#ifdef __cplusplus
extern "C" {
#endif

TESTLIB_API int testlib_init_handle(testlib_handle* p_handle);

TESTLIB_API int testlib_free_handle(testlib_handle* p_handle);

TESTLIB_API int testlib_set_simple_callback(
                    testlib_handle handle,
                    testlib_simple_callback simple_callback);

TESTLIB_API int testlib_set_complex_callback(
                    testlib_handle handle,
                    testlib_complex_callback complex_callback);

TESTLIB_API int testlib_call_simple_callback(testlib_handle handle);

TESTLIB_API int testlib_call_complex_callback(
                    testlib_handle handle,
                    const char* const_char_param,
                    double double_param,
                    void* void_ptr_param,
                    int* p_result_out);

#ifdef __cplusplus
}
#endif
