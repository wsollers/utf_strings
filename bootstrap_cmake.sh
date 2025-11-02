#!/usr/bin/env bash
set -euo pipefail

# ------------------------------------------------------------
# UTF Strings Cross-Platform Bootstrap Script
#  - Detects OS and compiler environment once
#  - Installs required development tools
#  - Configures cross-platform builds (Linux GCC/Clang, Windows MSVC)
#  - Sets up all targets: tests, profiling, fuzzing, benchmarks
#
# Usage:
#   ./bootstrap_cmake.sh                 # Auto-detect and full setup
#   ./bootstrap_cmake.sh --compiler gcc  # Force GCC on Linux
#   ./bootstrap_cmake.sh --compiler clang # Force Clang on Linux
#   ./bootstrap_cmake.sh --compiler msvc # Force MSVC on Windows
#   ./bootstrap_cmake.sh --config Debug  # Only Debug build
#   ./bootstrap_cmake.sh --config Release # Only Release build
#   ./bootstrap_cmake.sh --skip-tests    # Skip running tests
#   ./bootstrap_cmake.sh --skip-bench    # Skip benchmarks
#   ./bootstrap_cmake.sh --clean         # Clean build directory first
# ------------------------------------------------------------

# Colors for output (cross-platform)
if [[ "${TERM:-}" != "dumb" ]] && [[ -t 1 ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    MAGENTA='\033[0;35m'
    CYAN='\033[0;36m'
    NC='\033[0m'
else
    RED='' GREEN='' YELLOW='' BLUE='' MAGENTA='' CYAN='' NC=''
fi

log_info() { echo -e "${BLUE}ℹ️  $1${NC}"; }
log_success() { echo -e "${GREEN}✅ $1${NC}"; }
log_warning() { echo -e "${YELLOW}⚠️  $1${NC}"; }
log_error() { echo -e "${RED}❌ $1${NC}"; }
log_header() { echo -e "${MAGENTA}🚀 $1${NC}"; }

# ------------------------------------------------------------
# Cross-Platform Environment Detection
# ------------------------------------------------------------

detect_platform() {
    local os_type=""
    local arch=""
    
    # Detect OS
    case "$OSTYPE" in
        linux-gnu*) os_type="Linux" ;;
        darwin*) os_type="macOS" ;;
        msys*|cygwin*|mingw*) os_type="Windows" ;;
        *) 
            if [[ -n "${OS:-}" && "$OS" == "Windows_NT" ]]; then
                os_type="Windows"
            else
                os_type="Unknown"
            fi
            ;;
    esac
    
    # Detect architecture
    arch=$(uname -m 2>/dev/null || echo "unknown")
    case "$arch" in
        x86_64|amd64) arch="x64" ;;
        i386|i686) arch="x86" ;;
        aarch64|arm64) arch="arm64" ;;
        *) arch="unknown" ;;
    esac
    
    echo "$os_type-$arch"
}

detect_available_compilers() {
    local platform="$1"
    local compilers=()
    
    case "$platform" in
        Linux-*)
            command -v gcc >/dev/null 2>&1 && compilers+=("gcc")
            command -v clang >/dev/null 2>&1 && compilers+=("clang")
            ;;
        macOS-*)
            command -v clang >/dev/null 2>&1 && compilers+=("clang")
            command -v gcc >/dev/null 2>&1 && compilers+=("gcc")
            ;;
        Windows-*)
            # Check for MSVC
            if command -v cl >/dev/null 2>&1 || [[ -n "${VCINSTALLDIR:-}" ]]; then
                compilers+=("msvc")
            fi
            # Check for MinGW/MSYS2 GCC
            command -v gcc >/dev/null 2>&1 && compilers+=("gcc")
            command -v clang >/dev/null 2>&1 && compilers+=("clang")
            ;;
    esac
    
    printf '%s\n' "${compilers[@]}"
}

setup_compiler_environment() {
    local platform="$1"
    local compiler="$2"
    
    case "$platform" in
        Linux-*|macOS-*)
            case "$compiler" in
                gcc)
                    export CC=gcc
                    export CXX=g++
                    ;;
                clang)
                    export CC=clang
                    export CXX=clang++
                    ;;
            esac
            ;;
        Windows-*)
            case "$compiler" in
                msvc)
                    # MSVC environment should be set up by vcvarsall.bat
                    if [[ -z "${VCINSTALLDIR:-}" ]]; then
                        log_warning "MSVC environment not detected. Please run from Developer Command Prompt or run vcvarsall.bat"
                    fi
                    ;;
                gcc)
                    export CC=gcc
                    export CXX=g++
                    ;;
                clang)
                    export CC=clang
                    export CXX=clang++
                    ;;
            esac
            ;;
    esac
}

