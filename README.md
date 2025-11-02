
# UTF Strings - Cross-Platform C++23 Library

Modern C++23 UTF utilities (UTF-8/16/32) with explicit endianness policy and comprehensive testing.

**Cross-Platform Support:**
- **Linux**: GCC 13+ or Clang 16+  
- **Windows**: Visual Studio 2022 (MSVC)
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
