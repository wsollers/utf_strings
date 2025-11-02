@echo off
setlocal enabledelayedexpansion

REM ============================================================================
REM UTF Strings Windows Bootstrap Script
REM  - Detects Windows environment and Visual Studio/MSVC
REM  - Installs required development tools via package managers
REM  - Configures cross-platform builds with MSVC 2022
REM  - Sets up all targets: tests, profiling, benchmarks
REM
REM Usage:
REM   bootstrap_cmake.bat                       REM Auto-detect and full setup
REM   bootstrap_cmake.bat --config Debug       REM Only Debug build  
REM   bootstrap_cmake.bat --config Release     REM Only Release build
REM   bootstrap_cmake.bat --skip-tests         REM Skip running tests
REM   bootstrap_cmake.bat --skip-bench         REM Skip benchmarks
REM   bootstrap_cmake.bat --clean              REM Clean build directory first
REM ============================================================================

set "RED="
set "GREEN="
set "YELLOW="
set "BLUE="
set "MAGENTA="
set "CYAN="
set "NC="

REM Enable colored output if supported
if not "%ConEmuANSI%"=="ON" if not "%ANSICON%"=="" (
    set "RED=[0;31m"
    set "GREEN=[0;32m"
    set "YELLOW=[1;33m"
    set "BLUE=[0;34m"
    set "MAGENTA=[0;35m"
    set "CYAN=[0;36m"
    set "NC=[0m"
)

REM ============================================================================
REM Logging Functions
REM ============================================================================

:log_info
echo %BLUE%ℹ️  %~1%NC%
goto :eof

:log_success  
echo %GREEN%✅ %~1%NC%
goto :eof

:log_warning
echo %YELLOW%⚠️  %~1%NC%
goto :eof

:log_error
echo %RED%❌ %~1%NC%
goto :eof

:log_header
echo %MAGENTA%🚀 %~1%NC%
goto :eof

REM ============================================================================
REM Environment Detection
REM ============================================================================

:detect_platform
set "PLATFORM=Windows-x64"
if "%PROCESSOR_ARCHITECTURE%"=="x86" set "PLATFORM=Windows-x86"
if "%PROCESSOR_ARCHITECTURE%"=="ARM64" set "PLATFORM=Windows-arm64"
goto :eof

:detect_msvc
set "COMPILER=msvc"
set "MSVC_FOUND=0"

REM Check if we're already in a Visual Studio Command Prompt
if not "%VCINSTALLDIR%"=="" (
    set "MSVC_FOUND=1"
    goto :eof
)

REM Try to find Visual Studio 2022
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARSALL=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
    set "MSVC_FOUND=1"
    goto :eof
)

if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARSALL=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
    set "MSVC_FOUND=1"
    goto :eof
)

if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARSALL=%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
    set "MSVC_FOUND=1"
    goto :eof
)

if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARSALL=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
    set "MSVC_FOUND=1"
    goto :eof
)

if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARSALL=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
    set "MSVC_FOUND=1"
    goto :eof
)

if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set "VCVARSALL=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
    set "MSVC_FOUND=1"
    goto :eof
)

goto :eof

:setup_msvc_environment
if "%MSVC_FOUND%"=="0" (
    call :log_error "Visual Studio 2022 not found. Please install Visual Studio 2022 with C++ tools."
    exit /b 1
)

if "%VCINSTALLDIR%"=="" (
    call :log_info "Setting up MSVC environment..."
    call "%VCVARSALL%" x64
    if errorlevel 1 (
        call :log_error "Failed to set up MSVC environment"
        exit /b 1
    )
)
goto :eof

REM ============================================================================
REM Tool Installation Functions  
REM ============================================================================

:check_and_install_cmake
where cmake >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=3" %%i in ('cmake --version 2^>nul ^| findstr /R "^cmake"') do set "CMAKE_VERSION=%%i"
    call :log_success "CMake found: !CMAKE_VERSION!"
    goto :eof
)

call :log_info "CMake not found — attempting to install..."

REM Try winget first (Windows 10 1809+)
where winget >nul 2>&1
if %errorlevel%==0 (
    winget install Kitware.CMake
    if %errorlevel%==0 goto :check_cmake_again
)

REM Try chocolatey
where choco >nul 2>&1
if %errorlevel%==0 (
    choco install cmake -y
    if %errorlevel%==0 goto :check_cmake_again
)

REM Try scoop
where scoop >nul 2>&1  
if %errorlevel%==0 (
    scoop install cmake
    if %errorlevel%==0 goto :check_cmake_again
)

call :log_error "Could not install CMake automatically. Please install CMake 3.25+ manually from: https://cmake.org/download/"
exit /b 1

:check_cmake_again
where cmake >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=3" %%i in ('cmake --version 2^>nul ^| findstr /R "^cmake"') do set "CMAKE_VERSION=%%i"
    call :log_success "CMake installed: !CMAKE_VERSION!"
) else (
    call :log_error "CMake installation failed"
    exit /b 1
)
goto :eof

