#!/bin/bash

# TFW Simple 绀轰緥椤圭洰鏋勫缓鑴氭湰 (浠庢牴鐩綍鎵ц)

echo "=== Building TFW Simple Examples ==="

# 纭繚鍦ㄦ牴鐩綍鐨刡uild鐩綍涓?if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
fi

cd build

# 鍒涘缓杈撳嚭鐩綍
mkdir -p out/c out/cpp

# 鏋勫缓C鐗堟湰
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

# 鏋勫缓C++鐗堟湰
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

# 鍒涘缓杞摼鎺ュ埌鏍圭洰褰?echo "Creating symbolic links in root directory..."

# 鍒犻櫎鏃х殑杞摼鎺ワ紙濡傛灉瀛樺湪锛?rm -f tfw_simple_c tfw_simple_cpp

# 鍒涘缓杞摼鎺?ln -sf build/out/c/tfw_simple_c tfw_simple_c
ln -sf build/out/cpp/tfw_simple_cpp tfw_simple_cpp

echo "=== All examples built successfully ==="
echo "Output files:"
echo "  C version: build/out/c/tfw_simple_c"
echo "  C++ version: build/out/cpp/tfw_simple_cpp"
echo "Symbolic links created in root directory:"
echo "  tfw_simple_c -> build/out/c/tfw_simple_c"
echo "  tfw_simple_cpp -> build/out/cpp/tfw_simple_cpp"
