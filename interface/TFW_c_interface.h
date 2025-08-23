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
 * 初始化核心模块
 * @return 0表示成功，负数表示错误
 */
int TFW_CORE_INITIALIZE();

/**
 * 退出核心模块
 * @return 0表示成功，负数表示错误
 */
int TFW_CORE_EXIT();

/**
 * 检查核心是否已初始化
 * @return 1表示已初始化，0表示未初始化
 */
int TFW_CORE_IS_INITIALIZED();

/**
 * 获取一个值
 * @param key 键名
 * @param value 输出缓冲区
 * @param maxLen 缓冲区最大长度
 * @return 0表示成功，负数表示错误
 */
int TFW_CORE_GET_VALUE(const char* key, char* value, int maxLen);

/**
 * 设置一个值
 * @param key 键名
 * @param value 值
 * @return 0表示成功，负数表示错误
 */
int TFW_CORE_SET_VALUE(const char* key, const char* value);

/**
 * 执行一个空动作
 * @param action 动作名称
 * @return 0表示成功，负数表示错误
 */
int TFW_CORE_ACTION(const char* action);

#ifdef __cplusplus
}
#endif

#endif // TFW_C_INTERFACE_H