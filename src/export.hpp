
#pragma once

#if defined(_WIN32)
#if defined(UTF_STRINGS_STATIC_BUILD)
// Static library - no import/export needed
#define UTF_STRINGS_API
#elif defined(UTF_STRINGS_DLL)
// Building shared library
#define UTF_STRINGS_API __declspec(dllexport)
#else
// Using shared library
#define UTF_STRINGS_API __declspec(dllimport)
#endif
#else
// Non-Windows platforms
#define UTF_STRINGS_API
#endif
