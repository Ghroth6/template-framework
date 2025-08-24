#ifndef TFW_ERRORNO_H
#define TFW_ERRORNO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 错误码定义 - C语言兼容
// ============================================================================

// 统一错误码枚举
typedef enum {
    // 通用错误码
    TFW_ERROR = -1000,           // 标准错误
    TFW_ERROR_INVALID_PARAM,     // 无效参数
    TFW_ERROR_NOT_INIT,          // 未初始化
    TFW_ERROR_ALREADY_INIT,      // 已经初始化
    TFW_ERROR_OPERATION_FAIL,    // 操作失败

    // 任务错误码
    TFW_ERROR_TASK_FAIL = -200,  // 任务失败
    TFW_ERROR_TIMEOUT,           // 超时错误
    TFW_ERROR_NOT_FOUND,         // 未找到
    
    // 锁相关错误码
    TFW_ERROR_LOCK_FAILED = -300, // 锁操作失败
    TFW_ERROR_NO_MEMORY = -301,   // 内存不足
    
    // 系统错误码
    TFW_ERROR_SYSTEM = -100,     // 系统错误
    TFW_ERROR_MEMORY,            // 内存错误
    TFW_ERROR_FILE,              // 文件错误
    TFW_ERROR_NETWORK,           // 网络错误

    // 成功码
    TFW_SUCCESS = 0              // 成功
} TFW_ErrorCode;



// 通用错误码类型（推荐使用）
typedef int32_t TFW_Result;

#ifdef __cplusplus
}
#endif

#endif // TFW_ERRORNO_H
