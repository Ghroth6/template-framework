/**
 * @file TFW_config.h
 * @brief TFW Configuration Management Module
 * @brief TFW配置管理模块
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file provides configuration management functionality for the TFW framework
 * 本文件为TFW框架提供配置管理功能
 * Supports key-value based configuration with type safety
 * 支持基于键值对的类型安全配置
 */

#ifndef TFW_CONFIG_H
#define TFW_CONFIG_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "TFW_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Configuration Management Interface
// 配置管理接口
// ============================================================================

/**
 * Initialize configuration module
 * 初始化配置模块
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_Init(void);

/**
 * Deinitialize configuration module
 * 反初始化配置模块
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_Deinit(void);



// ============================================================================
// Type-safe Configuration Access (Optional)
// 类型安全配置访问（可选）
// ============================================================================

/**
 * Get string configuration value
 * 获取字符串配置值
 * @param key Configuration key / 配置键
 * @param value Output string buffer / 输出字符串缓冲区
 * @param buffer_size Buffer size / 缓冲区大小
 * @param default_value Default value if key not found / 键不存在时的默认值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_GetString(TFW_ConfigKey key, char* value, size_t buffer_size, const char* default_value);

/**
 * Get integer configuration value
 * 获取整数配置值
 * @param key Configuration key / 配置键
 * @param value Output integer pointer / 输出整数指针
 * @param default_value Default value if key not found / 键不存在时的默认值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_GetInt(TFW_ConfigKey key, int32_t* value, int32_t default_value);

/**
 * Get boolean configuration value
 * 获取布尔配置值
 * @param key Configuration key / 配置键
 * @param value Output boolean pointer / 输出布尔指针
 * @param default_value Default value if key not found / 键不存在时的默认值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_GetBool(TFW_ConfigKey key, bool* value, bool default_value);

/**
 * Get float configuration value
 * 获取浮点配置值
 * @param key Configuration key / 配置键
 * @param value Output float pointer / 输出浮点指针
 * @param default_value Default value if key not found / 键不存在时的默认值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_GetFloat(TFW_ConfigKey key, float* value, float default_value);

/**
 * Set string configuration value
 * 设置字符串配置值
 * @param key Configuration key / 配置键
 * @param value String value to set / 要设置的字符串值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_SetString(TFW_ConfigKey key, const char* value);

/**
 * Set integer configuration value
 * 设置整数配置值
 * @param key Configuration key / 配置键
 * @param value Integer value to set / 要设置的整数值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_SetInt(TFW_ConfigKey key, int32_t value);

/**
 * Set boolean configuration value
 * 设置布尔配置值
 * @param key Configuration key / 配置键
 * @param value Boolean value to set / 要设置的布尔值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_SetBool(TFW_ConfigKey key, bool value);

/**
 * Set float configuration value
 * 设置浮点配置值
 * @param key Configuration key / 配置键
 * @param value Float value to set / 要设置的浮点值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_SetFloat(TFW_ConfigKey key, float value);

// ============================================================================
// Configuration Management Utilities
// 配置管理工具
// ============================================================================

/**
 * Get configuration key name
 * 获取配置键名称
 * @param key Configuration key / 配置键
 * @param name Output name buffer / 输出名称缓冲区
 * @param buffer_size Buffer size / 缓冲区大小
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Config_GetKeyName(TFW_ConfigKey key, char* name, size_t buffer_size);

/**
 * Get configuration key type
 * 获取配置键类型
 * @param key Configuration key / 配置键
 * @return Configuration value type / 配置值类型
 */
TFW_ConfigValueType TFW_Config_GetKeyType(TFW_ConfigKey key);

/**
 * Check if configuration module is initialized
 * 检查配置模块是否已初始化
 * @return true if initialized, false otherwise / 已初始化返回true，否则返回false
 */
bool TFW_Config_IsInitialized(void);

#ifdef __cplusplus
}
#endif

#endif // TFW_CONFIG_H
