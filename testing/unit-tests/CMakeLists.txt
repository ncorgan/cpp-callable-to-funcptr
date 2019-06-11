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

#
# For GCC or Clang, if the caller specified a standard, override the
# default (C++11).
#
if(NOT "$ENV{STD}" STREQUAL "")
    SET(CPP_STD "$ENV{STD}" CACHE INTERNAL "C++ standard")
elseif(MSVC)
    # This is the first version supported by this flag.
    SET(CPP_STD "c++14" CACHE INTERNAL "C++ standard")
else()
    SET(CPP_STD "c++11" CACHE INTERNAL "C++ standard")
endif()

if(WIN32)
    if(MINGW)
        set_source_files_properties(lambda-to-funcptr-test.cpp
            PROPERTIES COMPILE_FLAGS "-std=${CPP_STD}"
        )
    else()
        # The /std: flag was added in MSVC 14.0
        if(MSVC AND (MSVC_VERSION GREATER 1800))
            set_source_files_properties(lambda-to-funcptr-test.cpp
                PROPERTIES COMPILE_FLAGS "/MP /EHsc /std:${CPP_STD}"
            )
        else()
            set_source_files_properties(lambda-to-funcptr-test.cpp
                PROPERTIES COMPILE_FLAGS "/MP /EHsc"
            )
        endif()
    endif()

    add_test(
        lambda-to-funcptr-test
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lambda-to-funcptr-test.exe
    )
else()
    set_source_files_properties(lambda-to-funcptr-test.cpp
        PROPERTIES COMPILE_FLAGS "-std=${CPP_STD}"
    )

    add_test(
        lambda-to-funcptr-test
        ${CMAKE_CURRENT_BINARY_DIR}/lambda-to-funcptr-test
    )
endif()
