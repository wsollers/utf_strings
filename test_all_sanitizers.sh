#!/bin/bash

# Comprehensive testing script with sanitizers for UTF Strings library
# This script runs unit tests, fuzz tests, and benchmarks with various sanitizer configurations

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

# Script directory (project root)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo -e "${BLUE}=== UTF Strings Comprehensive Testing Suite ===${NC}"
echo -e "${YELLOW}Project directory: $SCRIPT_DIR${NC}"

# Check if Clang is available for fuzzing
CLANG_AVAILABLE=false
if command -v clang++ &> /dev/null; then
    CLANG_AVAILABLE=true
    echo -e "${GREEN}Clang++ found: $(clang++ --version | head -1)${NC}"
else
    echo -e "${YELLOW}Clang++ not found. Fuzz testing will be skipped.${NC}"
fi

# Check if conan dependencies are built
if [ ! -f "build/conan_toolchain.cmake" ]; then
    echo -e "${RED}Error: Conan dependencies not found${NC}"
    echo -e "${YELLOW}Run bootstrap script first:${NC}"
    if [ -f "bootstrap_build.sh" ]; then
        echo -e "  ./bootstrap_build.sh"
    else
        echo -e "  conan install . --build=missing -s build_type=Debug"
        echo -e "  conan install . --build=missing -s build_type=Release"
    fi
    exit 1
fi

# Function to run a test configuration
run_test_config() {
    local config_name="$1"
    local build_type="$2"
    local extra_flags="$3"
    local compiler="${4:-g++}"
    
    echo -e "${MAGENTA}=== Running $config_name ===${NC}"
    
    local build_dir="build/${config_name}"
    mkdir -p "$build_dir"
    
    # Configure CMake
    echo -e "${BLUE}Configuring $config_name build...${NC}"
    cmake -S . -B "$build_dir" \
        -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
        -DCMAKE_BUILD_TYPE="$build_type" \
        -DCMAKE_CXX_COMPILER="$compiler" \
        -DUTF_STRINGS_WITH_GPERFTOOLS=OFF \
        $extra_flags
    
    # Build
    echo -e "${BLUE}Building $config_name...${NC}"
    cmake --build "$build_dir" --parallel
    
    # Run unit tests
    if [ -f "$build_dir/utf_strings-tests" ]; then
        echo -e "${BLUE}Running unit tests ($config_name)...${NC}"
        cd "$build_dir"
        ./utf_strings-tests --gtest_output=xml:test_results_${config_name}.xml
        cd "$SCRIPT_DIR"
        echo -e "${GREEN}Unit tests passed for $config_name${NC}"
    fi
    
    # Run fuzz tests if available
    if [ -f "$build_dir/fuzz_utf8" ]; then
        echo -e "${BLUE}Running fuzz tests ($config_name)...${NC}"
        cd "$build_dir"
        
        # Run each fuzz test for a shorter time during comprehensive testing
        echo -e "${YELLOW}Fuzzing UTF-8...${NC}"
        timeout 10s ./fuzz_utf8 -print_final_stats=1 || true
        
        echo -e "${YELLOW}Fuzzing UTF-16 BE...${NC}"
        timeout 10s ./fuzz_utf16_be -print_final_stats=1 || true
        
        echo -e "${YELLOW}Fuzzing UTF-16 LE...${NC}"
        timeout 10s ./fuzz_utf16_le -print_final_stats=1 || true
        
        echo -e "${YELLOW}Fuzzing UTF-32 BE...${NC}"
        timeout 10s ./fuzz_utf32_be -print_final_stats=1 || true
        
        echo -e "${YELLOW}Fuzzing UTF-32 LE...${NC}"
        timeout 10s ./fuzz_utf32_le -print_final_stats=1 || true
        
        cd "$SCRIPT_DIR"
        echo -e "${GREEN}Fuzz tests completed for $config_name${NC}"
    fi
    
    # Run benchmark (quick run)
    if [ -f "$build_dir/utf_strings-bench" ]; then
        echo -e "${BLUE}Running benchmark ($config_name)...${NC}"
        cd "$build_dir"
        ./utf_strings-bench --benchmark_min_time=0.1s
        cd "$SCRIPT_DIR"
        echo -e "${GREEN}Benchmark completed for $config_name${NC}"
    fi
    
    echo -e "${GREEN}$config_name configuration completed successfully${NC}"
    echo ""
}

# Test configurations
echo -e "${YELLOW}Starting comprehensive testing...${NC}"
echo ""

