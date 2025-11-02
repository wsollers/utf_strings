#!/usr/bin/env bash
set -euo pipefail

# ------------------------------------------------------------
# utf_strings bootstrap build script
#  - Installs Conan deps (Debug + Release or chosen config)
#  - Configures and builds with CMake
#  - Runs unit tests (Debug + Release)
#  - Runs benchmarks (Release)
#
# Usage:
#   ./bootstrap_build.sh                 # Full flow (Debug + Release)
#   ./bootstrap_build.sh -c Debug       # Only Debug
#   ./bootstrap_build.sh -c Release     # Only Release
#   ./bootstrap_build.sh --skip-bench   # Don't run benchmarks
#   ./bootstrap_build.sh --clean        # Clean build/ first
# ------------------------------------------------------------

# ------------------------------------------------------------
# Ensure Conan is installed
# ------------------------------------------------------------
check_and_install_conan() {
  if command -v conan >/dev/null 2>&1; then
    echo "✅ Conan found: $(conan --version)"
  else
    echo "⚙️  Conan not found — installing via pip..."
    if command -v pip3 >/dev/null 2>&1; then
      pip3 install --user conan
    elif command -v pip >/dev/null 2>&1; then
      pip install --user conan
    else
      echo "❌ ERROR: pip not found. Please install Python + pip first." >&2
      exit 1
    fi
    # Add ~/.local/bin to PATH if needed
    export PATH="$HOME/.local/bin:$PATH"
    if ! command -v conan >/dev/null 2>&1; then
      echo "❌ ERROR: Conan install failed or not in PATH. Add ~/.local/bin to PATH and re-run."
      exit 1
    fi
    echo "✅ Conan installed successfully: $(conan --version)"
  fi
}

# ------------------------------------------------------------
# Ensure CMake is installed
# ------------------------------------------------------------
check_and_install_cmake() {
  if command -v cmake >/dev/null 2>&1; then
    echo "✅ CMake found: $(cmake --version | head -n1)"
  else
    echo "⚙️  CMake not found — installing..."
    
    # Detect OS and install accordingly
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
      # Linux
      if command -v apt >/dev/null 2>&1; then
        echo "Installing CMake via apt..."
        sudo apt update
        sudo apt install -y cmake
      elif command -v yum >/dev/null 2>&1; then
        echo "Installing CMake via yum..."
        sudo yum install -y cmake
      elif command -v dnf >/dev/null 2>&1; then
        echo "Installing CMake via dnf..."
        sudo dnf install -y cmake
      elif command -v pacman >/dev/null 2>&1; then
        echo "Installing CMake via pacman..."
        sudo pacman -S --noconfirm cmake
      else
        echo "❌ ERROR: Unable to determine package manager. Please install CMake manually."
        exit 1
      fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
      # macOS
      if command -v brew >/dev/null 2>&1; then
        echo "Installing CMake via Homebrew..."
        brew install cmake
      else
        echo "❌ ERROR: Homebrew not found. Please install Homebrew first or install CMake manually."
        exit 1
      fi
    elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
      # Windows (MSYS2/Cygwin)
      echo "❌ ERROR: Please install CMake manually on Windows."
      echo "  ➤ Download from: https://cmake.org/download/"
      exit 1
    else
      echo "❌ ERROR: Unsupported OS: $OSTYPE. Please install CMake manually."
      exit 1
    fi
    
    # Verify installation
    if command -v cmake >/dev/null 2>&1; then
      echo "✅ CMake installed successfully: $(cmake --version | head -n1)"
    else
      echo "❌ ERROR: CMake installation failed or not in PATH."
      exit 1
    fi
  fi
}

# ------------------------------------------------------------
# Ensure clang-format is installed
# ------------------------------------------------------------
check_and_install_clang_format() {
  if command -v clang-format >/dev/null 2>&1; then
    echo "✅ clang-format found: $(clang-format --version | head -n1)"
  else
    echo "⚙️  clang-format not found — installing..."
    
    # Detect OS and install accordingly
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
      # Linux
      if command -v apt >/dev/null 2>&1; then
        echo "Installing clang-format via apt..."
        sudo apt update
        sudo apt install -y clang-format
      elif command -v yum >/dev/null 2>&1; then
        echo "Installing clang-format via yum..."
        sudo yum install -y clang-tools-extra
      elif command -v dnf >/dev/null 2>&1; then
        echo "Installing clang-format via dnf..."
        sudo dnf install -y clang-tools-extra
      elif command -v pacman >/dev/null 2>&1; then
        echo "Installing clang-format via pacman..."
        sudo pacman -S --noconfirm clang
      else
        echo "❌ ERROR: Unable to determine package manager. Please install clang-format manually."
        exit 1
      fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
      # macOS
      if command -v brew >/dev/null 2>&1; then
        echo "Installing clang-format via Homebrew..."
        brew install clang-format
      else
        echo "❌ ERROR: Homebrew not found. Please install Homebrew first or install clang-format manually."
        exit 1
      fi
    elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
      # Windows (MSYS2/Cygwin)
      echo "❌ ERROR: Please install clang-format manually on Windows."
      echo "  ➤ Download from: https://releases.llvm.org/download.html"
      echo "  ➤ Or use: scoop install llvm (if using Scoop)"
      echo "  ➤ Or use: choco install llvm (if using Chocolatey)"
      exit 1
    else
      echo "❌ ERROR: Unsupported OS: $OSTYPE. Please install clang-format manually."
      exit 1
    fi
    
    # Verify installation
    if command -v clang-format >/dev/null 2>&1; then
      echo "✅ clang-format installed successfully: $(clang-format --version | head -n1)"
    else
      echo "❌ ERROR: clang-format installation failed or not in PATH."
      exit 1
    fi
  fi
}

