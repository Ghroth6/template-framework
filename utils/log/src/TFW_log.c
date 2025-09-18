// 1. system headers
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 2. project headers
#include "TFW_common_defines.h"
#include "TFW_errorno.h"
#include "TFW_log.h"
#include "TFW_log_internal.h"
#include "TFW_timer.h"
#include "TFW_types.h"
#include "TFW_file.h"
#include "TFW_thread.h"

// ============================================================================
// 内部结构体定义
// Internal structure definition
// ============================================================================

typedef struct TFW_LogContext {
    TFW_LogLevel logLevel;
    bool logInitialized;
    TFW_LogOutputMode logOutputMode;
} TFW_LogContext;

typedef struct {
    int32_t level;
    const char label[TFW_LOG_LABEL_LEN_MAX];
} TFW_LogLabel;

typedef struct {
    int32_t module;
    const char name[TFW_MODULE_LABEL_LEN_MAX];
} TFW_ModuleLabel;

// ============================================================================
// 全局变量
// Global variables
// ============================================================================
static TFW_LogContext g_logContext = {
    .logLevel = TFW_LOG_LEVEL_INFO,
    .logInitialized = false,
    .logOutputMode = TFW_LOG_OUTPUT_CONSOLE
};

static TFW_LogLabel g_logLabel[] = {
    {.level = TFW_LOG_LEVEL_TRACE, .label = "TRACE"},
    {.level = TFW_LOG_LEVEL_DEBUG, .label = "DEBUG"},
    {.level = TFW_LOG_LEVEL_INFO, .label = "INFO"},
    {.level = TFW_LOG_LEVEL_WARNING, .label = "WARNING"},
    {.level = TFW_LOG_LEVEL_ERROR, .label = "ERROR"},
    {.level = TFW_LOG_LEVEL_FATAL, .label = "FATAL"},
};

static TFW_ModuleLabel g_moduleLabel[] = {
    {.module = TFW_LOG_MODULE_CORE, .name = "CORE"},
    {.module = TFW_LOG_MODULE_UTILS, .name = "UTILS"},
    {.module = TFW_LOG_MODULE_SDK, .name = "SDK"},
    {.module = TFW_LOG_MODULE_SYSTEM, .name = "SYSTEM"},
};

// ============================================================================
// 内部函数
// Internal functions
// ============================================================================

/**
 * Get log level string
 * 获取日志等级字符串
 */
static TFW_UNUSED const char *TFW_Log_GetLevelString(int32_t level) {
    switch (level) {
        case TFW_LOG_LEVEL_TRACE:
            return g_logLabel[TFW_LOG_LEVEL_TRACE].label;
        case TFW_LOG_LEVEL_DEBUG:
            return g_logLabel[TFW_LOG_LEVEL_DEBUG].label;
        case TFW_LOG_LEVEL_INFO:
            return g_logLabel[TFW_LOG_LEVEL_INFO].label;
        case TFW_LOG_LEVEL_WARNING:
            return g_logLabel[TFW_LOG_LEVEL_WARNING].label;
        case TFW_LOG_LEVEL_ERROR:
            return g_logLabel[TFW_LOG_LEVEL_ERROR].label;
        case TFW_LOG_LEVEL_FATAL:
            return g_logLabel[TFW_LOG_LEVEL_FATAL].label;
        default:
            return g_logLabel[TFW_LOG_LEVEL_ERROR].label;
    }
}

/**
 * 获取日志等级字符（单字母表示）
 * Get log level character (single letter representation)
 */
static TFW_UNUSED char TFW_Log_GetLevelChar(int32_t level) {
    switch (level) {
        case TFW_LOG_LEVEL_TRACE:
            return 'T';
        case TFW_LOG_LEVEL_DEBUG:
            return 'D';
        case TFW_LOG_LEVEL_INFO:
            return 'I';
        case TFW_LOG_LEVEL_WARNING:
            return 'W';
        case TFW_LOG_LEVEL_ERROR:
            return 'E';
        case TFW_LOG_LEVEL_FATAL:
            return 'F';
        default:
            return '?';
    }
}

/**
 * 获取模块名称字符串
 * Get module name string
 */
static TFW_UNUSED const char *TFW_Log_GetModuleString(int32_t module) {
    switch (module) {
        case TFW_LOG_MODULE_CORE:
            return g_moduleLabel[TFW_LOG_MODULE_CORE].name;
        case TFW_LOG_MODULE_UTILS:
            return g_moduleLabel[TFW_LOG_MODULE_UTILS].name;
        case TFW_LOG_MODULE_SDK:
            return g_moduleLabel[TFW_LOG_MODULE_SDK].name;
        case TFW_LOG_MODULE_SYSTEM:
            return g_moduleLabel[TFW_LOG_MODULE_SYSTEM].name;
        default:
            return g_moduleLabel[TFW_LOG_MODULE_SDK].name;
    }
}

