# UTF Strings - Cross-Platform Build System

## Overview

The UTF Strings library now supports **single bootstrap configuration** for cross-platform development:

- **Linux** (x64/arm64): GCC 13+ or Clang 16+
- **Windows** (x64): Visual Studio 2022 (MSVC)
- **macOS** (x64/arm64): Clang 16+ (Apple Clang or LLVM)

## Key Features ✨

### 🚀 Single Command Setup
```bash
# Linux/macOS
./bootstrap_cmake.sh

# Windows  
bootstrap_cmake.bat
```

### 🔍 Automatic Detection
- **Platform detection**: Linux, Windows, macOS with architecture
- **Compiler detection**: Available GCC, Clang, MSVC installations
- **Tool installation**: Missing CMake, Conan, clang-format automatically installed

### ⚙️ Compiler Selection
```bash
# Auto-select best compiler for platform
./bootstrap_cmake.sh

# Force specific compiler
./bootstrap_cmake.sh --compiler gcc    # Linux/macOS
./bootstrap_cmake.sh --compiler clang  # Linux/macOS  
./bootstrap_cmake.sh --compiler msvc   # Windows (if available)
```

### 🎯 Build Configuration Options
```bash
# Full setup (default)
./bootstrap_cmake.sh

# Specific configuration
./bootstrap_cmake.sh --config Debug
./bootstrap_cmake.sh --config Release

# Skip tests/benchmarks  
./bootstrap_cmake.sh --skip-tests --skip-bench

# Clean build
./bootstrap_cmake.sh --clean
```

## Platform-Specific Features

### Linux/macOS (GCC/Clang)
- ✅ **Security Hardening**: Stack protection, FORTIFY_SOURCE, Control Flow Integrity
- ✅ **Optimization**: LTO, native CPU optimization, vectorization  
- ✅ **Sanitizers**: AddressSanitizer, UBSan, ThreadSanitizer, LeakSanitizer
- ✅ **Fuzz Testing**: libFuzzer integration with comprehensive UTF coverage
- ✅ **Performance Profiling**: perf integration, flame graphs, tcmalloc

### Windows (MSVC 2022)
- ✅ **Security Hardening**: Buffer Security Check (/GS), ASLR, DEP, High Entropy ASLR
- ✅ **Optimization**: Whole Program Optimization (/GL), AVX2 instructions
- ✅ **Sanitizers**: AddressSanitizer (/fsanitize=address), Static Analysis (/analyze)
- ✅ **Fuzz Testing**: Fuzz harnesses (use with external fuzzer like WinAFL)
- ✅ **Performance**: Link-time optimization, intrinsic functions

## CMake Integration

### Cross-Platform Compiler Support
The CMakeLists.txt now includes comprehensive compiler support:

```cmake
# Automatic compiler detection and flag application
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC/Clang specific flags
    set(SECURITY_FLAGS -fstack-protector-strong -D_FORTIFY_SOURCE=2 ...)
    set(OPTIMIZATION_FLAGS -O3 -march=native -flto ...)
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC specific flags  
    set(SECURITY_FLAGS /GS /sdl /DYNAMICBASE /NXCOMPAT ...)
    set(OPTIMIZATION_FLAGS /O2 /Oi /GL /arch:AVX2 ...)
endif()
```

### Sanitizer Support
```cmake
# Cross-platform sanitizer support
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(SANITIZER_FLAGS -fsanitize=address -fsanitize=undefined ...)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(SANITIZER_FLAGS /fsanitize=address /Zi ...)
endif()
```

### Fuzz Testing
```cmake
# Cross-platform fuzz testing
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # libFuzzer integration
    set(FUZZ_FLAGS -fsanitize=fuzzer -fsanitize=address ...)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")  
    # Fuzz harnesses for external fuzzers
    set(FUZZ_FLAGS /fsanitize=address /Zi ...)
endif()
```

## Development Workflow

### 1. Initial Setup
```bash
# Clone repository
git clone <repo-url>
cd utf_strings

# Single command setup
./bootstrap_cmake.sh  # Linux/macOS
bootstrap_cmake.bat   # Windows
```

### 2. Development Cycle
```bash
# Code formatting (cross-platform)
cmake --build --preset conan-release --target format

# Format checking  
cmake --build --preset conan-release --target format-check

# Build and test
cmake --build --preset conan-debug
build/Debug/utf_strings-tests      # Linux/macOS
build\Debug\utf_strings-tests.exe  # Windows

# Performance benchmarks
build/build/utf_strings-bench --benchmark_min_time=0.1s
```

### 3. Comprehensive Testing
```bash
# All sanitizer builds (Linux/macOS)
./test_all_sanitizers.sh

# Fuzz testing (Linux/macOS with Clang)
./run_fuzz_tests.sh

# Performance profiling (Linux/macOS)
./profile_bench.sh
```

## Tool Dependencies

### Automatically Installed
- **CMake 3.25+**: Build system
- **Conan 2.0+**: Package manager (via pip)
- **clang-format**: Code formatting

### Package Manager Detection
The bootstrap script automatically detects and uses available package managers:

**Linux:**
- apt (Ubuntu/Debian)
- yum/dnf (Red Hat/Fedora)  
- pacman (Arch)

**Windows:**
- winget (Windows 10 1809+)
- chocolatey
- scoop

**macOS:**
- Homebrew

### Manual Installation (if needed)
- **Visual Studio 2022** (Windows): Install with C++ tools
- **Xcode Command Line Tools** (macOS): `xcode-select --install`
- **Python 3.8+**: For Conan package manager

## Build Artifacts

### Debug Build
- **Linux/macOS**: `build/Debug/`
- **Windows**: `build\Debug\`

### Release Build  
- **Linux/macOS**: `build/build/`
- **Windows**: `build\build\`

### Executables
- **Tests**: `utf_strings-tests[.exe]`
- **Benchmarks**: `utf_strings-bench[.exe]`
- **Fuzz Targets**: `fuzz_utf8[.exe]`, `fuzz_utf16_be[.exe]`, etc.

## Performance

All platforms achieve high performance through:
- **Native CPU optimization**: -march=native (GCC/Clang), /arch:AVX2 (MSVC)
- **Link-time optimization**: -flto (GCC/Clang), /LTCG (MSVC)
- **Vectorization**: Automatic SIMD optimization
- **Security without performance cost**: Modern hardening techniques

## Next Steps

1. **Verify Setup**: Run `./bootstrap_cmake.sh --help` to see all options
2. **Start Development**: The project is ready for cross-platform development
3. **CI/CD Integration**: Bootstrap scripts work in automated environments
4. **Extend Support**: Easy to add new compilers or platforms using the existing framework

The UTF Strings library now provides a **enterprise-ready, cross-platform development environment** that can be set up with a single command on any supported platform! 🚀