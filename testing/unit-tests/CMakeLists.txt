#
# Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Build GoogleTest
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
ADD_SUBDIRECTORY(googletest/googletest)

add_executable(lambda-to-funcptr-test lambda-to-funcptr-test.cpp)
target_include_directories(lambda-to-funcptr-test
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries(lambda-to-funcptr-test testlib gtest gtest_main)

if(WIN32)
    add_test(
        lambda-to-funcptr-test
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lambda-to-funcptr-test.exe
    )
else()
    add_test(
        lambda-to-funcptr-test
        ${CMAKE_CURRENT_BINARY_DIR}/lambda-to-funcptr-test
    )
endif()
