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
int32_t TFW_CORE_INITIALIZE();

/**
 * 退出核心模块
 * @return 0表示成功，负数表示错误
 */
int32_t TFW_CORE_EXIT();

/**
 * 执行一个空动作
 * @param action 动作名称
 * @return 0表示成功，负数表示错误
 */
int32_t TFW_CORE_ACTION(const char* action);

/**
 * 获取数据
 * @return 获取到的数据，负数表示错误
 */
int32_t TFW_CORE_GET_DATA();

/**
 * 设置数据
 * @param data 数据
 * @return 0表示成功，负数表示错误
 */
int32_t TFW_CORE_SET_DATA(const int32_t data);

#ifdef __cplusplus
}
#endif

#endif // TFW_C_INTERFACE_H