:check_and_install_conan
where conan >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=2" %%i in ('conan --version 2^>nul') do set "CONAN_VERSION=%%i"
    call :log_success "Conan found: !CONAN_VERSION!"
    goto :check_conan_profile
)

call :log_info "Conan not found — installing via pip..."

REM Try pip3 first, then pip
where pip3 >nul 2>&1
if %errorlevel%==0 (
    pip3 install --user conan
) else (
    where pip >nul 2>&1
    if %errorlevel%==0 (
        pip install --user conan
    ) else (
        call :log_error "pip not found. Please install Python 3.8+ with pip first."
        exit /b 1
    )  
)

REM Refresh PATH to include newly installed conan
set "PATH=%USERPROFILE%\AppData\Roaming\Python\Scripts;%PATH%"

where conan >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=2" %%i in ('conan --version 2^>nul') do set "CONAN_VERSION=%%i"
    call :log_success "Conan installed: !CONAN_VERSION!"
) else (
    call :log_error "Conan installation failed or not in PATH"
    exit /b 1
)

:check_conan_profile
conan profile list 2>nul | findstr /x "default" >nul
if %errorlevel%==0 goto :eof

call :log_info "Creating Conan default profile..."
conan profile detect --force
goto :eof

:check_and_install_clang_format
where clang-format >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=3" %%i in ('clang-format --version 2^>nul ^| findstr /R "version"') do set "CLANG_FORMAT_VERSION=%%i"
    call :log_success "clang-format found: !CLANG_FORMAT_VERSION!"
    goto :eof
)

call :log_info "clang-format not found — attempting to install..."

REM Try winget first
where winget >nul 2>&1
if %errorlevel%==0 (
    winget install LLVM.LLVM
    if %errorlevel%==0 goto :check_clang_format_again
)

REM Try chocolatey
where choco >nul 2>&1
if %errorlevel%==0 (
    choco install llvm -y
    if %errorlevel%==0 goto :check_clang_format_again
)

REM Try scoop
where scoop >nul 2>&1
if %errorlevel%==0 (
    scoop install llvm
    if %errorlevel%==0 goto :check_clang_format_again
)

call :log_error "Could not install clang-format automatically."
call :log_error "Please install LLVM/Clang manually from: https://releases.llvm.org/download.html"
call :log_error "Or use a package manager:"
call :log_error "  ➤ winget install LLVM.LLVM"
call :log_error "  ➤ choco install llvm"
call :log_error "  ➤ scoop install llvm"
exit /b 1

:check_clang_format_again
REM Refresh PATH to include newly installed tools
set "PATH=%ProgramFiles%\LLVM\bin;%PATH%"

where clang-format >nul 2>&1
if %errorlevel%==0 (
    for /f "tokens=3" %%i in ('clang-format --version 2^>nul ^| findstr /R "version"') do set "CLANG_FORMAT_VERSION=%%i"
    call :log_success "clang-format installed: !CLANG_FORMAT_VERSION!"
) else (
    call :log_error "clang-format installation failed or not in PATH"
    exit /b 1
)
goto :eof

REM ============================================================================
REM Build Functions
REM ============================================================================

:configure_cmake
set "CONFIG=%~1"
call :log_info "Configuring CMake (%CONFIG%)..."

if /i "%CONFIG%"=="Debug" (
    cmake --preset conan-debug
) else if /i "%CONFIG%"=="Release" (
    cmake --preset conan-release
) else (
    call :log_error "Unknown configuration: %CONFIG%"
    exit /b 1
)

if errorlevel 1 (
    call :log_error "CMake configuration failed"
    exit /b 1
)
goto :eof

:build_cmake
set "CONFIG=%~1"
call :log_info "Building (%CONFIG%)..."

if /i "%CONFIG%"=="Debug" (
    cmake --build --preset conan-debug --parallel
) else if /i "%CONFIG%"=="Release" (
    cmake --build --preset conan-release --parallel
) else (
    call :log_error "Unknown configuration: %CONFIG%"
    exit /b 1
)

if errorlevel 1 (
    call :log_error "Build failed"
    exit /b 1
)
goto :eof

:run_tests
set "CONFIG=%~1"
call :log_info "Running tests (%CONFIG%)..."

if /i "%CONFIG%"=="Debug" (
    set "BUILD_DIR=build\Debug"
) else (
    set "BUILD_DIR=build\build"
)

set "TEST_BINARY=%BUILD_DIR%\utf_strings-tests.exe"
if exist "%TEST_BINARY%" (
    "%TEST_BINARY%"
    if errorlevel 1 (
        call :log_warning "Tests failed"
        exit /b 1
    )
) else (
    call :log_warning "Test binary not found: %TEST_BINARY%"
    exit /b 1
)
goto :eof

:run_benchmarks
call :log_info "Running benchmarks (Release)..."

set "BENCH_BINARY=build\build\utf_strings-bench.exe"
if exist "%BENCH_BINARY%" (
    "%BENCH_BINARY%" --benchmark_min_time=0.1s --benchmark_time_unit=us
    if errorlevel 1 (
        call :log_warning "Benchmarks failed"
        exit /b 1  
    )
) else (
    call :log_warning "Benchmark binary not found: %BENCH_BINARY%"
    exit /b 1
)
goto :eof

