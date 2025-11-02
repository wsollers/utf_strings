cmake_minimum_required(VERSION 3.25)
project(utf_strings LANGUAGES CXX VERSION 0.1.0)

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------
option(UTF_STRINGS_BUILD_TESTS "Build unit tests" ON)
option(UTF_STRINGS_BUILD_BENCHMARKS "Build benchmarks" ON)
option(UTF_STRINGS_BUILD_FUZZ_TESTS "Build fuzz tests" OFF)
option(UTF_STRINGS_WITH_GPERFTOOLS "Link gperftools (if available)" ON)
option(UTF_STRINGS_ENABLE_SANITIZERS "Enable sanitizers (AddressSanitizer, UBSan, etc.)" OFF)
option(UTF_STRINGS_ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer (mutually exclusive with AddressSanitizer)" OFF)

# --------------------------------------------------------------------
# Compiler setup
# --------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Security and optimization flags
set(SECURITY_FLAGS "")
set(OPTIMIZATION_FLAGS "")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Security flags (enabled by default for all builds)
    set(SECURITY_FLAGS
        -fstack-protector-strong          # Stack protection
        -D_FORTIFY_SOURCE=2               # Buffer overflow detection
        -Wformat                          # Format string vulnerabilities
        -Wformat-security                 # Format security warnings
        -fPIE                             # Position Independent Executable
    )
    
    # Additional security flags for release builds
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        list(APPEND SECURITY_FLAGS
            -fstack-clash-protection      # Stack clash protection
            -fcf-protection=full          # Control Flow Integrity (Intel CET)
        )
    endif()
    
    # Optimization flags for release builds
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(OPTIMIZATION_FLAGS
            -O3                           # Aggressive optimization
            -march=native                 # Optimize for local CPU
            -mtune=native                 # Tune for local CPU
            -flto                         # Link Time Optimization
            -DNDEBUG                      # Disable debug assertions
            -fomit-frame-pointer          # Omit frame pointers for better performance
            -funroll-loops                # Unroll loops
            -ffast-math                   # Fast math (be careful with this)
        )
        
        # Additional Clang-specific optimizations
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            list(APPEND OPTIMIZATION_FLAGS
                -fvectorize                # Enable vectorization
                -fslp-vectorize           # Enable SLP vectorization
            )
        endif()
        
        # Additional GCC-specific optimizations
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            list(APPEND OPTIMIZATION_FLAGS
                -ftree-vectorize          # Enable vectorization
                -fgraphite-identity       # Enable Graphite optimizations
                -floop-nest-optimize      # Loop nest optimizations
            )
        endif()
    endif()
    
    # Debug flags for debug builds
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPTIMIZATION_FLAGS
            -Og                           # Debug-friendly optimization
            -g3                           # Maximum debug info
            -fno-omit-frame-pointer       # Keep frame pointers for debugging
        )
    endif()
endif()

# Warning flags (always enabled)
set(WARNING_FLAGS "")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(WARNING_FLAGS
        -Wall                             # Enable most warnings
        -Wextra                           # Enable extra warnings
        -Wpedantic                        # Enable pedantic warnings
        -Wconversion                      # Warn about type conversions
        -Wsign-conversion                 # Warn about sign conversions
        -Wshadow                          # Warn about variable shadowing
        -Wnon-virtual-dtor                # Warn about non-virtual destructors
        -Wold-style-cast                  # Warn about C-style casts
        -Wcast-align                      # Warn about alignment issues
        -Wunused                          # Warn about unused variables
        -Woverloaded-virtual              # Warn about overloaded virtuals
        -Wformat=2                        # Strict format checking
        -Wnull-dereference                # Warn about null dereferences
        -Wdouble-promotion                # Warn about float to double promotion
        -Wimplicit-fallthrough            # Warn about fallthrough in switch
    )
    
    # Additional Clang warnings
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        list(APPEND WARNING_FLAGS
            -Wmost                        # Enable most Clang warnings
            -Wthread-safety               # Thread safety analysis
            -Wthread-safety-negative      # Negative thread safety analysis
        )
    endif()
    
    # Additional GCC warnings
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND WARNING_FLAGS
            -Wduplicated-cond             # Warn about duplicated conditions
            -Wduplicated-branches         # Warn about duplicated branches
            -Wlogical-op                  # Warn about logical operations
            -Wrestrict                    # Warn about restrict violations
            -Wuseless-cast                # Warn about useless casts
        )
    endif()
