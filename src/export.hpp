
#pragma once

#if defined(_WIN32)
#if defined(UTF_STRINGS_DLL)
#define UTF_STRINGS_API __declspec(dllexport)
#else
#define UTF_STRINGS_API __declspec(dllimport)
#endif
#else
#define UTF_STRINGS_API
#endif
