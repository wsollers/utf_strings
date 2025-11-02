#!/bin/bash

# Simple fuzz testing script for UTF Strings library
# Builds and runs fuzz tests with sanitizers

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

echo -e "${BLUE}=== UTF Strings Fuzz Testing ===${NC}"

# Check if Clang is available
if ! command -v clang++ &> /dev/null; then
    echo -e "${RED}Error: Clang++ is required for fuzz testing${NC}"
    echo -e "${YELLOW}Install with: sudo apt install clang${NC}"
    exit 1
fi

echo -e "${GREEN}Using Clang: $(clang++ --version | head -1)${NC}"

# Check dependencies
if [ ! -f "build/conan_toolchain.cmake" ]; then
    echo -e "${RED}Error: Conan dependencies not found${NC}"
    echo -e "${YELLOW}Run bootstrap script first${NC}"
    exit 1
fi

# Parse command line arguments
DURATION=60
ENCODING="all"

while [[ $# -gt 0 ]]; do
    case $1 in
        -t|--time)
            DURATION="$2"
            shift 2
            ;;
        -e|--encoding)
            ENCODING="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -t, --time SECONDS    Duration to run each fuzz test (default: 60)"
            echo "  -e, --encoding ENC    Encoding to test (utf8, utf16_be, utf16_le, utf32_be, utf32_le, all)"
            echo "  -h, --help           Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

echo -e "${YELLOW}Fuzz test duration: ${DURATION} seconds per encoding${NC}"
echo -e "${YELLOW}Target encoding: ${ENCODING}${NC}"

# Build directory
BUILD_DIR="build/Fuzz"
mkdir -p "$BUILD_DIR"

# Configure CMake for fuzzing
echo -e "${BLUE}=== Configuring fuzz build ===${NC}"
cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DUTF_STRINGS_BUILD_TESTS=OFF \
    -DUTF_STRINGS_BUILD_BENCHMARKS=OFF \
    -DUTF_STRINGS_BUILD_FUZZ_TESTS=ON \
    -DUTF_STRINGS_WITH_GPERFTOOLS=OFF

# Build fuzz targets
echo -e "${BLUE}=== Building fuzz targets ===${NC}"
cmake --build "$BUILD_DIR" --parallel

cd "$BUILD_DIR"

# Function to run a single fuzz test
run_fuzz_test() {
    local name="$1"
    local binary="$2"
    
    if [ ! -f "$binary" ]; then
        echo -e "${RED}Fuzz binary not found: $binary${NC}"
        return 1
    fi
    
    echo -e "${YELLOW}=== Fuzzing $name for ${DURATION} seconds ===${NC}"
    
    # Create corpus directory
    mkdir -p "corpus_$name"
    
    # Run the fuzz test
    timeout "${DURATION}s" "./$binary" \
        "corpus_$name" \
        -print_final_stats=1 \
        -print_corpus_stats=1 \
        -print_coverage=1 \
        -max_total_time="$DURATION" \
        || {
            local exit_code=$?
            if [ $exit_code -eq 124 ]; then
                echo -e "${GREEN}Fuzz test completed (timeout reached)${NC}"
            else
                echo -e "${RED}Fuzz test failed with exit code $exit_code${NC}"
                return $exit_code
            fi
        }
    
    echo -e "${GREEN}$name fuzz test completed${NC}"
    echo ""
}

# Run fuzz tests based on encoding selection
case $ENCODING in
    "utf8")
        run_fuzz_test "UTF-8" "fuzz_utf8"
        ;;
    "utf16_be")
        run_fuzz_test "UTF-16 BE" "fuzz_utf16_be"
        ;;
    "utf16_le")
        run_fuzz_test "UTF-16 LE" "fuzz_utf16_le"
        ;;
    "utf32_be")
        run_fuzz_test "UTF-32 BE" "fuzz_utf32_be"
        ;;
    "utf32_le")
        run_fuzz_test "UTF-32 LE" "fuzz_utf32_le"
        ;;
    "all")
        echo -e "${BLUE}=== Running all fuzz tests ===${NC}"
        run_fuzz_test "UTF-8" "fuzz_utf8"
        run_fuzz_test "UTF-16 BE" "fuzz_utf16_be"
        run_fuzz_test "UTF-16 LE" "fuzz_utf16_le"
        run_fuzz_test "UTF-32 BE" "fuzz_utf32_be"
        run_fuzz_test "UTF-32 LE" "fuzz_utf32_le"
        ;;
    *)
        echo -e "${RED}Invalid encoding: $ENCODING${NC}"
        echo -e "${YELLOW}Valid options: utf8, utf16_be, utf16_le, utf32_be, utf32_le, all${NC}"
        exit 1
        ;;
esac

cd "$SCRIPT_DIR"

echo -e "${GREEN}=== Fuzz Testing Summary ===${NC}"
echo -e "${GREEN}✓ All requested fuzz tests completed${NC}"
echo -e "${YELLOW}Corpus and artifacts saved in: $BUILD_DIR/corpus_*${NC}"
echo -e "${YELLOW}To reproduce issues, run: $BUILD_DIR/fuzz_* <crash_file>${NC}"

echo -e "${BLUE}=== Fuzz testing completed successfully! ===${NC}"