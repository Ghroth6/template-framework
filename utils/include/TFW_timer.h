#ifndef TFW_TIMER_H
#define TFW_TIMER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// 时间相关常量与宏定义声明
// Time related constants and macro definitions declaration
// ============================================================================
#define TFW_TIME_SEC_TO_MS(sec) ((sec)*1000)
#define TFW_TIME_MS_TO_SEC(ms) ((ms) / 1000)
#define TFW_TIME_SEC_TO_US(sec) ((sec) * (1000 * 1000))
#define TFW_TIME_US_TO_SEC(us) ((us) / (1000 * 1000))
#define TFW_TIME_SEC_TO_NS(sec) ((sec) * (1000 * 1000 * 1000))
#define TFW_TIME_NS_TO_SEC(ns) ((ns) / (1000 * 1000 * 1000))
#define TFW_TIME_NS_TO_MS(ns) ((ns) / (1000 * 1000))
#define TFW_TIME_MS_TO_NS(ms) ((ms) * (1000 * 1000))
#define TFW_TIME_FORMAT_DEFAULT "%Y-%m-%d %H:%M:%S.mmm"

// 时间结构体定义
typedef struct {
    int64_t sec;   // 秒
    int64_t nsec;  // 纳秒
} TFW_SysTime;

// ============================================================================
// Time related function declarations
// 时间相关函数声明
// ============================================================================

/**
 * Get current timestamp string
 * 获取当前时间戳字符串
 * @param timestamp Output buffer for timestamp string / 输出时间戳字符串的缓冲区
 * @param buffer_size Size of the output buffer / 输出缓冲区大小
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_GetTimestamp(char* timestamp, size_t buffer_size);

/**
 * Get current monotonic timestamp (millisecond precision)
 * 获取当前单调时间戳（毫秒精度）
 * @return Millisecond timestamp (monotonic time)
 */
int64_t TFW_GetTimestampMs();

/**
 * Get current monotonic timestamp (microsecond precision)
 * 获取当前单调时间戳（微秒精度）
 * @return Microsecond timestamp (monotonic time)
 */
int64_t TFW_GetTimestampUs();

/**
 * Get current monotonic timestamp (nanosecond precision)
 * 获取当前单调时间戳（纳秒精度）
 * @return Nanosecond timestamp (monotonic time)
 */
int64_t TFW_GetTimestampNs();

/**
 * Get current system time
 * 获取当前系统时间
 * @param sysTime System time structure / 系统时间结构体
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_GetTime(TFW_SysTime* sysTime);

/**
 * Compare two time values
 * 比较两个时间值
 * @param a First time value / 第一个时间值
 * @param b Second time value / 第二个时间值
 * @return Returns 1 if a > b, -1 if a < b, 0 if equal / a>b返回1，a<b返回-1，相等返回0
 */
int32_t TFW_TimeCompare(const TFW_SysTime* a, const TFW_SysTime* b);

/**
 * Add two time values
 * 两个时间值相加
 * @param a First time value / 第一个时间值
 * @param b Second time value / 第二个时间值
 * @param result Result time value / 结果时间值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_TimeAdd(const TFW_SysTime* a, const TFW_SysTime* b, TFW_SysTime* result);

/**
 * Subtract two time values
 * 两个时间值相减
 * @param a First time value / 第一个时间值
 * @param b Second time value / 第二个时间值
 * @param result Result time value (a - b) / 结果时间值(a - b)
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_TimeSub(const TFW_SysTime* a, const TFW_SysTime* b, TFW_SysTime* result);

#ifdef __cplusplus
}
#endif

#endif  // TFW_TIMER_H