/**
 * @file TFW_config.c
 * @brief TFW Configuration Management Module Implementation
 * @brief TFW配置管理模块实现
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file implements the configuration management functionality for the TFW framework
 * 本文件实现TFW框架的配置管理功能
 * Provides simplified key-value based configuration with type safety
 * 提供简化的基于键值对的类型安全配置
 */

#include "TFW_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "TFW_utils_log.h"
#include "TFW_common_defines.h"
#include "TFW_errorno.h"
#include "TFW_mem.h"
#include "TFW_file.h"

// ============================================================================
// Internal Configuration Manager Structure
// 内部配置管理器结构
// ============================================================================

typedef struct {
    char config_path[TFW_PATH_LEN_MAX];
    TFW_ConfigItem configs[TFW_CONFIG_KEY_COUNT];
    bool initialized;
    bool config_loaded;
} ConfigManager;

// Global configuration manager instance
// 全局配置管理器实例
static ConfigManager g_config_manager = {0};

// ============================================================================
// Internal Helper Functions
// 内部辅助函数
// ============================================================================





/**
 * Get configuration key type
 * 获取配置键类型
 * @param key Configuration key / 配置键
 * @return Configuration value type / 配置值类型
 */
static TFW_ConfigValueType GetKeyType(TFW_ConfigKey key) {
    switch (key) {
        case TFW_CONFIG_MAIN_VERSION:
        case TFW_CONFIG_LOGGING_OUTPUT:
        case TFW_CONFIG_LOGGING_FILE_PATH:
        case TFW_CONFIG_LOGGING_FILE_PREFIX:
        case TFW_CONFIG_RESOURCES_CACHE_PATH:
            return TFW_CONFIG_TYPE_STRING;
        case TFW_CONFIG_MAIN_DEBUG:
        case TFW_CONFIG_RESOURCES_AUTO_UPDATE:
            return TFW_CONFIG_TYPE_BOOL;
        case TFW_CONFIG_MAIN_LOG_LEVEL:
        case TFW_CONFIG_LOGGING_LEVEL:
        case TFW_CONFIG_LOGGING_MAX_FILE_SIZE:
        case TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS:
        case TFW_CONFIG_RESOURCES_UPDATE_INTERVAL:
        case TFW_CONFIG_SYSTEM_MAX_THREADS:
        case TFW_CONFIG_SYSTEM_TIMEOUT:
            return TFW_CONFIG_TYPE_INT;
        default:
            return TFW_CONFIG_TYPE_INVALID;  // 无效key返回无效类型
    }
}

/**
 * Get configuration key name
 * 获取配置键名称
 * @param key Configuration key / 配置键
 * @param name Output name buffer / 输出名称缓冲区
 * @param buffer_size Buffer size / 缓冲区大小
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
static int32_t GetKeyName(TFW_ConfigKey key, char* name, size_t buffer_size) {
    if (name == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    const char* key_name = NULL;
    switch (key) {
        case TFW_CONFIG_MAIN_VERSION:
            key_name = TFW_CONFIG_NAME_MAIN_VERSION;
            break;
        case TFW_CONFIG_MAIN_DEBUG:
            key_name = TFW_CONFIG_NAME_MAIN_DEBUG;
            break;
        case TFW_CONFIG_MAIN_LOG_LEVEL:
            key_name = TFW_CONFIG_NAME_MAIN_LOG_LEVEL;
            break;
        case TFW_CONFIG_LOGGING_LEVEL:
            key_name = TFW_CONFIG_NAME_LOGGING_LEVEL;
            break;
        case TFW_CONFIG_LOGGING_OUTPUT:
            key_name = TFW_CONFIG_NAME_LOGGING_OUTPUT;
            break;
        case TFW_CONFIG_LOGGING_FILE_PATH:
            key_name = TFW_CONFIG_NAME_LOGGING_FILE_PATH;
            break;
        case TFW_CONFIG_LOGGING_FILE_PREFIX:
            key_name = TFW_CONFIG_NAME_LOGGING_FILE_PREFIX;
            break;
        case TFW_CONFIG_LOGGING_MAX_FILE_SIZE:
            key_name = TFW_CONFIG_NAME_LOGGING_MAX_FILE_SIZE;
            break;
        case TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS:
            key_name = TFW_CONFIG_NAME_LOGGING_MAX_RETENTION_DAYS;
            break;
        case TFW_CONFIG_RESOURCES_AUTO_UPDATE:
            key_name = TFW_CONFIG_NAME_RESOURCES_AUTO_UPDATE;
            break;
        case TFW_CONFIG_RESOURCES_UPDATE_INTERVAL:
            key_name = TFW_CONFIG_NAME_RESOURCES_UPDATE_INTERVAL;
            break;
        case TFW_CONFIG_RESOURCES_CACHE_PATH:
            key_name = TFW_CONFIG_NAME_RESOURCES_CACHE_PATH;
            break;
        case TFW_CONFIG_SYSTEM_MAX_THREADS:
            key_name = TFW_CONFIG_NAME_SYSTEM_MAX_THREADS;
            break;
        case TFW_CONFIG_SYSTEM_TIMEOUT:
            key_name = TFW_CONFIG_NAME_SYSTEM_TIMEOUT;
            break;
        default:
            key_name = "unknown";
            break;
    }

    // Copy key name to output buffer using TFW safe string function
    // 使用TFW安全字符串函数将键名复制到输出缓冲区
    if (TFW_Strcpy_S(name, buffer_size, key_name) != TFW_SUCCESS) {
        return TFW_ERROR_INVALID_PARAM;  // Buffer too small or other error
    }
    return TFW_SUCCESS;
}

/**
 * Initialize configuration items with default values
 * 使用默认值初始化配置项
 */
