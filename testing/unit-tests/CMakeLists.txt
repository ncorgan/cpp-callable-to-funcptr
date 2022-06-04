#
# Copyright (c) 2019,2022 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Build GoogleTest
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
ADD_SUBDIRECTORY(googletest/googletest)

add_executable(callable_to_funcptr_test callable_to_funcptr_test.cpp)
target_include_directories(callable_to_funcptr_test PRIVATE
    ${BOOST_INCLUDE_DIRS})
target_link_libraries(callable_to_funcptr_test
    testlib
    callable-to-funcptr
    gtest
    gtest_main)

if(MSVC)
    add_test(
        callable_to_funcptr_test
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/callable_to_funcptr_test.exe
    )
else()
    add_test(
        callable_to_funcptr_test
        ${CMAKE_CURRENT_BINARY_DIR}/callable_to_funcptr_test
    )
endif()
