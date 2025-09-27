#!/bin/bash

# TFW项目构建脚本 (Linux/macOS/WSL)

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

# 检查必要的工具
check_requirements() {
    print_info "检查构建工具..."

    if ! command -v cmake &> /dev/null; then
        print_error "CMake未安装，请先安装CMake"
        exit 1
    fi

    if ! command -v make &> /dev/null; then
        print_error "Make未安装，请先安装Make"
        exit 1
    fi

    print_success "构建工具检查完成"
}

# 运行清理脚本
run_clean_script() {
    print_info "运行清理脚本..."

    if [ -f "./clean.sh" ]; then
        print_info "执行现有的clean.sh脚本"
        bash ./clean.sh
        print_success "清理脚本执行完成"
    else
        print_warning "未找到clean.sh脚本，跳过清理步骤"
    fi
}

# 创建必要的目录
create_directories() {
    print_info "创建构建目录..."

    mkdir -p build
    mkdir -p out/bin
    mkdir -p out/lib

    print_success "目录创建完成"
}

# 配置CMake项目
configure_project() {
    print_info "配置CMake项目..."

    cd build

    # 根据平台选择生成器
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        print_info "检测到macOS，使用Unix Makefiles生成器"
        cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    else
        # Linux/WSL
        print_info "检测到Linux/WSL，使用Unix Makefiles生成器"
        cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    fi

    cd ..
    print_success "CMake配置完成"
}

# 编译项目
build_project() {
    print_info "开始编译项目..."

    cd build

    # 使用多核编译
    local cpu_count=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    print_info "使用 $cpu_count 个核心进行编译"

    make -j$cpu_count

    cd ..
    print_success "编译完成"
}

# 显示构建结果
show_results() {
    print_info "构建结果："

    if [ -f "build/compile_commands.json" ]; then
        print_success "✓ 编译数据库已生成 (build/compile_commands.json)"
        print_info "  IDE可以自动加载此文件进行智能提示"
    else
        print_warning "⚠ 编译数据库未生成"
    fi

    if [ -d "out/bin" ] && [ "$(ls -A out/bin 2>/dev/null)" ]; then
        print_success "✓ 可执行文件已生成 (out/bin/)"
        ls -la out/bin/
    else
        print_warning "⚠ 可执行文件未生成"
    fi

    if [ -d "out/lib" ] && [ "$(ls -A out/lib 2>/dev/null)" ]; then
        print_success "✓ 库文件已生成 (out/lib/)"
        ls -la out/lib/
    else
        print_warning "⚠ 库文件未生成"
    fi
}

# 主函数
main() {
    print_info "开始构建TFW项目..."
    print_info "构建类型: Debug"
    print_info "输出目录: out/"

    check_requirements
    run_clean_script
    create_directories
    configure_project
    build_project
    show_results

    print_success "TFW项目构建完成！"
    print_info "可执行文件位置: out/bin/"
    print_info "库文件位置: out/lib/"
}

# 脚本入口
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
