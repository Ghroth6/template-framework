/**
 * @file TFW_types.h
 * @brief TFW Framework Type Definitions
 * @brief TFW框架类型定义
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file contains basic type definitions for the TFW framework
 * 本文件包含TFW框架的基本类型定义
 * All types are designed to be C language compatible for cross-platform support
 * 所有类型都设计为C语言兼容，以支持跨平台
 */

#ifndef TFW_TYPES_H
#define TFW_TYPES_H

#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Default types - C language compatible
// 默认类型 - C语言兼容
// ============================================================================

// Basic type template
// 基础类型模板
typedef struct {
    int32_t id;
    int32_t value;
} TFW_Type;

// ============================================================================
// Configuration types - Configuration management
// 配置类型 - 配置管理
// ============================================================================

// Configuration key enumeration
// 配置键枚举
typedef enum {
    // Main configuration / 主配置
    TFW_CONFIG_MAIN_VERSION = 0,
    TFW_CONFIG_MAIN_DEBUG,
    TFW_CONFIG_MAIN_LOG_LEVEL,

    // Logging configuration / 日志配置
    TFW_CONFIG_LOGGING_LEVEL,
    TFW_CONFIG_LOGGING_OUTPUT,
    TFW_CONFIG_LOGGING_FILE_PATH,
    TFW_CONFIG_LOGGING_FILE_PREFIX,
    TFW_CONFIG_LOGGING_MAX_FILE_SIZE,
    TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS,

    // Resources configuration / 资源配置
    TFW_CONFIG_RESOURCES_AUTO_UPDATE,
    TFW_CONFIG_RESOURCES_UPDATE_INTERVAL,
    TFW_CONFIG_RESOURCES_CACHE_PATH,

    // System configuration / 系统配置
    TFW_CONFIG_SYSTEM_MAX_THREADS,
    TFW_CONFIG_SYSTEM_TIMEOUT,

    // Configuration key count / 配置键总数
    TFW_CONFIG_KEY_COUNT
} TFW_ConfigKey;

// Configuration value type
// 配置值类型
typedef enum {
    TFW_CONFIG_TYPE_STRING = 0,
    TFW_CONFIG_TYPE_INT,
    TFW_CONFIG_TYPE_BOOL,
    TFW_CONFIG_TYPE_FLOAT,
    TFW_CONFIG_TYPE_INVALID  // Invalid type for error handling / 无效类型，用于错误处理
} TFW_ConfigValueType;

// Configuration value union
// 配置值联合体
typedef union {
    const char* string_value;
    int32_t int_value;
    bool bool_value;
    float float_value;
} TFW_ConfigValue;

// Configuration item structure
// 配置项结构
typedef struct {
    TFW_ConfigKey key;
    TFW_ConfigValueType type;
    TFW_ConfigValue value;
} TFW_ConfigItem;

// ============================================================================
// Log level types - Logging system
// 日志等级类型 - 日志系统
// ============================================================================

// Log level enumeration
// 日志等级枚举
typedef enum {
    TFW_LOG_LEVEL_TRACE = 0,    // 最详细的调试信息
    TFW_LOG_LEVEL_DEBUG = 1,    // 调试信息
    TFW_LOG_LEVEL_INFO = 2,     // 一般信息
    TFW_LOG_LEVEL_WARNING = 3,  // 警告信息
    TFW_LOG_LEVEL_ERROR = 4,    // 错误信息
    TFW_LOG_LEVEL_FATAL = 5,    // 致命错误
    TFW_LOG_LEVEL_COUNT         // 日志等级总数
} TFW_LogLevel;

#ifdef __cplusplus
}
#endif

#endif  // TFW_TYPES_H
