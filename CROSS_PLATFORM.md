# UTF Strings - Cross-Platform Build System

## Overview

The UTF Strings library now supports **single bootstrap configuration** for cross-platform development:

- **Linux** (x64/arm64): GCC 13+ or Clang 18+
- **Windows** (x64): MSVC 2022 or Clang-CL 16+
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
./bootstrap_cmake.sh --compiler gcc      # Linux/macOS
./bootstrap_cmake.sh --compiler clang    # Linux/macOS  
./bootstrap_cmake.sh --compiler msvc     # Windows MSVC
./bootstrap_cmake.sh --compiler clang-cl # Windows Clang-CL
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

### Windows (MSVC 2022 / Clang-CL 16+)
- ✅ **Security Hardening**: Buffer Security Check (/GS), ASLR, DEP, High Entropy ASLR
- ✅ **Optimization**: Whole Program Optimization (/GL), AVX2 instructions, Clang optimizations
- ✅ **Sanitizers**: AddressSanitizer (/fsanitize=address), Static Analysis (/analyze)
- ✅ **Fuzz Testing**: Fuzz harnesses (use with external fuzzer like WinAFL or libFuzzer with Clang-CL)
- ✅ **Performance**: Link-time optimization, intrinsic functions, Clang vectorization
- ✅ **Compiler Choice**: Full MSVC and Clang-CL support in CI matrix

## CMake Integration

### External Compiler Configuration System
The build system now uses an advanced **external flag configuration** approach that provides complete control over compiler behavior across all platforms:

```cmake
# External configuration flags (set by CI or manual builds)
-DCOMPILER_TYPE=GCC|CLANG|MSVC         # Explicit compiler identification
-DUSE_LTO=ON|OFF                       # Link Time Optimization control
-DUSE_NATIVE_ARCH=ON|OFF               # Native CPU optimization control
-DUSE_MSVC_LTO=ON|OFF                  # MSVC-specific LTO flags (/LTCG, /GL)
-DUSE_LIBC_PLUS_PLUS=ON|OFF            # Standard library selection (Clang)
-DENABLE_SHARED_LIBRARY=ON|OFF         # Shared library building control
```

### Platform-Specific Configurations

**Linux GCC (Production Optimized):**
```bash
cmake --preset conan-release \
  -DCOMPILER_TYPE=GCC \
  -DUSE_LTO=ON \
  -DUSE_NATIVE_ARCH=ON \
  -DENABLE_SHARED_LIBRARY=ON
```
- Security: `-fstack-protector-strong -D_FORTIFY_SOURCE=2 -fcf-protection=full`
- Optimization: `-O3 -march=native -flto -ftree-vectorize -fgraphite-identity`

**Linux Clang (Development):**
```bash
cmake --preset conan-release \
  -DCOMPILER_TYPE=CLANG \
  -DUSE_LTO=ON \
  -DUSE_NATIVE_ARCH=ON \
  -DUSE_LIBC_PLUS_PLUS=OFF \
  -DENABLE_SHARED_LIBRARY=ON
```
- Security: Same as GCC + thread safety analysis
- Optimization: `-O3 -march=native -flto -fvectorize -fslp-vectorize`
- Standard Library: `libstdc++` (system compatible)

**Linux Clang (Maximum Performance):**
```bash
cmake --preset conan-release \
  -DCOMPILER_TYPE=CLANG \
  -DUSE_LTO=ON \
  -DUSE_NATIVE_ARCH=ON \
  -DUSE_LIBC_PLUS_PLUS=ON \
  -DENABLE_SHARED_LIBRARY=ON
```
- Standard Library: `libc++` (optimized for performance)
- Use Case: Benchmarking and performance analysis

**Windows MSVC:**
```cmd
cmake --preset conan-release ^
  -DUSE_MSVC_LTO=ON ^
  -DENABLE_SHARED_LIBRARY=ON
```
- Security: `/GS /sdl /DYNAMICBASE /NXCOMPAT /HIGHENTROPYVA`
- Optimization: `/O2 /Oi /GL /LTCG /arch:AVX2`

**Windows Clang-CL:**
```cmd
cmake --preset conan-release ^
  -DCMAKE_CXX_COMPILER=clang-cl ^
  -DCMAKE_C_COMPILER=clang-cl ^
  -DUSE_MSVC_LTO=OFF ^
  -DENABLE_SHARED_LIBRARY=OFF
```
- Avoids MSVC-specific flags (`/LTCG`, `/GL`) that Clang-CL doesn't support
- Static libraries only (avoids export definition issues)

**Debug/Sanitizer Builds:**
```bash
cmake --preset conan-debug \
  -DCOMPILER_TYPE=CLANG \
  -DUSE_LTO=OFF \
  -DUSE_NATIVE_ARCH=OFF \
  -DENABLE_SHARED_LIBRARY=OFF
```
- Disables aggressive optimizations for better debugging
- Compatible with sanitizers and fuzz testing

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