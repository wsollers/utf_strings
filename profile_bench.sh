#!/bin/bash

# UTF Strings Benchmark Profiler Script
# This script builds the release version and runs performance profiling

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

echo -e "${BLUE}=== UTF Strings Benchmark Profiler ===${NC}"
echo -e "${YELLOW}Project directory: $SCRIPT_DIR${NC}"

# Check if perf is available and install if needed
if ! command -v perf &> /dev/null; then
    echo -e "${YELLOW}perf is not installed. Attempting to install...${NC}"
    
    # Ask user for permission first
    echo -e "${BLUE}This script will attempt to install perf using sudo.${NC}"
    echo -e "${YELLOW}Alternatively, you can run './run_bench.sh' for basic benchmarking without profiling.${NC}"
    echo ""
    read -p "Continue with perf installation? (y/N): " -r
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${YELLOW}Installation cancelled.${NC}"
        echo -e "${BLUE}Running basic benchmark instead...${NC}"
        echo ""
        exec ./run_bench.sh
    fi
    
    # Detect the distribution and install perf accordingly
    if command -v apt &> /dev/null; then
        # Debian/Ubuntu based systems
        echo -e "${BLUE}Installing perf on Debian/Ubuntu system...${NC}"
        
        # Update package list
        echo -e "${YELLOW}Updating package list...${NC}"
        if ! sudo apt update; then
            echo -e "${RED}Failed to update package list. Continuing anyway...${NC}"
        fi
        
        # Try multiple installation strategies
        KERNEL_VERSION=$(uname -r)
        INSTALL_SUCCESS=false
        
        echo -e "${YELLOW}Trying to install linux-tools-$KERNEL_VERSION...${NC}"
        if sudo apt install -y "linux-tools-$KERNEL_VERSION" 2>/dev/null; then
            echo -e "${GREEN}Successfully installed linux-tools-$KERNEL_VERSION${NC}"
            INSTALL_SUCCESS=true
        else
            echo -e "${YELLOW}Kernel-specific version failed, trying linux-tools-generic...${NC}"
            if sudo apt install -y linux-tools-generic 2>/dev/null; then
                echo -e "${GREEN}Successfully installed linux-tools-generic${NC}"
                INSTALL_SUCCESS=true
            else
                echo -e "${YELLOW}Generic version failed, trying to fix dependencies...${NC}"
                # Try to fix broken dependencies
                sudo apt --fix-broken install -y 2>/dev/null || true
                
                # Try once more with generic
                if sudo apt install -y linux-tools-generic 2>/dev/null; then
                    echo -e "${GREEN}Successfully installed linux-tools-generic after fixing dependencies${NC}"
                    INSTALL_SUCCESS=true
                fi
            fi
        fi
        
        if [ "$INSTALL_SUCCESS" = false ]; then
            echo -e "${RED}Automatic installation failed. Please try manually:${NC}"
            echo -e "${YELLOW}  sudo apt update${NC}"
            echo -e "${YELLOW}  sudo apt install linux-tools-\$(uname -r)${NC}"
            echo -e "${YELLOW}  or sudo apt install linux-tools-generic${NC}"
            echo -e "${YELLOW}  or sudo apt --fix-broken install${NC}"
            echo ""
            echo -e "${BLUE}Common issues and solutions:${NC}"
            echo -e "${YELLOW}  - If you see dependency conflicts, try: sudo apt autoremove && sudo apt update${NC}"
            echo -e "${YELLOW}  - On WSL or containers: perf may not work properly${NC}"
            echo -e "${YELLOW}  - Try rebooting if kernel tools don't match running kernel${NC}"
            exit 1
        fi
        
    elif command -v yum &> /dev/null; then
        # Red Hat/CentOS based systems
        echo -e "${BLUE}Installing perf on Red Hat/CentOS system...${NC}"
        if sudo yum install -y perf; then
            echo -e "${GREEN}Installed perf${NC}"
        else
            echo -e "${RED}Failed to install perf. Please install manually: sudo yum install perf${NC}"
            exit 1
        fi
        
    elif command -v dnf &> /dev/null; then
        # Fedora based systems
        echo -e "${BLUE}Installing perf on Fedora system...${NC}"
        if sudo dnf install -y perf; then
            echo -e "${GREEN}Installed perf${NC}"
        else
            echo -e "${RED}Failed to install perf. Please install manually: sudo dnf install perf${NC}"
            exit 1
        fi
        
    elif command -v pacman &> /dev/null; then
        # Arch Linux based systems
        echo -e "${BLUE}Installing perf on Arch Linux system...${NC}"
        if sudo pacman -S --noconfirm perf; then
            echo -e "${GREEN}Installed perf${NC}"
        else
            echo -e "${RED}Failed to install perf. Please install manually: sudo pacman -S perf${NC}"
            exit 1
        fi
        
    else
        echo -e "${RED}Unsupported package manager. Please install perf manually:${NC}"
        echo -e "${YELLOW}Debian/Ubuntu: sudo apt install linux-tools-generic${NC}"
        echo -e "${YELLOW}Red Hat/CentOS: sudo yum install perf${NC}"
        echo -e "${YELLOW}Fedora: sudo dnf install perf${NC}"
        echo -e "${YELLOW}Arch: sudo pacman -S perf${NC}"
        exit 1
    fi
    
    # Verify installation
    if ! command -v perf &> /dev/null; then
        echo -e "${RED}perf installation completed but perf is still not available${NC}"
        echo -e "${YELLOW}This might happen if:${NC}"
        echo -e "${YELLOW}  - You're running in WSL or a container${NC}"
        echo -e "${YELLOW}  - The kernel version doesn't match installed tools${NC}"
        echo -e "${YELLOW}  - You need to restart your shell${NC}"
        echo ""
        echo -e "${YELLOW}Try running: hash -r${NC}"
        
        # Try to refresh the command hash
        hash -r 2>/dev/null || true
        
        if ! command -v perf &> /dev/null; then
            echo -e "${RED}perf is still not available. Please check your system configuration.${NC}"
            exit 1
        fi
    fi
    
    echo -e "${GREEN}perf installed and verified successfully!${NC}"
