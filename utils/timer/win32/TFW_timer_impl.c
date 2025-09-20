#include <stdio.h>
#include <windows.h>

#include "TFW_log.h"
#include "TFW_timer.h"
#include "TFW_errorno.h"
#include "TFW_mem.h"
// ============================================================================
// Windows平台时间实现
// Windows platform time implementation
// ============================================================================

int32_t TFW_GetTimestamp(char* timestamp, size_t buffer_size) {
    if (timestamp == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：使用 GetSystemTime()
    // Windows platform: use GetSystemTime()
    SYSTEMTIME st;
    GetSystemTime(&st);

    // Format timestamp: YYYY-MM-DD HH:MM:SS.mmm
    // 格式化时间戳：YYYY-MM-DD HH:MM:SS.mmm
    int ret = sprintf_s(timestamp, buffer_size, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                       st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    if (ret < 0 || (size_t)ret >= buffer_size) {
        return TFW_ERROR_INVALID_PARAM;  // Buffer too small or formatting error
    }

    return TFW_SUCCESS;
}

uint64_t TFW_GetTimestampMs() {
    // Windows平台：使用 QueryPerformanceCounter 获取单调时间
    // Windows platform: use QueryPerformanceCounter to get monotonic time
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为毫秒
    // convert to millisecond
    return (uint64_t)(TFW_TIME_SEC_TO_MS(counter.QuadPart) / frequency.QuadPart);
}

uint64_t TFW_GetTimestampUs() {
    // Windows平台：使用 QueryPerformanceCounter 获取单调时间
    // Windows platform: use QueryPerformanceCounter to get monotonic time
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为微秒
    // convert to microsecond
    return (uint64_t)(TFW_TIME_SEC_TO_US(counter.QuadPart) / frequency.QuadPart);
}

uint64_t TFW_GetTimestampNs() {
    // Windows平台：使用 QueryPerformanceCounter 获取单调时间
    // Windows platform: use QueryPerformanceCounter to get monotonic time
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为纳秒
    // convert to nanosecond
    return (uint64_t)(TFW_TIME_SEC_TO_NS(counter.QuadPart) / frequency.QuadPart);
}