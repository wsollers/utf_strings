# Code Coverage CI Job

This document describes the new code coverage job added to the CI pipeline.

## Job: `linux-clang-coverage`

### Purpose
Generates comprehensive code coverage reports for the UTF Strings library using Clang's instrumentation-based coverage tools.

### Configuration
- **Runner**: Ubuntu 24.04
- **Compiler**: Clang 18 with coverage instrumentation
- **Build Type**: Debug
- **Coverage Flags**: 
  - `--coverage` (GCC compatibility)
  - `-fprofile-instr-generate` (Clang instrumentation)
  - `-fcoverage-mapping` (Source mapping)

### Process
1. **Build with Coverage**: Compiles the library and tests with coverage instrumentation
2. **Run Tests**: Executes all 65 unit tests while collecting coverage data
3. **Process Coverage**: Converts raw profile data to indexed format using `llvm-profdata`
4. **Generate Reports**:
   - **Text Report**: Summary coverage statistics (`coverage_report.txt`)
   - **HTML Report**: Detailed line-by-line coverage (`coverage_html/index.html`)
   - **JSON Summary**: Machine-readable coverage data (`coverage_summary.json`)

### Outputs
- Coverage percentage calculation with color-coded badge
- Comprehensive HTML coverage report with source highlighting
- JSON data for integration with external tools
- PR comments with coverage summary (for pull requests)
- Codecov integration (optional, non-blocking)

### Coverage Exclusions
The following paths are excluded from coverage reporting:
- Test files (`test`)
- GoogleTest framework (`gtest`)
- Benchmark files (`benchmark`)
- Conan dependencies (`conan`)
- Build artifacts (`build`)

### Badge Colors
- 🟢 **90%+**: Bright Green
- 🟢 **80-89%**: Green  
- 🟡 **70-79%**: Yellow Green
- 🟡 **60-69%**: Yellow
- 🟠 **50-59%**: Orange
- 🔴 **<50%**: Red

### Artifacts
All coverage reports are uploaded as CI artifacts with 30-day retention:
- `coverage-reports-linux-clang`

### Integration
- Runs in parallel with other CI jobs
- Results included in test result publishing
- PR comments provide immediate feedback on coverage changes