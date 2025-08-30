/**
 * @file TFW_c_interface.h
 * @brief TFW Framework C Language Interface
 * @brief TFW框架C语言接口
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file provides C language interface for the TFW framework
 * 本文件为TFW框架提供C语言接口
 * It allows C language projects to use TFW framework functionality
 * 它允许C语言项目使用TFW框架功能
 */

#ifndef TFW_C_INTERFACE_H
#define TFW_C_INTERFACE_H

#include "TFW_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Core module C interface
// 核心模块C接口
// ============================================================================

/**
 * Initialize core module
 * 初始化核心模块
 *
 * @return TFW_SUCCESS means success, negative means error
 * @return TFW_SUCCESS表示成功，负值表示错误
 *
 * Note: This function should be called before using any other TFW functionality
 * 注意：在使用任何其他TFW功能之前应调用此函数
 */
int32_t TFW_CoreInit();

/**
 * Deinitialize core module
 * 反初始化核心模块
 *
 * @return TFW_SUCCESS means success, negative means error
 * @return TFW_SUCCESS表示成功，负值表示错误
 *
 * Note: This function should be called when TFW functionality is no longer needed
 * 注意：当不再需要TFW功能时应调用此函数
 */
int32_t TFW_CoreDeinit();

#ifdef __cplusplus
}
#endif

#endif // TFW_C_INTERFACE_H
