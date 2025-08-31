#include <stdio.h>
#include <windows.h>

#include "TFW_log.h"
#include "TFW_timer.h"
// ============================================================================
// Windows平台时间实现
// Windows platform time implementation
// ============================================================================

const char* TFW_GetTimestamp() {
    static char timestamp[64];

    // Windows平台：使用 GetSystemTime()
    // Windows platform: use GetSystemTime()
    SYSTEMTIME st;
    GetSystemTime(&st);
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour,
             st.wMinute, st.wSecond, st.wMilliseconds);

    return timestamp;
}

int64_t TFW_GetTimestampMs() {
    // Windows平台：使用 GetTickCount64() 获取毫秒级时间戳
    // Windows platform: use GetTickCount64() to get millisecond timestamp
    return (int64_t)GetTickCount64();
}

int64_t TFW_GetTimestampUs() {
    // Windows平台：使用 QueryPerformanceCounter 获取微秒级时间戳
    // Windows platform: use QueryPerformanceCounter to get microsecond timestamp
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为微秒
    // convert to microsecond
    return (int64_t)(TFW_TIME_SEC_TO_US(counter.QuadPart) / frequency.QuadPart);
}

int64_t TFW_GetTimestampNs() {
    // Windows平台：使用 QueryPerformanceCounter 获取纳秒级时间戳
    // Windows platform: use QueryPerformanceCounter to get nanosecond timestamp
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为纳秒
    // convert to nanosecond
    return (int64_t)(TFW_TIME_SEC_TO_NS(counter.QuadPart) / frequency.QuadPart);
}
