#ifndef TFW_UTILS_LOG_H
#define TFW_UTILS_LOG_H

#include "include/TFW_log.h"

// ============================================================================
// Utils妯″潡涓撶敤鏃ュ織瀹?
// ============================================================================

// 绠€鍖栫殑Utils妯″潡鏃ュ織瀹?
#define TFW_LOGT_UTILS(fmt, ...) \
    TFW_LOGT(TFW_LOG_MODULE_UTILS, fmt, ##__VA_ARGS__)

#define TFW_LOGD_UTILS(fmt, ...) \
    TFW_LOGD(TFW_LOG_MODULE_UTILS, fmt, ##__VA_ARGS__)

#define TFW_LOGI_UTILS(fmt, ...) \
    TFW_LOGI(TFW_LOG_MODULE_UTILS, fmt, ##__VA_ARGS__)

#define TFW_LOGW_UTILS(fmt, ...) \
    TFW_LOGW(TFW_LOG_MODULE_UTILS, fmt, ##__VA_ARGS__)

#define TFW_LOGE_UTILS(fmt, ...) \
    TFW_LOGE(TFW_LOG_MODULE_UTILS, fmt, ##__VA_ARGS__)

#define TFW_LOGF_UTILS(fmt, ...) \
    TFW_LOGF(TFW_LOG_MODULE_UTILS, fmt, ##__VA_ARGS__)

#endif // TFW_UTILS_LOG_H
