# Code Review Guidelines

## Pre-Push Review Requirements

**MANDATORY**: All code must be reviewed against these parameters before every push to origin.

## Code Review Parameters

### 1. **Performance Analysis** ⚡
- [ ] Evaluate efficiency and optimization opportunities
- [ ] Check for unnecessary allocations
- [ ] Identify performance bottlenecks  
- [ ] Assess algorithmic complexity (Big-O analysis)
- [ ] Verify move semantics are used appropriately
- [ ] Check for redundant copies and temporary objects

### 2. **Security Analysis** 🔒
- [ ] Check for undefined behavior (UB)
- [ ] Validate memory safety (no dangling pointers, use-after-free)
- [ ] Look for buffer overflows
- [ ] Check for integer overflows/underflows
- [ ] Identify potential vulnerabilities
- [ ] Assess input validation (bounds checking, null checks)
- [ ] Verify proper error handling for security-critical paths

### 3. **Correctness Issues** 🐛
- [ ] Identify bugs and logic errors
- [ ] Check for edge cases (empty inputs, max values, etc.)
- [ ] Validate error handling (exceptions, optional returns)
- [ ] Ensure proper initialization of all variables
- [ ] Check const correctness and immutability
- [ ] Verify control flow logic

### 4. **C++ Core Guidelines Compliance** 📋
- [ ] Check adherence to modern C++ best practices
- [ ] Validate RAII usage (Resource Acquisition Is Initialization)
- [ ] Check exception safety (basic/strong/no-throw guarantee)
- [ ] Verify proper use of special member functions (Rule of 0/3/5)
- [ ] Ensure proper ownership semantics (unique_ptr, shared_ptr)

### 5. **Modern C++ Usage (C++23)** 🚀
- [ ] Check use of modern features (concepts, constexpr, etc.)
- [ ] Validate use of `[[nodiscard]]`, `noexcept` attributes
- [ ] Check for proper move semantics and perfect forwarding
- [ ] Assess use of std library features vs. custom implementations
- [ ] Verify template metaprogramming best practices
- [ ] Check for appropriate use of `std::optional`, `std::variant`

### 6. **Design Issues** 🏗️
- [ ] Evaluate API design consistency
- [ ] Check naming conventions (snake_case, CamelCase consistency)
- [ ] Assess abstraction levels and encapsulation
- [ ] Review for unnecessary complexity or over-engineering
- [ ] Check interface segregation and single responsibility
- [ ] Verify dependency management

### 7. **Missing Features** ➕
- [ ] Identify gaps in functionality
- [ ] Suggest useful additions based on use cases
- [ ] Compare to standard library patterns and idioms
- [ ] Check for missing convenience functions
- [ ] Assess completeness of API surface

### 8. **Documentation** 📚
- [ ] Check for adequate inline comments
- [ ] Verify API documentation completeness
- [ ] Validate code examples and usage patterns
- [ ] Check for missing preconditions/postconditions
- [ ] Ensure README and documentation are up-to-date
- [ ] Verify documentation matches implementation

### 9. **Test Coverage** 🧪
- [ ] Verify comprehensive unit test coverage
- [ ] Check for edge case testing
- [ ] Validate error path testing
- [ ] Assess integration test coverage
- [ ] Check for performance regression tests

### 10. **Build and CI** 🔧
- [ ] Verify all CI jobs pass
- [ ] Check compiler warnings (should be warning-free)
- [ ] Validate cross-platform compatibility
- [ ] Ensure proper dependency management
- [ ] Check code formatting consistency

## Review Output Format

### **Recommendations Summary**
Categorize all findings by severity:

- 🔴 **Critical** - Must fix before production (security, UB, crashes)
- 🟡 **Important** - Should fix (performance, correctness, maintainability)
- 🟢 **Nice to have** - Optional improvements (style, minor optimizations)

### **Score Card**
Rate each category (A+ to F):

| Category | Grade | Notes |
|----------|-------|-------|
| Performance | ? | ... |
| Security | ? | ... |
| Correctness | ? | ... |
| C++ Guidelines | ? | ... |
| Modern C++ | ? | ... |
| Design | ? | ... |
| Documentation | ? | ... |
| Test Coverage | ? | ... |

**Overall Grade**: ?/10
**Production Ready**: Yes/No

### **Critical Fixes Required**
For all 🔴 Critical issues:
- Provide exact code fixes
- Explain the issue clearly
- Show before/after code examples

---

## Key Focus Areas

Based on project requirements, emphasize:

- **Production readiness** - Code must be deployable
- **Security** - Especially UB, overflow, memory safety
- **Performance optimization** - UTF processing efficiency
- **Modern C++ best practices** - C++23 features and idioms
- **Comprehensive feedback** - Actionable and prioritized

## Review Checklist for Reviewers

Before approving any PR:

1. ✅ All CI checks pass
2. ✅ Code coverage maintains/improves percentage
3. ✅ No security vulnerabilities identified
4. ✅ Performance benchmarks show no regressions
5. ✅ All critical and important issues addressed
6. ✅ Documentation updated as needed
7. ✅ Tests added for new functionality

---

**Remember**: This is a UTF string processing library focused on performance and correctness. Every change should maintain the high standards for production-ready C++ code.