# ------------------------------------------------------------
# Tool Installation Functions
# ------------------------------------------------------------

check_and_install_conan() {
    if command -v conan >/dev/null 2>&1; then
        log_success "Conan found: $(conan --version)"
    else
        log_info "Conan not found — installing via pip..."
        if command -v pip3 >/dev/null 2>&1; then
            pip3 install --user conan
        elif command -v pip >/dev/null 2>&1; then
            pip install --user conan
        else
            log_error "pip not found. Please install Python 3.8+ with pip first."
            exit 1
        fi
        
        # Add to PATH
        case "$PLATFORM" in
            Windows-*) 
                export PATH="$HOME/AppData/Roaming/Python/Scripts:$PATH"
                ;;
            *)
                export PATH="$HOME/.local/bin:$PATH"
                ;;
        esac
        
        if ! command -v conan >/dev/null 2>&1; then
            log_error "Conan install failed or not in PATH."
            exit 1
        fi
        
        log_success "Conan installed successfully: $(conan --version)"
    fi

    # Ensure default profile exists
    if ! conan profile list 2>/dev/null | grep -qx "default"; then
        log_info "Creating Conan default profile..."
        conan profile detect --force
    fi
}

check_and_install_clang_format() {
    if command -v clang-format >/dev/null 2>&1; then
        log_success "clang-format found: $(clang-format --version | head -n1)"
        return
    fi
    
    log_info "clang-format not found — installing..."
    
    case "$PLATFORM" in
        Linux-*)
            if command -v apt >/dev/null 2>&1; then
                sudo apt update && sudo apt install -y clang-format
            elif command -v yum >/dev/null 2>&1; then
                sudo yum install -y clang-tools-extra
            elif command -v dnf >/dev/null 2>&1; then
                sudo dnf install -y clang-tools-extra
            elif command -v pacman >/dev/null 2>&1; then
                sudo pacman -S --noconfirm clang
            else
                log_error "Unable to determine package manager. Please install clang-format manually."
                exit 1
            fi
            ;;
        macOS-*)
            if command -v brew >/dev/null 2>&1; then
                brew install clang-format
            else
                log_error "Homebrew not found. Please install Homebrew or clang-format manually."
                exit 1
            fi
            ;;
        Windows-*)
            log_error "Please install clang-format manually on Windows:"
            echo "  ➤ Download LLVM from: https://releases.llvm.org/download.html"
            echo "  ➤ Or use Scoop: scoop install llvm"
            echo "  ➤ Or use Chocolatey: choco install llvm"
            echo "  ➤ Or use vcpkg: vcpkg install llvm-tools"
            exit 1
            ;;
    esac
    
    if command -v clang-format >/dev/null 2>&1; then
        log_success "clang-format installed: $(clang-format --version | head -n1)"
    else
        log_error "clang-format installation failed."
        exit 1
    fi
}

check_cmake() {
    if ! command -v cmake >/dev/null 2>&1; then
        log_error "CMake not found. Please install CMake 3.25+ first."
        exit 1
    fi
    
    local cmake_version
    cmake_version=$(cmake --version | head -n1 | grep -o '[0-9]\+\.[0-9]\+\.[0-9]\+')
    local required_version="3.25.0"
    
    if ! printf '%s\n%s\n' "$required_version" "$cmake_version" | sort -V -C; then
        log_error "CMake version $cmake_version is too old. Required: >= $required_version"
        exit 1
    fi
    
    log_success "CMake found: $cmake_version (>= $required_version)"
}