# ------------------------------------------------------------
# Run tool checks before proceeding
# ------------------------------------------------------------
echo "[0/8] Checking tools..."
check_and_install_conan
check_and_install_cmake
check_and_install_clang_format


CONFIGS=("Debug" "Release")
SKIP_BENCH=0
DO_CLEAN=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    -c|--config) CONFIGS=("$2"); shift 2;;
    --skip-bench) SKIP_BENCH=1; shift;;
    --clean) DO_CLEAN=1; shift;;
    -h|--help)
      grep '^# ' "$0" | sed 's/^# //'
      exit 0;;
    *) echo "Unknown arg: $1" >&2; exit 1;;
  esac
done

require() {
  command -v "$1" >/dev/null 2>&1 || { echo "ERROR: '$1' not found in PATH"; exit 1; }
}

echo "[0/8] Checking tools..."
require conan
require cmake
require clang-format

# OS detection
UNAME=$(uname -s || echo "Unknown")
echo "Detected OS: $UNAME"
echo "Using CMake build system"

if [[ $DO_CLEAN -eq 1 ]]; then
  echo "[1/8] Cleaning ./build ..."
  rm -rf build
fi

mkdir -p build

# 1) Conan deps for each selected config
step=2
for cfg in "${CONFIGS[@]}"; do
  echo "[$step/8] Conan install ($cfg) ..."
  conan install . -s build_type="$cfg" --output-folder=build --build=missing
  step=$((step+1))
done

# 2) Configure and build with CMake for each config
build_one() {
  local cfg="$1"
  echo "[$step/8] Configuring CMake ($cfg) ..."
  
  # Use the appropriate Conan preset
  local preset
  if [[ "$cfg" == "Debug" ]]; then
    preset="conan-debug"
  else
    preset="conan-release"
  fi
  
  # Configure CMake with Conan preset
  echo "[Configure] cmake --preset $preset"
  cmake --preset "$preset"
  
  step=$((step+1))
  echo "[$step/8] Building ($cfg) ..."
  
  # Build with CMake
  echo "[Build] cmake --build --preset $preset --parallel"
  cmake --build --preset "$preset" --parallel
}

# 3) Test & bench runners for CMake builds
run_tests() {
  local cfg="$1"
  local build_dir="build/build"  # CMake build directory
  local exe=
  
  if [[ "$UNAME" == "MINGW"* || "$UNAME" == "MSYS"* || "$UNAME" == "CYGWIN"* ]]; then
    exe="./$build_dir/utf_strings-tests.exe"
  else
    exe="./$build_dir/utf_strings-tests"
  fi
  
  echo "[Test] Running tests for $cfg configuration"
  echo "[Test] $exe"
  if [[ -x "$exe" ]]; then
    "$exe"
  else
    echo "WARNING: Test binary not found: $exe"
    echo "Available files in $build_dir:"
    ls -la "$build_dir/" 2>/dev/null || echo "Build directory not found"
  fi
}

run_bench() {
  local build_dir="build/build"  # CMake build directory
  local exe=
  
  if [[ "$UNAME" == "MINGW"* || "$UNAME" == "MSYS"* || "$UNAME" == "CYGWIN"* ]]; then
    exe="./$build_dir/utf_strings-bench.exe"
  else
    exe="./$build_dir/utf_strings-bench"
  fi
  
  echo "[Bench] Running benchmarks"
  echo "[Bench] $exe"
  if [[ -x "$exe" ]]; then
    "$exe" --benchmark_min_time=0.05
  else
    echo "WARNING: Benchmark binary not found: $exe"
    echo "Available files in $build_dir:"
    ls -la "$build_dir/" 2>/dev/null || echo "Build directory not found"
  fi
}

# 3) Execute per config
for cfg in "${CONFIGS[@]}"; do
  build_one "$cfg"
  step=$((step+1))
  echo "[$step/8] Running unit tests ($cfg) ..."
  run_tests "$cfg"
  step=$((step+1))
done

# 6) Release benchmark (unless skipped)
if [[ $SKIP_BENCH -eq 0 ]]; then
  if printf '%s\n' "${CONFIGS[@]}" | grep -q '^Release$'; then
    echo "[$step/8] Running benchmarks (Release) ..."
    run_bench
  else
    echo "[$step/8] Skipping benchmarks (Release not built). Use: -c Release"
  fi
fi

echo "✅ Done."
echo "Artifacts:"
echo "  - build/build        : CMake build directory with executables"
echo "  - build/             : Conan dependencies and CMake configuration"
echo ""
echo "Available executables:"
echo "  - ./build/build/utf_strings-tests  : Unit tests"
echo "  - ./build/build/utf_strings-bench  : Benchmarks"

