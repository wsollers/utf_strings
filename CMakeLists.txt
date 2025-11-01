
cmake_minimum_required(VERSION 3.25)
project(utf_strings LANGUAGES CXX VERSION 0.1.0)

option(UTF_STRINGS_BUILD_TESTS "Build unit tests" ON)
option(UTF_STRINGS_BUILD_BENCHMARKS "Build benchmarks" ON)
option(UTF_STRINGS_WITH_GPERFTOOLS "Link gperftools (if available)" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
set(UTF_STRINGS_HEADERS
    include/utf/utf_strings.hpp
    src/export.hpp
)
set(UTF_STRINGS_SOURCES
    src/utf_strings.cpp
)

# Static library
add_library(utf_strings_static STATIC ${UTF_STRINGS_HEADERS} ${UTF_STRINGS_SOURCES})
target_include_directories(utf_strings_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(utf_strings_static PUBLIC cxx_std_23)

# Shared library
add_library(utf_strings_shared SHARED ${UTF_STRINGS_HEADERS} ${UTF_STRINGS_SOURCES})
target_include_directories(utf_strings_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(utf_strings_shared PRIVATE UTF_STRINGS_DLL)
set_target_properties(utf_strings_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_features(utf_strings_shared PUBLIC cxx_std_23)

# gperftools (optional, via Conan)
if(UTF_STRINGS_WITH_GPERFTOOLS)
  find_package(gperftools QUIET CONFIG)
  if(gperftools_FOUND)
    message(STATUS "gperftools found; benchmarks will link it")
    set(HAVE_GPERFTOOLS TRUE)
  endif()
endif()

# Unit tests
if(UTF_STRINGS_BUILD_TESTS)
  include(CTest)
  enable_testing()
  find_package(gtest CONFIG REQUIRED)
  add_executable(utf_strings-tests tests/utf8_tests.cpp)
  target_link_libraries(utf_strings-tests PRIVATE utf_strings_static GTest::gtest GTest::gtest_main)
  target_compile_features(utf_strings-tests PRIVATE cxx_std_23)
  include(GoogleTest)
  gtest_discover_tests(utf_strings-tests)
endif()

# Benchmarks
if(UTF_STRINGS_BUILD_BENCHMARKS)
  find_package(benchmark CONFIG REQUIRED)
  add_executable(utf_strings-bench benchmarks/utf8_bench.cpp)
  target_link_libraries(utf_strings-bench PRIVATE utf_strings_static benchmark::benchmark)
  if(HAVE_GPERFTOOLS)
    target_compile_definitions(utf_strings-bench PRIVATE HAVE_GPERFTOOLS)
    target_link_libraries(utf_strings-bench PRIVATE profiler)
  endif()
  target_compile_features(utf_strings-bench PRIVATE cxx_std_23)
endif()
