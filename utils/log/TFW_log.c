#include "../include/TFW_log.h"
#include "../include/TFW_common_defines.h"
#include "../include/TFW_timer.h"
#include "../include/TFW_thread.h"
#include "../include/TFW_file.h"
#include "../include/TFW_config.h"
#include "TFW_log_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ============================================================================
// 全局变量
// ============================================================================
static int32_t g_logLevel = TFW_LOG_LEVEL_INFO;
static int32_t g_logInitialized = 0;
static TFW_LogOutputMode g_logOutputMode = TFW_LOG_OUTPUT_CONSOLE;

// ============================================================================
// 内部函数
// ============================================================================


/**

 * 获取日志等级字符串
 */
static TFW_UNUSED const char* TFW_Log_GetLevelString(int32_t level) {
    switch (level) {
        case TFW_LOG_LEVEL_TRACE:   return "TRACE";
        case TFW_LOG_LEVEL_DEBUG:   return "DEBUG";
        case TFW_LOG_LEVEL_INFO:    return "INFO";
        case TFW_LOG_LEVEL_WARNING: return "WARN";
        case TFW_LOG_LEVEL_ERROR:   return "ERROR";
        case TFW_LOG_LEVEL_FATAL:   return "FATAL";
        default:                     return "UNKNOWN";
    }
}

/**
 * 获取日志等级字符（单字母表示）
 */
static TFW_UNUSED char TFW_Log_GetLevelChar(int32_t level) {
    switch (level) {
        case TFW_LOG_LEVEL_TRACE:   return 'T';
        case TFW_LOG_LEVEL_DEBUG:   return 'D';
        case TFW_LOG_LEVEL_INFO:    return 'I';
        case TFW_LOG_LEVEL_WARNING: return 'W';
        case TFW_LOG_LEVEL_ERROR:   return 'E';
        case TFW_LOG_LEVEL_FATAL:   return 'F';
        default:                     return '?';
    }
}

/**
 * 获取模块名称字符串
 */
static TFW_UNUSED const char* TFW_Log_GetModuleString(int32_t module) {
    switch (module) {
        case TFW_LOG_MODULE_CORE:   return "CORE";
        case TFW_LOG_MODULE_UTILS:  return "UTILS";
        case TFW_LOG_MODULE_SDK:    return "SDK";
        case TFW_LOG_MODULE_SYSTEM: return "SYSTEM";
        default:                     return "UNKNOWN";
    }
}

/**
 * 获取文件名（去掉路径）
 */
static TFW_UNUSED const char* TFW_Log_GetFileName(const char* file_path) {
    return TFW_GetFileName(file_path);
}

/**
 * 输出日志到控制台
 * 这是一个静态函数，仅可在该文件内使用
 * 即使不被使用也不报错的编译声明
 */
static TFW_UNUSED void TFW_Log_OutputToStdout(const char* message) {
    printf("%s\n", message);
    fflush(stdout);
}

// ============================================================================
// 公共接口实现
// ============================================================================

/**
 * 日志实现函数 - 由宏调用
 */
int32_t TFW_LOG_IMPL(int32_t module, int32_t level, const char* file, int32_t line,
                  const char* function, const char* fmt, ...) {
    int32_t ret = TFW_SUCCESS;
    // 自动初始化日志系统（如果还没有初始化）
    // if (!g_logInitialized) {
    //     // 尝试从配置文件初始化日志系统
    //     ret = TFW_Log_InitializeFromConfig();
    //     if (ret != TFW_SUCCESS && ret != TFW_ERROR_ALREADY_INIT) {
    //         // 如果配置初始化失败，使用最基本的日志输出
    //         g_logLevel = TFW_LOG_LEVEL_INFO;
    //         g_logOutputMode = TFW_LOG_OUTPUT_CONSOLE;
    //         g_logInitialized = 1;
    //     } else {
    //         g_logInitialized = 1;
    //     }
    // }

    // 检查日志等级
    if (level < g_logLevel) {
        return TFW_SUCCESS; // 低于当前设置的等级，不输出
    }



    // 构建日志消息
    char logMessage[2048];
    int32_t offset = 0;

    // 添加时间戳
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "%s ", TFW_GetTimestamp());

    // 添加进程ID和线程ID
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "%d:%lu ", TFW_GetProcessId(), TFW_GetThreadId());

    // 添加日志等级（单字母表示）
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "%c ", TFW_Log_GetLevelChar(level));

    // 添加模块
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[%s] ", TFW_Log_GetModuleString(module));

    // 添加文件名和行号
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[%s:%d] ", TFW_Log_GetFileName(file), line);

    // 添加函数名
    if (function) {
        offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                          "[%s] ", function);
    }

    // 添加格式化的消息内容
    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(logMessage + offset, sizeof(logMessage) - offset, fmt, args);
    va_end(args);

    // 输出日志到标准输出
    if (g_logOutputMode == TFW_LOG_OUTPUT_CONSOLE || g_logOutputMode == TFW_LOG_OUTPUT_BOTH) {
        TFW_Log_OutputToStdout(logMessage);
    }

    // 根据配置决定是否输出到文件
    if (g_logOutputMode == TFW_LOG_OUTPUT_FILE || g_logOutputMode == TFW_LOG_OUTPUT_BOTH) {
        TFW_Log_WriteToFile(logMessage);
    }

    return TFW_SUCCESS;
}

/**
 * 从配置文件初始化日志系统
 */
