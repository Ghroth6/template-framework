#include "../../include/TFW_timer.h"
#include "../../include/TFW_log.h"
#include <windows.h>
#include <stdio.h>

// ============================================================================
// Windows平台时间实现
// ============================================================================

const char* TFW_GetTimestamp() {
    static char timestamp[64];
    
    // Windows平台：使用 GetSystemTime()
    SYSTEMTIME st;
    GetSystemTime(&st);
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    
    return timestamp;
}

int64_t TFW_GetTimestampMs() {
    // Windows平台：使用 GetTickCount64() 获取毫秒级时间戳
    return (int64_t)GetTickCount64();
}

int64_t TFW_GetTimestampUs() {
    // Windows平台：使用 QueryPerformanceCounter 获取微秒级时间戳
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    
    // 转换为微秒
    return (int64_t)((counter.QuadPart * 1000000) / frequency.QuadPart);
}

int64_t TFW_GetTimestampNs() {
    // Windows平台：使用 QueryPerformanceCounter 获取纳秒级时间戳
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    
    // 转换为纳秒
    return (int64_t)((counter.QuadPart * 1000000000) / frequency.QuadPart);
}
