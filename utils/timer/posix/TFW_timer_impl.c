#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "TFW_log.h"
#include "TFW_timer.h"
// ============================================================================
// POSIX平台时间实现
// POSIX platform time implementation
// ============================================================================

const char* TFW_GetTimestamp() {
    static char timestamp[64];

    // Linux/Unix平台：使用 clock_gettime()
    // Linux/Unix platform: use clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t now = ts.tv_sec;
    struct tm* tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    char ms_part[16];
    snprintf(ms_part, sizeof(ms_part), ".%03ld", TFW_TIME_NS_TO_MS(ts.tv_nsec));
    strcat(timestamp, ms_part);

    return timestamp;
}

int64_t TFW_GetTimestampMs() {
    // Linux/Unix平台：使用 clock_gettime()
    // Linux/Unix platform: use clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(TFW_TIME_SEC_TO_MS(ts.tv_sec) + TFW_TIME_NS_TO_MS(ts.tv_nsec));
}

int64_t TFW_GetTimestampUs() {
    // Linux/Unix平台：使用 clock_gettime()
    // Linux/Unix platform: use clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(TFW_TIME_SEC_TO_US(ts.tv_sec) + TFW_TIME_NS_TO_MS(ts.tv_nsec));
}

int64_t TFW_GetTimestampNs() {
    // Linux/Unix平台：使用 clock_gettime()
    // Linux/Unix platform: use clock_gettime()
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(TFW_TIME_SEC_TO_NS(ts.tv_sec) + ts.tv_nsec);
}
