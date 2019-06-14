#
# Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

add_library(testlib SHARED testlib.c)
target_include_directories(testlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(testlib PROPERTIES DEFINE_SYMBOL "TESTLIB_DLL_EXPORTS")
