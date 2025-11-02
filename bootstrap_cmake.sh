#!/usr/bin/env bash
set -euo pipefail

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

log_success() { echo -e "${GREEN}✅ $1${NC}"; }
log_warning() { echo -e "${YELLOW}⚠️  $1${NC}"; }
log_error() { echo -e "${RED}❌ $1${NC}"; }

check_and_install_clang_format() {
    if command -v clang-format >/dev/null 2>&1; then
        log_success "clang-format found: $(clang-format --version | head -n1)"
    else
        log_warning "clang-format not found — installing..."
        if command -v apt >/dev/null 2>&1; then
            sudo apt update && sudo apt install -y clang-format
        elif command -v brew >/dev/null 2>&1; then
            brew install clang-format
        else
            log_error "Please install clang-format manually"
            exit 1
        fi
        log_success "clang-format installed: $(clang-format --version | head -n1)"
    fi
}

echo "=== UTF Strings Enhanced Bootstrap ==="
check_and_install_clang_format
echo "Now running the comprehensive development workflow..."
