#include "../../include/TFW_timer.h"
#include "../../include/TFW_log.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

// ============================================================================
// POSIX平台时间实现
// ============================================================================

const char* TFW_GetTimestamp() {
    static char timestamp[64];
    
    // Linux/Unix平台：使用 clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t now = ts.tv_sec;
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    char ms_part[16];
    snprintf(ms_part, sizeof(ms_part), ".%03ld", ts.tv_nsec / 1000000);
    strcat(timestamp, ms_part);
    
    return timestamp;
}

int64_t TFW_GetTimestampMs() {
    // Linux/Unix平台：使用 clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

int64_t TFW_GetTimestampUs() {
    // Linux/Unix平台：使用 clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}

int64_t TFW_GetTimestampNs() {
    // Linux/Unix平台：使用 clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(ts.tv_sec * 1000000000 + ts.tv_nsec);
}
