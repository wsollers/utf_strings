# GitHub Actions Workflows for UTF Strings

This directory contains comprehensive CI/CD workflows for the UTF Strings C++23 library, ensuring cross-platform compatibility, security, and performance.

## Workflow Overview

### 🔄 [ci.yml](.github/workflows/ci.yml) - Main CI/CD Pipeline

**Triggers:** Push, Pull Request, Release
**Purpose:** Primary continuous integration pipeline

**Jobs:**
- **Linux GCC 13** (x64): Debug + Release builds, tests, benchmarks
- **Linux Clang 16** (x64): Debug + Release builds, tests, benchmarks  
- **Windows MSVC 2022 & Clang-CL** (x64): Debug + Release builds, tests, benchmarks
- **Linux Fuzz Testing**: libFuzzer with all UTF encodings (UTF-8/16/32 BE/LE)
- **Linux Sanitizers**: AddressSanitizer + ThreadSanitizer (GCC & Clang)
- **Windows Sanitizers**: AddressSanitizer (MSVC & Clang-CL)
- **Performance Tracking**: Baseline performance monitoring
- **Test Results Publishing**: Unified test result reporting

**Features:**
- ✅ Cross-platform builds (Linux GCC/Clang, Windows MSVC/Clang-CL)
- ✅ Comprehensive testing (unit tests, fuzz tests, sanitizers)
- ✅ Performance benchmarking with tracking
- ✅ Code formatting validation (clang-format)
- ✅ Artifact collection and reporting

### 🔒 [codeql.yml](.github/workflows/codeql.yml) - Security Analysis

**Triggers:** Push, Pull Request, Release, Weekly Schedule
**Purpose:** Advanced security analysis using GitHub CodeQL

**Jobs:**
- **CodeQL Analysis**: Linux x64 Clang Release build analysis
- **Enhanced Security Analysis**: Additional security tools for releases

**Features:**
- ✅ **CodeQL scanning** with security-extended queries
- ✅ **Release security validation** (clang-tidy, cppcheck, valgrind)
- ✅ **Custom query configuration** for C++ security patterns
- ✅ **Binary security analysis** (stack protection, ASLR, PIE, NX bit)
- ✅ **SARIF results** uploaded to GitHub Security tab

**Security Checks:**
- Buffer overflow detection (CWE-119, CWE-120)
- Integer overflow detection (CWE-190)
- Use-after-free detection (CWE-416)
- Out-of-bounds access (CWE-787, CWE-788)

### 🧪 [extended-fuzz.yml](.github/workflows/extended-fuzz.yml) - Extended Fuzz Testing

**Triggers:** Daily Schedule (2 AM UTC), Manual Dispatch
**Purpose:** Long-running fuzz testing for thorough security validation

**Jobs:**
- **Extended Fuzz Testing**: 1+ hour fuzzing sessions for all UTF encodings

**Features:**
- ✅ **Configurable duration** (default 1 hour, manual override)
- ✅ **Comprehensive coverage** (UTF-8, UTF-16 BE/LE, UTF-32 BE/LE)
- ✅ **Corpus management** with persistent test cases
- ✅ **Automatic issue creation** on crash detection
- ✅ **Detailed reporting** with statistics and artifacts

### 🚀 [release-validation.yml](.github/workflows/release-validation.yml) - Release Validation

**Triggers:** Release Published, Manual Dispatch
**Purpose:** Comprehensive validation of release builds

**Jobs:**
- **Cross-Platform Release Builds**: Linux (GCC/Clang) + Windows (MSVC/Clang-CL)
- **Security Validation**: Enhanced security analysis for releases
- **Release Summary**: Automated release documentation

**Features:**
- ✅ **Multi-platform validation** with artifact generation
- ✅ **Binary security analysis** with hardening verification
- ✅ **Performance validation** with benchmark execution
- ✅ **Automated release notes** with validation status
- ✅ **Release artifact packaging** for distribution

## Configuration Files

### 🔧 [codeql-config.yml](.github/codeql/codeql-config.yml) - CodeQL Configuration

**Purpose:** Custom CodeQL analysis configuration

**Features:**
- Security-focused query suites
- Path filtering for relevant source code
- C++-specific build configuration
- Additional security query packs

## Workflow Matrix Coverage

