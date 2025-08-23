#ifndef TFW_LOG_H
#define TFW_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 鏃ュ織绛夌骇瀹氫箟
// ============================================================================
enum TFW_LogLevel {
    TFW_LOG_LEVEL_TRACE = 0,    // 璺熻釜淇℃伅
    TFW_LOG_LEVEL_DEBUG = 1,    // 璋冭瘯淇℃伅
    TFW_LOG_LEVEL_INFO = 2,     // 涓€鑸俊鎭?    TFW_LOG_LEVEL_WARNING = 3,  // 璀﹀憡淇℃伅
    TFW_LOG_LEVEL_ERROR = 4,    // 閿欒淇℃伅
    TFW_LOG_LEVEL_FATAL = 5     // 鑷村懡閿欒
};

// ============================================================================
// 鏃ュ織妯″潡鍒嗙被
// ============================================================================
enum TFW_LogModule {
    TFW_LOG_MODULE_CORE = 0,    // 鏍稿績妯″潡
    TFW_LOG_MODULE_UTILS = 1,   // 宸ュ叿妯″潡
    TFW_LOG_MODULE_SDK = 2,     // SDK妯″潡
    TFW_LOG_MODULE_SYSTEM = 3   // 绯荤粺妯″潡
};

// ============================================================================
// 鏃ュ織瀹忓畾涔?// ============================================================================

// 鍩虹鏃ュ織瀹?#define TFW_LOG_INNER(module, level, fmt, ...) \
    (void)TFW_LOG_IMPL(module, level, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

// 绛夌骇鏃ュ織瀹?#define TFW_LOGT(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

#define TFW_LOGD(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

#define TFW_LOGI(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)

#define TFW_LOGW(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_WARNING, fmt, ##__VA_ARGS__)

#define TFW_LOGE(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

#define TFW_LOGF(module, fmt, ...) \
    TFW_LOG_INNER(module, TFW_LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

// 鏃ュ織瀹炵幇鍑芥暟澹版槑锛堢敱log鐩綍涓嬬殑瀹炵幇鏂囦欢鎻愪緵锛?int TFW_LOG_IMPL(int module, int level, const char* file, int line,
                  const char* function, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // TFW_LOG_H
