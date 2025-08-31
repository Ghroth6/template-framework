#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "TFW_log.h"
#include "TFW_timer.h"

// ============================================================================
// macOS平台时间实现
// macOS platform time implementation
// ============================================================================

const char* TFW_GetTimestamp() {
    static char timestamp[64];

    // macOS平台：使用 gettimeofday() 确保兼容性
    // macOS platform: use gettimeofday() to ensure compatibility
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    time_t now = tv.tv_sec;
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    char ms_part[16];
    snprintf(ms_part, sizeof(ms_part), ".%03ld", TFW_TIME_NS_TO_MS(tv.tv_usec));
    strcat(timestamp, ms_part);

    return timestamp;
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
