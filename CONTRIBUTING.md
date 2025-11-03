# Contributing to utf_strings

## Development Setup

### Code Formatting

This project uses `clang-format` to maintain consistent code formatting. 

#### Automatic Formatting (Recommended)

Install the pre-commit hook to automatically format your code:

```bash
./setup-hooks.sh
```

This will:
- ✅ Automatically format C++ files before each commit
- ✅ Prevent CI failures due to formatting issues
- ✅ Use the project's `.clang-format` configuration

#### Manual Formatting

If you prefer manual control:

```bash
# Format ALL files in repository (recommended after cloning)
./format-all.sh

# Format all files via CMake
cmake --build build --target format

# Check formatting without changes
cmake --build build --target format-check

# Format specific file
clang-format -i path/to/file.hpp
```

#### Requirements

- **clang-format**: Install via your package manager
  - Ubuntu/Debian: `sudo apt install clang-format`  
  - macOS: `brew install clang-format`
  - Windows: Install LLVM/Clang tools

## Building

### Quick Setup
```bash
# Bootstrap automatically detects and configures everything
./bootstrap_cmake.sh                 # Linux/macOS
bootstrap_cmake.bat                  # Windows

# Manual build
cmake --preset conan-release         # or conan-debug
cmake --build --preset conan-release --parallel

# Test
cd build/build && ./utf_strings-tests

# Benchmark
cd build/build && ./utf_strings-bench
```

### Advanced Compiler Configuration

For specific testing scenarios or performance optimization:

```bash
# Development build (fast iteration)
cmake --preset conan-debug \
  -DCOMPILER_TYPE=CLANG \
  -DUSE_LTO=OFF \
  -DUSE_NATIVE_ARCH=OFF \
  -DENABLE_SHARED_LIBRARY=OFF

# Performance testing build
cmake --preset conan-release \
  -DCOMPILER_TYPE=CLANG \
  -DUSE_LTO=ON \
  -DUSE_NATIVE_ARCH=ON \
  -DUSE_LIBC_PLUS_PLUS=ON \
  -DENABLE_SHARED_LIBRARY=ON

# Cross-platform compatibility testing
cmake --preset conan-release \
  -DCOMPILER_TYPE=GCC \
  -DUSE_LTO=ON \
  -DUSE_NATIVE_ARCH=OFF \
  -DENABLE_SHARED_LIBRARY=ON
```

### Available Configuration Flags

| Flag | Values | Purpose |
|------|--------|---------|
| `COMPILER_TYPE` | `GCC`\|`CLANG`\|`MSVC` | Explicit compiler identification for debugging |
| `USE_LTO` | `ON`\|`OFF` | Enable/disable Link Time Optimization |
| `USE_NATIVE_ARCH` | `ON`\|`OFF` | Enable/disable native CPU optimization |
| `USE_MSVC_LTO` | `ON`\|`OFF` | MSVC-specific LTO flags (`/LTCG`, `/GL`) |
| `USE_LIBC_PLUS_PLUS` | `ON`\|`OFF` | Use libc++ instead of libstdc++ (Clang only) |
| `ENABLE_SHARED_LIBRARY` | `ON`\|`OFF` | Build shared libraries |

## Code Style

- Follow the `.clang-format` configuration in the repository root
- Use C++23 features where appropriate with fallbacks for compatibility
- Prefer standard library functions over platform-specific alternatives
- Write clear, self-documenting code with appropriate comments

## Code Review Requirements

**⚠️ MANDATORY**: All code must undergo comprehensive review before every push to origin.

### Pre-Push Review Checklist

Before submitting any changes, review your code against our **[Code Review Guidelines](CODE_REVIEW_GUIDELINES.md)**:

- [ ] **Security Analysis**: No undefined behavior, memory safety issues, or vulnerabilities
- [ ] **Performance Analysis**: Efficient algorithms, no unnecessary allocations
- [ ] **Correctness**: Edge cases handled, proper error handling, const correctness
- [ ] **C++ Guidelines**: RAII, exception safety, modern C++23 features used appropriately
- [ ] **Design Quality**: Consistent API, proper abstraction levels, clear naming
- [ ] **Documentation**: Adequate comments, API docs, up-to-date examples
- [ ] **Test Coverage**: Comprehensive unit tests including edge cases

