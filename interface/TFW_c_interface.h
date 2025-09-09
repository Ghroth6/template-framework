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

/**
 * Get all configuration items from the framework
 * 从框架中获取所有配置项
 *
 * @param config_array Pointer to the array of configuration items that will be allocated by the function
 *                     指向配置项数组的指针，该数组将由函数分配
 * @param count        Pointer to store the number of configuration items returned
 *                     指向存储返回的配置项数量的指针
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 *
 * Note: The caller is responsible for freeing the memory using TFW_CoreFreeAllConfigItems
 * 注意：调用者需要使用TFW_CoreFreeAllConfigItem释放内存
 */
int32_t TFW_CoreGetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count);

/**
 * Free the memory allocated by TFW_CoreGetAllConfigItem
 * 释放由TFW_CoreGetAllConfigItem分配的内存
 *
 * @param config_array Pointer to the configuration items array to be freed
 *                     指向要释放的配置项数组的指针
 *
 * Note: This function should only be used to free memory allocated by TFW_CoreGetAllConfigItems
 * 注意：此函数应仅用于释放由TFW_CoreGetAllConfigItems分配的内存
 */
void TFW_CoreFreeAllConfigItems(TFW_ConfigItem *config_array);

#ifdef __cplusplus
}
#endif

#endif // TFW_C_INTERFACE_H