else
    echo -e "${GREEN}perf is already available${NC}"
fi

# Set the correct perf path (handle kernel version mismatches)
PERF_CMD="perf"

# Test if system perf works, if not find alternative
if ! perf --version &>/dev/null; then
    echo -e "${YELLOW}System perf has kernel version mismatch, finding alternative...${NC}"
    # Try to find perf in linux-tools directories
    PERF_BINARY=$(find /usr/lib/linux-tools* -name "perf" 2>/dev/null | head -1)
    if [ -n "$PERF_BINARY" ] && [ -x "$PERF_BINARY" ]; then
        PERF_CMD="$PERF_BINARY"
        echo -e "${YELLOW}Using perf from: $PERF_CMD${NC}"
        # Test the alternative perf
        if ! $PERF_CMD --version &>/dev/null; then
            echo -e "${RED}Alternative perf binary also not working${NC}"
            exit 1
        fi
    else
        echo -e "${RED}No working perf binary found${NC}"
        exit 1
    fi
fi

# Check if conan dependencies are built
if [ ! -f "build/conan_toolchain.cmake" ]; then
    echo -e "${RED}Error: Conan dependencies not found. Run bootstrap scripts first.${NC}"
    echo -e "${YELLOW}Run: ./bootstrap_build.sh${NC}"
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

echo -e "${BLUE}=== Running Benchmark with perf profiling ===${NC}"
echo -e "${YELLOW}This will create perf.data in the current directory${NC}"

# Run perf profiling
echo -e "${GREEN}Recording performance data...${NC}"
$PERF_CMD record -g --call-graph=dwarf -o perf.data "./$BENCH_EXECUTABLE"

# Check if perf.data was created
if [ -f "perf.data" ]; then
    echo -e "${GREEN}Performance data recorded successfully!${NC}"
    
    # Show perf report
    echo -e "${BLUE}=== Performance Report ===${NC}"
    echo -e "${YELLOW}Generating performance report...${NC}"
    $PERF_CMD report --stdio | head -50
    
    echo ""
    echo -e "${GREEN}=== Profiling Complete ===${NC}"
    echo -e "${YELLOW}Full report available with: $PERF_CMD report${NC}"
    echo -e "${YELLOW}Performance data saved as: perf.data${NC}"
    
    # Optional: Generate flame graph if tools are available
    if [ -x "$PERF_CMD" ]; then
        echo -e "${BLUE}=== Generating Flame Graph Data ===${NC}"
        $PERF_CMD script > perf.script
        echo -e "${YELLOW}Flame graph data saved as: perf.script${NC}"
        echo -e "${YELLOW}To generate flame graph:${NC}"
        echo -e "  git clone https://github.com/brendangregg/FlameGraph.git"
        echo -e "  cat perf.script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl > flamegraph.svg"
    fi
    
    # Show quick stats
    echo -e "${BLUE}=== Quick Performance Stats ===${NC}"
    $PERF_CMD stat "./$BENCH_EXECUTABLE" 2>&1 | grep -E "(task-clock|cycles|instructions|branches|branch-misses)" || true
    
else
    echo -e "${RED}Error: perf.data was not created${NC}"
    exit 1
fi

echo -e "${GREEN}=== Script completed successfully ===${NC}"