REM ============================================================================
REM Command Line Argument Parsing
REM ============================================================================

set "CONFIGS=Debug Release"
set "SKIP_TESTS=0"
set "SKIP_BENCH=0" 
set "DO_CLEAN=0"

:parse_args
if "%~1"=="" goto :args_done
if /i "%~1"=="--config" (
    set "CONFIGS=%~2"
    shift /1
    shift /1
    goto :parse_args
)
if /i "%~1"=="--skip-tests" (
    set "SKIP_TESTS=1"
    shift /1
    goto :parse_args
)
if /i "%~1"=="--skip-bench" (
    set "SKIP_BENCH=1"
    shift /1
    goto :parse_args  
)
if /i "%~1"=="--clean" (
    set "DO_CLEAN=1"
    shift /1
    goto :parse_args
)
if /i "%~1"=="-h" goto :show_help
if /i "%~1"=="--help" goto :show_help
call :log_error "Unknown argument: %~1"
exit /b 1

:show_help
echo UTF Strings Windows Bootstrap Script
echo.
echo Usage:
echo   bootstrap_cmake.bat                       Auto-detect and full setup
echo   bootstrap_cmake.bat --config Debug       Only Debug build  
echo   bootstrap_cmake.bat --config Release     Only Release build
echo   bootstrap_cmake.bat --skip-tests         Skip running tests
echo   bootstrap_cmake.bat --skip-bench         Skip benchmarks
echo   bootstrap_cmake.bat --clean              Clean build directory first
echo   bootstrap_cmake.bat --help               Show this help
exit /b 0

:args_done

REM ============================================================================
REM Main Bootstrap Process  
REM ============================================================================

call :log_header "=== UTF Strings Windows Bootstrap ==="
echo.

REM Step 1: Detect platform and compiler
call :detect_platform
call :log_info "Platform: %PLATFORM%"

call :detect_msvc
if "%MSVC_FOUND%"=="0" (
    call :log_error "Visual Studio 2022 not found"
    exit /b 1
)
call :log_success "Selected compiler: %COMPILER%"

REM Step 2: Setup environment
call :setup_msvc_environment

REM Step 3: Check and install tools
call :log_info "[1/6] Checking and installing required tools..."
call :check_and_install_cmake
if errorlevel 1 exit /b 1
call :check_and_install_conan  
if errorlevel 1 exit /b 1
call :check_and_install_clang_format
if errorlevel 1 exit /b 1
echo.

REM Step 4: Clean if requested
if "%DO_CLEAN%"=="1" (
    call :log_info "[2/6] Cleaning build directory..."
    if exist build rmdir /s /q build
)
if not exist build mkdir build
echo.

REM Step 5: Install Conan dependencies
set "STEP=3"
for %%C in (%CONFIGS%) do (
    call :log_info "[!STEP!/6] Installing Conan dependencies (%%C)..."
    conan install . -s build_type=%%C --output-folder=build --build=missing
    if errorlevel 1 (
        call :log_error "Conan install failed for %%C"
        exit /b 1
    )
    set /a STEP+=1
)
echo.

REM Step 6: Configure and build
for %%C in (%CONFIGS%) do (
    call :log_info "[!STEP!/6] Configuring and building (%%C)..."
    call :configure_cmake %%C
    if errorlevel 1 exit /b 1
    
    call :build_cmake %%C
    if errorlevel 1 exit /b 1
    
    REM Run tests unless skipped
    if "%SKIP_TESTS%"=="0" (
        call :run_tests %%C
        if errorlevel 1 call :log_warning "Tests failed for %%C"
    )
    
    set /a STEP+=1
)
echo.

REM Step 7: Run benchmarks (Release only)
if "%SKIP_BENCH%"=="0" (
    echo "%CONFIGS%" | findstr /i "Release" >nul
    if not errorlevel 1 (
        call :log_info "[!STEP!/6] Running benchmarks..."
        call :run_benchmarks
        if errorlevel 1 call :log_warning "Benchmarks failed"
    ) else (
        call :log_info "[!STEP!/6] Skipping benchmarks (Release not built)..."
    )
) else (
    call :log_info "[!STEP!/6] Skipping benchmarks..."
)
echo.

REM Summary
call :log_header "=== Bootstrap Complete! ==="
echo.
call :log_success "Platform: %PLATFORM%"
call :log_success "Compiler: %COMPILER%"
echo.
echo Available build artifacts:
for %%C in (%CONFIGS%) do (
    if /i "%%C"=="Release" (
        echo   %%C build: build\build\
    ) else (
        echo   %%C build: build\%%C\
    )
)
echo.
echo Windows-specific commands available:
echo   • Code formatting:       cmake --build --preset conan-release --target format
echo   • Format checking:       cmake --build --preset conan-release --target format-check
echo   • Run unit tests:        build\Debug\utf_strings-tests.exe
echo   • Run benchmarks:        build\build\utf_strings-bench.exe
echo.
call :log_success "Project ready for development on %PLATFORM% with %COMPILER%! 🚀"

endlocal