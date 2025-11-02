# UTF Strings Development Workflow

This document provides complete information for developing, testing, and maintaining the UTF Strings C++23 library.

## Table of Contents

- [Quick Start](#quick-start)
- [Project Setup](#project-setup)
- [Build System](#build-system)
- [Testing Framework](#testing-framework)
- [Code Quality](#code-quality)
- [Performance Analysis](#performance-analysis)
- [Development Workflow](#development-workflow)
- [CI/CD Integration](#cicd-integration)
- [Troubleshooting](#troubleshooting)

## Quick Start

```bash
# Clone and setup
git clone <repository-url>
cd utf_strings

# Bootstrap the project (installs dependencies including clang-format)
./bootstrap_cmake.sh

# Build and test
cmake --preset conan-release
cmake --build --preset conan-release
./build/build/utf_strings-tests

# Run comprehensive tests
./test_all_sanitizers.sh
```

## Project Setup

### Prerequisites

**Required:**
- CMake 3.25+
- C++23 compatible compiler (GCC 13+, Clang 16+)
- Python 3.8+ (for Conan)
- Git

**Optional (auto-installed by bootstrap):**
- Conan 2.0+
- clang-format
- perf (for profiling)

### Initial Setup

1. **Bootstrap the project:**
   ```bash
   ./bootstrap_cmake.sh
   ```
   This script automatically:
   - Installs Conan if missing
   - Installs clang-format if missing
   - Sets up the build environment
   - Downloads dependencies

2. **Verify installation:**
   ```bash
   cmake --preset conan-release
   cmake --build --preset conan-release
   ```

## Build System

### Build Configurations

The project supports multiple build configurations optimized for different use cases:

#### Release Build (Production)
```bash
cmake --preset conan-release
cmake --build --preset conan-release
```

**Features:**
- Aggressive optimization (`-O3`, `-march=native`)
- Link Time Optimization (LTO)
- Security hardening (`-fstack-protector-strong`, `-D_FORTIFY_SOURCE=2`)
- Position Independent Executables (`-fPIE`)
- Comprehensive warning flags with `-Werror`

#### Debug Build (Development)
```bash
cmake --preset conan-debug
cmake --build --preset conan-debug
```

**Features:**
- Debug-friendly optimization (`-Og`)
- Maximum debug info (`-g3`)
- Frame pointers preserved
- Sanitizer support available

#### Sanitizer Builds (Testing)
```bash
# AddressSanitizer + UndefinedBehaviorSanitizer
cmake --preset conan-debug -DUTF_STRINGS_ENABLE_SANITIZERS=ON
cmake --build --preset conan-debug

# ThreadSanitizer (separate build)
cmake --preset conan-debug -DUTF_STRINGS_ENABLE_THREAD_SANITIZER=ON
cmake --build --preset conan-debug
```

#### Fuzz Testing Build
```bash
# Requires Clang
CC=clang CXX=clang++ cmake --preset conan-debug -DUTF_STRINGS_BUILD_FUZZ_TESTS=ON
cmake --build --preset conan-debug
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `UTF_STRINGS_BUILD_TESTS` | `ON` | Build unit tests |
| `UTF_STRINGS_BUILD_BENCHMARKS` | `ON` | Build performance benchmarks |
| `UTF_STRINGS_BUILD_FUZZ_TESTS` | `OFF` | Build fuzz testing targets |
| `UTF_STRINGS_ENABLE_SANITIZERS` | `OFF` | Enable AddressSanitizer + UBSan |
| `UTF_STRINGS_ENABLE_THREAD_SANITIZER` | `OFF` | Enable ThreadSanitizer |
| `UTF_STRINGS_WITH_GPERFTOOLS` | `ON` | Enable gperftools integration |

### Available Targets

| Target | Description |
|--------|-------------|
| `utf_strings_static` | Static library |
| `utf_strings_shared` | Shared library |
| `utf_strings-tests` | Unit tests |
| `utf_strings-bench` | Performance benchmarks |
| `fuzz_utf8` | UTF-8 fuzz testing |
| `fuzz_utf16_be` | UTF-16 Big Endian fuzz testing |
| `fuzz_utf16_le` | UTF-16 Little Endian fuzz testing |
| `fuzz_utf32_be` | UTF-32 Big Endian fuzz testing |
| `fuzz_utf32_le` | UTF-32 Little Endian fuzz testing |

## Testing Framework

### Unit Testing

**Run all unit tests:**
```bash
./build/build/utf_strings-tests
```

**With verbose output:**
```bash
./build/build/utf_strings-tests --gtest_verbose
```

### Sanitizer Testing

**Comprehensive sanitizer testing:**
```bash
./test_all_sanitizers.sh
```

This script runs tests with:
- AddressSanitizer + UndefinedBehaviorSanitizer
- ThreadSanitizer
- LeakSanitizer
- Multiple build configurations

**Individual sanitizer runs:**
```bash
# AddressSanitizer + UBSan
cmake --preset conan-debug -DUTF_STRINGS_ENABLE_SANITIZERS=ON
cmake --build --preset conan-debug
./build/Debug_ASan_UBSan/utf_strings-tests

# ThreadSanitizer
cmake --preset conan-debug -DUTF_STRINGS_ENABLE_THREAD_SANITIZER=ON
cmake --build --preset conan-debug
./build/Debug_TSan/utf_strings-tests
```

### Fuzz Testing

**Run all fuzz tests:**
```bash
./run_fuzz_tests.sh
```

**Specific encoding with custom duration:**
```bash
./run_fuzz_tests.sh -t 300 -e utf8        # 5 minutes UTF-8 only
./run_fuzz_tests.sh -t 60 -e utf16_be     # 1 minute UTF-16 BE only
./run_fuzz_tests.sh -t 3600 -e all        # 1 hour all encodings
```

**Available encodings:**
- `utf8`: UTF-8 encoding
- `utf16_be`: UTF-16 Big Endian
- `utf16_le`: UTF-16 Little Endian  
- `utf32_be`: UTF-32 Big Endian
- `utf32_le`: UTF-32 Little Endian
- `all`: All encodings (default)

### Benchmark Testing

**Run performance benchmarks:**
```bash
./run_bench.sh
```

**Custom benchmark options:**
```bash
./build/build/utf_strings-bench --benchmark_time_unit=us
./build/build/utf_strings-bench --benchmark_filter="BM_Length.*"
./build/build/utf_strings-bench --benchmark_repetitions=3
```

## Code Quality

### Code Formatting

The project uses Google C++ style with clang-format.

**Check formatting:**
```bash
cmake --build --preset conan-release --target format-check
```

**Apply formatting:**
```bash
cmake --build --preset conan-release --target format
```

**Show formatting differences:**
```bash
cmake --build --preset conan-release --target format-diff
```

### Configuration

Format configuration is in `.clang-format`:
- Based on Google style
- 100 character line limit
- C++23 optimizations enabled
- Consistent indentation and spacing

### Pre-commit Integration

Add to your git hooks (`.git/hooks/pre-commit`):
```bash
#!/bin/bash
cmake --build --preset conan-release --target format-check
if [ $? -ne 0 ]; then
    echo "Code formatting check failed. Run 'cmake --build --preset conan-release --target format' to fix."
    exit 1
fi
```

## Performance Analysis

### Profiling

**Run benchmarks with profiling:**
```bash
./profile_bench.sh
```

This generates:
- `perf.data`: Raw profiling data
- `flame_graph.svg`: Interactive flame graph visualization
- Console performance report

**Manual profiling:**
```bash
perf record -g ./build/build/utf_strings-bench
perf report
```

### Memory Profiling

**With gperftools (tcmalloc):**
```bash
HEAPPROFILE=/tmp/utf_strings ./build/build/utf_strings-tests
```

**With AddressSanitizer:**
```bash
# Build with ASan
cmake --preset conan-debug -DUTF_STRINGS_ENABLE_SANITIZERS=ON
cmake --build --preset conan-debug

# Run with memory checking
./build/Debug_ASan_UBSan/utf_strings-tests
```

### Performance Monitoring

**Key metrics to monitor:**
- UTF-8 validation speed (GB/s)
- UTF-16/32 conversion performance
- Memory allocation patterns
- Cache miss rates

**Benchmark interpretation:**
```bash
# Example output:
BM_Length_Mixed    7.71 us    7.71 us    17971 bytes_per_second=1.45Gi/s
# This shows 1.45 GiB/s processing speed for mixed UTF-8 content
```

## Development Workflow

### Standard Development Cycle

1. **Setup development environment:**
   ```bash
   ./bootstrap_cmake.sh
   cmake --preset conan-debug
   ```

2. **Make changes to source code**

3. **Format code:**
   ```bash
   cmake --build --preset conan-debug --target format
   ```

4. **Build and test:**
   ```bash
   cmake --build --preset conan-debug
   ./build/Debug/utf_strings-tests
   ```

5. **Run sanitizer tests:**
   ```bash
   ./test_all_sanitizers.sh
   ```

6. **Performance validation:**
   ```bash
   cmake --preset conan-release
   cmake --build --preset conan-release
   ./run_bench.sh
   ```

7. **Fuzz testing (for critical changes):**
   ```bash
   ./run_fuzz_tests.sh -t 300  # 5 minute fuzz test
   ```

### Release Preparation

1. **Full test suite:**
   ```bash
   ./test_all_sanitizers.sh
   ./run_fuzz_tests.sh -t 3600  # 1 hour comprehensive fuzzing
   ```

2. **Performance validation:**
   ```bash
   ./profile_bench.sh
   ```

3. **Code quality check:**
   ```bash
   cmake --build --preset conan-release --target format-check
   ```

4. **Release build verification:**
   ```bash
   cmake --preset conan-release
   cmake --build --preset conan-release
   ./build/build/utf_strings-tests
   ```

### Adding New Features

1. **Write tests first (TDD approach)**
2. **Implement feature**
3. **Add fuzz testing if handling external input**
4. **Add benchmarks for performance-critical code**
5. **Update documentation**

## CI/CD Integration

### GitHub Actions Example

```yaml
name: UTF Strings CI

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        build_type: [Debug, Release]
        sanitizer: [none, asan, tsan]
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Bootstrap
      run: ./bootstrap_cmake.sh
    
    - name: Configure
      run: |
        if [ "${{ matrix.sanitizer }}" = "asan" ]; then
          cmake --preset conan-${{ matrix.build_type }} -DUTF_STRINGS_ENABLE_SANITIZERS=ON
        elif [ "${{ matrix.sanitizer }}" = "tsan" ]; then
          cmake --preset conan-${{ matrix.build_type }} -DUTF_STRINGS_ENABLE_THREAD_SANITIZER=ON
        else
          cmake --preset conan-${{ matrix.build_type }}
        fi
    
    - name: Build
      run: cmake --build --preset conan-${{ matrix.build_type }}
    
    - name: Test
      run: ./build/${{ matrix.build_type }}/utf_strings-tests
    
    - name: Fuzz Test
      if: matrix.build_type == 'Debug'
      run: ./run_fuzz_tests.sh -t 60
```

### Jenkins Pipeline Example

```groovy
pipeline {
    agent any
    
    stages {
        stage('Bootstrap') {
            steps {
                sh './bootstrap_cmake.sh'
            }
        }
        
        stage('Test Matrix') {
            parallel {
                stage('Unit Tests') {
                    steps {
                        sh 'cmake --preset conan-debug'
                        sh 'cmake --build --preset conan-debug'
                        sh './build/Debug/utf_strings-tests'
                    }
                }
                
                stage('Sanitizer Tests') {
                    steps {
                        sh './test_all_sanitizers.sh'
                    }
                }
                
                stage('Fuzz Tests') {
                    steps {
                        sh './run_fuzz_tests.sh -t 300'
                    }
                }
                
                stage('Performance Tests') {
                    steps {
                        sh 'cmake --preset conan-release'
                        sh 'cmake --build --preset conan-release'
                        sh './run_bench.sh'
                    }
                }
            }
        }
        
        stage('Code Quality') {
            steps {
                sh 'cmake --build --preset conan-release --target format-check'
            }
        }
    }
    
    post {
        always {
            archiveArtifacts artifacts: 'build/**/utf_strings-tests', allowEmptyArchive: true
            publishTestResults testResultsPattern: 'build/**/test_results.xml'
        }
    }
}
```

## Troubleshooting

### Common Build Issues

**Issue: Conan not found**
```bash
# Solution: Run bootstrap
./bootstrap_cmake.sh
```

**Issue: clang-format not found**
```bash
# Solution: Bootstrap will install it
./bootstrap_cmake.sh

# Or install manually:
sudo apt install clang-format  # Ubuntu/Debian
brew install clang-format     # macOS
```

**Issue: C++23 features not available**
```bash
# Solution: Update compiler
sudo apt install gcc-13 g++-13  # Ubuntu
export CC=gcc-13 CXX=g++-13
```

### Sanitizer Issues

**AddressSanitizer out of memory:**
```bash
# Reduce virtual memory usage
export ASAN_OPTIONS=detect_odr_violation=0:abort_on_error=1
```

**ThreadSanitizer false positives:**
```bash
# Create suppression file
export TSAN_OPTIONS=suppressions=tsan.supp
```

### Performance Issues

**Slow debug builds:**
- Use `-Og` optimization for faster debug builds
- Consider using Release builds for performance testing

**High memory usage in tests:**
- AddressSanitizer increases memory usage 2-3x
- ThreadSanitizer increases memory usage 5-10x
- Use smaller test datasets for sanitizer runs

### Fuzz Testing Issues

**Fuzz targets crash immediately:**
```bash
# Check build configuration
CC=clang CXX=clang++ cmake --preset conan-debug -DUTF_STRINGS_BUILD_FUZZ_TESTS=ON

# Verify sanitizer integration
export ASAN_OPTIONS=abort_on_error=1
```

**Slow fuzz testing:**
- Ensure using Clang compiler
- Use Release build for maximum performance
- Adjust test duration with `-t` flag

### IDE Integration

**Visual Studio Code:**
- Install C/C++ extension
- Configure `.vscode/settings.json`:
```json
{
  "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "clang-format.executable": "/usr/bin/clang-format"
}
```

**CLion:**
- Open CMakeLists.txt as project
- Configure toolchain to use system compilers
- Enable clang-format integration

### Getting Help

1. **Check build logs** for specific error messages
2. **Run with verbose output**: `cmake --build --preset conan-debug --verbose`
3. **Check sanitizer output** for memory/thread issues
4. **Profile performance** with `./profile_bench.sh`
5. **Review test output** from `./test_all_sanitizers.sh`

---

This workflow provides comprehensive guidance for maintaining high code quality, security, and performance in the UTF Strings library. Regular use of these tools and processes ensures robust, production-ready code.