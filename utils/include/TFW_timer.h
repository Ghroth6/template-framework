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
uint64_t TFW_GetTimestampMs();

/**
 * Get current monotonic timestamp (microsecond precision)
 * 获取当前单调时间戳（微秒精度）
 * @return Microsecond timestamp (monotonic time)
 */
uint64_t TFW_GetTimestampUs();

/**
 * Get current monotonic timestamp (nanosecond precision)
 * 获取当前单调时间戳（纳秒精度）
 * @return Nanosecond timestamp (monotonic time)
 */
uint64_t TFW_GetTimestampNs();

#ifdef __cplusplus
}
#endif

#endif  // TFW_TIMER_H