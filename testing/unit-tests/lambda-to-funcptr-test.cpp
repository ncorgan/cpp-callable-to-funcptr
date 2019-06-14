/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "funcptr_helper.hpp"

#include "testlib.h"

#include <gtest/gtest.h>

#include <boost/core/demangle.hpp>

#include <iostream>
#include <string>

void foo() {}

TEST(lambda_to_funcptr_test, test_lambda_to_funcptr)
{
    testlib_handle handle;
    testlib_init_handle(&handle);

    const std::string simple_callback_message = "Simple callback message";
    const std::string complex_callback_message = "Complex callback message";

    auto simple_callback_impl = [&]() -> void
    {
        std::cout << simple_callback_message << std::endl;
    };
    auto complex_callback_impl = [&](
        const char* str,
        double dbl,
        void* ptr) -> int
    {
        std::cout << complex_callback_message << " " << str << " "
                  << dbl << " " << ptr << std::endl;
        return 0;
    };

    using simple_callback_type = std::function<void(void)>;
    using complex_callback_type = std::function<int(const char*, double, void*)>;

    auto simple_callback_ptr = get_funcptr<1>(foo);
    auto complex_callback_ptr = get_funcptr<0>(complex_callback_type(complex_callback_impl));

    //auto simple_callback_ptr = get_funcptr<0>(simple_callback_impl);

    std::cout << detail::has_parentheses<decltype(foo)>::value << std::endl;
    std::cout << detail::has_parentheses<decltype(simple_callback_impl)>::value << std::endl;

    testlib_set_simple_callback(
        handle,
        simple_callback_ptr);
    /*testlib_set_complex_callback(
        handle,
        complex_callback_ptr);*/

    testlib_call_simple_callback(handle);

    int result = 0;
    //testlib_call_complex_callback(handle, "foo", 5.0, nullptr, &result);

    testlib_free_handle(&handle);
}
