#ifndef TFW_ERRORNO_H
#define TFW_ERRORNO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 閿欒鐮佸畾涔?- C璇█鍏煎
// ============================================================================

// 缁熶竴閿欒鐮佹灇涓?
typedef enum {
    // 閫氱敤閿欒鐮?
    TFW_ERROR = -1000,           // 鏍囧噯閿欒
    TFW_ERROR_INVALID_PARAM,     // 鏃犳晥鍙傛暟
    TFW_ERROR_NOT_INIT,          // 鏈垵濮嬪寲
    TFW_ERROR_ALREADY_INIT,      // 宸茬粡鍒濆鍖?
    TFW_ERROR_OPERATION_FAIL,    // 鎿嶄綔澶辫触

    // 浠诲姟閿欒鐮?
    TFW_ERROR_TASK_FAIL = -200,  // 浠诲姟澶辫触
    TFW_ERROR_TIMEOUT,           // 瓒呮椂閿欒
    TFW_ERROR_NOT_FOUND,         // 鏈壘鍒?
    // 绯荤粺閿欒鐮?
    TFW_ERROR_SYSTEM = -100,     // 绯荤粺閿欒
    TFW_ERROR_MEMORY,            // 鍐呭瓨閿欒
    TFW_ERROR_FILE,              // 鏂囦欢閿欒
    TFW_ERROR_NETWORK,           // 缃戠粶閿欒

    // 鎴愬姛鐮?
    TFW_SUCCESS = 0              // 鎴愬姛
} TFW_ErrorCode;



// 閫氱敤閿欒鐮佺被鍨嬶紙鎺ㄨ崘浣跨敤锛?
typedef int32_t TFW_Result;

#ifdef __cplusplus
}

// C++鍛藉悕绌洪棿鍒悕锛屼繚鎸佸悜鍚庡吋瀹?
namespace TFW {
    using ErrorCode = TFW_ErrorCode;
    using Result = TFW_Result;
}

#endif

#endif // TFW_ERRORNO_H
