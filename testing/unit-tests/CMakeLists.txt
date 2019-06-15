#
# Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Build GoogleTest
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
ADD_SUBDIRECTORY(googletest/googletest)

add_executable(callable_to_funcptr_test callable_to_funcptr_test.cpp)
target_include_directories(callable_to_funcptr_test
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${BOOST_INCLUDE_DIRS})
target_link_libraries(callable_to_funcptr_test testlib gtest gtest_main)

set_target_properties(
    callable_to_funcptr_test
    PROPERTIES COMPILE_FLAGS "-DCALLABLE_TO_FUNCPTR_CLIENT_NAMESPACE=test_namespace")

#
# For GCC or Clang, if the caller specified a standard, override the
# default (C++17).
#
if(MANUALLY_SET_STANDARD_FLAGS)
    if(NOT "$ENV{STD}" STREQUAL "")
        SET(CPP_STD "$ENV{STD}" CACHE INTERNAL "C++ standard")
    elseif(MSVC)
        # This is the first version supported by this flag.
        SET(CPP_STD "c++17" CACHE INTERNAL "C++ standard")
    else()
        SET(CPP_STD "c++17" CACHE INTERNAL "C++ standard")
    endif()
endif()

if(WIN32)
    if(MANUALLY_SET_STANDARD_FLAGS)
        if(MINGW)
            set_source_files_properties(callable_to_funcptr_test.cpp
                PROPERTIES COMPILE_FLAGS "-std=${CPP_STD}"
            )
        else()
            # The /std: flag was added in MSVC 14.0
            if(MSVC AND (MSVC_VERSION GREATER 1800))
                set_source_files_properties(callable_to_funcptr_test.cpp
                    PROPERTIES COMPILE_FLAGS "/MP /EHsc /std:${CPP_STD}"
                )
            else()
                set_source_files_properties(callable_to_funcptr_test.cpp
                    PROPERTIES COMPILE_FLAGS "/MP /EHsc"
                )
            endif()
        endif()
    endif()

    add_test(
        callable_to_funcptr_test
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/callable_to_funcptr_test.exe
    )
else()
    if(MANUALLY_SET_STANDARD_FLAGS)
        set_source_files_properties(callable_to_funcptr_test.cpp
            PROPERTIES COMPILE_FLAGS "-std=${CPP_STD}"
        )
    endif()

    add_test(
        callable_to_funcptr_test
        ${CMAKE_CURRENT_BINARY_DIR}/callable_to_funcptr_test
    )
endif()
