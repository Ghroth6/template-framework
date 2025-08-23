# TemplateFrameWork (TFW) - 模板框架

一个支持C/C++双接口的极简开发框架模板，采用分层日志架构和统一的错误码系统。

## 项目结构

```
template-framework/
├── interface/                    # 接口定义
│   ├── TFW_errorno.h            # 统一错误码定义
│   ├── TFW_types.h              # 基础类型定义
│   ├── TFW_interface.h          # C++接口定义
│   └── TFW_c_interface.h        # C接口定义
├── core/                        # 核心模块
│   ├── TFW_core.h               # 核心类定义
│   ├── TFW_core.cpp             # 核心实现
│   ├── TFW_core_log.h           # Core模块日志宏
│   ├── TFW_interface.cpp        # C++接口实现
│   ├── TFW_c_interface.cpp      # C接口实现
│   └── CMakeLists.txt           # Core模块构建配置
├── utils/                       # 工具模块
│   ├── include/
│   │   └── TFW_log.h            # 基础日志框架
│   ├── log/
│   │   └── TFW_log.c            # 日志实现
│   ├── TFW_utils_log.h          # Utils模块日志宏
│   └── CMakeLists.txt           # Utils模块构建配置
├── simple/                      # 示例项目
│   ├── c/                       # C语言示例
│   │   ├── main.c
│   │   └── CMakeLists.txt
│   ├── cpp/                     # C++示例
│   │   ├── main.cpp
│   │   └── CMakeLists.txt
│   ├── README.md                # 示例说明
│   └── CMakeLists.txt           # 示例构建配置
├── docs/                        # 文档
│   └── 编码规范.md              # 编码规范文档
├── CMakeLists.txt               # 主构建配置
├── build_simple.sh              # Linux/macOS构建脚本
├── build_simple.bat             # Windows构建脚本
└── README.md                    # 项目说明
```

## 核心特性

### 1. 双接口支持
- **C接口**: 完全兼容C语言，使用函数式API
- **C++接口**: 面向对象设计，支持单例模式

### 2. 分层日志框架
- **基础日志宏**: `TFW_LOGT`, `TFW_LOGD`, `TFW_LOGI`, `TFW_LOGW`, `TFW_LOGE`, `TFW_LOGF`
- **模块化设计**: 每个模块有独立的日志宏定义
- **线程安全**: 支持多线程环境

### 3. 统一错误码系统
- **C兼容**: 使用`typedef enum`和`int32_t`返回值
- **模块化**: 每个模块预留1000个错误码槽位
- **可扩展**: 易于添加新的错误码

### 4. 模块化设计
- **Core模块**: 核心功能实现
- **Utils模块**: 通用工具函数
- **Interface层**: 统一的接口定义

## 日志框架

### 日志等级
- `TFW_LOG_LEVEL_TRACE` (0) - 调试追踪
- `TFW_LOG_LEVEL_DEBUG` (1) - 调试信息
- `TFW_LOG_LEVEL_INFO` (2) - 一般信息
- `TFW_LOG_LEVEL_WARNING` (3) - 警告信息
- `TFW_LOG_LEVEL_ERROR` (4) - 错误信息
- `TFW_LOG_LEVEL_FATAL` (5) - 致命错误

### 使用示例
```c
// 基础日志宏
TFW_LOGI(TFW_LOG_MODULE_CORE, "初始化完成");

// 模块专用宏（在各模块的日志头文件中定义）
TFW_LOGI_CORE("Core模块初始化完成");
TFW_LOGE_UTILS("Utils模块出现错误: %s", error_msg);
```

## 错误码系统

### 错误码范围
- **成功**: `TFW_SUCCESS = 0`
- **通用错误**: `-1000 到 -1` (TFW_ERROR = -1000)
- **Core模块**: `-2000 到 -1001`
- **Utils模块**: `-3000 到 -2001`
- **其他模块**: 依此类推，每模块1000个槽位

### 主要错误码
```c
typedef enum {
    // 通用错误 (-1000 到 -1)
    TFW_ERROR = -1000,              // 标准错误
    TFW_ERROR_INVALID_PARAM,        // 无效参数
    TFW_ERROR_NULL_POINTER,         // 空指针
    TFW_ERROR_MEMORY_ALLOCATION,    // 内存分配失败
    
    // 任务错误 (-200 到 -101)
    TFW_ERROR_TASK_FAIL = -200,     // 任务失败
    TFW_ERROR_TASK_TIMEOUT,         // 任务超时
    
    // 系统错误 (-100 到 -1)
    TFW_ERROR_SYSTEM = -100,        // 系统错误
    TFW_ERROR_NOT_INITIALIZED,      // 未初始化
    TFW_ERROR_ALREADY_INITIALIZED,  // 已初始化
    
    // 成功
    TFW_SUCCESS = 0                 // 成功
} TFW_ErrorCode;
```

## 快速开始

### 主项目构建
```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
cmake --build . --config Release
```

### 示例项目构建
```bash
# Linux/macOS
./build_simple.sh

# Windows
./build_simple.bat
```

构建完成后，可执行文件输出到：
- `build/out/c/tfw_simple_c` (C版本)
- `build/out/cpp/tfw_simple_cpp` (C++版本)

根目录会自动创建软链接/快捷方式，方便直接运行。

## C接口兼容性

所有C接口函数都使用`extern "C"`包装，完全兼容C语言项目：

```c
// C接口示例
#include "TFW_c_interface.h"

int main() {
    // 初始化
    TFW_Result result = TFW_CORE_INITIALIZE();
    if (result != TFW_SUCCESS) {
        return result;
    }
    
    // 使用功能
    TFW_CORE_ACTION();
    
    // 清理
    TFW_CORE_EXIT();
    return TFW_SUCCESS;
}
```

## 编码规范

项目遵循统一的编码规范，详见 [`docs/编码规范.md`](docs/编码规范.md)。

## 许可证

本项目采用MIT许可证，详见LICENSE文件。