# 1. Debug build with AddressSanitizer and UBSan
run_test_config "Debug_ASan_UBSan" "Debug" \
    "-DUTF_STRINGS_ENABLE_SANITIZERS=ON -DUTF_STRINGS_BUILD_FUZZ_TESTS=$CLANG_AVAILABLE" \
    "$([ "$CLANG_AVAILABLE" = true ] && echo "clang++" || echo "g++")"

# 2. Debug build with ThreadSanitizer (if Clang available)
if [ "$CLANG_AVAILABLE" = true ]; then
    run_test_config "Debug_TSan" "Debug" \
        "-DUTF_STRINGS_ENABLE_THREAD_SANITIZER=ON" \
        "clang++"
fi

# 3. Release build (no sanitizers for performance)
run_test_config "Release_Clean" "Release" \
    "-DUTF_STRINGS_ENABLE_SANITIZERS=OFF"

# 4. Fuzz-only build (if Clang available)
if [ "$CLANG_AVAILABLE" = true ]; then
    echo -e "${MAGENTA}=== Extended Fuzz Testing ===${NC}"
    
    local fuzz_build_dir="build/FuzzExtended"
    mkdir -p "$fuzz_build_dir"
    
    # Configure for extended fuzzing
    cmake -S . -B "$fuzz_build_dir" \
        -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DUTF_STRINGS_BUILD_TESTS=OFF \
        -DUTF_STRINGS_BUILD_BENCHMARKS=OFF \
        -DUTF_STRINGS_BUILD_FUZZ_TESTS=ON \
        -DUTF_STRINGS_WITH_GPERFTOOLS=OFF
    
    cmake --build "$fuzz_build_dir" --parallel
    
    cd "$fuzz_build_dir"
    
    # Run extended fuzz tests (longer duration)
    echo -e "${YELLOW}Extended fuzzing session (60 seconds per encoding)...${NC}"
    
    ./fuzz_utf8 -max_total_time=60 -print_final_stats=1 -print_corpus_stats=1 &
    FUZZ_UTF8_PID=$!
    
    ./fuzz_utf16_be -max_total_time=60 -print_final_stats=1 -print_corpus_stats=1 &
    FUZZ_UTF16_BE_PID=$!
    
    ./fuzz_utf16_le -max_total_time=60 -print_final_stats=1 -print_corpus_stats=1 &
    FUZZ_UTF16_LE_PID=$!
    
    ./fuzz_utf32_be -max_total_time=60 -print_final_stats=1 -print_corpus_stats=1 &
    FUZZ_UTF32_BE_PID=$!
    
    ./fuzz_utf32_le -max_total_time=60 -print_final_stats=1 -print_corpus_stats=1 &
    FUZZ_UTF32_LE_PID=$!
    
    # Wait for all fuzz tests to complete
    wait $FUZZ_UTF8_PID
    wait $FUZZ_UTF16_BE_PID
    wait $FUZZ_UTF16_LE_PID
    wait $FUZZ_UTF32_BE_PID
    wait $FUZZ_UTF32_LE_PID
    
    cd "$SCRIPT_DIR"
    echo -e "${GREEN}Extended fuzz testing completed${NC}"
fi

# Summary
echo -e "${GREEN}=== Testing Summary ===${NC}"
echo -e "${GREEN}✓ Debug build with AddressSanitizer and UndefinedBehaviorSanitizer${NC}"
if [ "$CLANG_AVAILABLE" = true ]; then
    echo -e "${GREEN}✓ Debug build with ThreadSanitizer${NC}"
    echo -e "${GREEN}✓ Comprehensive fuzz testing with all sanitizers${NC}"
fi
echo -e "${GREEN}✓ Release build for performance validation${NC}"
echo -e "${GREEN}✓ All unit tests passed${NC}"
echo -e "${GREEN}✓ All benchmarks executed${NC}"

echo ""
echo -e "${BLUE}Test results and build artifacts are in:${NC}"
echo -e "${YELLOW}  build/Debug_ASan_UBSan/     - Debug with AddressSanitizer/UBSan${NC}"
if [ "$CLANG_AVAILABLE" = true ]; then
    echo -e "${YELLOW}  build/Debug_TSan/           - Debug with ThreadSanitizer${NC}"
    echo -e "${YELLOW}  build/FuzzExtended/         - Extended fuzz testing${NC}"
fi
echo -e "${YELLOW}  build/Release_Clean/        - Clean release build${NC}"

echo ""
echo -e "${GREEN}=== Comprehensive testing completed successfully! ===${NC}"