/**
 * 获取文件名（去掉路径）
 * Get file name (remove path)
 * @param file_path 输入文件路径 / Input file path
 * @param filename 输出文件名缓冲区 / Output filename buffer
 * @param buffer_size 缓冲区大小 / Buffer size
 * @return TFW_SUCCESS 成功，负值表示错误 / TFW_SUCCESS on success, negative value on error
 */
static TFW_UNUSED int32_t TFW_Log_GetFileName(const char *file_path, char *filename, size_t buffer_size) {
    if (file_path == NULL || filename == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = TFW_GetFileName(file_path, filename, buffer_size);

        if (ret != TFW_SUCCESS) {
        return ret;
    }
    return TFW_SUCCESS;
}

/**
 * 输出日志到控制台
 * Output log to console
 */
static TFW_UNUSED void TFW_Log_OutputToStdout(const char *message) {
    printf("%s\n", message);
    fflush(stdout);
}

// TODO: 实现日志写入文件的功能
static TFW_UNUSED void TFW_Log_OutputToFile(const char *message) {
    printf("Log to file: %s\n", message);
    fflush(stdout);
}

// ============================================================================
// 公共接口实现
// Public interface implementation
// ============================================================================

/**
 * Log implementation function - called by macro
 * 日志实现函数 - 由宏调用
 */
int32_t TFW_LOG_IMPL(int32_t module, int32_t level, const char *file,
                     int32_t line, const char *function, const char *fmt, ...) {
    int32_t ret = TFW_SUCCESS;

    // 检查日志等级
    // Check log level
    if (level < g_logContext.logLevel) {
        return TFW_SUCCESS; // 低于当前设置的等级，不输出
    }

    // 构建日志消息
    // Build log message
    char logMessage[TFW_LOG_MAX_MESSAGE_LENGTH] = {0};
    int32_t offset = 0;

    // 添加时间戳
    // add timestamp
    char timestamp[TFW_TIMESTAMP_BUFFER_SIZE];
    if (TFW_GetTimestamp(timestamp, sizeof(timestamp)) == TFW_SUCCESS) {
        offset += snprintf(logMessage + offset, sizeof(logMessage) - offset, "%s ", timestamp);
    }

    // 添加进程ID和线程ID
    // add process id and thread id
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                        "%d:%" PRIu64 " ", TFW_GetProcessId(), TFW_GetThreadId());

    // 添加日志等级（单字母表示）
    // add log level (single letter representation)
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset, "%c ",
                        TFW_Log_GetLevelChar(level));

    // 添加模块
    // add module name
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset, "[%s] ",
                        TFW_Log_GetModuleString(module));

    // 添加文件名和行号
    // add file name and line number
    char filename[TFW_FILENAME_LEN_MAX] = {0};
    ret = TFW_Log_GetFileName(file, filename, TFW_FILENAME_LEN_MAX);
    if (ret != TFW_SUCCESS) {
        // 如果获取文件名失败，使用默认值
        // if failed to get filename, use default value
        strncpy(filename, "unknown", TFW_FILENAME_LEN_MAX - 1);
        filename[TFW_FILENAME_LEN_MAX - 1] = '\0';
    }
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                        "[%s:%d] ", filename, line);

    // 添加函数名
    // add function name
    if (function) {
        offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                        "[%s] ", function);
    }

    // 添加格式化的消息内容
    // add formatted message content
    va_list args;
    va_start(args, fmt);
    offset +=
        vsnprintf(logMessage + offset, sizeof(logMessage) - offset, fmt, args);
    va_end(args);

    // 根据配置决定是否输出日志到标准输出
    // output log to console according to configuration
    if (g_logContext.logOutputMode == TFW_LOG_OUTPUT_CONSOLE ||
        g_logContext.logOutputMode == TFW_LOG_OUTPUT_BOTH) {
        TFW_Log_OutputToStdout(logMessage);
    }

    // 根据配置决定是否输出到文件
    // output log to file according to configuration
    if (g_logContext.logOutputMode == TFW_LOG_OUTPUT_FILE ||
        g_logContext.logOutputMode == TFW_LOG_OUTPUT_BOTH) {
        TFW_Log_OutputToFile(logMessage);
    }

    return TFW_SUCCESS;
}

/**
     * 从配置文件初始化日志系统
     * Initialize log system from configuration file
     */
int32_t TFW_Log_Init(void) {
    // 如果已经初始化过，直接返回
    // if already initialized, return directly

    return TFW_SUCCESS;
}

/**
 * 退出日志系统
 */
int32_t TFW_Log_Deinit(void) {
    return TFW_SUCCESS;
}
