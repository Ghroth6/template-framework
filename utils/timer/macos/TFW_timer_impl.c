#include "../../include/TFW_timer.h"
#include "../../include/TFW_log.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

// ============================================================================
// macOS平台时间实现
// ============================================================================

const char* TFW_GetTimestamp() {
    static char timestamp[64];
    
    // macOS平台：使用 gettimeofday() 确保兼容性
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    time_t now = tv.tv_sec;
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    char ms_part[16];
    snprintf(ms_part, sizeof(ms_part), ".%03ld", tv.tv_usec / 1000);
    strcat(timestamp, ms_part);
    
    return timestamp;
}

int64_t TFW_GetTimestampMs() {
    // macOS平台：使用 gettimeofday()
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (int64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int64_t TFW_GetTimestampUs() {
    // macOS平台：使用 gettimeofday()
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (int64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
}

int64_t TFW_GetTimestampNs() {
    // macOS平台：尝试使用 clock_gettime()，如果不可用则回退到 gettimeofday()
    #ifdef CLOCK_REALTIME
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
            return (int64_t)(ts.tv_sec * 1000000000 + ts.tv_nsec);
        }
    #endif
    
    // 回退到 gettimeofday()，转换为纳秒
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (int64_t)(tv.tv_sec * 1000000000 + tv.tv_usec * 1000);
}
