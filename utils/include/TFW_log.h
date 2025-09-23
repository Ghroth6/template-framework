#ifndef TFW_LOG_H
#define TFW_LOG_H

#include <stdint.h>
#include "TFW_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 日志模块分类
// Log module classification
// ============================================================================
typedef enum {
    TFW_LOG_MODULE_CORE = 0,    // 核心模块/Core module
    TFW_LOG_MODULE_UTILS,      // 工具模块/Utils module
    TFW_LOG_MODULE_SDK,     // SDK模块/SDK module
    TFW_LOG_MODULE_SYSTEM   // 系统模块/System module
} TFW_LogModule;

// ============================================================================
// 日志输出目标（使用位标志）
// Log output targets (using bit flags)
// ============================================================================
typedef enum {
    TFW_LOG_OUTPUT_CONSOLE = 0,             // 控制台输出/Console output
    TFW_LOG_OUTPUT_FILE    = 1,             // 文件输出/File output
    TFW_LOG_OUTPUT_MAX     = 32             // 当前定义的最大输出目标位置
} TFW_LogOutputTarget;

// ============================================================================
// 日志宏定义
// Log macro definition
// ============================================================================

// 基础日志宏
// Basic log macro
#define TFW_LOG_INNER(module, level, fmt, ...) \
    (void)TFW_LOG_IMPL(module, level, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

// 等级日志宏
// Level log macro
#define TFW_LOGT(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

#define TFW_LOGD(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

#define TFW_LOGI(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)

#define TFW_LOGW(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_WARNING, fmt, ##__VA_ARGS__)

#define TFW_LOGE(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

#define TFW_LOGF(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

// 日志系统初始化（从配置文件读取配置）
// Log system initialization (read configuration from configuration file)
int32_t TFW_LogInit(void);

// 日志系统反初始化
// Log system deinit
int32_t TFW_LogDeinit(void);


// 日志实现函数声明（由log目录下的实现文件提供）
// Log implementation function declaration (provided by the implementation file in the log directory)
int32_t TFW_LOG_IMPL(int32_t module, int32_t level, const char* file, int32_t line,
                  const char* function, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // TFW_LOG_H
