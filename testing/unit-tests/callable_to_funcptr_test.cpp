/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <callable_to_funcptr/callable_to_funcptr.hpp>

#include "testlib.h"

#include <gtest/gtest.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/core/demangle.hpp>

#include <iostream>
#include <string>

extern "C" {

    void testlib_simple_callback_c_function()
    {
    }

    int testlib_complex_callback_c_function(
        const char* const_char_ptr,
        double dbl,
        void* void_ptr)
    {
        return 0;
    }
}

class callable_to_funcptr_test: public ::testing::Test
{
    public:

        virtual void SetUp()
        {
            testlib_init_handle(&m_handle);
        }

        virtual void TearDown()
        {
            testlib_free_handle(&m_handle);
        }

        template <typename S, typename C>
        void test_setting_testlib_callbacks_common(
            S simple_callback_ptr,
            C complex_callback_ptr)
        {
            std::cout << "After:" << std::endl
                      << " * Simple:  " << boost::core::demangle(typeid(simple_callback_ptr).name()) << std::endl
                      << " * Complex: " << boost::core::demangle(typeid(complex_callback_ptr).name()) << std::endl;

            testlib_set_simple_callback(
                m_handle,
                simple_callback_ptr);
            testlib_set_complex_callback(
                m_handle,
                complex_callback_ptr);

            testlib_call_simple_callback(m_handle);

            int result = 0;
            testlib_call_complex_callback(
                m_handle,
                "Test string",
                0.0,
                nullptr,
                &result);
        }

        template <typename S, typename C>
        void test_setting_testlib_callbacks(
            S simple_callback_input,
            C complex_callback_input)
        {
            std::cout << "Before:" << std::endl
                      << " * Simple:  " << boost::core::demangle(typeid(simple_callback_input).name()) << std::endl
                      << " * Complex: " << boost::core::demangle(typeid(complex_callback_input).name()) << std::endl;

            test_setting_testlib_callbacks_common(
                test_namespace::callable_to_funcptr<0>(simple_callback_input),
                test_namespace::callable_to_funcptr<1>(complex_callback_input));
        }

        template <typename S, typename C>
        void test_setting_testlib_callbacks_with_types(
            S simple_callback_input,
            C complex_callback_input)
        {
            std::cout << "Before:" << std::endl
                      << " * Simple:  " << boost::core::demangle(typeid(simple_callback_input).name()) << std::endl
                      << " * Complex: " << boost::core::demangle(typeid(complex_callback_input).name()) << std::endl;

            test_setting_testlib_callbacks_common(
                test_namespace::callable_to_funcptr<0, S, void>(simple_callback_input),
                test_namespace::callable_to_funcptr<1, C, int, const char*, double, void*>(complex_callback_input));
        }

        void testlib_simple_callback_member_function()
        {
        }

        int testlib_complex_callback_member_function(
            const char* const_char_ptr,
            double dbl,
            void* void_ptr)
        {
            return 0;
        }

        testlib_handle m_handle;
};

static void testlib_simple_callback_cpp_function()
{
}

static int testlib_complex_callback_cpp_function(
    const char* const_char_ptr,
    double dbl,
    void* void_ptr)
{
    return 0;
}

struct testlib_simple_callback_functor
{
    void operator()() {}
};

struct testlib_complex_callback_functor
{
    int operator()(
        const char* const_char_ptr,
        double dbl,
        void* void_ptr)
    {
        return 0;
    }
};

//
// Tests below
//

TEST_F(callable_to_funcptr_test, c_funcptr)
{
    test_setting_testlib_callbacks(
        &testlib_simple_callback_c_function,
        &testlib_complex_callback_c_function);
}

TEST_F(callable_to_funcptr_test, cpp_funcptr)
{
    test_setting_testlib_callbacks(
        &testlib_simple_callback_cpp_function,
        &testlib_complex_callback_cpp_function);
}

TEST_F(callable_to_funcptr_test, functor)
{
    /*test_setting_testlib_callbacks(
        &testlib_simple_callback_functor,
        &testlib_complex_callback_functor);*/
}

TEST_F(callable_to_funcptr_test, noncapturing_lambda)
{
    auto simple_callback_lambda = []()
    {
        testlib_simple_callback_cpp_function();
    };
    auto complex_callback_lambda = [](
        const char* const_char_ptr,
        double dbl,
        void* void_ptr) -> int
    {
        return testlib_complex_callback_cpp_function(
                   const_char_ptr,
                   dbl,
                   void_ptr);
    };

    test_setting_testlib_callbacks(
        simple_callback_lambda,
        complex_callback_lambda);
}

TEST_F(callable_to_funcptr_test, capturing_lambda)
{
    auto simple_callback_lambda = [this]()
    {
        this->testlib_simple_callback_member_function();
    };
    auto complex_callback_lambda = [this](
        const char* const_char_ptr,
        double dbl,
        void* void_ptr) -> int
    {
        return this->testlib_complex_callback_member_function(
                   const_char_ptr,
                   dbl,
                   void_ptr);
    };

    test_setting_testlib_callbacks(
        simple_callback_lambda,
        complex_callback_lambda);
}

TEST_F(callable_to_funcptr_test, std_function)
{
    using simple_callback_t = std::function<void(void)>;
    using complex_callback_t = std::function<int(const char*, double, void*)>;

    simple_callback_t simple_callback_std_function(testlib_simple_callback_cpp_function);
    complex_callback_t complex_callback_std_function(testlib_complex_callback_cpp_function);

    test_setting_testlib_callbacks(
        simple_callback_std_function,
        complex_callback_std_function);
}

TEST_F(callable_to_funcptr_test, std_bind)
{
    auto simple_callback_std_bind = std::bind(
                                        &callable_to_funcptr_test::testlib_simple_callback_member_function,
                                        this);
    auto complex_callback_std_bind = std::bind(
                                        &callable_to_funcptr_test::testlib_complex_callback_member_function,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2,
                                        std::placeholders::_3);

    test_setting_testlib_callbacks_with_types(
        simple_callback_std_bind,
        complex_callback_std_bind);
}

TEST_F(callable_to_funcptr_test, boost_function)
{
    using simple_callback_t = boost::function<void(void)>;
    using complex_callback_t = boost::function<int(const char*, double, void*)>;

    simple_callback_t simple_callback_boost_function(testlib_simple_callback_cpp_function);
    complex_callback_t complex_callback_boost_function(testlib_complex_callback_cpp_function);

    test_setting_testlib_callbacks(
        simple_callback_boost_function,
        complex_callback_boost_function);
}

TEST_F(callable_to_funcptr_test, boost_bind)
{
    auto simple_callback_boost_bind = boost::bind(
                                          &callable_to_funcptr_test::testlib_simple_callback_member_function,
                                          this);
    auto complex_callback_boost_bind = boost::bind(
                                          &callable_to_funcptr_test::testlib_complex_callback_member_function,
                                          this,
                                          boost::placeholders::_1,
                                          boost::placeholders::_2,
                                          boost::placeholders::_3);

    test_setting_testlib_callbacks_with_types(
        simple_callback_boost_bind,
        complex_callback_boost_bind);
}
