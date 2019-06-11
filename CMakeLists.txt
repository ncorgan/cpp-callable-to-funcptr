#
# Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# CMake configuration
cmake_minimum_required(VERSION 3.8)
project(cpp-lambda-to-funcptr C CXX)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Build test library
add_subdirectory(c-testlib)
