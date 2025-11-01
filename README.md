
# utf_strings (CMake, full implementation)

Modern C++23 UTF utilities (UTF-8/16/32) with explicit endianness policy (big/little/native).
Build system: **CMake + Conan 2**. Tests: **GoogleTest**. Benchmarks: **Google Benchmark**. Optional **gperftools**.

## Quickstart

```bash
# Install deps via Conan (Debug + Release)
conan install . -s build_type=Debug   --output-folder=build --build=missing
conan install . -s build_type=Release --output-folder=build --build=missing

# Configure
cmake -S . -B build/Debug   -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B build/Release -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build/Debug   --config Debug   -- -j
cmake --build build/Release --config Release -- -j

# Tests
ctest --test-dir build/Debug   -C Debug   --output-on-failure
ctest --test-dir build/Release -C Release --output-on-failure

# Benchmarks (Release)
./build/Release/utf_strings-bench --benchmark_min_time=0.05
```