check_compiler() {
    local compiler="$1"
    
    case "$compiler" in
        gcc)
            if ! command -v g++ >/dev/null 2>&1; then
                log_error "GCC not found. Please install GCC 13+ first."
                exit 1
            fi
            local gcc_version
            gcc_version=$(g++ --version | head -n1 | grep -o '[0-9]\+\.[0-9]\+' | head -n1)
            log_success "GCC found: $gcc_version"
            ;;
        clang)
            if ! command -v clang++ >/dev/null 2>&1; then
                log_error "Clang not found. Please install Clang 16+ first."
                exit 1
            fi
            local clang_version
            clang_version=$(clang++ --version | head -n1 | grep -o '[0-9]\+\.[0-9]\+' | head -n1)
            log_success "Clang found: $clang_version"
            ;;
        msvc)
            if ! command -v cl >/dev/null 2>&1 && [[ -z "${VCINSTALLDIR:-}" ]]; then
                log_error "MSVC not found. Please run from Developer Command Prompt or install Visual Studio 2022."
                exit 1
            fi
            if command -v cl >/dev/null 2>&1; then
                local msvc_version
                msvc_version=$(cl 2>&1 | head -n1 | grep -o '[0-9]\+\.[0-9]\+\.[0-9]\+')
                log_success "MSVC found: $msvc_version"
            else
                log_success "MSVC environment detected"
            fi
            ;;
    esac
}

# ------------------------------------------------------------
# Cross-Platform Build Functions
# ------------------------------------------------------------

get_build_dir() {
    local config="$1"
    case "$PLATFORM" in
        Windows-*)
            echo "build/$config"
            ;;
        *)
            if [[ "$config" == "Release" ]]; then
                echo "build/build"  # Keep existing convention for Release
            else
                echo "build/$config"
            fi
            ;;
    esac
}

get_executable_extension() {
    case "$PLATFORM" in
        Windows-*) echo ".exe" ;;
        *) echo "" ;;
    esac
}

configure_cmake() {
    local config="$1"
    log_info "Configuring CMake ($config)..."
    
    local cmake_args=()
    
    # Add compiler-specific options
    case "$COMPILER" in
        clang)
            if [[ "$config" == "Debug" && "${ENABLE_FUZZ:-}" == "1" ]]; then
                cmake_args+=("-DUTF_STRINGS_BUILD_FUZZ_TESTS=ON")
                log_info "Fuzz testing enabled with Clang"
            fi
            ;;
        msvc)
            # MSVC-specific configurations
            cmake_args+=("-DCMAKE_GENERATOR_PLATFORM=x64")
            ;;
    esac
    
    # Configure using presets
    case "$config" in
        Debug)
            cmake --preset conan-debug "${cmake_args[@]}"
            ;;
        Release)
            cmake --preset conan-release "${cmake_args[@]}"
            ;;
    esac
}

build_cmake() {
    local config="$1"
    log_info "Building ($config)..."
    
    case "$config" in
        Debug)
            cmake --build --preset conan-debug --parallel
            ;;
        Release)
            cmake --build --preset conan-release --parallel
            ;;
    esac
}

run_tests() {
    local config="$1"
    local build_dir
    build_dir=$(get_build_dir "$config")
    local exe_ext
    exe_ext=$(get_executable_extension)
    
    log_info "Running tests ($config)..."
    
    local test_binary="$build_dir/utf_strings-tests$exe_ext"
    if [[ -x "$test_binary" ]]; then
        "$test_binary"
        return $?
    else
        log_warning "Test binary not found: $test_binary"
        return 1
    fi
}

run_benchmarks() {
    local build_dir
    build_dir=$(get_build_dir "Release")
    local exe_ext
    exe_ext=$(get_executable_extension)
    
    log_info "Running benchmarks (Release)..."
    
    local bench_binary="$build_dir/utf_strings-bench$exe_ext"
    if [[ -x "$bench_binary" ]]; then
        "$bench_binary" --benchmark_min_time=0.1s --benchmark_time_unit=us
        return $?
    else
        log_warning "Benchmark binary not found: $bench_binary"
        return 1
    fi
}

# ------------------------------------------------------------
# Command Line Argument Parsing
# ------------------------------------------------------------

CONFIGS=("Debug" "Release")
SKIP_TESTS=0
SKIP_BENCH=0
DO_CLEAN=0
COMPILER=""
ENABLE_FUZZ=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        --config) CONFIGS=("$2"); shift 2;;
        --compiler) COMPILER="$2"; shift 2;;
        --skip-tests) SKIP_TESTS=1; shift;;
        --skip-bench) SKIP_BENCH=1; shift;;
        --clean) DO_CLEAN=1; shift;;
        --enable-fuzz) ENABLE_FUZZ=1; shift;;
        -h|--help)
            grep '^# ' "$0" | sed 's/^# //'
            exit 0;;
        *) log_error "Unknown argument: $1"; exit 1;;
    esac
done

# ------------------------------------------------------------
# Main Bootstrap Process
# ------------------------------------------------------------

