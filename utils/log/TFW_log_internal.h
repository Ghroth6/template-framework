#ifndef TFW_LOG_INTERNAL_H
#define TFW_LOG_INTERNAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 日志输出模式（内部使用）
typedef enum {
    TFW_LOG_OUTPUT_CONSOLE = 0x01,    // 控制台输出
    TFW_LOG_OUTPUT_FILE = 0x02,       // 文件输出
    TFW_LOG_OUTPUT_BOTH = 0x03        // 两者都输出
} TFW_LogOutputMode;

// 日志文件配置（内部使用）
typedef struct {
    TFW_LogOutputMode outputMode;      // 输出模式
    char logDir[512];                  // 日志目录
    char logPrefix[64];                // 日志文件前缀
    size_t maxFileSize;                // 最大文件大小（字节）
    int32_t maxRetentionDays;              // 最大保留天数
} TFW_LogFileConfig;

// 内部日志文件管理接口（仅log模块内部使用）
int32_t TFW_Log_InitFileOutput(const char* logDir, const char* logPrefix, size_t maxFileSize, int32_t maxRetentionDays);
int32_t TFW_Log_CloseFileOutput(void);
int32_t TFW_Log_WriteToFile(const char* message);
int32_t TFW_Log_CheckAndRotate(void);
int32_t TFW_Log_CleanupOldLogs(void);

#ifdef __cplusplus
}
#endif

#endif // TFW_LOG_INTERNAL_H
