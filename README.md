# TemplateFrameWork (TFW) - 模板框架

一个支持C/C++双接口的极简开发框架模板，采用分层日志架构、统一的错误码系统和跨平台兼容性。

## 项目结构

```
template-framework/
├── interface/                    # 接口定义
│   ├── TFW_errorno.h            # 统一错误码定义
│   ├── TFW_types.h              # 基础类型定义
│   ├── TFW_interface.h          # C++接口定义
│   └── TFW_c_interface.h        # C接口定义
├── core/                        # 核心模块
│   ├── include/
│   │   └── TFW_core.h           # 核心类定义
│   ├── core_frame/
│   │   └── TFW_core.cpp         # 核心实现
│   ├── TFW_interface.cpp        # C++接口实现
│   ├── TFW_c_interface.cpp      # C接口实现
│   └── CMakeLists.txt           # Core模块构建配置
├── utils/                       # 工具模块
│   ├── include/
│   │   ├── TFW_log.h            # 基础日志框架
│   │   ├── TFW_thread.h         # 统一线程/互斥锁接口
│   │   ├── TFW_timer.h          # 统一时间戳接口
│   │   ├── TFW_file.h           # 统一文件/路径接口
│   │   └── TFW_mem.h            # 统一内存管理接口
│   ├── log/
│   │   └── TFW_log.c            # 日志实现
│   ├── thread/
│   │   ├── win32/               # Windows线程实现
│   │   │   └── TFW_thread_impl.c
│   │   └── posix/               # POSIX线程实现
│   │       └── TFW_thread_impl.c
│   ├── timer/
│   │   ├── win32/               # Windows时间实现
│   │   │   └── TFW_timer_impl.c
│   │   ├── macos/               # macOS时间实现
│   │   │   └── TFW_timer_impl.c
│   │   └── posix/               # POSIX时间实现
│   │       └── TFW_timer_impl.c
│   ├── file/
│   │   ├── win32/               # Windows文件实现
│   │   │   └── TFW_file_impl.c
│   │   └── posix/               # POSIX文件实现
│   │       └── TFW_file_impl.c
│   ├── mem/
│   │   ├── win32/               # Windows内存管理实现
│   │   │   └── TFW_mem_impl.c
│   │   └── posix/               # POSIX内存管理实现
│   │       └── TFW_mem_impl.c
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
- **Core模块**: 核心功能实现，采用分层架构（core_frame → interface → c_interface）
- **Utils模块**: 通用工具函数，支持跨平台兼容性
- **Interface层**: 统一的接口定义，支持C/C++双接口

### 5. 跨平台兼容性
- **Windows**: 使用Windows API实现
- **macOS**: 使用POSIX标准实现
- **Linux/Unix**: 使用POSIX标准实现
- **统一接口**: 所有平台提供相同的API接口

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

// 日志格式：[时间] 线程ID 日志等级 模块 消息
// 示例：[2024-01-15 14:30:25.123] T12345 I CORE 初始化完成
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
    // 通用错误码 (-1000 到 -1)
    TFW_ERROR = -1000,              // 标准错误
    TFW_ERROR_INVALID_PARAM,        // 无效参数
    TFW_ERROR_NOT_INIT,             // 未初始化
    TFW_ERROR_ALREADY_INIT,         // 已经初始化
    TFW_ERROR_OPERATION_FAIL,       // 操作失败

    // 任务错误码 (-200 到 -101)
    TFW_ERROR_TASK_FAIL = -200,     // 任务失败
    TFW_ERROR_TIMEOUT,              // 超时错误
    TFW_ERROR_NOT_FOUND,            // 未找到

    // 锁相关错误码 (-300 到 -201)
    TFW_ERROR_LOCK_FAILED = -300,   // 锁操作失败
    TFW_ERROR_NO_MEMORY = -301,     // 内存不足

    // 系统错误码 (-100 到 -1)
    TFW_ERROR_SYSTEM = -100,        // 系统错误
    TFW_ERROR_MEMORY,               // 内存错误
    TFW_ERROR_FILE,                 // 文件错误
    TFW_ERROR_NETWORK,              // 网络错误

    // 成功码
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

## 新增功能

### 内存管理模块
- **TFW_Malloc()**: 分配内存（最大512MB）
- **TFW_Calloc()**: 分配并清零内存
- **TFW_Free()**: 释放内存
- **TFW_GetMemoryStats()**: 获取内存使用统计

### 跨平台工具函数
- **线程管理**: 统一的互斥锁接口（Windows/POSIX）
- **时间戳**: 毫秒级精度时间戳（Windows/macOS/POSIX）
- **文件操作**: 统一的文件路径处理（Windows/POSIX）

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
