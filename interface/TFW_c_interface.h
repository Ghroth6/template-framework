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

/**
 * Get integer configuration value by key
 * 根据键获取整型配置值
 *
 * @param key   Configuration key / 配置键
 * @param value Pointer to store the integer value / 指向存储整型值的指针
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreGetIntConfigValue(TFW_ConfigKey key, int32_t *value);

/**
 * Set integer configuration value by key
 * 根据键设置整型配置值
 *
 * @param key   Configuration key / 配置键
 * @param value Integer value to set / 要设置的整型值
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreSetIntConfigValue(TFW_ConfigKey key, int32_t value);

/**
 * Get float configuration value by key
 * 根据键获取浮点型配置值
 *
 * @param key   Configuration key / 配置键
 * @param value Pointer to store the float value / 指向存储浮点值的指针
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreGetFloatConfigValue(TFW_ConfigKey key, float *value);

/**
 * Set float configuration value by key
 * 根据键设置浮点型配置值
 *
 * @param key   Configuration key / 配置键
 * @param value Float value to set / 要设置的浮点值
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreSetFloatConfigValue(TFW_ConfigKey key, float value);

/**
 * Get boolean configuration value by key
 * 根据键获取布尔型配置值
 *
 * @param key   Configuration key / 配置键
 * @param value Pointer to store the boolean value / 指向存储布尔值的指针
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreGetBoolConfigValue(TFW_ConfigKey key, bool *value);

/**
 * Set boolean configuration value by key
 * 根据键设置布尔型配置值
 *
 * @param key   Configuration key / 配置键
 * @param value Boolean value to set / 要设置的布尔值
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreSetBoolConfigValue(TFW_ConfigKey key, bool value);

/**
 * Get string configuration value by key
 * 根据键获取字符串配置值
 *
 * @param key   Configuration key / 配置键
 * @param value Pointer to store the string value / 指向存储字符串值的指针
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreGetStringConfigValue(TFW_ConfigKey key, const char **value);

/**
 * Set string configuration value by key
 * 根据键设置字符串配置值
 *
 * @param key   Configuration key / 配置键
 * @param value String value to set / 要设置的字符串值
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreSetStringConfigValue(TFW_ConfigKey key, const char *value);

/**
 * Get configuration value type by key
 * 根据键获取配置值类型
 *
 * @param key  Configuration key / 配置键
 * @param type Pointer to store the configuration value type / 指向存储配置值类型的指针
 * @return TFW_SUCCESS means success, negative means error
 *         TFW_SUCCESS表示成功，负值表示错误
 */
int32_t TFW_CoreGetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type);

#ifdef __cplusplus
}
#endif

#endif // TFW_C_INTERFACE_H
