#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "TFW_log.h"
#include "TFW_timer.h"
#include "TFW_errorno.h"
#include "TFW_mem.h"
#include "TFW_common_defines.h"

// ============================================================================
// macOS平台时间实现
// macOS platform time implementation
// ============================================================================

int32_t TFW_GetTimestamp(char* timestamp, size_t buffer_size) {
    if (timestamp == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // macOS平台：使用 gettimeofday() 确保兼容性
    // macOS platform: use gettimeofday() to ensure compatibility
    struct timeval tv;
    struct timezone tz;
    if (gettimeofday(&tv, &tz) != 0) {
        return TFW_ERROR;
    }

    time_t now = tv.tv_sec;
    struct tm* tm_info = localtime(&now);
    if (tm_info == NULL) {
        return TFW_ERROR;
    }

    // Format timestamp: YYYY-MM-DD HH:MM:SS.mmm
    // 格式化时间戳：YYYY-MM-DD HH:MM:SS.mmm
    char time_str[TFW_TIMESTAMP_LEN_MAX];
    if (strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info) == 0) {
        return TFW_ERROR;
    }

    char ms_part[16];  // ".mmm" + null terminator = 5 characters, 16 is safe
    snprintf(ms_part, sizeof(ms_part), ".%03ld", TFW_TIME_NS_TO_MS(tv.tv_usec));

    // Check if the combined string fits in the buffer
    // 检查组合字符串是否适合缓冲区
    size_t time_str_len = strlen(time_str);
    size_t ms_part_len = strlen(ms_part);
    size_t total_len = time_str_len + ms_part_len + 1;  // +1 for null terminator
    if (total_len > buffer_size) {
        return TFW_ERROR_INVALID_PARAM;  // Buffer too small
    }

    // Use TFW safe string functions
    // 使用TFW安全字符串函数
    if (TFW_Strcpy_S(timestamp, buffer_size, time_str) != TFW_SUCCESS) {
        return TFW_ERROR;
    }
    if (TFW_Strcat_S(timestamp, buffer_size, ms_part) != TFW_SUCCESS) {
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

uint64_t TFW_GetTimestampMs() {
    // macOS平台：使用 clock_gettime() 获取单调时间
    // macOS platform: use clock_gettime() to get monotonic time
    struct timespec ts;
#ifdef CLOCK_MONOTONIC
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (uint64_t)(TFW_TIME_SEC_TO_MS(ts.tv_sec) + TFW_TIME_NS_TO_MS(ts.tv_nsec));
    }
#endif

    // 回退方案：使用 gettimeofday()，但注意这仍然不是单调时间
    // fallback: use gettimeofday(), but note this is still not monotonic time
    // 在不支持CLOCK_MONOTONIC的系统上，此实现可能受系统时间调整影响
    // On systems that do not support CLOCK_MONOTONIC, this implementation may be affected by system time adjustments
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (uint64_t)(TFW_TIME_SEC_TO_MS(tv.tv_sec) + TFW_TIME_NS_TO_MS(tv.tv_usec));
}

uint64_t TFW_GetTimestampUs() {
    // macOS平台：使用 clock_gettime() 获取单调时间
    // macOS platform: use clock_gettime() to get monotonic time
    struct timespec ts;
#ifdef CLOCK_MONOTONIC
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (uint64_t)(TFW_TIME_SEC_TO_US(ts.tv_sec) + TFW_TIME_NS_TO_MS(ts.tv_nsec));
    }
#endif

    // 回退方案：使用 gettimeofday()，但注意这仍然不是单调时间
    // fallback: use gettimeofday(), but note this is still not monotonic time
    // 在不支持CLOCK_MONOTONIC的系统上，此实现可能受系统时间调整影响
    // On systems that do not support CLOCK_MONOTONIC, this implementation may be affected by system time adjustments
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (uint64_t)(TFW_TIME_SEC_TO_US(tv.tv_sec) + TFW_TIME_NS_TO_MS(tv.tv_usec));
}

uint64_t TFW_GetTimestampNs() {
    // macOS平台：使用 clock_gettime() 获取单调时间
    // macOS platform: use clock_gettime() to get monotonic time
#ifdef CLOCK_MONOTONIC
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (uint64_t)(TFW_TIME_SEC_TO_NS(ts.tv_sec) + ts.tv_nsec);
    }
#endif

    // 回退方案：使用 gettimeofday()，但注意这仍然不是单调时间
    // fallback: use gettimeofday(), but note this is still not monotonic time
    // 在不支持CLOCK_MONOTONIC的系统上，此实现可能受系统时间调整影响
    // On systems that do not support CLOCK_MONOTONIC, this implementation may be affected by system time adjustments
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (uint64_t)(TFW_TIME_SEC_TO_NS(tv.tv_sec) + TFW_TIME_NS_TO_MS(tv.tv_usec));
}