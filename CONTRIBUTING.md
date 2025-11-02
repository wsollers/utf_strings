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
# Format all files
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

```bash
# Configure
cmake --preset conan-release  # or conan-debug

# Build
cmake --build --preset conan-release --parallel

# Test
cd build/build && ./utf_strings-tests

# Benchmark
cd build/build && ./utf_strings-bench
```

## Code Style

- Follow the `.clang-format` configuration in the repository root
- Use C++23 features where appropriate with fallbacks for compatibility
- Prefer standard library functions over platform-specific alternatives
- Write clear, self-documenting code with appropriate comments

## Submitting Changes

1. Make sure your changes build successfully
2. Run the tests to ensure functionality is preserved
3. The pre-commit hook will automatically format your code
4. Submit a pull request with a clear description of changes

## CI/CD

The project runs comprehensive CI tests on:
- **Linux**: GCC 13, Clang 18
- **Windows**: MSVC 2022  
- **Sanitizers**: AddressSanitizer, ThreadSanitizer (Clang)
- **Fuzzing**: libFuzzer-based testing (Clang)

All builds use C++23 standard and include:
- Unit tests
- Benchmarks  
- Code formatting checks
- Sanitizer testing
- Fuzz testing