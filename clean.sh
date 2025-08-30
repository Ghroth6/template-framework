#!/bin/bash

# TFW项目清理脚本 (Linux/macOS/WSL)
# 基于编码规范9.4节要求

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 清理构建文件
clean_build() {
    print_info "清理构建文件..."

    if [ -d "build" ]; then
        rm -rf build
        print_success "已删除build目录"
    else
        print_info "build目录不存在，跳过"
    fi
}

# 清理输出文件
clean_output() {
    print_info "清理输出文件..."

    if [ -d "out" ]; then
        rm -rf out
        print_success "已删除out目录"
    else
        print_info "out目录不存在，跳过"
    fi
}

# 清理CMake缓存
clean_cmake_cache() {
    print_info "清理CMake缓存..."

    if [ -f "CMakeCache.txt" ]; then
        rm -f CMakeCache.txt
        print_success "已删除CMakeCache.txt"
    fi

    if [ -d "CMakeFiles" ]; then
        rm -rf CMakeFiles
        print_success "已删除CMakeFiles目录"
    fi
}

# 清理IDE临时文件
clean_ide_files() {
    print_info "清理IDE临时文件..."

    # VSCode/Cursor临时文件
    if [ -d ".vscode" ]; then
        find .vscode -name "*.log" -delete 2>/dev/null || true
        find .vscode -name "*.tmp" -delete 2>/dev/null || true
        print_info "已清理VSCode临时文件"
    fi

    # 其他可能的临时文件
    find . -name "*.o" -delete 2>/dev/null || true
    find . -name "*.obj" -delete 2>/dev/null || true
    find . -name "*.exe" -delete 2>/dev/null || true
    find . -name "*.dll" -delete 2>/dev/null || true
    find . -name "*.so" -delete 2>/dev/null || true
    find . -name "*.dylib" -delete 2>/dev/null || true
    find . -name "*.a" -delete 2>/dev/null || true
    find . -name "*.lib" -delete 2>/dev/null || true

    print_info "已清理编译产物文件"
}

# 显示清理结果
show_clean_result() {
    print_info "清理完成！"
    print_info "已清理的内容："
    print_info "  - 构建目录 (build/)"
    print_info "  - 输出目录 (out/)"
    print_info "  - CMake缓存文件"
    print_info "  - IDE临时文件"
    print_info "  - 编译产物文件"

    print_success "项目已恢复到干净状态，可以重新构建"
}

# 主函数
main() {
    print_info "开始清理TFW项目..."

    clean_build
    clean_output
    clean_cmake_cache
    clean_ide_files
    show_clean_result

    print_success "TFW项目清理完成！"
}

# 脚本入口
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
