#!/usr/bin/env bash
set -euo pipefail

# ------------------------------------------------------------
# utf_strings bootstrap build script
#  - Installs Conan deps (Debug + Release or chosen config)
#  - Generates projects with Premake (gmake2 / vs2022)
#  - Builds everything (make/MSBuild)
#  - Runs unit tests (Debug + Release)
#  - Runs benchmarks (Release)
#
# Usage:
#   ./bootstrap_build.sh                 # Full flow (Debug + Release)
#   ./bootstrap_build.sh -c Debug       # Only Debug
#   ./bootstrap_build.sh -c Release     # Only Release
#   ./bootstrap_build.sh -g gmake2      # Force gmake2
#   ./bootstrap_build.sh -g vs2022      # Force VS solution (Windows)
#   ./bootstrap_build.sh --skip-bench   # Don’t run benchmarks
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
# Ensure Premake is installed
# ------------------------------------------------------------
check_premake() {
  if command -v premake5 >/dev/null 2>&1; then
    echo "✅ Premake5 found: $(premake5 --version)"
  else
    echo "❌ NOT ERROR: premake5 not found."
    echo "  ➤ Install manually from https://premake.github.io/download/"
    echo "  or use your package manager (e.g. sudo apt install premake5)."
    sudo apt update
    sudo apt install premake5
  fi
}

# ------------------------------------------------------------
# Run tool checks before proceeding
# ------------------------------------------------------------
echo "[0/7] Checking tools..."
check_and_install_conan
check_premake


CONFIGS=("Debug" "Release")
GEN=""                      # auto-detect
SKIP_BENCH=0
DO_CLEAN=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    -c|--config) CONFIGS=("$2"); shift 2;;
    -g|--generator) GEN="$2"; shift 2;;
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

echo "[0/7] Checking tools..."
require conan
require premake5

# OS & generator detection
UNAME=$(uname -s || echo "Unknown")
case "$UNAME" in
  Linux|Darwin) DEFAULT_GEN="gmake2" ;;
  MINGW*|MSYS*|CYGWIN*) DEFAULT_GEN="vs2022" ;;  # Git Bash on Windows
  *) DEFAULT_GEN="gmake2" ;;
esac
GEN=${GEN:-$DEFAULT_GEN}
echo "Detected OS: $UNAME"
echo "Using Premake generator: $GEN"

if [[ $DO_CLEAN -eq 1 ]]; then
  echo "[1/7] Cleaning ./build ..."
  rm -rf build
fi

mkdir -p build

# 1) Conan deps for each selected config
step=2
for cfg in "${CONFIGS[@]}"; do
  echo "[$step/7] Conan install ($cfg) ..."
  conan install . -s build_type="$cfg" --output-folder=build --build=missing
  step=$((step+1))
done

# 2) Generate project files with Premake
echo "[$step/7] Generating project files with Premake ($GEN) ..."
pushd build >/dev/null
case "$GEN" in
  gmake2) premake5 gmake2 ;;
  vs2022) premake5 vs2022 ;;
  *) echo "Unsupported generator: $GEN"; exit 1;;
esac
popd >/dev/null
step=$((step+1))

# 3) Build per platform/generator
build_one() {
  local cfg="$1"
  if [[ "$GEN" == "gmake2" ]]; then
    pushd build >/dev/null
    if [[ "$cfg" == "Debug" ]]; then
      echo "[Build] make config=debug"
      make config=debug
    else
      echo "[Build] make config=release"
      make config=release
    fi
    popd >/dev/null
  else
    # vs2022 (MSBuild)
    if command -v MSBuild.exe >/dev/null 2>&1; then
      echo "[Build] MSBuild utf_strings.sln ($cfg)"
      MSBuild.exe build/utf_strings.sln -p:Configuration="$cfg"
    else
      echo "WARNING: MSBuild.exe not found. Open build/utf_strings.sln in Visual Studio 2022 and build '$cfg' there."
    fi
  fi
}

# 4) Test & bench runners
run_tests() {
  local cfg="$1"
  local exe=
  if [[ "$UNAME" == "MINGW"* || "$UNAME" == "MSYS"* || "$UNAME" == "CYGWIN"* ]]; then
    exe="./build/bin/$cfg/utf_strings-tests.exe"
  else
    exe="./build/bin/$cfg/utf_strings-tests"
  fi
  echo "[Test] $exe"
  if [[ -x "$exe" ]]; then
    "$exe"
  else
    echo "WARNING: Test binary not found: $exe"
  fi
}

run_bench() {
  local exe=
  if [[ "$UNAME" == "MINGW"* || "$UNAME" == "MSYS"* || "$UNAME" == "CYGWIN"* ]]; then
    exe="./build/bin/Release/utf_strings-bench.exe"
  else
    exe="./build/bin/Release/utf_strings-bench"
  fi
  echo "[Bench] $exe"
  if [[ -x "$exe" ]]; then
    "$exe" --benchmark_min_time=0.05
  else
    echo "WARNING: Benchmark binary not found: $exe"
  fi
}

# 5) Execute per config
for cfg in "${CONFIGS[@]}"; do
  echo "[$step/7] Building ($cfg) ..."
  build_one "$cfg"
  step=$((step+1))
  echo "[$step/7] Running unit tests ($cfg) ..."
  run_tests "$cfg"
  step=$((step+1))
done

# 6) Release benchmark (unless skipped)
if [[ $SKIP_BENCH -eq 0 ]]; then
  if printf '%s\n' "${CONFIGS[@]}" | grep -q '^Release$'; then
    echo "[$step/7] Running benchmarks (Release) ..."
    run_bench
  else
    echo "[$step/7] Skipping benchmarks (Release not built). Use: -c Release"
  fi
fi

echo "✅ Done."
echo "Artifacts:"
echo "  - build/bin/Debug    : tests, libs"
echo "  - build/bin/Release  : tests, benchmarks, libs"

