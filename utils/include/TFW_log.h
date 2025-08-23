#ifndef TFW_LOG_H
#define TFW_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 日志等级定义
// ============================================================================
enum TFW_LogLevel {
    TFW_LOG_LEVEL_TRACE = 0,    // 追踪信息
    TFW_LOG_LEVEL_DEBUG = 1,    // 调试信息
    TFW_LOG_LEVEL_INFO = 2,     // 一般信息
    TFW_LOG_LEVEL_WARNING = 3,  // 警告信息
    TFW_LOG_LEVEL_ERROR = 4,    // 错误信息
    TFW_LOG_LEVEL_FATAL = 5     // 致命错误
};

// ============================================================================
// 日志模块分类
// ============================================================================
enum TFW_LogModule {
    TFW_LOG_MODULE_CORE = 0,    // 核心模块
    TFW_LOG_MODULE_UTILS = 1,   // 工具模块
    TFW_LOG_MODULE_SDK = 2,     // SDK模块
    TFW_LOG_MODULE_SYSTEM = 3   // 系统模块
};

// ============================================================================
// 日志宏定义
// ============================================================================

// 基础日志宏
#define TFW_LOG_INNER(module, level, fmt, ...) \
    (void)TFW_LOG_IMPL(module, level, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

// 等级日志宏
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

// 日志实现函数声明（由log目录下的实现文件提供）
int TFW_LOG_IMPL(int module, int level, const char* file, int line,
                  const char* function, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // TFW_LOG_H
