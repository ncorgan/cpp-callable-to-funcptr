/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "testlib.h"

#include <stdlib.h>

// Opaque struct definition
struct testlib_struct
{
    testlib_simple_callback simple_callback;
    testlib_complex_callback complex_callback;
};

int
testlib_init_handle(testlib_handle* p_handle)
{
    if(!p_handle)
    {
        return -1;
    }

    (*p_handle) = calloc(1, sizeof(struct testlib_struct));

    return 0;
}

int
testlib_free_handle(testlib_handle* p_handle)
{
    if(!p_handle)
    {
        return -1;
    }

    free(*p_handle);
    (*p_handle) = NULL;

    return 0;
}

int
testlib_set_simple_callback(
    testlib_handle handle,
    testlib_simple_callback simple_callback)
{
    if(!handle || !simple_callback)
    {
        return -1;
    }

    handle->simple_callback = simple_callback;

    return 0;
}

int
testlib_set_complex_callback(
    testlib_handle handle,
    testlib_complex_callback complex_callback)
{
    if(!handle || !complex_callback)
    {
        return -1;
    }

    handle->complex_callback = complex_callback;

    return 0;
}

int
testlib_call_simple_callback(testlib_handle handle)
{
    if(!handle || !handle->simple_callback)
    {
        return -1;
    }

    handle->simple_callback();

    return 0;
}

int
testlib_call_complex_callback(
    testlib_handle handle,
    const char* const_char_param,
    double double_param,
    void* void_ptr_param,
    int* p_result_out)
{
    if(!handle || !handle->complex_callback || !p_result_out)
    {
        return -1;
    }

    *p_result_out = handle->complex_callback(
                        const_char_param,
                        double_param,
                        void_ptr_param);

    return 0;
}
