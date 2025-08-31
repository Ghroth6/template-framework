#ifndef TFW_SDK_LOG_H
#define TFW_SDK_LOG_H

#include "TFW_log.h"

// ============================================================================
// SDK模块专用日志宏
// SDK module专用日志宏
// ============================================================================

// 简化的SDK模块日志宏
// Simplified Core module log macro
#define TFW_LOGT_SDK(fmt, ...) TFW_LOGT(TFW_LOG_MODULE_SDK, fmt, ##__VA_ARGS__)

#define TFW_LOGD_SDK(fmt, ...) TFW_LOGD(TFW_LOG_MODULE_SDK, fmt, ##__VA_ARGS__)

#define TFW_LOGI_SDK(fmt, ...) TFW_LOGI(TFW_LOG_MODULE_SDK, fmt, ##__VA_ARGS__)

#define TFW_LOGW_SDK(fmt, ...) TFW_LOGW(TFW_LOG_MODULE_SDK, fmt, ##__VA_ARGS__)

#define TFW_LOGE_SDK(fmt, ...) TFW_LOGE(TFW_LOG_MODULE_SDK, fmt, ##__VA_ARGS__)

#define TFW_LOGF_SDK(fmt, ...) TFW_LOGF(TFW_LOG_MODULE_SDK, fmt, ##__VA_ARGS__)

#endif  // TFW_SDK_LOG_H
