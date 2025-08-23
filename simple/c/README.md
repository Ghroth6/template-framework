# C语言版本示例

使用TFW框架的C接口实现的简单示例程序。

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

构建完成后，可执行文件位于`out/c/` 目录中：

```bash
# Windows
./out/c/tfw_simple_c.exe

# Linux/macOS
./out/c/tfw_simple_c
```

## 代码特点

- 使用纯C语言实现
- 调用TFW_C_接口函数
- 包含完整的错误处理
- 演示了基本的TFW框架使用流程
