# C++版本示例

使用TFW框架的C++接口实现的简单示例程序。

## 功能

1. 初始化TFW核心
2. 调用核心接口执行操作
3. 获取操作结果
4. 正常退出

## 构建

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 运行

构建完成后，可执行文件位于`out/cpp/` 目录中：

```bash
# Windows
./out/cpp/tfw_simple_cpp.exe

# Linux/macOS
./out/cpp/tfw_simple_cpp
```

## 代码特点

- 使用现代C++实现
- 调用TFW C++接口类
- 包含完整的错误处理
- 演示了基本的TFW框架使用流程
- 使用RAII和异常安全的设计
