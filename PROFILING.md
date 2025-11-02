# Profiling Guide for UTF Strings

This guide explains how to profile the UTF Strings benchmarks and analyze performance using perf and flame graphs.

## Quick Start

### Run Basic Benchmarking
```bash
./run_bench.sh
```

### Run Full Profiling
```bash
./profile_bench.sh
```

## Detailed Profiling Workflow

### 1. Run Performance Profiling

The `profile_bench.sh` script automatically:
- Builds the release version
- Installs perf if needed (handles kernel version mismatches)
- Records performance data with detailed call graphs
- Generates initial reports

```bash
./profile_bench.sh
```

**Output files created:**
- `perf.data` - Raw performance data (binary format)
- `perf.script` - Flame graph ready data (text format)

### 2. View Detailed Performance Reports

After running the profiling script, you can view detailed reports:

```bash
# Interactive report browser (recommended)
/usr/lib/linux-tools/6.8.0-87-generic/perf report

# Text-based report
/usr/lib/linux-tools/6.8.0-87-generic/perf report --stdio

# Focus on specific functions
/usr/lib/linux-tools/6.8.0-87-generic/perf report --stdio --symbol=BM_Length_Mixed

# Show assembly code with performance annotations
/usr/lib/linux-tools/6.8.0-87-generic/perf annotate BM_Length_Mixed
```

### 3. Generate Flame Graphs

Flame graphs provide excellent visual representation of where CPU time is spent.

#### Step 1: Install FlameGraph Tools
```bash
git clone https://github.com/brendangregg/FlameGraph.git
```

#### Step 2: Generate Flame Graph
```bash
# Generate flame graph from the perf.script file
cat perf.script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl > flamegraph.svg

# Open in web browser
firefox flamegraph.svg
# or
google-chrome flamegraph.svg
# or
xdg-open flamegraph.svg
```

#### Step 3: Generate Different Types of Flame Graphs

```bash
# CPU flame graph (default)
cat perf.script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl > cpu-flamegraph.svg

# Reverse flame graph (callees first)
cat perf.script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl --reverse > reverse-flamegraph.svg

# Differential flame graph (compare two profiles)
# First run: ./profile_bench.sh && mv perf.script perf1.script
# After changes: ./profile_bench.sh && mv perf.script perf2.script
FlameGraph/difffolded.pl perf1.script perf2.script | FlameGraph/flamegraph.pl > diff-flamegraph.svg
```

## Advanced Profiling

### Hardware Performance Counters

```bash
# Profile specific hardware events
/usr/lib/linux-tools/6.8.0-87-generic/perf stat -e cycles,instructions,cache-misses,branch-misses ./build/Release/utf_strings-bench

# Detailed cache analysis
/usr/lib/linux-tools/6.8.0-87-generic/perf stat -e L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./build/Release/utf_strings-bench

# Branch prediction analysis
/usr/lib/linux-tools/6.8.0-87-generic/perf stat -e branches,branch-misses,branch-loads,branch-load-misses ./build/Release/utf_strings-bench
```

### Memory Profiling

```bash
# Record memory access patterns
/usr/lib/linux-tools/6.8.0-87-generic/perf record -e cache-misses -g ./build/Release/utf_strings-bench

# Memory bandwidth analysis
/usr/lib/linux-tools/6.8.0-87-generic/perf stat -e cpu/mem-loads/,cpu/mem-stores/ ./build/Release/utf_strings-bench
```

### Function-Level Analysis

```bash
# Profile only specific functions
/usr/lib/linux-tools/6.8.0-87-generic/perf record -e cycles -g --call-graph dwarf ./build/Release/utf_strings-bench

# View function-level hotspots
/usr/lib/linux-tools/6.8.0-87-generic/perf report --sort=symbol

# Top functions by CPU usage
/usr/lib/linux-tools/6.8.0-87-generic/perf report --stdio --sort=overhead
```

## Understanding the Output

### Performance Metrics Explanation

- **task-clock**: CPU time used by the benchmark
- **cycles**: Number of CPU cycles consumed
- **instructions**: Number of instructions executed
- **IPC (Instructions Per Cycle)**: Higher is better (>1.0 is good)
- **branch-misses**: Branch mispredictions (lower % is better)
- **cache-misses**: Memory cache misses (lower is better)

### Flame Graph Interpretation

- **Width**: Proportional to time spent in function
- **Height**: Call stack depth
- **Color**: Random (not meaningful for CPU graphs)
- **Hotspots**: Wide bars indicate performance bottlenecks

## Troubleshooting

### Kernel Version Mismatch
If you see warnings about kernel version mismatch, the script automatically handles this by finding the correct perf binary. No action needed.

### Permission Issues
If you encounter permission errors:
```bash
# Temporarily allow perf without sudo (session only)
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid

# Or add your user to the perf_users group (if exists)
sudo usermod -a -G perf_users $USER
```

### Missing Dependencies
The script automatically installs required dependencies, but if you encounter issues:
```bash
# Manual installation
sudo apt update
sudo apt install linux-tools-generic

# For specific kernel
sudo apt install linux-tools-$(uname -r)
```

