# UTF Strings Documentation System

This document describes the comprehensive documentation pipeline for the UTF Strings library, including API documentation generation, deployment, and maintenance.

## 🏗️ Architecture

The documentation system consists of several integrated components:

### 1. **Doxygen Configuration**
- **File:** `Doxyfile`
- **Purpose:** Generates comprehensive API documentation from source code comments
- **Features:**
  - C++23 support with concepts and requires clauses
  - SVG diagrams with Graphviz integration
  - Interactive search functionality
  - XML output for integration with other tools
  - Modern responsive styling

### 2. **GitHub Actions Workflow**
- **File:** `.github/workflows/docs.yml`
- **Purpose:** Automated documentation building and deployment
- **Triggers:** Push to main/develop, pull requests, releases
- **Features:**
  - Multi-step documentation generation
  - Quality metrics and statistics reporting
  - PR comments with build status
  - Automatic deployment to GitHub Pages

### 3. **Custom Styling**
- **File:** `docs/assets/css/doxygen-custom.css`
- **Purpose:** Modern, GitHub-style appearance
- **Features:**
  - Dark mode support
  - Responsive design
  - Clean typography and spacing
  - Consistent branding

### 4. **CMake Integration**
- **Targets:** `docs`, `clean-docs`
- **Purpose:** Local documentation generation
- **Requirements:** Doxygen and Graphviz

## 📚 Generated Documentation

The system generates comprehensive documentation including:

### API Reference
- **Classes:** All public classes with detailed member documentation
- **Namespaces:** Complete namespace hierarchy and organization  
- **Functions:** All public functions with parameters and return values
- **Types:** Type aliases, concepts, and enumerations
- **Examples:** Code examples and usage patterns

### Visual Elements
- **Class Diagrams:** Inheritance and collaboration relationships
- **Dependency Graphs:** Include and dependency relationships
- **Directory Structure:** Visual project organization
- **Interactive Navigation:** Searchable tree view

### Quality Metrics
- **Coverage Statistics:** Documentation coverage by file and class
- **Build Quality:** Warnings and errors from generation
- **File Statistics:** Counts of classes, functions, and documentation

## 🚀 Usage

### Local Documentation Generation

#### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install doxygen graphviz

# macOS with Homebrew  
brew install doxygen graphviz

# Windows with Chocolatey
choco install doxygen graphviz
```

#### Build Commands
```bash
# Using CMake targets (recommended)
cmake --build --preset=conan-debug --target docs
cmake --build --preset=conan-debug --target clean-docs

# Direct Doxygen invocation
doxygen Doxyfile

# Clean generated documentation
rm -rf docs/api/
```

### Accessing Documentation

#### Local Access
After building locally:
- **Main Index:** `docs/api/html/index.html`
- **API Reference:** `docs/api/html/annotated.html`
- **Search:** Available through the web interface

#### Online Access (GitHub Pages)
- **Base URL:** `https://wsollers.github.io/utf_strings/`
- **API Docs:** `https://wsollers.github.io/utf_strings/api/`
- **Performance:** `https://wsollers.github.io/utf_strings/performance/`

## 🔧 Configuration

### Doxygen Settings

Key configuration options in `Doxyfile`:

```
PROJECT_NAME           = "UTF Strings Library"
PROJECT_VERSION        = "v1.3.0"
INPUT                  = include/ src/ README.md docs/
RECURSIVE              = YES
GENERATE_HTML          = YES
GENERATE_XML           = YES
USE_MATHJAX            = YES
HAVE_DOT               = YES
INTERACTIVE_SVG        = YES
```

### GitHub Actions Configuration

The workflow is configured for:
- **Build Matrix:** Ubuntu latest with Doxygen 1.9.8+
- **Deployment:** GitHub Pages with proper permissions
- **Artifacts:** 30-day retention for documentation builds
- **Comments:** Automatic PR status updates

### Custom CSS Variables

The styling system uses CSS custom properties:

```css
:root {
  --primary-color: #0366d6;      /* Links and accents */
  --background-color: #ffffff;    /* Main background */
  --surface-color: #f6f8fa;      /* Cards and surfaces */
  --text-color: #24292e;         /* Primary text */
  --code-background: #f6f8fa;    /* Code blocks */
}
```

## 📊 Quality Metrics

The documentation system tracks:

### Coverage Statistics
- **Header Files:** Count of documented header files
- **Source Files:** Count of documented implementation files  
- **Classes/Structs:** Count of documented classes and structures
- **Functions:** Count of documented functions and methods
- **Generated Pages:** Total HTML pages created

### Build Quality
- **Doxygen Warnings:** Parser and generation warnings
- **Doxygen Errors:** Critical errors preventing generation
- **Missing Documentation:** Undocumented public APIs
- **Link Resolution:** Broken cross-references

