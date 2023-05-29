#pragma once

/* If we are we on Windows, we want a single define for it.*/
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
    #define _WIN32
#endif /* _WIN32 */

#if defined(_WIN32) && defined(GAME_ENGINE_BUILD_DLL)
    /* We are building Hashing as a Win32 DLL */
    #define GAME_ENGINE_API __declspec(dllexport)
#elif defined(_WIN32)
    /* We are calling Hashing as a Win32 DLL */
    #define GAME_ENGINE_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(GAME_ENGINE_BUILD_DLL)
    /* We are building Hashing as a shared / dynamic library */
    #define GAME_ENGINE_API __attribute__((visibility("default")))
#else
    /* We are building or calling HASHING as a static library */
    #define GAME_ENGINE_API
#endif

#pragma warning(disable: 4251)