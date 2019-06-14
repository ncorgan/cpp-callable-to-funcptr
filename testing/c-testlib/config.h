/*
 * Copyright (c) 2019 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef TESTLIB_CONFIG_H
#define TESTLIB_CONFIG_H

/***************************************************************************
 * Define cross-platform macros
 ***************************************************************************/
#if defined(_MSC_VER)
#    define TESTLIB_EXPORT         __declspec(dllexport)
#    define TESTLIB_IMPORT         __declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define TESTLIB_EXPORT         __attribute__((visibility("default")))
#    define TESTLIB_IMPORT         __attribute__((visibility("default")))
#else
#    define TESTLIB_EXPORT
#    define TESTLIB_IMPORT
#endif

#ifdef TESTLIB_DLL_EXPORTS
#    define TESTLIB_API TESTLIB_EXPORT
#else
#    define TESTLIB_API TESTLIB_IMPORT
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#    define TESTLIB_PLATFORM_LINUX
#elif defined(__MINGW32__) || defined(MINGW32)
#    define TESTLIB_PLATFORM_MINGW
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#    define TESTLIB_PLATFORM_WIN32
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#    define TESTLIB_PLATFORM_MACOS
#endif

#endif /* TESTLIB_CONFIG_H */
