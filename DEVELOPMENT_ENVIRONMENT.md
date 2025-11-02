# Development Environment Setup

This document outlines the development environment configuration and alignment with CI/CD.

## Environment Alignment

### Local Development (Recommended)
- **OS**: Ubuntu 24.04 LTS (noble) or 22.04 LTS (jammy)
- **GCC**: 13.x (specifically 13.3.0+ recommended)
- **Clang**: 18.x (specifically 18.1.3+ recommended)
- **CMake**: 3.25+
- **Conan**: 2.0+

### CI/CD Environment (GitHub Actions)
- **OS**: Ubuntu 22.04 (jammy)
- **GCC**: 13 (from ubuntu repos: `gcc-13`)
- **Clang**: 18 (from LLVM repos: `clang-18`)
- **CMake**: Latest stable
- **Conan**: Latest stable

## Quick Environment Check

Run these commands to verify your local environment:

```bash
# Check OS version
lsb_release -a

# Check compiler versions
gcc --version
clang --version

# Check build tools
cmake --version
conan --version
```

### Expected Output (Local Ubuntu 24.04):
```
# OS
Ubuntu 24.04.x LTS

# Compilers  
gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
Ubuntu clang version 18.1.3

# Build tools
cmake version 3.28.x
Conan version 2.x.x  
```

## Bootstrap Scripts

The bootstrap scripts automatically detect and configure your environment:

### Linux/macOS
```bash
./bootstrap_cmake.sh                 # Auto-detect best compiler
./bootstrap_cmake.sh --compiler gcc  # Force GCC
./bootstrap_cmake.sh --compiler clang # Force Clang
```

### Windows
```cmd
bootstrap_cmake.bat                  # Auto-detect MSVC/Clang-CL
bootstrap_cmake.bat --compiler msvc     # Force MSVC
bootstrap_cmake.bat --compiler clang-cl # Force Clang-CL
```

## Advanced Compiler Configuration

The build system provides fine-grained control over compiler behavior through external flags. This system allows you to match specific CI configurations or optimize for your use case.

### Configuration Flags

| Flag | Values | Description |
|------|--------|-------------|
| `COMPILER_TYPE` | `GCC`\|`CLANG`\|`MSVC` | Explicit compiler identification |
| `USE_LTO` | `ON`\|`OFF` | Link Time Optimization |
| `USE_NATIVE_ARCH` | `ON`\|`OFF` | Native CPU optimization (`-march=native`) |
| `USE_MSVC_LTO` | `ON`\|`OFF` | MSVC-specific LTO flags (`/LTCG`, `/GL`) |
| `USE_LIBC_PLUS_PLUS` | `ON`\|`OFF` | Use libc++ instead of libstdc++ (Clang only) |
| `ENABLE_SHARED_LIBRARY` | `ON`\|`OFF` | Build shared libraries |

### Common Configurations

**Maximum Performance (Benchmarking):**
```bash
cmake --preset conan-release \
  -DCOMPILER_TYPE=CLANG \
  -DUSE_LTO=ON \
  -DUSE_NATIVE_ARCH=ON \
  -DUSE_LIBC_PLUS_PLUS=ON \
  -DENABLE_SHARED_LIBRARY=ON
```

**Debug-Friendly (Sanitizers/Fuzzing):**
```bash
cmake --preset conan-debug \
  -DCOMPILER_TYPE=CLANG \
  -DUSE_LTO=OFF \
  -DUSE_NATIVE_ARCH=OFF \
  -DENABLE_SHARED_LIBRARY=OFF
```

**Windows MSVC:**
```cmd
cmake --preset conan-release ^
  -DUSE_MSVC_LTO=ON ^
  -DENABLE_SHARED_LIBRARY=ON
```

**Windows Clang-CL:**
```cmd
cmake --preset conan-release ^
  -DCMAKE_CXX_COMPILER=clang-cl ^
  -DCMAKE_C_COMPILER=clang-cl ^
  -DUSE_MSVC_LTO=OFF ^
  -DENABLE_SHARED_LIBRARY=OFF
```

### CI Configuration Matching

The CI uses different configurations for different purposes:

**Linux GCC (Production):**
- `COMPILER_TYPE=GCC`
- `USE_LTO=ON` 
- `USE_NATIVE_ARCH=ON`
- `ENABLE_SHARED_LIBRARY=ON`

**Linux Clang (Development):**
- `COMPILER_TYPE=CLANG`
- `USE_LTO=ON`
- `USE_NATIVE_ARCH=ON` 
- `USE_LIBC_PLUS_PLUS=OFF`
- `ENABLE_SHARED_LIBRARY=ON`

**Linux Clang (Performance):**
- `COMPILER_TYPE=CLANG`
- `USE_LTO=ON`
- `USE_NATIVE_ARCH=ON`
- `USE_LIBC_PLUS_PLUS=ON`
- `ENABLE_SHARED_LIBRARY=ON`

**Sanitizer/Fuzz Testing:**
- `COMPILER_TYPE=CLANG`
- `USE_LTO=OFF`
- `USE_NATIVE_ARCH=OFF`
- Various sanitizer flags enabled

## Environment Setup

### Ubuntu 24.04 Setup (Recommended)

```bash
# Install GCC 13
sudo apt update
sudo apt install gcc-13 g++-13

# Install Clang 18 (if not already installed)
sudo apt install clang-18 clang++-18

# Set as default (optional)
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 100
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100

# Install build tools
sudo apt install cmake ninja-build
pip3 install --user conan
```

### Ubuntu 22.04 Setup (CI Match)

```bash
# Install GCC 13
sudo apt update  
sudo apt install gcc-13 g++-13

# Install Clang 18 from LLVM repos
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main"
sudo apt update
sudo apt install clang-18 clang++-18

# Set as default
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100

# Install build tools
sudo apt install cmake ninja-build
pip3 install --user conan
```

## Verification

After setup, verify alignment with CI:

```bash
# Run the bootstrap script
./bootstrap_cmake.sh

# Check that builds work with both compilers
./bootstrap_cmake.sh --compiler gcc
./bootstrap_cmake.sh --compiler clang

# Run comprehensive tests
./test_all_sanitizers.sh
```

## Troubleshooting

### Version Mismatches
- **Minor version differences** (like GCC 13.2 vs 13.3) are acceptable
- **Major version differences** (like GCC 12 vs 13) should be avoided
- Use `update-alternatives` to manage multiple compiler versions

### Package Conflicts
- Ubuntu 24.04 packages are generally compatible with 22.04 workflows
- If issues arise, consider using Ubuntu 22.04 in Docker for exact CI matching

### Conan Issues
- Ensure Conan profile matches CI: `conan profile show --profile:host=default`
- Update Conan frequently: `pip3 install --user --upgrade conan`

## CI/CD Alignment Status

| Component | Local (Ubuntu 24.04) | CI (Ubuntu 22.04) | Status |
|-----------|----------------------|-------------------|--------|
| **GCC**   | 13.3.0               | 13.x              | ✅ Compatible |
| **Clang** | 18.1.3               | 18.x              | ✅ Compatible |
| **OS**    | 24.04 LTS            | 22.04 LTS         | ✅ Compatible |
| **CMake** | 3.28+                | 3.25+             | ✅ Compatible |
| **Conan** | 2.x                  | 2.x               | ✅ Compatible |

Your environment is **well-aligned** with CI/CD! 🎯