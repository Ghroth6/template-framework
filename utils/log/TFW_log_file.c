#include "TFW_log_internal.h"
#include "../include/TFW_thread.h"
#include "../include/TFW_file.h"
#include "../include/TFW_timer.h"
#include "../include/TFW_mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// 日志文件管理器
static struct {
    bool initialized;
    TFW_LogOutputMode outputMode;
    char logDir[512];
    char logPrefix[64];
    FILE* currentLogFile;
    char currentDate[11];  // YYYY-MM-DD
    size_t maxFileSize;
    int32_t maxRetentionDays;
    TFW_Mutex_t fileMutex;
    size_t currentFileSize;
} g_logFileManager = {0};

// 内部函数声明
static int32_t CreateLogDirectory(const char* logDir);
static int32_t OpenLogFile(const char* date);
static int32_t CloseCurrentLogFile(void);
static int32_t RotateLogFile(void);
static int32_t GetDateString(char* dateStr, size_t size);
static int32_t ParseDateString(const char* dateStr, int32_t* year, int32_t* month, int32_t* day);
static int32_t IsDateOlderThan(const char* dateStr, int32_t days);

// 初始化日志文件输出
int32_t TFW_Log_InitFileOutput(const char* logDir, const char* logPrefix, size_t maxFileSize, int32_t maxRetentionDays) {
    if (g_logFileManager.initialized) {
        return TFW_ERROR_ALREADY_INIT;
    }
    
    if (!logDir || !logPrefix) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    // 初始化互斥锁
    TFW_MutexAttr_t mutexAttr;
    if (TFW_MutexAttr_Init(&mutexAttr) != TFW_SUCCESS) {
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    if (TFW_Mutex_Init(&g_logFileManager.fileMutex, &mutexAttr) != TFW_SUCCESS) {
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    // 设置配置
    strncpy(g_logFileManager.logDir, logDir, sizeof(g_logFileManager.logDir) - 1);
    g_logFileManager.logDir[sizeof(g_logFileManager.logDir) - 1] = '\0';
    
    strncpy(g_logFileManager.logPrefix, logPrefix, sizeof(g_logFileManager.logPrefix) - 1);
    g_logFileManager.logPrefix[sizeof(g_logFileManager.logPrefix) - 1] = '\0';
    
    g_logFileManager.maxFileSize = maxFileSize;
    g_logFileManager.maxRetentionDays = maxRetentionDays;
    g_logFileManager.outputMode = TFW_LOG_OUTPUT_FILE;
    
    // 创建日志目录
    if (CreateLogDirectory(logDir) != TFW_SUCCESS) {
        TFW_Mutex_Destroy(&g_logFileManager.fileMutex);
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    // 获取当前日期并打开日志文件
    if (GetDateString(g_logFileManager.currentDate, sizeof(g_logFileManager.currentDate)) != TFW_SUCCESS) {
        TFW_Mutex_Destroy(&g_logFileManager.fileMutex);
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    if (OpenLogFile(g_logFileManager.currentDate) != TFW_SUCCESS) {
        TFW_Mutex_Destroy(&g_logFileManager.fileMutex);
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    g_logFileManager.initialized = true;
    return TFW_SUCCESS;
}

// 关闭日志文件输出
int32_t TFW_Log_CloseFileOutput(void) {
    if (!g_logFileManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    TFW_Mutex_Lock(&g_logFileManager.fileMutex);
    
    if (g_logFileManager.currentLogFile) {
        CloseCurrentLogFile();
    }
    
    TFW_Mutex_Unlock(&g_logFileManager.fileMutex);
    TFW_Mutex_Destroy(&g_logFileManager.fileMutex);
    
    g_logFileManager.initialized = false;
    return TFW_SUCCESS;
}

// 设置输出模式
int32_t TFW_Log_SetOutputMode(TFW_LogOutputMode mode) {
    if (!g_logFileManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    if (mode < TFW_LOG_OUTPUT_CONSOLE || mode > TFW_LOG_OUTPUT_BOTH) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    g_logFileManager.outputMode = mode;
    return TFW_SUCCESS;
}

// 获取输出模式
int32_t TFW_Log_GetOutputMode(TFW_LogOutputMode* mode) {
    if (!g_logFileManager.initialized || !mode) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    *mode = g_logFileManager.outputMode;
    return TFW_SUCCESS;
}

// 写入日志文件
int32_t TFW_Log_WriteToFile(const char* message) {
    if (!g_logFileManager.initialized || !message) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    if (g_logFileManager.outputMode == TFW_LOG_OUTPUT_CONSOLE) {
        return TFW_SUCCESS; // 不需要写入文件
    }
    
    TFW_Mutex_Lock(&g_logFileManager.fileMutex);
    
    // 检查是否需要轮转日志文件
    if (TFW_Log_CheckAndRotate() != TFW_SUCCESS) {
        TFW_Mutex_Unlock(&g_logFileManager.fileMutex);
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    // 写入日志消息
    if (g_logFileManager.currentLogFile) {
        size_t messageLen = strlen(message);
        size_t written = fwrite(message, 1, messageLen, g_logFileManager.currentLogFile);
        
        if (written == messageLen) {
            // 添加换行符
            fputc('\n', g_logFileManager.currentLogFile);
            fflush(g_logFileManager.currentLogFile);
            
            g_logFileManager.currentFileSize += written + 1;
        }
    }
    
    TFW_Mutex_Unlock(&g_logFileManager.fileMutex);
    return TFW_SUCCESS;
}

// 检查并轮转日志文件
int32_t TFW_Log_CheckAndRotate(void) {
    if (!g_logFileManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    char currentDate[11];
    if (GetDateString(currentDate, sizeof(currentDate)) != TFW_SUCCESS) {
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    // 检查日期是否变化
    if (strcmp(currentDate, g_logFileManager.currentDate) != 0) {
        // 日期变化，轮转日志文件
        if (RotateLogFile() != TFW_SUCCESS) {
            return TFW_ERROR_OPERATION_FAIL;
        }
        
        strncpy(g_logFileManager.currentDate, currentDate, sizeof(g_logFileManager.currentDate) - 1);
        g_logFileManager.currentDate[sizeof(g_logFileManager.currentDate) - 1] = '\0';
        
        if (OpenLogFile(currentDate) != TFW_SUCCESS) {
            return TFW_ERROR_OPERATION_FAIL;
        }
    }
    
    // 检查文件大小
    if (g_logFileManager.currentFileSize >= g_logFileManager.maxFileSize) {
        // 文件大小超限，轮转日志文件
        if (RotateLogFile() != TFW_SUCCESS) {
            return TFW_ERROR_OPERATION_FAIL;
        }
        
        if (OpenLogFile(currentDate) != TFW_SUCCESS) {
            return TFW_ERROR_OPERATION_FAIL;
        }
    }
    
    return TFW_SUCCESS;
}

// 清理旧日志文件
int32_t TFW_Log_CleanupOldLogs(void) {
    if (!g_logFileManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    // 这里可以实现清理逻辑，暂时返回成功
    // TODO: 实现清理超过maxRetentionDays的日志文件
    return TFW_SUCCESS;
}

// 获取日志文件配置
int32_t TFW_Log_GetFileConfig(TFW_LogFileConfig* config) {
    if (!g_logFileManager.initialized || !config) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    config->outputMode = g_logFileManager.outputMode;
    strncpy(config->logDir, g_logFileManager.logDir, sizeof(config->logDir) - 1);
    config->logDir[sizeof(config->logDir) - 1] = '\0';
    
    strncpy(config->logPrefix, g_logFileManager.logPrefix, sizeof(config->logPrefix) - 1);
    config->logPrefix[sizeof(config->logPrefix) - 1] = '\0';
    
    config->maxFileSize = g_logFileManager.maxFileSize;
    config->maxRetentionDays = g_logFileManager.maxRetentionDays;
    
    return TFW_SUCCESS;
}

// 获取当前日志文件路径
int32_t TFW_Log_GetCurrentLogFile(char* filePath, size_t filePathSize) {
    if (!g_logFileManager.initialized || !filePath) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    snprintf(filePath, filePathSize, "%s/%s_%s.log", 
             g_logFileManager.logDir, 
             g_logFileManager.logPrefix, 
             g_logFileManager.currentDate);
    
    return TFW_SUCCESS;
}

// 内部函数实现

// 创建日志目录
static int32_t CreateLogDirectory(const char* logDir) {
    if (TFW_FileExists(logDir)) {
        return TFW_SUCCESS;
    }
    
    if (TFW_CreateDirectory(logDir)) {
        return TFW_SUCCESS;
    }
    
    return TFW_ERROR_OPERATION_FAIL;
}

// 打开日志文件
static int32_t OpenLogFile(const char* date) {
    char filePath[512];
    snprintf(filePath, sizeof(filePath), "%s/%s_%s.log", 
             g_logFileManager.logDir, 
             g_logFileManager.logPrefix, 
             date);
    
    // 关闭当前文件
    CloseCurrentLogFile();
    
    // 打开新文件
    g_logFileManager.currentLogFile = fopen(filePath, "a");
    if (!g_logFileManager.currentLogFile) {
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    // 获取文件大小
    struct stat st;
    if (stat(filePath, &st) == 0) {
        g_logFileManager.currentFileSize = st.st_size;
    } else {
        g_logFileManager.currentFileSize = 0;
    }
    
    return TFW_SUCCESS;
}

// 关闭当前日志文件
static int32_t CloseCurrentLogFile(void) {
    if (g_logFileManager.currentLogFile) {
        fclose(g_logFileManager.currentLogFile);
        g_logFileManager.currentLogFile = NULL;
        g_logFileManager.currentFileSize = 0;
    }
    return TFW_SUCCESS;
}

// 轮转日志文件
static int32_t RotateLogFile(void) {
    if (!g_logFileManager.currentLogFile) {
        return TFW_SUCCESS;
    }
    
    // 关闭当前文件
    CloseCurrentLogFile();
    
    // 清理旧日志文件
    TFW_Log_CleanupOldLogs();
    
    return TFW_SUCCESS;
}

// 获取日期字符串
static int32_t GetDateString(char* dateStr, size_t size) {
    if (!dateStr || size < 11) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    snprintf(dateStr, size, "%04d-%02d-%02d", 
             tm_info->tm_year + 1900,
             tm_info->tm_mon + 1,
             tm_info->tm_mday);
    
    return TFW_SUCCESS;
}

// 解析日期字符串
static int32_t ParseDateString(const char* dateStr, int32_t* year, int32_t* month, int32_t* day) {
    if (!dateStr || !year || !month || !day) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    if (sscanf(dateStr, "%d-%d-%d", year, month, day) != 3) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    return TFW_SUCCESS;
}

// 检查日期是否超过指定天数
static int32_t IsDateOlderThan(const char* dateStr, int32_t days) {
    int32_t year, month, day;
    if (ParseDateString(dateStr, &year, &month, &day) != TFW_SUCCESS) {
        return false;
    }
    
    time_t targetTime = 0;
    struct tm target_tm = {0};
    target_tm.tm_year = year - 1900;
    target_tm.tm_mon = month - 1;
    target_tm.tm_mday = day;
    targetTime = mktime(&target_tm);
    
    time_t now = time(NULL);
    time_t diff = now - targetTime;
    
    return (diff > (days * 24 * 3600));
}
