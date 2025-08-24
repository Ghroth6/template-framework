#!/bin/bash

# TFW Simple Example Project Build Script (Execute from root directory)

echo "=== Building TFW Simple Examples ==="

# Ensure we are in the build directory under root
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
fi

cd build

# Create output directories
mkdir -p out/c out/cpp out/lib

# Build core libraries first
echo "Building core libraries..."
cmake -B core_build -S ../core
if [ $? -eq 0 ]; then
    cmake --build core_build --config Release
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

# Build utils library
echo "Building utils library..."
cmake -B utils_build -S ../utils
if [ $? -eq 0 ]; then
    cmake --build utils_build --config Release
    if [ $? -eq 0 ]; then
        echo "Utils library built successfully"
    else
        echo "Failed to build utils library"
        exit 1
    fi
else
    echo "Failed to configure utils library"
    exit 1
fi

# Build C version
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

# Build C++ version
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

# Create symbolic links in root directory
echo "Creating symbolic links in root directory..."

# Remove old symbolic links if they exist
rm -f tfw_simple_c tfw_simple_cpp

# Create symbolic links
ln -sf out/c/tfw_simple_c tfw_simple_c
ln -sf out/cpp/tfw_simple_cpp tfw_simple_cpp

echo "=== All examples built successfully ==="
echo "Output files:"
echo "  C version: out/c/tfw_simple_c"
echo "  C++ version: out/cpp/tfw_simple_cpp"
echo "Symbolic links created in root directory:"
echo "  tfw_simple_c -> out/c/tfw_simple_c"
echo "  tfw_simple_cpp -> out/cpp/tfw_simple_cpp"