endif()

# Helper function to apply security, optimization, and warning flags to a target
function(apply_build_flags target_name)
    if(SECURITY_FLAGS OR OPTIMIZATION_FLAGS OR WARNING_FLAGS)
        target_compile_options(${target_name} PRIVATE 
            ${SECURITY_FLAGS} 
            ${OPTIMIZATION_FLAGS} 
            ${WARNING_FLAGS}
        )
        
        # Apply link-time optimization and security flags to linker
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_link_options(${target_name} PRIVATE
                -flto                     # Link Time Optimization
                -pie                      # Position Independent Executable
                -Wl,-z,relro              # Read-only relocations
                -Wl,-z,now                # Immediate binding
                -Wl,-z,noexecstack        # Non-executable stack
            )
        endif()
    endif()
endfunction()

# Sanitizer setup
set(SANITIZER_FLAGS "")
set(SANITIZER_LINK_FLAGS "")

if(UTF_STRINGS_ENABLE_SANITIZERS AND UTF_STRINGS_ENABLE_THREAD_SANITIZER)
    message(FATAL_ERROR "Cannot enable both general sanitizers and ThreadSanitizer simultaneously")
endif()

if(UTF_STRINGS_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(SANITIZER_FLAGS 
            -fsanitize=address                    # AddressSanitizer - detects buffer overflows, use-after-free, etc.
            -fsanitize=undefined                  # UndefinedBehaviorSanitizer - detects undefined behavior
            -fsanitize=leak                       # LeakSanitizer - detects memory leaks (part of ASan)
            -fsanitize=bounds                     # Array bounds checking
            -fsanitize=null                       # Null pointer dereference detection
            -fsanitize=return                     # Return value checking
            -fsanitize=signed-integer-overflow    # Signed integer overflow detection
            -fsanitize=unreachable               # Unreachable code detection
            -fsanitize=vla-bound                 # Variable length array bounds
            -fno-omit-frame-pointer              # Keep frame pointers for better stack traces
            -fno-optimize-sibling-calls          # Better stack traces
            -g                                   # Debug info for better error reports
        )
        set(SANITIZER_LINK_FLAGS ${SANITIZER_FLAGS})
        message(STATUS "Enabled AddressSanitizer, UndefinedBehaviorSanitizer, and related checks")
    else()
        message(WARNING "Sanitizers requested but compiler doesn't support them")
    endif()
endif()

if(UTF_STRINGS_ENABLE_THREAD_SANITIZER)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(SANITIZER_FLAGS 
            -fsanitize=thread                    # ThreadSanitizer - detects data races
            -fno-omit-frame-pointer              # Keep frame pointers for better stack traces
            -g                                   # Debug info for better error reports
        )
        set(SANITIZER_LINK_FLAGS ${SANITIZER_FLAGS})
        message(STATUS "Enabled ThreadSanitizer")
    else()
        message(WARNING "ThreadSanitizer requested but compiler doesn't support it")
    endif()
endif()

# Helper function to apply sanitizers to a target
function(apply_sanitizers target_name)
    if(SANITIZER_FLAGS)
        target_compile_options(${target_name} PRIVATE ${SANITIZER_FLAGS})
        target_link_options(${target_name} PRIVATE ${SANITIZER_LINK_FLAGS})
    endif()
endfunction()

# --------------------------------------------------------------------
# Sources
# --------------------------------------------------------------------
set(UTF_STRINGS_HEADERS
    include/utf/utf_strings.hpp
    src/export.hpp
)
set(UTF_STRINGS_SOURCES
    src/utf_strings.cpp
)

# --------------------------------------------------------------------
# Libraries (static + shared)
# --------------------------------------------------------------------
add_library(utf_strings_static STATIC ${UTF_STRINGS_HEADERS} ${UTF_STRINGS_SOURCES})
target_include_directories(utf_strings_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(utf_strings_static PRIVATE UTF_STRINGS_STATIC_BUILD)
target_compile_features(utf_strings_static PUBLIC cxx_std_23)
apply_build_flags(utf_strings_static)

add_library(utf_strings_shared SHARED ${UTF_STRINGS_HEADERS} ${UTF_STRINGS_SOURCES})
target_include_directories(utf_strings_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(utf_strings_shared PRIVATE UTF_STRINGS_DLL)
set_target_properties(utf_strings_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_features(utf_strings_shared PUBLIC cxx_std_23)
apply_build_flags(utf_strings_shared)

# --------------------------------------------------------------------
# Tests (GoogleTest via Conan)
# --------------------------------------------------------------------
if(UTF_STRINGS_BUILD_TESTS)
  include(CTest)
  enable_testing()

  find_package(GTest CONFIG REQUIRED)

  add_executable(utf_strings-tests tests/utf8_tests.cpp)
  target_link_libraries(utf_strings-tests PRIVATE utf_strings_static GTest::gtest GTest::gtest_main)
  target_compile_features(utf_strings-tests PRIVATE cxx_std_23)
  
  # Apply build flags and sanitizers to tests
  apply_build_flags(utf_strings-tests)
  apply_sanitizers(utf_strings-tests)

  include(GoogleTest)
  gtest_discover_tests(utf_strings-tests)
endif()

# --------------------------------------------------------------------
# Benchmarks (Google Benchmark via Conan)
# --------------------------------------------------------------------
if(UTF_STRINGS_BUILD_BENCHMARKS)
  find_package(benchmark CONFIG REQUIRED)

  add_executable(utf_strings-bench benchmarks/utf8_bench.cpp)
  target_link_libraries(utf_strings-bench PRIVATE utf_strings_static benchmark::benchmark)
  target_compile_features(utf_strings-bench PRIVATE cxx_std_23)
  
  # Apply build flags and sanitizers to benchmarks
  apply_build_flags(utf_strings-bench)
  apply_sanitizers(utf_strings-bench)
  
  # Add custom target for profiling with perf
  find_program(PERF_EXECUTABLE perf)
  if(PERF_EXECUTABLE)
    add_custom_target(profile-bench
      COMMAND ${PERF_EXECUTABLE} record -g --call-graph=dwarf $<TARGET_FILE:utf_strings-bench>
      COMMAND ${PERF_EXECUTABLE} report
      DEPENDS utf_strings-bench
      COMMENT "Profiling utf_strings-bench with perf"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endif()

  # Optional gperftools (must come after target creation)
  if(UTF_STRINGS_WITH_GPERFTOOLS)
    find_package(gperftools QUIET CONFIG)
    if(gperftools_FOUND)
      # Check if profiler.h is available (some packages only include tcmalloc)
      find_path(GPERFTOOLS_PROFILER_INCLUDE_DIR 
                gperftools/profiler.h 
                PATHS ${gperftools_INCLUDE_DIRS} 
                NO_DEFAULT_PATH)
      
      if(GPERFTOOLS_PROFILER_INCLUDE_DIR)
        message(STATUS "gperftools found with profiler support; benchmarks will link it")
        target_compile_definitions(utf_strings-bench PRIVATE HAVE_GPERFTOOLS)
        target_link_libraries(utf_strings-bench PRIVATE gperftools::gperftools)
      else()
        message(STATUS "gperftools found but without profiler support; using tcmalloc only")
        target_link_libraries(utf_strings-bench PRIVATE gperftools::tcmalloc_minimal)
      endif()
    else()
      message(WARNING "gperftools requested but not found. Continuing without it.")
    endif()
  endif()
endif()

# --------------------------------------------------------------------
# Fuzz Tests (libFuzzer/Clang)
# --------------------------------------------------------------------
if(UTF_STRINGS_BUILD_FUZZ_TESTS)
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "Fuzz testing requires Clang compiler")
  endif()

  # Common fuzzing flags
  set(FUZZ_FLAGS 
    -fsanitize=fuzzer                      # Enable libFuzzer
    -fsanitize=address                     # AddressSanitizer
    -fsanitize=undefined                   # UndefinedBehaviorSanitizer  
    -fsanitize=bounds                      # Array bounds checking
    -fsanitize=null                        # Null pointer dereference detection
    -fsanitize=signed-integer-overflow     # Signed integer overflow detection
    -fno-omit-frame-pointer               # Better stack traces
    -g                                    # Debug info
    -O1                                   # Light optimization for better performance
  )

  # UTF-8 fuzz target
  add_executable(fuzz_utf8 fuzz/fuzz_utf8.cpp)
  target_link_libraries(fuzz_utf8 PRIVATE utf_strings_static)
  target_compile_features(fuzz_utf8 PRIVATE cxx_std_23)
  target_compile_options(fuzz_utf8 PRIVATE ${FUZZ_FLAGS})
  target_link_options(fuzz_utf8 PRIVATE ${FUZZ_FLAGS})

  # UTF-16 Big Endian fuzz target
  add_executable(fuzz_utf16_be fuzz/fuzz_utf16_be.cpp)
  target_link_libraries(fuzz_utf16_be PRIVATE utf_strings_static)
  target_compile_features(fuzz_utf16_be PRIVATE cxx_std_23)
  target_compile_options(fuzz_utf16_be PRIVATE ${FUZZ_FLAGS})
  target_link_options(fuzz_utf16_be PRIVATE ${FUZZ_FLAGS})

  # UTF-16 Little Endian fuzz target
  add_executable(fuzz_utf16_le fuzz/fuzz_utf16_le.cpp)
  target_link_libraries(fuzz_utf16_le PRIVATE utf_strings_static)
  target_compile_features(fuzz_utf16_le PRIVATE cxx_std_23)
  target_compile_options(fuzz_utf16_le PRIVATE ${FUZZ_FLAGS})
  target_link_options(fuzz_utf16_le PRIVATE ${FUZZ_FLAGS})

  # UTF-32 Big Endian fuzz target
  add_executable(fuzz_utf32_be fuzz/fuzz_utf32_be.cpp)
  target_link_libraries(fuzz_utf32_be PRIVATE utf_strings_static)
  target_compile_features(fuzz_utf32_be PRIVATE cxx_std_23)
  target_compile_options(fuzz_utf32_be PRIVATE ${FUZZ_FLAGS})
  target_link_options(fuzz_utf32_be PRIVATE ${FUZZ_FLAGS})

  # UTF-32 Little Endian fuzz target
  add_executable(fuzz_utf32_le fuzz/fuzz_utf32_le.cpp)
  target_link_libraries(fuzz_utf32_le PRIVATE utf_strings_static)
  target_compile_features(fuzz_utf32_le PRIVATE cxx_std_23)
  target_compile_options(fuzz_utf32_le PRIVATE ${FUZZ_FLAGS})
  target_link_options(fuzz_utf32_le PRIVATE ${FUZZ_FLAGS})

  # Note: Fuzz targets don't use apply_build_flags() since they have their own specific flags

  # Custom targets for running fuzz tests
  add_custom_target(run_fuzz_utf8
    COMMAND $<TARGET_FILE:fuzz_utf8> -max_total_time=30 -print_final_stats=1
    DEPENDS fuzz_utf8
    COMMENT "Running UTF-8 fuzz test for 30 seconds"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

  add_custom_target(run_fuzz_utf16_be
    COMMAND $<TARGET_FILE:fuzz_utf16_be> -max_total_time=30 -print_final_stats=1
    DEPENDS fuzz_utf16_be
    COMMENT "Running UTF-16 BE fuzz test for 30 seconds"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

  add_custom_target(run_fuzz_utf16_le
    COMMAND $<TARGET_FILE:fuzz_utf16_le> -max_total_time=30 -print_final_stats=1
    DEPENDS fuzz_utf16_le
    COMMENT "Running UTF-16 LE fuzz test for 30 seconds"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

  add_custom_target(run_fuzz_utf32_be
    COMMAND $<TARGET_FILE:fuzz_utf32_be> -max_total_time=30 -print_final_stats=1
    DEPENDS fuzz_utf32_be
    COMMENT "Running UTF-32 BE fuzz test for 30 seconds"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

  add_custom_target(run_fuzz_utf32_le
    COMMAND $<TARGET_FILE:fuzz_utf32_le> -max_total_time=30 -print_final_stats=1
    DEPENDS fuzz_utf32_le
    COMMENT "Running UTF-32 LE fuzz test for 30 seconds"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

  # Meta target to run all fuzz tests
  add_custom_target(run_all_fuzz_tests
    DEPENDS run_fuzz_utf8 run_fuzz_utf16_be run_fuzz_utf16_le run_fuzz_utf32_be run_fuzz_utf32_le
    COMMENT "Running all fuzz tests"
  )

  message(STATUS "Fuzz testing enabled with AddressSanitizer and UndefinedBehaviorSanitizer")
  message(STATUS "Use 'make run_all_fuzz_tests' to run all fuzz tests")
  message(STATUS "Use 'make run_fuzz_<encoding>' to run specific fuzz tests")
endif()

# --------------------------------------------------------------------
# Code Formatting (clang-format)
# --------------------------------------------------------------------
find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format clang-format-18 clang-format-17 clang-format-16 clang-format-15
    DOC "Path to clang-format executable"
)

if(CLANG_FORMAT_EXECUTABLE)
    # Collect all source files for formatting
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/fuzz/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/fuzz/*.hpp
    )
    
    # Target to check if formatting is needed
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${ALL_CXX_SOURCE_FILES}
        COMMENT "Checking code formatting..."
        VERBATIM
    )
    
    # Target to apply formatting
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting code with clang-format..."
        VERBATIM
    )
    
    # Target to show formatting differences
    add_custom_target(format-diff
        COMMAND ${CMAKE_COMMAND} -E echo "Showing formatting differences..."
        COMMAND ${CMAKE_COMMAND} -E echo "Note: This version of clang-format doesn't support --diff flag"
        COMMAND ${CMAKE_COMMAND} -E echo "Use 'git diff' after running 'format' target to see changes"
        COMMENT "Showing formatting differences (see output for instructions)"
        VERBATIM
    )
    
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXECUTABLE}")
    message(STATUS "Available targets:")
    message(STATUS "  format       - Apply formatting to all source files")
    message(STATUS "  format-check - Check if formatting is needed (fails if changes required)")
    message(STATUS "  format-diff  - Show what formatting changes would be made")
else()
    message(STATUS "clang-format not found. Code formatting targets not available.")
    message(STATUS "Install with: sudo apt install clang-format")
endif()

# --------------------------------------------------------------------
# Build Information Summary
# --------------------------------------------------------------------
message(STATUS "")
message(STATUS "=== UTF Strings Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "")
message(STATUS "Options:")
message(STATUS "  UTF_STRINGS_BUILD_TESTS: ${UTF_STRINGS_BUILD_TESTS}")
message(STATUS "  UTF_STRINGS_BUILD_BENCHMARKS: ${UTF_STRINGS_BUILD_BENCHMARKS}")
message(STATUS "  UTF_STRINGS_BUILD_FUZZ_TESTS: ${UTF_STRINGS_BUILD_FUZZ_TESTS}")
message(STATUS "  UTF_STRINGS_ENABLE_SANITIZERS: ${UTF_STRINGS_ENABLE_SANITIZERS}")
message(STATUS "  UTF_STRINGS_ENABLE_THREAD_SANITIZER: ${UTF_STRINGS_ENABLE_THREAD_SANITIZER}")
message(STATUS "  UTF_STRINGS_WITH_GPERFTOOLS: ${UTF_STRINGS_WITH_GPERFTOOLS}")
message(STATUS "")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build optimizations enabled:")
    message(STATUS "  - Aggressive optimization (-O3)")
    message(STATUS "  - Native CPU optimization (-march=native)")
    message(STATUS "  - Link Time Optimization (LTO)")
    message(STATUS "  - Security hardening (stack protection, FORTIFY_SOURCE)")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build features enabled:")
    message(STATUS "  - Debug-friendly optimization (-Og)")
    message(STATUS "  - Maximum debug info (-g3)")
    message(STATUS "  - Frame pointers preserved")
endif()
message(STATUS "==========================================")
message(STATUS "")