### Review Standards

Every change must meet:
- 🔴 **Zero Critical Issues**: No security vulnerabilities, UB, or crashes
- 🟡 **Minimal Important Issues**: Address performance and correctness concerns
- ✅ **Production Ready**: Code must be deployable to production

See **[CODE_REVIEW_GUIDELINES.md](CODE_REVIEW_GUIDELINES.md)** for complete review parameters.

## Submitting Changes

1. **Pre-Push Review**: Complete comprehensive code review using guidelines above
2. **Build Verification**: Ensure changes build successfully across all configurations
3. **Test Validation**: Run full test suite to ensure functionality is preserved  
4. **Code Formatting**: Pre-commit hook will automatically format your code
5. **Submit PR**: Create pull request with clear description referencing review checklist

## CI/CD

The project runs comprehensive CI tests with compiler-specific optimizations:

### Compiler Matrix
- **Linux GCC 13**: Production-optimized builds with LTO and native arch optimization
- **Linux Clang 18**: Development builds with libstdc++ and performance builds with libc++
- **Windows MSVC 2022**: Full optimization with `/LTCG` and `/GL` flags
- **Windows Clang-CL 16+**: Clang optimization without MSVC-specific flags

### Testing Framework
- **Unit Tests**: GoogleTest with comprehensive UTF-8/16/32 coverage
- **Sanitizer Testing**: AddressSanitizer, UndefinedBehaviorSanitizer, ThreadSanitizer (Clang)
- **Fuzz Testing**: libFuzzer-based testing for all UTF encodings (Clang)
- **Performance Testing**: Automated benchmarking with flame graph generation
- **Security Analysis**: CodeQL static analysis and security hardening validation

### Build Configurations
Each compiler uses optimized configuration flags:
- **GCC**: `-O3 -march=native -flto -ftree-vectorize` with security hardening
- **Clang**: `-O3 -march=native -flto -fvectorize` with optional libc++
- **MSVC**: `/O2 /GL /LTCG /arch:AVX2` with security flags
- **Clang-CL**: Clang optimizations without MSVC-specific flags

### Quality Gates
All builds include:
- Comprehensive warning flags with `-Werror`/`/WX`
- Code formatting checks with clang-format
- Memory safety validation with sanitizers
- Performance regression detection
- Cross-platform compatibility verification

Your pull request must pass all CI checks before merging. The CI automatically tests your changes across all supported compiler and platform combinations.

## 🤖 AI Assistant Guidelines

This project includes context files to improve AI assistant interactions:

### Using AI Assistants Effectively

**Context Files Available:**
- **`.ai-context`** - Comprehensive project overview and guidelines
- **`.copilot-instructions.md`** - GitHub Copilot-specific patterns

**Best Practices:**
1. **Reference project context**: "Use the coding patterns from .ai-context"
2. **Specify C++23 requirements**: "Follow C++23 standards and UTF handling patterns"  
3. **Request comprehensive testing**: "Include unit tests and consider fuzz testing scenarios"
4. **Mention performance**: "Consider performance implications and benchmarking"

**Effective Prompts:**
```
# For new features
"Generate a UTF-16 to UTF-8 converter following the UTF Strings project context and C++23 patterns"

# For optimization  
"Optimize this UTF processing function for performance, considering SIMD and the project's benchmarking approach"

# For testing
"Create comprehensive tests for this UTF validation function, including edge cases and invalid sequences"

# For debugging
"Help debug this endianness issue in UTF-32 conversion, using the project's endian policy patterns"
```

**Code Generation Guidelines:**
- Always use C++23 features appropriately
- Follow project naming conventions (snake_case functions, PascalCase types)
- Include proper error handling and input validation
- Consider endianness for multi-byte encodings
- Add appropriate documentation comments
- Create corresponding unit tests

The AI context ensures consistent, high-quality assistance across ChatGPT, Claude, and GitHub Copilot! 🚀