log_header "=== UTF Strings Cross-Platform Bootstrap ==="
echo

# Step 1: Detect platform and compilers
PLATFORM=$(detect_platform)
log_info "Detected platform: $PLATFORM"

available_compilers=($(detect_available_compilers "$PLATFORM"))
if [[ ${#available_compilers[@]} -eq 0 ]]; then
    log_error "No suitable compilers found for platform $PLATFORM"
    exit 1
fi

log_info "Available compilers: ${available_compilers[*]}"

# Step 2: Select compiler
if [[ -z "$COMPILER" ]]; then
    # Auto-select best compiler for platform
    case "$PLATFORM" in
        Linux-*)
            if printf '%s\n' "${available_compilers[@]}" | grep -q "^gcc$"; then
                COMPILER="gcc"
            elif printf '%s\n' "${available_compilers[@]}" | grep -q "^clang$"; then
                COMPILER="clang"  
            fi
            ;;
        macOS-*)
            if printf '%s\n' "${available_compilers[@]}" | grep -q "^clang$"; then
                COMPILER="clang"
            elif printf '%s\n' "${available_compilers[@]}" | grep -q "^gcc$"; then
                COMPILER="gcc"
            fi
            ;;
        Windows-*)
            if printf '%s\n' "${available_compilers[@]}" | grep -q "^msvc$"; then
                COMPILER="msvc"
            elif printf '%s\n' "${available_compilers[@]}" | grep -q "^clang$"; then
                COMPILER="clang"
            elif printf '%s\n' "${available_compilers[@]}" | grep -q "^gcc$"; then
                COMPILER="gcc"
            fi
            ;;
    esac
else
    # Validate requested compiler
    if ! printf '%s\n' "${available_compilers[@]}" | grep -q "^$COMPILER$"; then
        log_error "Requested compiler '$COMPILER' not available. Available: ${available_compilers[*]}"
        exit 1
    fi
fi

log_success "Selected compiler: $COMPILER"

# Step 3: Setup environment
setup_compiler_environment "$PLATFORM" "$COMPILER"

# Step 4: Check and install tools
log_info "[1/6] Checking and installing required tools..."
check_cmake
check_compiler "$COMPILER"
check_and_install_conan
check_and_install_clang_format
echo

# Step 5: Clean if requested
if [[ $DO_CLEAN -eq 1 ]]; then
    log_info "[2/6] Cleaning build directory..."
    rm -rf build
fi
mkdir -p build
echo

# Step 6: Install Conan dependencies
step=3
for cfg in "${CONFIGS[@]}"; do
    log_info "[$step/6] Installing Conan dependencies ($cfg)..."
    conan install . -s build_type="$cfg" --output-folder=build --build=missing
    step=$((step+1))
done
echo

# Step 7: Configure and build
for cfg in "${CONFIGS[@]}"; do
    log_info "[$step/6] Configuring and building ($cfg)..."
    configure_cmake "$cfg"
    build_cmake "$cfg"
    
    # Run tests unless skipped
    if [[ $SKIP_TESTS -eq 0 ]]; then
        run_tests "$cfg" || log_warning "Tests failed for $cfg"
    fi
    
    step=$((step+1))
done
echo

# Step 8: Run benchmarks (Release only)
if [[ $SKIP_BENCH -eq 0 ]] && printf '%s\n' "${CONFIGS[@]}" | grep -q '^Release$'; then
    log_info "[$step/6] Running benchmarks..."
    run_benchmarks || log_warning "Benchmarks failed"
else
    log_info "[$step/6] Skipping benchmarks..."
fi
echo

# Summary
log_header "=== Bootstrap Complete! ==="
echo
log_success "Platform: $PLATFORM"
log_success "Compiler: $COMPILER"
echo
echo "Available build artifacts:"
for cfg in "${CONFIGS[@]}"; do
    build_dir=$(get_build_dir "$cfg")
    echo "  $cfg build: $build_dir/"
done
echo
echo "Cross-platform scripts available:"
echo "  • Comprehensive testing: ./test_all_sanitizers.sh"
echo "  • Fuzz testing (Clang):  ./run_fuzz_tests.sh"
echo "  • Code formatting:       cmake --build --preset conan-release --target format"
echo "  • Performance profiling: ./profile_bench.sh"
echo
log_success "Project ready for development on $PLATFORM with $COMPILER! 🚀"
