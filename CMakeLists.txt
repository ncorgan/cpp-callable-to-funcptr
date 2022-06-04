#
# Copyright (c) 2019,2022 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# CMake configuration
cmake_minimum_required(VERSION 3.8)
project(cpp-callable-to-funcptr C CXX)

set(CMAKE_CXX_STANDARD 17)

#
# CMake target
#

include(GNUInstallDirs)

add_library(callable-to-funcptr INTERFACE)
target_include_directories(callable-to-funcptr INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

option(CALLABLE_TO_FUNCPTR_INSTALL_HEADERS "Install callable-to-funcptr headers")
if(CALLABLE_TO_FUNCPTR_INSTALL_HEADERS)
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/nc
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

#
# Testing
#

# For testing only, not a requirement for the module itself
find_package(Boost 1.65)

if(Boost_FOUND)
    enable_testing()
    add_subdirectory(testing)
endif()