static void InitializeConfigItems(void) {
    // Initialize each configuration item with its default value
    // 使用默认值初始化每个配置项

    // Main configuration / 主配置
    g_config_manager.configs[TFW_CONFIG_MAIN_VERSION].key = TFW_CONFIG_MAIN_VERSION;
    g_config_manager.configs[TFW_CONFIG_MAIN_VERSION].type = TFW_CONFIG_TYPE_STRING;
    g_config_manager.configs[TFW_CONFIG_MAIN_VERSION].value.string_value = TFW_Strdup(TFW_VERSION_STRING);

    g_config_manager.configs[TFW_CONFIG_MAIN_DEBUG].key = TFW_CONFIG_MAIN_DEBUG;
    g_config_manager.configs[TFW_CONFIG_MAIN_DEBUG].type = TFW_CONFIG_TYPE_BOOL;
    g_config_manager.configs[TFW_CONFIG_MAIN_DEBUG].value.bool_value = false;

    g_config_manager.configs[TFW_CONFIG_MAIN_LOG_LEVEL].key = TFW_CONFIG_MAIN_LOG_LEVEL;
    g_config_manager.configs[TFW_CONFIG_MAIN_LOG_LEVEL].type = TFW_CONFIG_TYPE_INT;
    g_config_manager.configs[TFW_CONFIG_MAIN_LOG_LEVEL].value.int_value = TFW_LOG_LEVEL_INFO;

    // Logging configuration / 日志配置
    g_config_manager.configs[TFW_CONFIG_LOGGING_LEVEL].key = TFW_CONFIG_LOGGING_LEVEL;
    g_config_manager.configs[TFW_CONFIG_LOGGING_LEVEL].type = TFW_CONFIG_TYPE_INT;
    g_config_manager.configs[TFW_CONFIG_LOGGING_LEVEL].value.int_value = TFW_LOG_LEVEL_INFO;

    g_config_manager.configs[TFW_CONFIG_LOGGING_OUTPUT].key = TFW_CONFIG_LOGGING_OUTPUT;
    g_config_manager.configs[TFW_CONFIG_LOGGING_OUTPUT].type = TFW_CONFIG_TYPE_STRING;
    g_config_manager.configs[TFW_CONFIG_LOGGING_OUTPUT].value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_LOGGING_OUTPUT);

    g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PATH].key = TFW_CONFIG_LOGGING_FILE_PATH;
    g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PATH].type = TFW_CONFIG_TYPE_STRING;
    g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PATH].value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_LOGGING_FILE_PATH);

    g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PREFIX].key = TFW_CONFIG_LOGGING_FILE_PREFIX;
    g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PREFIX].type = TFW_CONFIG_TYPE_STRING;
    g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PREFIX].value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_LOGGING_FILE_PREFIX);

    g_config_manager.configs[TFW_CONFIG_LOGGING_MAX_FILE_SIZE].key = TFW_CONFIG_LOGGING_MAX_FILE_SIZE;
    g_config_manager.configs[TFW_CONFIG_LOGGING_MAX_FILE_SIZE].type = TFW_CONFIG_TYPE_INT;
    g_config_manager.configs[TFW_CONFIG_LOGGING_MAX_FILE_SIZE].value.int_value = 10485760;  // 10MB

    g_config_manager.configs[TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS].key = TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS;
    g_config_manager.configs[TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS].type = TFW_CONFIG_TYPE_INT;
    g_config_manager.configs[TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS].value.int_value = 30;

    // Resources configuration / 资源配置
    g_config_manager.configs[TFW_CONFIG_RESOURCES_AUTO_UPDATE].key = TFW_CONFIG_RESOURCES_AUTO_UPDATE;
    g_config_manager.configs[TFW_CONFIG_RESOURCES_AUTO_UPDATE].type = TFW_CONFIG_TYPE_BOOL;
    g_config_manager.configs[TFW_CONFIG_RESOURCES_AUTO_UPDATE].value.bool_value = true;

    g_config_manager.configs[TFW_CONFIG_RESOURCES_UPDATE_INTERVAL].key = TFW_CONFIG_RESOURCES_UPDATE_INTERVAL;
    g_config_manager.configs[TFW_CONFIG_RESOURCES_UPDATE_INTERVAL].type = TFW_CONFIG_TYPE_INT;
    g_config_manager.configs[TFW_CONFIG_RESOURCES_UPDATE_INTERVAL].value.int_value = 3600;  // 1 hour

    g_config_manager.configs[TFW_CONFIG_RESOURCES_CACHE_PATH].key = TFW_CONFIG_RESOURCES_CACHE_PATH;
    g_config_manager.configs[TFW_CONFIG_RESOURCES_CACHE_PATH].type = TFW_CONFIG_TYPE_STRING;
    g_config_manager.configs[TFW_CONFIG_RESOURCES_CACHE_PATH].value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_RESOURCES_CACHE_PATH);

    // System configuration / 系统配置
    g_config_manager.configs[TFW_CONFIG_SYSTEM_MAX_THREADS].key = TFW_CONFIG_SYSTEM_MAX_THREADS;
    g_config_manager.configs[TFW_CONFIG_SYSTEM_MAX_THREADS].type = TFW_CONFIG_TYPE_INT;
    g_config_manager.configs[TFW_CONFIG_SYSTEM_MAX_THREADS].value.int_value = 4;

    g_config_manager.configs[TFW_CONFIG_SYSTEM_TIMEOUT].key = TFW_CONFIG_SYSTEM_TIMEOUT;
    g_config_manager.configs[TFW_CONFIG_SYSTEM_TIMEOUT].type = TFW_CONFIG_TYPE_INT;
    g_config_manager.configs[TFW_CONFIG_SYSTEM_TIMEOUT].value.int_value = 30000;  // 30 seconds
}

