#include "../include/TFW_log.h"
#include "../include/TFW_common_defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif

// ============================================================================
// 全局变量
// ============================================================================
static int g_logLevel = TFW_LOG_LEVEL_INFO;
static int g_logInitialized = 0;

// ============================================================================
// 内部函数
// ============================================================================

/**
 * 获取当前时间戳字符串
 */
static const char* get_timestamp() {
    static char timestamp[64];
    
#ifdef _WIN32
    // Windows平台
    SYSTEMTIME st;
    GetSystemTime(&st);
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#elif defined(__APPLE__)
    // macOS平台
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    time_t now = tv.tv_sec;
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    char ms_part[16];
    snprintf(ms_part, sizeof(ms_part), ".%03ld", tv.tv_usec / 1000);
    strcat(timestamp, ms_part);
#else
    // Linux/Unix平台
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t now = ts.tv_sec;
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    char ms_part[16];
    snprintf(ms_part, sizeof(ms_part), ".%03ld", ts.tv_nsec / 1000000);
    strcat(timestamp, ms_part);
#endif
    
    return timestamp;
}

/**
 * 获取当前进程ID
 */
static int get_process_id() {
#ifdef _WIN32
    return _getpid();
#else
    return getpid();
#endif
}

/**
 * 获取当前线程ID
 */
static unsigned long get_thread_id() {
#ifdef _WIN32
    return (unsigned long)GetCurrentThreadId();
#else
    return (unsigned long)pthread_self();
#endif
}

/**
 * 获取日志等级字符串
 */
static TFW_UNUSED const char* get_level_string(int level) {
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
static TFW_UNUSED char get_level_char(int level) {
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
static TFW_UNUSED const char* get_module_string(int module) {
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
static TFW_UNUSED const char* get_file_name(const char* file_path) {
    const char* filename = strrchr(file_path, '/');
    return filename ? filename + 1 : file_path;
}

/**
 * 输出日志到控制台
 * 这是一个静态函数，仅可在该文件内使用
 * 即使不被使用也不报错的编译声明
 */
static TFW_UNUSED void output_log_to_stdout(const char* message) {
    printf("%s\n", message);
    fflush(stdout);
}

// ============================================================================
// 公共接口实现
// ============================================================================

/**
 * 日志实现函数 - 由宏调用
 */
int TFW_LOG_IMPL(int module, int level, const char* file, int line,
                  const char* function, const char* fmt, ...) {
    // 自动初始化日志系统（如果还没有初始化）
    if (!g_logInitialized) {
        g_logInitialized = 1;
        g_logLevel = TFW_LOG_LEVEL_INFO; // 默认设置为INFO级别
    }

    // 检查日志等级
    if (level < g_logLevel) {
        return 0; // 低于当前设置的等级，不输出
    }
    


    // 构建日志消息
    char logMessage[2048];
    int offset = 0;

    // 添加时间戳
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "%s ", get_timestamp());

    // 添加进程ID和线程ID
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "%d:%lu ", get_process_id(), get_thread_id());

    // 添加日志等级（单字母表示）
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "%c ", get_level_char(level));

    // 添加模块
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[%s] ", get_module_string(module));

    // 添加文件名和行号
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[%s:%d] ", get_file_name(file), line);

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
    output_log_to_stdout(logMessage);

    return 0;
}

/**
 * 初始化日志系统
 */
int TFW_LOG_INITIALIZE() {
    if (g_logInitialized) {
        return -1; // 已经初始化
    }

    g_logInitialized = 1;
    return 0;
}

/**
 * 设置日志等级
 */
int TFW_LOG_SET_LEVEL(int level) {
    if (!g_logInitialized) {
        return -1; // 未初始化
    }

    if (level < TFW_LOG_LEVEL_TRACE || level > TFW_LOG_LEVEL_FATAL) {
        return -2; // 无效的日志等级
    }

    g_logLevel = level;
    return 0;
}

/**
 * 退出日志系统
 */
int TFW_LOG_EXIT() {
    if (!g_logInitialized) {
        return -1; // 未初始化
    }

    g_logInitialized = 0;
    return 0;
}
