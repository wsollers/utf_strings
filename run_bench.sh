#!/bin/bash

# UTF Strings Simple Benchmark Script
# This script builds and runs benchmarks without profiling dependencies

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory (project root)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo -e "${BLUE}=== UTF Strings Benchmark Runner ===${NC}"
echo -e "${YELLOW}Project directory: $SCRIPT_DIR${NC}"

# Check if conan dependencies are built
if [ ! -f "build/conan_toolchain.cmake" ]; then
    echo -e "${RED}Error: Conan dependencies not found${NC}"
    echo -e "${YELLOW}Run bootstrap script first:${NC}"
    if [ -f "bootstrap_build.sh" ]; then
        echo -e "  ./bootstrap_build.sh"
    else
        echo -e "  conan install . --build=missing -s build_type=Release"
    fi
    exit 1
fi

# Configure CMake for Release build
echo -e "${BLUE}=== Configuring CMake (Release) ===${NC}"
cmake -S . -B build/Release \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DUTF_STRINGS_WITH_GPERFTOOLS=ON

# Build the project
echo -e "${BLUE}=== Building Release ===${NC}"
cmake --build build/Release

# Check if benchmark executable exists
BENCH_EXECUTABLE="build/Release/utf_strings-bench"
if [ ! -f "$BENCH_EXECUTABLE" ]; then
    echo -e "${RED}Error: Benchmark executable not found at $BENCH_EXECUTABLE${NC}"
    exit 1
fi

echo -e "${BLUE}=== Running Benchmark ===${NC}"
"./$BENCH_EXECUTABLE"

echo -e "${GREEN}=== Benchmark completed successfully ===${NC}"
echo -e "${YELLOW}For profiling, install perf and use: ./profile_bench.sh${NC}"