#ifndef TFW_TIMER_H
#define TFW_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 时间相关函数声明
// ============================================================================

/**
 * 获取当前时间戳字符串
 * @return 格式化的时间戳字符串，格式：YYYY-MM-DD HH:MM:SS.mmm
 */
const char* TFW_GetTimestamp();

/**
 * 获取当前时间戳（毫秒级精度）
 * @return 毫秒级时间戳
 */
int64_t TFW_GetTimestampMs();

/**
 * 获取当前时间戳（微秒级精度）
 * @return 微秒级时间戳
 */
int64_t TFW_GetTimestampUs();

/**
 * 获取当前时间戳（纳秒级精度）
 * @return 纳秒级时间戳
 */
int64_t TFW_GetTimestampNs();

#ifdef __cplusplus
}
#endif

#endif // TFW_TIMER_H
