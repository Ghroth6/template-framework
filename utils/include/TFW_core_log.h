#ifndef TFW_CORE_LOG_H
#define TFW_CORE_LOG_H

#include "TFW_log.h"

// ============================================================================
// Core模块专用日志宏
// Core module专用日志宏
// ============================================================================

// 简化的Core模块日志宏
// Simplified Core module log macro
#define TFW_LOGT_CORE(fmt, ...) TFW_LOGT(TFW_LOG_MODULE_CORE, fmt, ##__VA_ARGS__)

#define TFW_LOGD_CORE(fmt, ...) TFW_LOGD(TFW_LOG_MODULE_CORE, fmt, ##__VA_ARGS__)

#define TFW_LOGI_CORE(fmt, ...) TFW_LOGI(TFW_LOG_MODULE_CORE, fmt, ##__VA_ARGS__)

#define TFW_LOGW_CORE(fmt, ...) TFW_LOGW(TFW_LOG_MODULE_CORE, fmt, ##__VA_ARGS__)

#define TFW_LOGE_CORE(fmt, ...) TFW_LOGE(TFW_LOG_MODULE_CORE, fmt, ##__VA_ARGS__)

#define TFW_LOGF_CORE(fmt, ...) TFW_LOGF(TFW_LOG_MODULE_CORE, fmt, ##__VA_ARGS__)

#endif  // TFW_CORE_LOG_H