| Platform | Compiler | Debug | Release | Tests | Benchmarks | Fuzz | Sanitizers | Security |
|----------|----------|-------|---------|-------|------------|------|------------|----------|
| **Linux x64** | GCC 13 | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ | ✅ |
| **Linux x64** | Clang 16 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **Windows x64** | MSVC 2022 & Clang-CL | ✅ | ✅ | ✅ | ✅ | ⚠️* | ✅** | ✅ |

*Windows fuzz testing uses harnesses (no libFuzzer)  
**Windows sanitizers: AddressSanitizer only

## Security Analysis Features

### Static Analysis
- **CodeQL**: Advanced semantic analysis with security queries
- **clang-tidy**: Security-focused checks (security-*, cert-*, bugprone-*)
- **cppcheck**: Comprehensive static analysis with C++23 support

### Dynamic Analysis  
- **AddressSanitizer**: Memory error detection (Linux GCC/Clang, Windows MSVC/Clang-CL)
- **ThreadSanitizer**: Data race detection (Linux GCC/Clang)
- **UndefinedBehaviorSanitizer**: Undefined behavior detection (Linux GCC/Clang)
- **LeakSanitizer**: Memory leak detection (Linux GCC/Clang)
- **Valgrind**: Memory analysis for releases (Linux)

### Fuzz Testing
- **libFuzzer**: Comprehensive fuzzing with sanitizers (Linux Clang)
- **Extended sessions**: Daily 1+ hour fuzzing runs
- **Multi-encoding coverage**: UTF-8, UTF-16 BE/LE, UTF-32 BE/LE
- **Corpus management**: Persistent test case storage

### Binary Security Analysis
- **Stack protection**: Compiler-generated stack canaries
- **FORTIFY_SOURCE**: Buffer overflow protection
- **PIE/ASLR**: Address space layout randomization
- **NX bit**: Non-executable stack protection

## Performance Monitoring

### Benchmark Tracking
- **Google Benchmark**: Performance measurement framework
- **Baseline tracking**: Automated performance regression detection
- **Alert system**: 150% threshold for performance degradation
- **Cross-platform comparison**: Performance across different compilers

### Optimization Validation
- **Release builds**: Aggressive optimization verification (-O3, /O2)
- **LTO verification**: Link-time optimization effectiveness
- **Native CPU optimization**: Platform-specific instruction usage

## Artifact Management

### Build Artifacts
- **Binaries**: Tests and benchmark executables
- **Libraries**: Static and shared libraries (.a, .so, .lib, .dll)
- **Headers**: Complete include directory structure
- **Test results**: XML format for reporting integration

### Security Artifacts
- **SARIF files**: Security analysis results
- **Crash dumps**: Fuzz testing failures
- **Analysis reports**: clang-tidy, cppcheck, valgrind outputs
- **Binary analysis**: Security feature verification

### Performance Artifacts
- **Benchmark results**: JSON format with timing data
- **Performance baselines**: Historical performance tracking
- **Regression reports**: Performance change analysis

## Integration Points

### External Services
- **GitHub Security**: CodeQL SARIF upload, security advisories
- **GitHub Releases**: Automated validation and documentation
- **Artifact storage**: GitHub Actions artifact system
- **Issue tracking**: Automated issue creation on failures

### Notification System
- **PR comments**: Test results and performance impact
- **Release updates**: Automated release notes with validation status
- **Alert system**: Performance regression and security issue alerts
- **Email notifications**: Critical failure notifications

## Usage Examples

### Running Workflows

```bash
# Trigger extended fuzz testing manually (2 hours)
gh workflow run extended-fuzz.yml -f duration=7200

# Validate a specific release
gh workflow run release-validation.yml -f release_tag=v1.0.0

# Check workflow status
gh run list --workflow=ci.yml
```

### Local Testing

```bash
# Reproduce CI builds locally
./bootstrap_cmake.sh --compiler gcc --config Release
./bootstrap_cmake.sh --compiler clang --config Debug --enable-fuzz

# Run security analysis locally
cmake --build --preset conan-release --target format-check
clang-tidy src/*.cpp include/*.hpp -- -std=c++23
```

This comprehensive CI/CD system ensures the UTF Strings library maintains high quality, security, and performance standards across all supported platforms! 🚀