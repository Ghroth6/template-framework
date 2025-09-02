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

int64_t TFW_GetTimestampMs() {
    // macOS平台：使用 gettimeofday()
    // macOS platform: use gettimeofday()
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (int64_t)(TFW_TIME_SEC_TO_MS(tv.tv_sec) + TFW_TIME_NS_TO_MS(tv.tv_usec));
}

int64_t TFW_GetTimestampUs() {
    // macOS平台：使用 gettimeofday()
    // macOS platform: use gettimeofday()
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (int64_t)(TFW_TIME_SEC_TO_US(tv.tv_sec) + TFW_TIME_NS_TO_MS(tv.tv_usec));
}

int64_t TFW_GetTimestampNs() {
    // macOS平台：尝试使用 clock_gettime()，如果不可用则回退到 gettimeofday()
    // macOS platform: try to use clock_gettime(), if not available, fall back to gettimeofday()
#ifdef CLOCK_REALTIME
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
        return (int64_t)(TFW_TIME_SEC_TO_NS(ts.tv_sec) + ts.tv_nsec);
    }
#endif

    // 回退到 gettimeofday()，转换为纳秒
    // fallback to gettimeofday(), convert to nanoseconds
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (int64_t)(TFW_TIME_SEC_TO_NS(tv.tv_sec) + TFW_TIME_NS_TO_MS(tv.tv_usec));
}
