#ifndef TFW_TIMER_H
#define TFW_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Time related function declarations
// ============================================================================

/**
 * Get current timestamp string
 * @return Formatted timestamp string, format: YYYY-MM-DD HH:MM:SS.mmm
 */
const char* TFW_GetTimestamp();

/**
 * Get current timestamp (millisecond precision)
 * @return Millisecond timestamp
 */
int64_t TFW_GetTimestampMs();

/**
 * Get current timestamp (microsecond precision)
 * @return Microsecond timestamp
 */
int64_t TFW_GetTimestampUs();

/**
 * Get current timestamp (nanosecond precision)
 * @return Nanosecond timestamp
 */
int64_t TFW_GetTimestampNs();

#ifdef __cplusplus
}
#endif

#endif // TFW_TIMER_H