int32_t TFW_Log_InitializeFromConfig(void) {
    // 如果已经初始化过，直接返回
    if (g_logInitialized) {
        return TFW_ERROR_ALREADY_INIT;
    }

    // 读取日志配置
    char logLevel[32];
    char logOutput[32];
    char logFilePath[512];
    char logFilePrefix[64];
    char maxFileSizeStr[32];
    char maxRetentionDaysStr[32];

    // 设置默认值
    strcpy(logLevel, "I");
    strcpy(logOutput, "console");
    strcpy(logFilePath, "runtime/logs");
    strcpy(logFilePrefix, "TFW");
    strcpy(maxFileSizeStr, "10485760");
    strcpy(maxRetentionDaysStr, "30");

    // 尝试从配置管理器获取配置（如果配置管理器未准备好，使用默认值）
    int32_t configResult = TFW_Config_GetString(TFW_CONFIG_LOGGING_LEVEL, logLevel, sizeof(logLevel), "I");
    if (configResult == TFW_SUCCESS) {
        if (strcmp(logLevel, "D") == 0) {
            g_logLevel = TFW_LOG_LEVEL_DEBUG;
        } else if (strcmp(logLevel, "I") == 0) {
            g_logLevel = TFW_LOG_LEVEL_INFO;
        } else if (strcmp(logLevel, "W") == 0) {
            g_logLevel = TFW_LOG_LEVEL_WARNING;
        } else if (strcmp(logLevel, "E") == 0) {
            g_logLevel = TFW_LOG_LEVEL_ERROR;
        } else if (strcmp(logLevel, "F") == 0) {
            g_logLevel = TFW_LOG_LEVEL_FATAL;
        } else {
            g_logLevel = TFW_LOG_LEVEL_INFO; // 默认INFO级别
        }
    } else {
        printf("TFW_Config_GetString failed: TFW_CONFIG_LOGGING_LEVEL = %s\n", logLevel);
        g_logLevel = TFW_LOG_LEVEL_INFO;
    }

    // 获取输出模式
    configResult = TFW_Config_GetString(TFW_CONFIG_LOGGING_OUTPUT, logOutput, sizeof(logOutput), "console");
    if (configResult == TFW_SUCCESS) {
        if (strcmp(logOutput, "file") == 0) {
            printf("TFW_Log_InitializeFromConfig: logOutput = file\n");
            g_logOutputMode = TFW_LOG_OUTPUT_FILE;
        } else if (strcmp(logOutput, "both") == 0) {
            printf("TFW_Log_InitializeFromConfig: logOutput = both\n");
            g_logOutputMode = TFW_LOG_OUTPUT_BOTH;
        } else {
            printf("TFW_Log_InitializeFromConfig: logOutput = console\n");
            g_logOutputMode = TFW_LOG_OUTPUT_CONSOLE;
        }
    } else {
        printf("TFW_Config_GetString failed: TFW_CONFIG_LOGGING_OUTPUT = %s\n", logOutput);
        g_logOutputMode = TFW_LOG_OUTPUT_CONSOLE;
    }

    // 如果启用文件输出，初始化文件输出
    if (g_logOutputMode == TFW_LOG_OUTPUT_FILE || g_logOutputMode == TFW_LOG_OUTPUT_BOTH) {
        // 获取文件路径
        configResult = TFW_Config_GetString(TFW_CONFIG_LOGGING_FILE_PATH, logFilePath, sizeof(logFilePath), "runtime/logs");
        if (configResult == TFW_SUCCESS) {
            // 获取文件前缀
            configResult = TFW_Config_GetString(TFW_CONFIG_LOGGING_FILE_PREFIX, logFilePrefix, sizeof(logFilePrefix), "TFW");
            if (configResult == TFW_SUCCESS) {
                // 获取最大文件大小
                size_t maxFileSize = 10485760; // 默认10MB
                configResult = TFW_Config_GetString(TFW_CONFIG_LOGGING_MAX_FILE_SIZE, maxFileSizeStr, sizeof(maxFileSizeStr), "10485760");
                if (configResult == TFW_SUCCESS) {
                    maxFileSize = (size_t)atoi(maxFileSizeStr);
                }

                // 获取最大保留天数
                int maxRetentionDays = 30; // 默认30天
                configResult = TFW_Config_GetString(TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS, maxRetentionDaysStr, sizeof(maxRetentionDaysStr), "30");
                if (configResult == TFW_SUCCESS) {
                    maxRetentionDays = atoi(maxRetentionDaysStr);
                }

                // 初始化文件输出
                int32_t result = TFW_Log_InitFileOutput(logFilePath, logFilePrefix, maxFileSize, maxRetentionDays);
                if (result != TFW_SUCCESS) {
                    // 文件输出初始化失败，回退到仅控制台输出
                    g_logOutputMode = TFW_LOG_OUTPUT_CONSOLE;
                }
            }
        }
    }

    g_logInitialized = 1;
    return TFW_SUCCESS;
}

/**
 * 退出日志系统
 */
int32_t TFW_Log_Exit(void) {
    if (!g_logInitialized) {
        return TFW_ERROR_NOT_INIT;
    }

    // 关闭文件输出
    if (g_logOutputMode == TFW_LOG_OUTPUT_FILE || g_logOutputMode == TFW_LOG_OUTPUT_BOTH) {
        TFW_Log_CloseFileOutput();
    }

    g_logInitialized = 0;
    return TFW_SUCCESS;
}