/**
 * Free configuration items memory
 * 释放配置项内存
 */
static void FreeConfigItems(void) {
    // Only free memory for string type configuration items
    // 只释放字符串类型配置项的内存

    // Main configuration / 主配置
    if (g_config_manager.configs[TFW_CONFIG_MAIN_VERSION].value.string_value != NULL) {
        TFW_Free((void*)g_config_manager.configs[TFW_CONFIG_MAIN_VERSION].value.string_value);
        g_config_manager.configs[TFW_CONFIG_MAIN_VERSION].value.string_value = NULL;
    }
    // Note: TFW_CONFIG_MAIN_LOG_LEVEL is now INT type, no need to free
    // 注意：TFW_CONFIG_MAIN_LOG_LEVEL 现在是 INT 类型，无需释放

    // Logging configuration / 日志配置
    // Note: TFW_CONFIG_LOGGING_LEVEL is now INT type, no need to free
    // 注意：TFW_CONFIG_LOGGING_LEVEL 现在是 INT 类型，无需释放
    if (g_config_manager.configs[TFW_CONFIG_LOGGING_OUTPUT].value.string_value != NULL) {
        TFW_Free((void*)g_config_manager.configs[TFW_CONFIG_LOGGING_OUTPUT].value.string_value);
        g_config_manager.configs[TFW_CONFIG_LOGGING_OUTPUT].value.string_value = NULL;
    }
    if (g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PATH].value.string_value != NULL) {
        TFW_Free((void*)g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PATH].value.string_value);
        g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PATH].value.string_value = NULL;
    }
    if (g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PREFIX].value.string_value != NULL) {
        TFW_Free((void*)g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PREFIX].value.string_value);
        g_config_manager.configs[TFW_CONFIG_LOGGING_FILE_PREFIX].value.string_value = NULL;
    }

    // Resources configuration / 资源配置
    if (g_config_manager.configs[TFW_CONFIG_RESOURCES_CACHE_PATH].value.string_value != NULL) {
        TFW_Free((void*)g_config_manager.configs[TFW_CONFIG_RESOURCES_CACHE_PATH].value.string_value);
        g_config_manager.configs[TFW_CONFIG_RESOURCES_CACHE_PATH].value.string_value = NULL;
    }
}

