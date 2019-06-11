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

TEST(lambda_to_funcptr_test, test_lambda_to_funcptr)
{
    testlib_handle handle;
    testlib_init_handle(&handle);

    const std::string simple_callback_message = "Simple callback message";
    const std::string complex_callback_message = "Complex callback message";

    auto simple_callback_impl = [&]()
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

    std::cout << boost::core::demangle(typeid(simple_callback_impl).name()) << std::endl;
    std::cout << boost::core::demangle(typeid(complex_callback_impl).name()) << std::endl;

    auto simple = get_funcptr<0>(std::function<void()>(simple_callback_impl));
    std::cout << boost::core::demangle(typeid(simple).name()) << std::endl;

    testlib_free_handle(&handle);
}
