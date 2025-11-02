# VS Code Development Setup

This directory contains VS Code configuration files for optimal C++ development with the UTF Strings project.

## Quick Start

1. **Open the workspace:**
   ```bash
   code utf_strings.code-workspace
   ```

2. **Install recommended extensions** when prompted, or manually install:
   - C/C++ Extension Pack
   - CMake Tools
   - Python (for Conan)
   - Test Explorer

3. **Build and test:**
   - Press `Ctrl+Shift+P` → "Tasks: Run Task" → "Build Debug"
   - Or use `Ctrl+Shift+B` (default build task)
   - Press `F5` to debug tests

## Features Configured

### 🔧 **IntelliSense & Code Navigation**
- C++23 standard support
- GCC 13 compiler integration  
- Include paths for Conan dependencies
- Symbol navigation and auto-completion
- Error squiggles and diagnostics

### 🏗️ **Build System Integration**
- CMake Tools integration with Conan presets
- One-click build and test execution
- Automatic dependency installation
- Compile commands export for better IntelliSense

### 🧪 **Testing & Debugging**
- GoogleTest integration
- Debug configurations for tests and benchmarks
- Test Explorer integration
- GDB debugging with pretty-printing

### 📝 **Code Quality**
- clang-format integration (format on save)
- Pre-commit hook integration
- Code style consistency
- Search exclusions for build artifacts

## Available Tasks

Access via `Ctrl+Shift+P` → "Tasks: Run Task":

- **Conan Install Debug** - Install dependencies
- **Build Debug** - Build debug version (default: `Ctrl+Shift+B`)
- **Run Tests** - Execute all unit tests
- **Run Specific Test** - Run filtered tests (prompts for filter)
- **Run Benchmarks** - Execute performance benchmarks
- **Format Code** - Apply clang-format to all sources
- **Clean Build** - Remove build artifacts

## Debug Configurations

Available via `F5` or Debug panel:

- **Debug Tests** - Debug all unit tests
- **Debug Specific Test** - Debug filtered tests
- **Debug Benchmarks** - Debug performance tests
- **Attach to Process** - Attach debugger to running process

## File Structure

```
.vscode/
├── c_cpp_properties.json    # C++ IntelliSense configuration
├── extensions.json          # Recommended extensions
├── launch.json             # Debug configurations
├── settings.json           # VS Code settings
└── tasks.json              # Build/test tasks

utf_strings.code-workspace   # Main workspace file
```

## Troubleshooting

### IntelliSense Issues
1. Ensure CMake has configured: `Ctrl+Shift+P` → "CMake: Configure"
2. Check that `compile_commands.json` exists in `build/build/`
3. Reload VS Code window: `Ctrl+Shift+P` → "Developer: Reload Window"

### Build Issues  
1. Run "Conan Install Debug" task first
2. Ensure dependencies are installed: `conan install . --build=missing`
3. Check CMake output panel for detailed errors

### Testing Issues
1. Ensure build succeeded first
2. Check that executables exist in `build/build/`
3. Run tests manually in terminal to verify they work

## Customization

### Compiler Path
Edit `.vscode/c_cpp_properties.json` to change compiler:
```json
"compilerPath": "/usr/bin/clang++-18"
```

### Build Type
Modify tasks.json to use Release builds:
```json
"-s", "build_type=Release"
```

### Additional Include Paths
Add to `c_cpp_properties.json`:
```json
"includePath": [
    "${workspaceFolder}/your/additional/path"
]
```

This setup provides a professional C++ development environment with full integration between VS Code, CMake, Conan, and your testing framework.