// ============================================================================
// Public Configuration Management Interface
// 公共配置管理接口
// ============================================================================

int32_t TFW_Config_Init(const char* config_path) {
    if (config_path == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_manager.initialized) {
        return TFW_SUCCESS;  // Already initialized
    }

    // Copy configuration path
    // 复制配置路径
    strncpy(g_config_manager.config_path, config_path, sizeof(g_config_manager.config_path) - 1);
    g_config_manager.config_path[sizeof(g_config_manager.config_path) - 1] = '\0';

    // Initialize configuration items with default values
    // 使用默认值初始化配置项
    InitializeConfigItems();

    // TODO: Load configuration from file if exists
    // TODO: 如果文件存在，从文件加载配置

    g_config_manager.initialized = true;
    g_config_manager.config_loaded = true;

    return TFW_SUCCESS;
}

int32_t TFW_Config_Deinit(void) {
    if (!g_config_manager.initialized) {
        return TFW_SUCCESS;  // Already deinitialized
    }

    // TODO: Save configuration to file
    // TODO: 保存配置到文件

    // Free configuration items memory
    // 释放配置项内存
    FreeConfigItems();

    // Reset manager state
    // 重置管理器状态
    memset(&g_config_manager, 0, sizeof(g_config_manager));

    return TFW_SUCCESS;
}

int32_t TFW_Config_GetValueByKey(TFW_ConfigKey key, char* value, size_t buffer_size) {
    if (value == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_config_manager.initialized) {
        return TFW_ERROR;
    }

    if (key >= TFW_CONFIG_KEY_COUNT) {
        return TFW_ERROR_INVALID_PARAM;
    }

    TFW_ConfigItem* config_item = &g_config_manager.configs[key];
    int32_t ret = TFW_SUCCESS;

    // Convert value to string based on type
    // 根据类型将值转换为字符串
    switch (config_item->type) {
        case TFW_CONFIG_TYPE_STRING:
            if (config_item->value.string_value == NULL) {
                return TFW_ERROR;
            }
            {
                // Use TFW safe string function
                // 使用TFW安全字符串函数
                if (TFW_Strcpy_S(value, buffer_size, config_item->value.string_value) != TFW_SUCCESS) {
                    return TFW_ERROR;
                }
            }
            break;

        case TFW_CONFIG_TYPE_INT:
            ret = snprintf(value, buffer_size, "%d", config_item->value.int_value);
            if (ret < 0 || ret >= (int)buffer_size) {
                return TFW_ERROR;
            }
            break;

        case TFW_CONFIG_TYPE_BOOL:
            ret = snprintf(value, buffer_size, "%s", config_item->value.bool_value ? "true" : "false");
            if (ret < 0 || ret >= (int)buffer_size) {
                return TFW_ERROR;
            }
            break;

        case TFW_CONFIG_TYPE_FLOAT:
            ret = snprintf(value, buffer_size, "%.6f", config_item->value.float_value);
            if (ret < 0 || ret >= (int)buffer_size) {
                return TFW_ERROR;
            }
            break;

        case TFW_CONFIG_TYPE_INVALID:
        default:
            return TFW_ERROR;
    }

        return TFW_SUCCESS;
    }

