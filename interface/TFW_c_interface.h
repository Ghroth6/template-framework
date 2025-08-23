#ifndef TFW_C_INTERFACE_H
#define TFW_C_INTERFACE_H

#include "TFW_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 核心模块C接口
// ============================================================================

/**
 * 鍒濆鍖栨牳蹇冩鏋? * @return 0琛ㄧず鎴愬姛锛岃礋鏁拌〃绀洪敊璇? */
int TFW_CORE_INITIALIZE();

/**
 * 閫€鍑烘牳蹇冩鏋? * @return 0琛ㄧず鎴愬姛锛岃礋鏁拌〃绀洪敊璇? */
int TFW_CORE_EXIT();

/**
 * 妫€鏌ユ牳蹇冩槸鍚﹀凡鍒濆鍖? * @return 1琛ㄧず宸插垵濮嬪寲锛?琛ㄧず鏈垵濮嬪寲
 */
int TFW_CORE_IS_INITIALIZED();

/**
 * 鑾峰彇涓€涓€? * @param key 閿悕
 * @param value 杈撳嚭缂撳啿鍖? * @param maxLen 缂撳啿鍖烘渶澶ч暱搴? * @return 0琛ㄧず鎴愬姛锛岃礋鏁拌〃绀洪敊璇? */
int TFW_CORE_GET_VALUE(const char* key, char* value, int maxLen);

/**
 * 璁剧疆涓€涓€? * @param key 閿悕
 * @param value 鍊? * @return 0琛ㄧず鎴愬姛锛岃礋鏁拌〃绀洪敊璇? */
int TFW_CORE_SET_VALUE(const char* key, const char* value);

/**
 * 鎵ц涓€涓┖鍔ㄤ綔
 * @param action 鍔ㄤ綔鍚嶇О
 * @return 0琛ㄧず鎴愬姛锛岃礋鏁拌〃绀洪敊璇? */
int TFW_CORE_ACTION(const char* action);

#ifdef __cplusplus
}
#endif

#endif // TFW_C_INTERFACE_H