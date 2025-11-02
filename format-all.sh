#!/bin/bash

# Script to format all C++ files in the repository
# Use this to ensure the entire codebase is consistently formatted

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Formatting all C++ files in the repository...${NC}"

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo -e "${RED}Error: clang-format not found. Please install it:${NC}"
    echo "  Ubuntu/Debian: sudo apt install clang-format"
    echo "  macOS: brew install clang-format"
    exit 1
fi

# Find all C++ files, excluding build directories
CPP_FILES=$(find . -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.cc" -o -name "*.cxx" | grep -v build | grep -v ".git")

if [ -z "$CPP_FILES" ]; then
    echo -e "${YELLOW}No C++ files found.${NC}"
    exit 0
fi

echo -e "${YELLOW}Found C++ files:${NC}"
echo "$CPP_FILES" | while read file; do
    echo "  - $file"
done

# Count files before formatting
TOTAL_FILES=$(echo "$CPP_FILES" | wc -l)
echo -e "${YELLOW}Formatting $TOTAL_FILES files...${NC}"

# Format all files
echo "$CPP_FILES" | xargs clang-format -i

# Check if any files were changed
CHANGED_FILES=$(git diff --name-only)
if [ -n "$CHANGED_FILES" ]; then
    echo -e "${GREEN}✓ Formatting completed. The following files were changed:${NC}"
    echo "$CHANGED_FILES" | while read file; do
        echo "  - $file"
    done
    echo -e "${YELLOW}Don't forget to commit these changes!${NC}"
else
    echo -e "${GREEN}✓ All files were already properly formatted.${NC}"
fi