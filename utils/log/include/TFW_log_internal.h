#ifndef TFW_LOG_INTERNAL_H
#define TFW_LOG_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 日志输出模式（内部使用）
// Log output mode (internal use)
typedef enum {
    TFW_LOG_OUTPUT_NONE = 0,  // 不输出/No output
    TFW_LOG_OUTPUT_CONSOLE,   // 控制台输出/Console output
    TFW_LOG_OUTPUT_FILE,      // 文件输出/File output
    TFW_LOG_OUTPUT_BOTH,      // 两者都输出/Both output
} TFW_LogOutputMode;

#ifdef __cplusplus
}
#endif

#endif  // TFW_LOG_INTERNAL_H
