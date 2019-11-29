#
# Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# CMake configuration
cmake_minimum_required(VERSION 3.8)
project(cpp-callable-to-funcptr C CXX)

if(MANUALLY_SET_STANDARD_FLAGS)
    message(STATUS "")
    message(STATUS "Overriding default C++ standard flags.")
    message(STATUS "")
else()
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS ON)
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# For testing only, not a requirement for the module itself
find_package(Boost 1.65)

enable_testing()
add_subdirectory(testing)
