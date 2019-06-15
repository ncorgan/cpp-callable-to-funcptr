/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <callable_to_funcptr/callable_to_funcptr.hpp>

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

    auto simple_callback_ptr = test_namespace::callable_to_funcptr<1>(foo);


    std::cout << boost::core::demangle(typeid(complex_callback_impl).name()) << std::endl << std::endl;


    auto complex_callback_ptr = test_namespace::callable_to_funcptr<0>(complex_callback_impl);

    std::cout << boost::core::demangle(typeid(complex_callback_ptr).name()) << std::endl << std::endl;

    testlib_set_simple_callback(
        handle,
        simple_callback_ptr);
    testlib_set_complex_callback(
        handle,
        complex_callback_ptr);

    testlib_call_simple_callback(handle);

    int result = 0;
    testlib_call_complex_callback(handle, "foo", 5.0, nullptr, &result);

    testlib_free_handle(&handle);
}
