#include "../include/TFW_log.h"
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
#endif

// ============================================================================
// 鍏ㄥ眬鍙橀噺
// ============================================================================
static int g_logLevel = TFW_LOG_LEVEL_INFO;
static int g_logInitialized = 0;

// ============================================================================
// 鍐呴儴鍑芥暟
// ============================================================================

/**
 * 鑾峰彇褰撳墠鏃堕棿鎴冲瓧绗︿覆
 */
static const char* get_timestamp() {
    static char timestamp[32];
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    return timestamp;
}

/**
 * 鑾峰彇褰撳墠杩涚▼ID
 */
static int get_process_id() {
#ifdef _WIN32
    return _getpid();
#else
    return getpid();
#endif
}

/**
 * 鑾峰彇褰撳墠绾跨▼ID
 */
static unsigned long get_thread_id() {
#ifdef _WIN32
    return (unsigned long)GetCurrentThreadId();
#else
    return (unsigned long)pthread_self();
#endif
}

/**
 * 鑾峰彇鏃ュ織绛夌骇瀛楃涓? */
static const char* get_level_string(int level) {
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
 * 鑾峰彇妯″潡鍚嶇О瀛楃涓? */
static const char* get_module_string(int module) {
    switch (module) {
        case TFW_LOG_MODULE_CORE:   return "CORE";
        case TFW_LOG_MODULE_UTILS:  return "UTILS";
        case TFW_LOG_MODULE_SDK:    return "SDK";
        case TFW_LOG_MODULE_SYSTEM: return "SYSTEM";
        default:                     return "UNKNOWN";
    }
}

/**
 * 鑾峰彇鏂囦欢鍚嶏紙鍘绘帀璺緞锛? */
static const char* get_file_name(const char* file_path) {
    const char* filename = strrchr(file_path, '/');
    return filename ? filename + 1 : file_path;
}

/**
 * 杈撳嚭鏃ュ織鍒版帶鍒跺彴
 */
static void output_log(const char* message) {
    printf("%s\n", message);
    fflush(stdout);
}

// ============================================================================
// 鍏叡鎺ュ彛瀹炵幇
// ============================================================================

/**
 * 鏃ュ織瀹炵幇鍑芥暟 - 鐢卞畯璋冪敤
 */
int TFW_LOG_IMPL(int module, int level, const char* file, int line,
                  const char* function, const char* fmt, ...) {
    if (!g_logInitialized) {
        return -1; // 鏈垵濮嬪寲
    }

    // 妫€鏌ユ棩蹇楃瓑绾?    if (level < g_logLevel) {
        return 0; // 浣庝簬褰撳墠璁剧疆鐨勭瓑绾э紝涓嶈緭鍑?    }

    // 鏋勫缓鏃ュ織娑堟伅
    char logMessage[2048];
    int offset = 0;

    // 娣诲姞鏃堕棿鎴?    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[%s] ", get_timestamp());

    // 娣诲姞杩涚▼ID鍜岀嚎绋婭D
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[PID:%d][TID:%lu] ", get_process_id(), get_thread_id());

    // 娣诲姞妯″潡鍜岀瓑绾?    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[%s][%s] ", get_module_string(module), get_level_string(level));

    // 娣诲姞鏂囦欢鍚嶅拰琛屽彿
    offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                      "[%s:%d] ", get_file_name(file), line);

    // 娣诲姞鍑芥暟鍚?    if (function) {
        offset += snprintf(logMessage + offset, sizeof(logMessage) - offset,
                          "[%s] ", function);
    }

    // 娣诲姞鏍煎紡鍖栫殑娑堟伅鍐呭
    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(logMessage + offset, sizeof(logMessage) - offset, fmt, args);
    va_end(args);

    // 杈撳嚭鏃ュ織
    output_log(logMessage);

    return 0;
}

/**
 * 鍒濆鍖栨棩蹇楃郴缁? */
int TFW_LOG_INITIALIZE() {
    if (g_logInitialized) {
        return -1; // 宸茬粡鍒濆鍖?    }

    g_logInitialized = 1;
    return 0;
}

/**
 * 璁剧疆鏃ュ織绛夌骇
 */
int TFW_LOG_SET_LEVEL(int level) {
    if (!g_logInitialized) {
        return -1; // 鏈垵濮嬪寲
    }

    if (level < TFW_LOG_LEVEL_TRACE || level > TFW_LOG_LEVEL_FATAL) {
        return -2; // 鏃犳晥鐨勬棩蹇楃瓑绾?    }

    g_logLevel = level;
    return 0;
}

/**
 * 閫€鍑烘棩蹇楃郴缁? */
int TFW_LOG_EXIT() {
    if (!g_logInitialized) {
        return -1; // 鏈垵濮嬪寲
    }

    g_logInitialized = 0;
    return 0;
}