### Performance Metrics
- **Generation Time:** Time to build complete documentation
- **Output Size:** Total size of generated documentation
- **File Counts:** Number of HTML, CSS, JS, and image files

## 🔍 Troubleshooting

### Common Issues

#### 1. Doxygen Not Found
```bash
# Error: Doxygen not found
# Solution: Install Doxygen
sudo apt install doxygen graphviz
```

#### 2. Missing Diagrams
```bash  
# Error: Graphs not generated
# Solution: Install Graphviz
sudo apt install graphviz
```

#### 3. Build Warnings
```bash
# Warning: Undocumented members
# Solution: Add /// @brief comments to public APIs
```

#### 4. GitHub Pages Deployment Fails
```yaml
# Error: Permission denied
# Solution: Ensure workflow has pages: write permission
permissions:
  contents: read
  pages: write
  id-token: write
```

### Debug Commands

```bash
# Check Doxygen version and features
doxygen --version
doxygen -h

# Validate Doxyfile syntax
doxygen -s -g /tmp/test.doxyfile

# Test with verbose output
doxygen Doxyfile 2>&1 | tee doxygen.log

# Check generated file structure
find docs/api/html -name "*.html" | head -10
```

## 🚦 CI/CD Pipeline

### Workflow Stages

1. **Setup Phase**
   - Install Doxygen and Graphviz
   - Validate project structure
   - Count documentable items

2. **Generation Phase**
   - Run Doxygen with full configuration
   - Generate HTML and XML output
   - Create quality metrics

3. **Analysis Phase**
   - Count warnings and errors
   - Generate coverage statistics
   - Create quality report

4. **Deployment Phase** (main branch only)
   - Prepare for GitHub Pages
   - Upload artifacts
   - Deploy to live site

5. **Reporting Phase**
   - Comment on pull requests
   - Update deployment status
   - Archive build artifacts

### Status Badges

The system can generate status badges for:
- **Documentation Build:** ![Docs](https://img.shields.io/badge/docs-passing-green)
- **Coverage:** ![Coverage](https://img.shields.io/badge/coverage-85%25-yellow)
- **Quality:** ![Quality](https://img.shields.io/badge/warnings-2-orange)

## 🔄 Maintenance

### Regular Tasks

#### Monthly
- Review documentation coverage metrics
- Update Doxygen configuration for new features
- Check for broken links and references
- Update styling for consistency

#### Per Release
- Update version numbers in Doxyfile
- Regenerate full documentation
- Verify all examples and code snippets
- Update README and getting started guides

#### As Needed
- Fix Doxygen warnings
- Add missing API documentation
- Update custom CSS for new components
- Optimize build performance

### Version Management

Documentation versioning follows the library version:
- **Major Releases:** Full documentation rebuild
- **Minor Releases:** Incremental updates
- **Patch Releases:** Bug fixes and corrections

## 📝 Contributing

### Adding Documentation

1. **Source Comments:** Use Doxygen-style comments
   ```cpp
   /// @brief Brief description of the function
   /// @param param_name Description of parameter
   /// @return Description of return value
   /// @note Additional notes
   /// @warning Important warnings
   ```

2. **File Headers:** Document file purpose and usage
   ```cpp
   /**
    * @file filename.hpp
    * @brief Brief description of file contents
    * @details Detailed description of functionality
    */
   ```

3. **Examples:** Include usage examples
   ```cpp
   /// @example
   /// @code{.cpp}
   /// utf::string::Utf8String str{u8"Hello"};
   /// for (auto cp : str) {
   ///     // Process each code point
   /// }
   /// @endcode
   ```

### Modifying Styling

1. **CSS Changes:** Edit `docs/assets/css/doxygen-custom.css`
2. **Layout Changes:** Edit `docs/assets/DoxygenLayout.xml`
3. **Configuration:** Edit `Doxyfile` for structural changes

### Testing Changes

```bash
# Test locally before committing
cmake --build --preset=conan-debug --target docs
open docs/api/html/index.html

# Check for warnings
grep -i "warning\|error" doxygen.log
```

## 🎯 Future Enhancements

### Planned Features
- **Interactive Examples:** Live code examples with syntax highlighting
- **API Diff Reports:** Compare API changes between versions  
- **Integration Tests:** Validate documentation examples
- **Multi-language Support:** Internationalization for documentation
- **Advanced Search:** Full-text search with filters and facets

### Integration Opportunities
- **IDE Integration:** VSCode extension for documentation preview
- **Package Managers:** Integration with Conan and vcpkg documentation
- **Testing Frameworks:** Link test cases to documented functionality
- **Performance Monitoring:** Link benchmarks to API documentation

---

For questions or issues with the documentation system, please [open an issue](https://github.com/wsollers/utf_strings/issues) on GitHub.