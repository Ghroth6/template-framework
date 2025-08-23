# Simple 示例项目

本目录包含TFW框架的简单示例项目，分别提供C和C++版本的实现。

## 目录结构

```
simple/
├── c/                          # C语言示例
│   ├── main.c                  # C示例主程序
│   └── CMakeLists.txt          # C示例构建配置
├── cpp/                        # C++示例
│   ├── main.cpp                # C++示例主程序
│   └── CMakeLists.txt          # C++示例构建配置
├── CMakeLists.txt              # 示例项目总构建配置
└── README.md                   # 本文件
```

## 构建说明

### 从根目录构建（推荐）

**Linux/macOS:**
```bash
# 在项目根目录执行
./build_simple.sh
```

**Windows:**
```batch
# 在项目根目录执行
./build_simple.bat
```

### 手动构建

**构建C版本:**
```bash
cd build
cmake -B simple_c -S ../simple/c
cmake --build simple_c --config Release
```

**构建C++版本:**
```bash
cd build
cmake -B simple_cpp -S ../simple/cpp
cmake --build simple_cpp --config Release
```

## 输出位置

构建完成后，可执行文件输出到：
- **C版本**: `build/out/c/tfw_simple_c` (Linux/macOS) 或 `build/out/c/tfw_simple_c.exe` (Windows)
- **C++版本**: `build/out/cpp/tfw_simple_cpp` (Linux/macOS) 或 `build/out/cpp/tfw_simple_cpp.exe` (Windows)

根目录会自动创建符号链接（Linux/macOS）或快捷方式（Windows）方便直接运行：
- `tfw_simple_c` 或 `tfw_simple_c.exe.lnk`
- `tfw_simple_cpp` 或 `tfw_simple_cpp.exe.lnk`

## 示例说明

### C语言示例 (c/main.c)

使用TFW的C接口实现基本功能：
1. 初始化框架
2. 检查初始化状态
3. 执行操作
4. 设置和获取值
5. 清理资源

**主要特点：**
- 纯C语言实现
- 使用函数式API
- 完整的错误处理
- 资源清理

### C++示例 (cpp/main.cpp)

使用TFW的C++接口实现相同功能：
1. 获取核心接口实例
2. 初始化框架
3. 检查初始化状态
4. 执行操作
5. 设置和获取值
6. 清理资源

**主要特点：**
- 面向对象设计
- 单例模式
- RAII风格
- 异常安全

## 运行示例

构建完成后可以直接运行：

```bash
# 运行C版本
./tfw_simple_c          # Linux/macOS
.\tfw_simple_c.exe      # Windows

# 运行C++版本
./tfw_simple_cpp        # Linux/macOS
.\tfw_simple_cpp.exe    # Windows
```

## 预期输出

两个示例都会输出类似的内容：
```
[INFO] [CORE] 开始示例程序
[INFO] [CORE] 核心模块初始化完成
[INFO] [CORE] 初始化状态检查通过
[INFO] [CORE] 执行示例操作: ExampleOperation
[INFO] [CORE] 设置值: 42
[INFO] [CORE] 获取值: 42
[INFO] [CORE] 核心模块退出完成
[INFO] [CORE] 示例程序结束
```

## 自定义开发

这些示例可以作为您开发项目的起点：

1. **复制示例目录**作为新项目的基础
2. **修改CMakeLists.txt**中的项目名称和依赖
3. **替换main.c/main.cpp**中的业务逻辑
4. **添加自己的模块**和功能

## 故障排除

### 构建失败
- 确保已安装CMake
- 确保已正确构建主项目的核心库
- 检查编译器和工具链配置

### 运行失败
- 确保可执行文件有运行权限
- 检查动态库路径配置
- 查看日志输出中的错误信息
