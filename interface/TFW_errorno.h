/**
 * @file TFW_errorno.h
 * @brief TFW Framework Error Code Definitions
 * @brief TFW框架错误码定义
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file contains error code definitions for the TFW framework
 * 本文件包含TFW框架的错误码定义
 * All error codes are designed to be C language compatible for cross-platform support
 * 所有错误码都设计为C语言兼容，以支持跨平台
 */

#ifndef TFW_ERRORNO_H
#define TFW_ERRORNO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Error code definition - C language compatible
// 错误码定义 - C语言兼容
// ============================================================================

// Unified error code enumeration
// 统一错误码枚举
typedef enum {

    // Generic error code
    // 通用错误码
    TFW_ERROR_PUBLIC_ERR_BASE = -100,  // Base value for public error codes / 公共错误码基值
    TFW_ERROR_TIMEOUT,                 // Operation timeout / 操作超时
    TFW_ERROR_INVALID_PARAM,           // Invalid parameter / 无效参数
    TFW_ERROR_INIT_FAIL,               // Initialization failed / 初始化失败
    TFW_ERROR_NOT_INIT,                // Not initialized / 未初始化
    TFW_ERROR_ALREADY_INIT,            // Already initialized / 已经初始化
    TFW_ERROR_DEINIT_FAIL,             // Deinitialization failed / 反初始化失败
    TFW_ERROR_LOCK_FAILED,             // Lock operation failed / 锁定操作失败
    TFW_ERROR_FILE_ERROR,              // File operation error / 文件操作错误

    TFW_ERROR   = -1,                  // General error / 一般错误
    TFW_SUCCESS = 0                    // Success / 成功
} TFW_ErrorCode;

#ifdef __cplusplus
}
#endif

#endif // TFW_ERRORNO_H