## Profiling Different Build Types

### Debug Build Profiling
```bash
# Configure for debug
cmake -S . -B build/Debug -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DUTF_STRINGS_WITH_GPERFTOOLS=ON

# Build debug version
cmake --build build/Debug

# Profile debug build
/usr/lib/linux-tools/6.8.0-87-generic/perf record -g ./build/Debug/utf_strings-bench
```

### Profile with Different Optimization Levels
```bash
# Configure with specific optimization
cmake -S . -B build/Custom -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native" -DUTF_STRINGS_WITH_GPERFTOOLS=ON
```

## Files Created

After profiling, you'll have these files:
- `perf.data` - Raw perf data (binary)
- `perf.script` - Text format for flame graphs
- `flamegraph.svg` - Visual flame graph (after generation)
- `FlameGraph/` - Flame graph tools directory

## Tips for Better Profiling

1. **Disable CPU scaling**: `sudo cpupower frequency-set --governor performance`
2. **Close other applications**: Reduce system noise
3. **Run multiple times**: Results can vary between runs
4. **Profile different scenarios**: Various input sizes and patterns
5. **Compare before/after**: Use differential flame graphs for optimizations

## Example Workflow

```bash
# 1. Run profiling
./profile_bench.sh

# 2. Generate flame graph
git clone https://github.com/brendangregg/FlameGraph.git
cat perf.script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl > flamegraph.svg

# 3. View results
firefox flamegraph.svg
/usr/lib/linux-tools/6.8.0-87-generic/perf report

# 4. Make code changes...

# 5. Profile again and compare
./profile_bench.sh
cat perf.script | FlameGraph/stackcollapse-perf.pl > after.folded
# Compare with previous: FlameGraph/difffolded.pl before.folded after.folded | FlameGraph/flamegraph.pl > diff.svg
```

## Testing with Sanitizers

### Comprehensive Testing Script
```bash
# Run all tests with various sanitizer configurations
./test_all_sanitizers.sh
```

This script runs:
- Debug build with AddressSanitizer + UndefinedBehaviorSanitizer
- Debug build with ThreadSanitizer (if Clang available)  
- Release build (clean, no sanitizers)
- Extended fuzz testing (if Clang available)

### Individual Sanitizer Builds

#### AddressSanitizer + UndefinedBehaviorSanitizer
```bash
cmake -S . -B build/Debug_Sanitized \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DUTF_STRINGS_ENABLE_SANITIZERS=ON
cmake --build build/Debug_Sanitized
build/Debug_Sanitized/utf_strings-tests
```

#### ThreadSanitizer (requires Clang)
```bash
cmake -S . -B build/Debug_TSan \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DUTF_STRINGS_ENABLE_THREAD_SANITIZER=ON
cmake --build build/Debug_TSan
build/Debug_TSan/utf_strings-tests
```

## Fuzz Testing

### Quick Fuzz Testing
```bash
# Run all fuzz tests for 60 seconds each
./run_fuzz_tests.sh

# Run specific encoding for 30 seconds
./run_fuzz_tests.sh --encoding utf8 --time 30

# Available encodings: utf8, utf16_be, utf16_le, utf32_be, utf32_le, all
```

### Manual Fuzz Testing
```bash
# Build fuzz targets
cmake -S . -B build/Fuzz \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DUTF_STRINGS_BUILD_FUZZ_TESTS=ON
cmake --build build/Fuzz

# Run individual fuzz tests
build/Fuzz/fuzz_utf8 -max_total_time=300 -print_final_stats=1
build/Fuzz/fuzz_utf16_be -max_total_time=300 -print_final_stats=1
build/Fuzz/fuzz_utf16_le -max_total_time=300 -print_final_stats=1
build/Fuzz/fuzz_utf32_be -max_total_time=300 -print_final_stats=1
build/Fuzz/fuzz_utf32_le -max_total_time=300 -print_final_stats=1
```

### Fuzz Test Features

The fuzz tests include:
- **UTF-8**: Invalid sequences, overlong encodings, truncated multi-byte sequences
- **UTF-16 BE/LE**: Invalid surrogate pairs, truncated surrogates, endianness handling
- **UTF-32 BE/LE**: Invalid code points, surrogate range values, endianness handling
- **All encodings**: Boundary conditions, consistency checks, round-trip validation

Built-in sanitizers detect:
- Buffer overflows and underflows
- Use-after-free and double-free
- Memory leaks
- Undefined behavior
- Integer overflows
- Null pointer dereferences

## Resources

- [Perf Tutorial](https://perf.wiki.kernel.org/index.php/Tutorial)
- [Flame Graphs](http://www.brendangregg.com/flamegraphs.html)
- [Linux Performance](http://www.brendangregg.com/linuxperf.html)
- [Perf Examples](http://www.brendangregg.com/perf.html)
- [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)
- [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)
- [ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html)
- [LibFuzzer](https://llvm.org/docs/LibFuzzer.html)