int32_t TFW_Config_SetValueByKey(TFW_ConfigKey key, const char* value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_config_manager.initialized) {
        return TFW_ERROR;
    }

    if (key >= TFW_CONFIG_KEY_COUNT) {
        return TFW_ERROR_INVALID_PARAM;
    }

    TFW_ConfigItem* config_item = &g_config_manager.configs[key];

    // Set value based on type
    // 根据类型设置值
    switch (config_item->type) {
        case TFW_CONFIG_TYPE_STRING:
            // Free old string value
            // 释放旧的字符串值
            if (config_item->value.string_value != NULL) {
                TFW_Free((void*)config_item->value.string_value);
            }

            // Allocate and copy new string value using TFW memory management
            // 使用TFW内存管理分配并复制新的字符串值
            config_item->value.string_value = TFW_Strdup(value);
            if (config_item->value.string_value == NULL) {
                return TFW_ERROR;
            }
            break;

        case TFW_CONFIG_TYPE_INT:
            config_item->value.int_value = atoi(value);
            break;

        case TFW_CONFIG_TYPE_BOOL:
            if (strcmp(value, "true") == 0 || strcmp(value, "1") == 0) {
                config_item->value.bool_value = true;
            } else if (strcmp(value, "false") == 0 || strcmp(value, "0") == 0) {
                config_item->value.bool_value = false;
            } else {
                return TFW_ERROR;  // Invalid boolean value
            }
            break;

        case TFW_CONFIG_TYPE_FLOAT:
            config_item->value.float_value = atof(value);
            break;

        case TFW_CONFIG_TYPE_INVALID:
        default:
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

// ============================================================================
// Type-safe Configuration Access
// 类型安全配置访问
// ============================================================================

int32_t TFW_Config_GetString(TFW_ConfigKey key, char* value, size_t buffer_size, const char* default_value) {
    int32_t ret = TFW_Config_GetValueByKey(key, value, buffer_size);
    if (ret != TFW_SUCCESS && default_value != NULL) {
        strncpy(value, default_value, buffer_size - 1);
        value[buffer_size - 1] = '\0';
        return TFW_SUCCESS;
    }
    return ret;
}

int32_t TFW_Config_GetInt(TFW_ConfigKey key, int32_t* value, int32_t default_value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    char str_value[64];
    int32_t ret = TFW_Config_GetValueByKey(key, str_value, sizeof(str_value));
    if (ret == TFW_SUCCESS) {
        *value = atoi(str_value);
    } else {
        *value = default_value;
    }
    return TFW_SUCCESS;
}

int32_t TFW_Config_GetBool(TFW_ConfigKey key, bool* value, bool default_value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    char str_value[16];
    int32_t ret = TFW_Config_GetValueByKey(key, str_value, sizeof(str_value));
    if (ret == TFW_SUCCESS) {
        // Convert string to boolean
        // 将字符串转换为布尔值
        if (strcmp(str_value, "true") == 0 || strcmp(str_value, "1") == 0) {
            *value = true;
        } else if (strcmp(str_value, "false") == 0 || strcmp(str_value, "0") == 0) {
            *value = false;
        } else {
            *value = default_value;
        }
    } else {
        *value = default_value;
    }
    return TFW_SUCCESS;
}

int32_t TFW_Config_GetFloat(TFW_ConfigKey key, float* value, float default_value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    char str_value[64];
    int32_t ret = TFW_Config_GetValueByKey(key, str_value, sizeof(str_value));
    if (ret == TFW_SUCCESS) {
        *value = atof(str_value);
        } else {
        *value = default_value;
    }
    return TFW_SUCCESS;
}

int32_t TFW_Config_SetString(TFW_ConfigKey key, const char* value) {
    return TFW_Config_SetValueByKey(key, value);
}

int32_t TFW_Config_SetInt(TFW_ConfigKey key, int32_t value) {
    char str_value[32];
    snprintf(str_value, sizeof(str_value), "%d", value);
    return TFW_Config_SetValueByKey(key, str_value);
}

int32_t TFW_Config_SetBool(TFW_ConfigKey key, bool value) {
    return TFW_Config_SetValueByKey(key, value ? "true" : "false");
}

int32_t TFW_Config_SetFloat(TFW_ConfigKey key, float value) {
    char str_value[32];
    snprintf(str_value, sizeof(str_value), "%.6f", value);
    return TFW_Config_SetValueByKey(key, str_value);
}

// ============================================================================
// Configuration Management Utilities
// 配置管理工具
// ============================================================================

int32_t TFW_Config_GetKeyName(TFW_ConfigKey key, char* name, size_t buffer_size) {
    if (name == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (key >= TFW_CONFIG_KEY_COUNT) {
        if (buffer_size < 8) {  // "unknown" + null terminator
            return TFW_ERROR;
        }
        // Use TFW safe string function
        // 使用TFW安全字符串函数
        if (TFW_Strcpy_S(name, buffer_size, "unknown") != TFW_SUCCESS) {
            return TFW_ERROR;
        }
        return TFW_SUCCESS;
    }

    // Use the internal GetKeyName function with buffer passing
    // 使用内部 GetKeyName 函数进行缓冲区传递
    return GetKeyName(key, name, buffer_size);
}

TFW_ConfigValueType TFW_Config_GetKeyType(TFW_ConfigKey key) {
    if (key >= TFW_CONFIG_KEY_COUNT) {
        return TFW_CONFIG_TYPE_INVALID;  // 无效key返回无效类型
    }
    return GetKeyType(key);
}

bool TFW_Config_IsInitialized(void) {
    return g_config_manager.initialized;
}
