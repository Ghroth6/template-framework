#!/bin/bash

# TFW Simple 示例项目构建脚本 (从根目录执行)

echo "=== Building TFW Simple Examples ==="

# 确保在根目录的build目录中
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
fi

cd build

# 创建输出目录
mkdir -p out/c out/cpp out/lib

# 首先构建核心库和工具库
echo "Building core libraries..."
cmake -B core_libs -S ..
if [ $? -eq 0 ]; then
    cmake --build core_libs --config Release
    if [ $? -eq 0 ]; then
        echo "Core libraries built successfully"
    else
        echo "Failed to build core libraries"
        exit 1
    fi
else
    echo "Failed to configure core libraries"
    exit 1
fi

# 构建C版本
echo "Building C version..."
cmake -B simple_c -S ../simple/c
if [ $? -eq 0 ]; then
    cmake --build simple_c --config Release
    if [ $? -eq 0 ]; then
        echo "C version built successfully"
    else
        echo "Failed to build C version"
        exit 1
    fi
else
    echo "Failed to configure C version"
    exit 1
fi

# 构建C++版本
echo "Building C++ version..."
cmake -B simple_cpp -S ../simple/cpp
if [ $? -eq 0 ]; then
    cmake --build simple_cpp --config Release
    if [ $? -eq 0 ]; then
        echo "C++ version built successfully"
    else
        echo "Failed to build C++ version"
        exit 1
    fi
else
    echo "Failed to configure C++ version"
    exit 1
fi

cd ..

# 创建软链接到根目录
echo "Creating symbolic links in root directory..."

# 删除旧的软链接（如果存在）
rm -f tfw_simple_c tfw_simple_cpp

# 创建软链接
ln -sf out/c/tfw_simple_c tfw_simple_c
ln -sf out/cpp/tfw_simple_cpp tfw_simple_cpp

echo "=== All examples built successfully ==="
echo "Output files:"
echo "  C version: out/c/tfw_simple_c"
echo "  C++ version: out/cpp/tfw_simple_cpp"
echo "Symbolic links created in root directory:"
echo "  tfw_simple_c -> out/c/tfw_simple_c"
echo "  tfw_simple_cpp -> out/cpp/tfw_simple_cpp"
