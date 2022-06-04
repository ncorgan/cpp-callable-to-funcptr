/*
 * Copyright (c) 2019,2022 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <nc/callable_to_funcptr.hpp>

#include "testlib.h"

#include <gtest/gtest.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/core/demangle.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <type_traits>

static constexpr bool print_types = false;

class conversion_test: public ::testing::Test
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
            if(print_types)
            {
                std::cout << "After:" << std::endl
                          << " * Simple:  " << boost::core::demangle(typeid(simple_callback_ptr).name()) << std::endl
                          << " * Complex: " << boost::core::demangle(typeid(complex_callback_ptr).name()) << std::endl;
            }

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
            if(print_types)
            {
                std::cout << "Before:" << std::endl
                          << " * Simple:  " << boost::core::demangle(typeid(simple_callback_input).name()) << std::endl
                          << " * Complex: " << boost::core::demangle(typeid(complex_callback_input).name()) << std::endl;
            }

            test_setting_testlib_callbacks_common(
                nc::callable_to_funcptr<0>(simple_callback_input),
                nc::callable_to_funcptr<1>(complex_callback_input));
        }

        template <typename S, typename C>
        void test_setting_testlib_callbacks_with_types(
            S simple_callback_input,
            C complex_callback_input)
        {
            if(print_types)
            {
                std::cout << "Before:" << std::endl
                          << " * Simple:  " << boost::core::demangle(typeid(simple_callback_input).name()) << std::endl
                          << " * Complex: " << boost::core::demangle(typeid(complex_callback_input).name()) << std::endl;
            }

            test_setting_testlib_callbacks_common(
                nc::callable_to_funcptr<0, S, void>(simple_callback_input),
                nc::callable_to_funcptr<1, C, int, const char*, double, void*>(complex_callback_input));
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

template <size_t ID, typename T1, typename T2>
void test_inputs_use_same_template(T1 t1, T2 t2)
{
    // We need to evaluate this separately because the comma breaks the
    // ASSERT_TRUE macro.
    constexpr bool do_funcsigs_match =
        std::is_same<decltype(t1), decltype(t2)>::value;
    ASSERT_TRUE(do_funcsigs_match);

    // As a sanity check, make sure the compile-time hashing matches, or this
    // test has no point.
    using nc::detail::template_hash;
    using nc::detail::hash_t;

    const hash_t hash1 = template_hash<ID, decltype(t1)>();
    const hash_t hash2 = template_hash<ID, decltype(t2)>();
    ASSERT_EQ(hash1, hash2);
}

static void testlib_simple_callback_c_function()
{
}

static int testlib_complex_callback_c_function(
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

TEST_F(conversion_test, converting_c_funcptr)
{
    test_setting_testlib_callbacks(
        &testlib_simple_callback_c_function,
        &testlib_complex_callback_c_function);
}

TEST_F(conversion_test, converting_functor)
{
    test_setting_testlib_callbacks(
        testlib_simple_callback_functor(),
        testlib_complex_callback_functor());
}

TEST_F(conversion_test, converting_noncapturing_lambda)
{
    auto simple_callback_lambda = []()
    {
        testlib_simple_callback_c_function();
    };
    auto complex_callback_lambda = [](
        const char* const_char_ptr,
        double dbl,
        void* void_ptr) -> int
    {
        return testlib_complex_callback_c_function(
                   const_char_ptr,
                   dbl,
                   void_ptr);
    };

    test_setting_testlib_callbacks(
        simple_callback_lambda,
        complex_callback_lambda);
}

TEST_F(conversion_test, converting_capturing_lambda)
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

TEST_F(conversion_test, converting_std_function)
{
    using simple_callback_t = std::function<void(void)>;
    using complex_callback_t = std::function<int(const char*, double, void*)>;

    simple_callback_t simple_callback_std_function(testlib_simple_callback_c_function);
    complex_callback_t complex_callback_std_function(testlib_complex_callback_c_function);

    test_setting_testlib_callbacks(
        simple_callback_std_function,
        complex_callback_std_function);
}

TEST_F(conversion_test, converting_std_bind)
{
    auto simple_callback_std_bind = std::bind(
                                        &conversion_test::testlib_simple_callback_member_function,
                                        this);
    auto complex_callback_std_bind = std::bind(
                                        &conversion_test::testlib_complex_callback_member_function,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2,
                                        std::placeholders::_3);

    test_setting_testlib_callbacks_with_types(
        simple_callback_std_bind,
        complex_callback_std_bind);
}

TEST_F(conversion_test, converting_boost_function)
{
    using simple_callback_t = boost::function<void(void)>;
    using complex_callback_t = boost::function<int(const char*, double, void*)>;

    simple_callback_t simple_callback_boost_function(testlib_simple_callback_c_function);
    complex_callback_t complex_callback_boost_function(testlib_complex_callback_c_function);

    test_setting_testlib_callbacks(
        simple_callback_boost_function,
        complex_callback_boost_function);
}

TEST_F(conversion_test, converting_boost_bind)
{
    auto simple_callback_boost_bind = boost::bind(
                                          &conversion_test::testlib_simple_callback_member_function,
                                          this);
    auto complex_callback_boost_bind = boost::bind(
                                          &conversion_test::testlib_complex_callback_member_function,
                                          this,
                                          boost::placeholders::_1,
                                          boost::placeholders::_2,
                                          boost::placeholders::_3);

    test_setting_testlib_callbacks_with_types(
        simple_callback_boost_bind,
        complex_callback_boost_bind);
}

// When using this ID, get_one_and_two and get_three_and_four will evaluate
// to the same templated implementation.
static constexpr size_t template_id = 12345;

static void get_one_and_two(int* p_output1, int* p_output2)
{
    (*p_output1) = 1;
    (*p_output2) = 2;
}

static void get_three_and_four(int* p_output1, int* p_output2)
{
    (*p_output1) = 3;
    (*p_output2) = 4;
}

TEST(callable_to_funcptr_test, test_changing_function)
{
    ASSERT_NO_FATAL_FAILURE(test_inputs_use_same_template<template_id>(
        get_one_and_two,
        get_three_and_four));

    int output1 = 0;
    int output2 = 0;

    nc::callable_to_funcptr<template_id>(get_one_and_two)(
        &output1,
        &output2);
    ASSERT_EQ(1, output1);
    ASSERT_EQ(2, output2);

    nc::callable_to_funcptr<template_id>(get_three_and_four)(
        &output1,
        &output2);
    ASSERT_EQ(3, output1);
    ASSERT_EQ(4, output2);
}

static void thread1_fcn(int* p_output1, int* p_output2)
{
    nc::callable_to_funcptr<template_id>(get_one_and_two)(
        p_output1,
        p_output2);
}

static void thread2_fcn(int* p_output1, int* p_output2)
{
    nc::callable_to_funcptr<template_id>(get_three_and_four)(
        p_output1,
        p_output2);
}

TEST(callable_to_funcptr_test, test_thread_safety)
{
    ASSERT_NO_FATAL_FAILURE(test_inputs_use_same_template<template_id>(
        get_one_and_two,
        get_three_and_four));

    // As returning a stateless function pointer requires static storage, if
    // multiple threads call callable_to_funcptr with the same parameters, the
    // two callables will conflict, resulting in a race condition. We need to make
    // sure our thread-safety solution works.
    //
    // Run this scenario multiple times in a naive attempt to cause the situation
    // where the race condition would occur.
    constexpr size_t num_iterations = 10000;
    for(size_t i = 0; i < num_iterations; ++i)
    {
        int one_output = 0;
        int two_output = 0;
        int three_output = 0;
        int four_output = 0;

        std::thread thread1(thread1_fcn, &one_output, &two_output);
        std::thread thread2(thread2_fcn, &three_output, &four_output);
        thread1.join();
        thread2.join();

        ASSERT_EQ(1, one_output) << "iteration " << (i+1);
        ASSERT_EQ(2, two_output) << "iteration " << (i+1);
        ASSERT_EQ(3, three_output) << "iteration " << (i+1);
        ASSERT_EQ(4, four_output) << "iteration " << (i+1);
    }
}

TEST(callable_to_funcptr_test, test_location_macro)
{
    auto foo = [](int, const char*) -> void
    {
        return;
    };

    auto wat = CALLABLE_TO_FUNCPTR(foo);
}
