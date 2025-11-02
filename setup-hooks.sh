#!/bin/bash

# Setup script for installing git hooks for the utf_strings project
# This script installs pre-commit hooks that automatically format C++ code

set -e

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
HOOKS_DIR="$PROJECT_ROOT/.git/hooks"

echo "🔧 Setting up git hooks for utf_strings project..."

# Check if we're in a git repository
if [ ! -d "$PROJECT_ROOT/.git" ]; then
    echo "❌ Error: Not in a git repository"
    exit 1
fi

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo "⚠️  Warning: clang-format not found. Installing it is recommended:"
    echo "   Ubuntu/Debian: sudo apt install clang-format"
    echo "   macOS: brew install clang-format"
    echo "   The hook will still be installed but will warn if clang-format is missing."
fi

# Install pre-commit hook
echo "📝 Installing pre-commit hook..."

# Create backup of existing hook if it exists
if [ -f "$HOOKS_DIR/pre-commit" ] && [ ! -f "$HOOKS_DIR/pre-commit.backup" ]; then
    echo "💾 Backing up existing pre-commit hook to pre-commit.backup"
    cp "$HOOKS_DIR/pre-commit" "$HOOKS_DIR/pre-commit.backup"
fi

# The hook content is already created, so we just need to make sure it's executable
if [ -f "$HOOKS_DIR/pre-commit" ]; then
    chmod +x "$HOOKS_DIR/pre-commit"
    echo "✅ Pre-commit hook installed and made executable"
else
    echo "❌ Error: Pre-commit hook file not found at $HOOKS_DIR/pre-commit"
    echo "   Please make sure the hook file exists"
    exit 1
fi

echo ""
echo "🎉 Git hooks setup complete!"
echo ""
echo "📋 What this does:"
echo "   • Automatically formats C++ files (.cpp, .hpp, .cc, .cxx, .h) before commit"
echo "   • Uses your project's .clang-format configuration"
echo "   • Prevents formatting violations that would fail CI"
echo "   • Auto-stages the formatted files so the commit proceeds smoothly"
echo ""
echo "🚀 Next steps:"
echo "   • Try making a commit with some C++ changes to see it in action"
echo "   • The hook will automatically format your code and include it in the commit"
echo "   • No more CI failures due to formatting issues!"
echo ""
echo "💡 Tips:"
echo "   • To temporarily skip the hook: git commit --no-verify"
echo "   • To manually format all files: cmake --build build --target format"
echo "   • To check formatting: cmake --build build --target format-check"