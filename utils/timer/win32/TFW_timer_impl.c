#include <stdio.h>
#include <windows.h>

#include "TFW_log.h"
#include "TFW_timer.h"
#include "TFW_errorno.h"
#include "TFW_mem.h"
#include "TFW_utils_log.h"
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

int64_t TFW_GetTimestampMs() {
    // Windows平台：使用 QueryPerformanceCounter 获取单调时间
    // Windows platform: use QueryPerformanceCounter to get monotonic time
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为毫秒
    // convert to millisecond
    return (int64_t)(TFW_TIME_SEC_TO_MS(counter.QuadPart) / frequency.QuadPart);
}

int64_t TFW_GetTimestampUs() {
    // Windows平台：使用 QueryPerformanceCounter 获取单调时间
    // Windows platform: use QueryPerformanceCounter to get monotonic time
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为微秒
    // convert to microsecond
    return (int64_t)(TFW_TIME_SEC_TO_US(counter.QuadPart) / frequency.QuadPart);
}

int64_t TFW_GetTimestampNs() {
    // Windows平台：使用 QueryPerformanceCounter 获取单调时间
    // Windows platform: use QueryPerformanceCounter to get monotonic time
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    // 转换为纳秒
    // convert to nanosecond
    return (int64_t)(TFW_TIME_SEC_TO_NS(counter.QuadPart) / frequency.QuadPart);
}

int32_t TFW_GetTime(TFW_SysTime* sysTime) {
    if (sysTime == NULL) {
        TFW_LOGE_UTILS("TFW_GetTime sysTime is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：使用 GetSystemTimeAsFileTime 获取系统时间
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    // 将FILETIME转换为64位整数
    ULARGE_INTEGER ui;
    ui.LowPart = ft.dwLowDateTime;
    ui.HighPart = ft.dwHighDateTime;

    // 转换为Unix时间（从1970年1月1日开始的100纳秒间隔）
    // Windows FILETIME是从1601年1月1日开始的
    const int64_t UNIX_TIME_START = 116444736000000000LL; // Unix时间起始点到Windows时间起始点的差值（以100纳秒为单位）
    int64_t timeValue = ui.QuadPart - UNIX_TIME_START;

    sysTime->sec = timeValue / 10000000LL;  // 转换为秒
    sysTime->nsec = (timeValue % 10000000LL) * 100; // 转换为纳秒

    return TFW_SUCCESS;
}

int32_t TFW_TimeCompare(const TFW_SysTime* a, const TFW_SysTime* b) {
    if (a == NULL || b == NULL) {
        TFW_LOGE_UTILS("TFW_TimeCompare a or b is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (a->sec > b->sec) {
        return 1;
    } else if (a->sec < b->sec) {
        return -1;
    } else {
        if (a->nsec > b->nsec) {
            return 1;
        } else if (a->nsec < b->nsec) {
            return -1;
        } else {
            return 0;
        }
    }
}

int32_t TFW_TimeAdd(const TFW_SysTime* a, const TFW_SysTime* b, TFW_SysTime* result) {
    if (a == NULL || b == NULL || result == NULL) {
        TFW_LOGE_UTILS("TFW_TimeAdd a, b or result is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    const int64_t NS_PER_SECOND = 1000000000LL;

    result->sec = a->sec + b->sec;
    result->nsec = a->nsec + b->nsec;

    if (result->nsec >= NS_PER_SECOND) {
        result->sec += 1;
        result->nsec -= NS_PER_SECOND;
    }

    return TFW_SUCCESS;
}

int32_t TFW_TimeSub(const TFW_SysTime* a, const TFW_SysTime* b, TFW_SysTime* result) {
    if (a == NULL || b == NULL || result == NULL) {
        TFW_LOGE_UTILS("TFW_TimeSub a, b or result is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    const int64_t NS_PER_SECOND = 1000000000LL;

    if (TFW_TimeCompare(a, b) < 0) {
        TFW_LOGE_UTILS("TFW_TimeSub a < b, result would be negative");
        return TFW_ERROR;
    }

    result->sec = a->sec - b->sec;
    result->nsec = a->nsec - b->nsec;

    if (result->nsec < 0) {
        result->sec -= 1;
        result->nsec += NS_PER_SECOND;
    }

    return TFW_SUCCESS;
}
