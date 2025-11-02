
# UTF Strings - Cross-Platform C++23 Library

[![CI/CD Pipeline](https://github.com/wsollers/utf_strings/actions/workflows/ci.yml/badge.svg)](https://github.com/wsollers/utf_strings/actions/workflows/ci.yml)
[![CodeQL Security Analysis](https://github.com/wsollers/utf_strings/actions/workflows/codeql.yml/badge.svg)](https://github.com/wsollers/utf_strings/actions/workflows/codeql.yml)
[![Extended Fuzz Testing](https://github.com/wsollers/utf_strings/actions/workflows/extended-fuzz.yml/badge.svg)](https://github.com/wsollers/utf_strings/actions/workflows/extended-fuzz.yml)
[![Release Validation](https://github.com/wsollers/utf_strings/actions/workflows/release-validation.yml/badge.svg)](https://github.com/wsollers/utf_strings/actions/workflows/release-validation.yml)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-3.25+-064F8C.svg?style=flat&logo=cmake)](https://cmake.org/)
[![Conan](https://img.shields.io/badge/Conan-2.0+-6699CB.svg?style=flat&logo=conan)](https://conan.io/)
[![License](https://img.shields.io/github/license/wsollers/utf_strings.svg?style=flat)](./LICENSE)

[![Linux](https://img.shields.io/badge/Linux-GCC%2013%20%7C%20Clang%2018-FCC624.svg?style=flat&logo=linux&logoColor=black)](https://github.com/wsollers/utf_strings/actions/workflows/ci.yml)
[![Windows](https://img.shields.io/badge/Windows-MSVC%202022%20%7C%20Clang--CL-0078D4.svg?style=flat&logo=windows&logoColor=white)](https://github.com/wsollers/utf_strings/actions/workflows/ci.yml)
[![macOS](https://img.shields.io/badge/macOS-Clang%2016+-000000.svg?style=flat&logo=apple&logoColor=white)](https://github.com/wsollers/utf_strings/actions/workflows/ci.yml)

[![Performance](https://img.shields.io/badge/Performance-Benchmarks-orange.svg?style=flat&logo=speedtest)](https://wsollers.github.io/utf_strings/performance/)
[![Security](https://img.shields.io/badge/Security-Hardened-green.svg?style=flat&logo=security)](https://github.com/wsollers/utf_strings/security)
[![Quality](https://img.shields.io/badge/Code%20Quality-clang--format-brightgreen.svg?style=flat&logo=llvm)](https://clang.llvm.org/docs/ClangFormat.html)

Modern C++23 UTF utilities (UTF-8/16/32) with explicit endianness policy and comprehensive testing.

**Cross-Platform Support:**
- **Linux**: GCC 13+ or Clang 18+  
- **Windows**: MSVC 2022 or Clang-CL 16+
- **macOS**: Clang 16+ (Apple Clang or LLVM)

**Key Features:**
- 🚀 **Single Bootstrap Configuration** - One command for complete setup
- 🔒 **Security Hardened** - Stack protection, ASLR, DEP, Control Flow Integrity
- ⚡ **Performance Optimized** - LTO, native CPU optimization, vectorization
- 🧪 **Comprehensive Testing** - Unit tests, fuzz testing, sanitizers, benchmarks
- 🎯 **Code Quality** - clang-format integration, extensive warnings, static analysis

## Quick Start

### Automated Setup (Recommended)

**Linux/macOS:**
```bash
./bootstrap_cmake.sh                 # Auto-detect compiler and full setup
./bootstrap_cmake.sh --compiler gcc  # Force GCC 
./bootstrap_cmake.sh --compiler clang # Force Clang
```

**Windows (cmd/PowerShell):**
```cmd
bootstrap_cmake.bat                  # Auto-detect and full setup
```

That's it! The bootstrap script will:
- ✅ Detect your platform and available compilers
- ✅ Install missing tools (CMake, Conan, clang-format)
- ✅ Configure optimized builds (Debug + Release)
- ✅ Run comprehensive tests and benchmarks
- ✅ Set up development environment

### Manual Setup (Advanced)

If you prefer manual control:

```bash
# Install dependencies
conan install . -s build_type=Debug   --output-folder=build --build=missing
conan install . -s build_type=Release --output-folder=build --build=missing

# Configure (using presets)
cmake --preset conan-debug    # Debug build
cmake --preset conan-release  # Release build

# Build
cmake --build --preset conan-debug --parallel
cmake --build --preset conan-release --parallel

# Test
build/Debug/utf_strings-tests      # Linux/macOS
build\Debug\utf_strings-tests.exe  # Windows

# Benchmark  
build/build/utf_strings-bench --benchmark_min_time=0.1s
```
