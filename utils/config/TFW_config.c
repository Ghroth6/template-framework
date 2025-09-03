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
#include "TFW_json.h"

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

/**
 * Update configuration values from JSON object
 * 从JSON对象更新配置值
 * @param json_root JSON root object / JSON根对象
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
static int32_t UpdateConfigFromJson(const TFW_JsonValue* json_root) {
    if (json_root == NULL || json_root->type != TFW_JSON_TYPE_OBJECT) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Update each configuration item from JSON
    // 从JSON更新每个配置项
    for (TFW_ConfigKey key = 0; key < TFW_CONFIG_KEY_COUNT; key++) {
        char key_name[64];
        if (GetKeyName(key, key_name, sizeof(key_name)) != TFW_SUCCESS) {
            continue;
        }

        const TFW_JsonValue* json_value = TFW_Json_GetObjectValue(json_root, key_name);
        if (json_value == NULL) {
            continue; // Key not found in JSON, keep default value
        }

        TFW_ConfigItem* config_item = &g_config_manager.configs[key];

        // Update value based on type
        // 根据类型更新值
        switch (config_item->type) {
            case TFW_CONFIG_TYPE_STRING:
                if (json_value->type == TFW_JSON_TYPE_STRING) {
                    // Free old string value
                    // 释放旧的字符串值
                    if (config_item->value.string_value != NULL) {
                        TFW_Free((void*)config_item->value.string_value);
                    }
                    // Set new string value
                    // 设置新的字符串值
                    config_item->value.string_value = TFW_Strdup(json_value->value.string_value);
                }
                break;

            case TFW_CONFIG_TYPE_INT:
                if (json_value->type == TFW_JSON_TYPE_INT) {
                    config_item->value.int_value = (int32_t)json_value->value.int_value;
                } else if (json_value->type == TFW_JSON_TYPE_FLOAT) {
                    config_item->value.int_value = (int32_t)json_value->value.float_value;
                }
                break;

            case TFW_CONFIG_TYPE_BOOL:
                if (json_value->type == TFW_JSON_TYPE_BOOL) {
                    config_item->value.bool_value = json_value->value.bool_value;
                }
                break;

            case TFW_CONFIG_TYPE_FLOAT:
                if (json_value->type == TFW_JSON_TYPE_FLOAT) {
                    config_item->value.float_value = (float)json_value->value.float_value;
                } else if (json_value->type == TFW_JSON_TYPE_INT) {
                    config_item->value.float_value = (float)json_value->value.int_value;
                }
                break;

            default:
                break;
        }
    }

    return TFW_SUCCESS;
}

/**
 * Create JSON object from configuration values
 * 从配置值创建JSON对象
 * @param json_root JSON root object / JSON根对象
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
static int32_t CreateJsonFromConfig(TFW_JsonValue* json_root) {
    if (json_root == NULL || json_root->type != TFW_JSON_TYPE_OBJECT) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Add each configuration item to JSON object
    // 将每个配置项添加到JSON对象
    for (TFW_ConfigKey key = 0; key < TFW_CONFIG_KEY_COUNT; key++) {
        char key_name[64];
        if (GetKeyName(key, key_name, sizeof(key_name)) != TFW_SUCCESS) {
            continue;
        }

        TFW_ConfigItem* config_item = &g_config_manager.configs[key];
        TFW_JsonValue* json_value = NULL;

        // Create JSON value based on configuration type
        // 根据配置类型创建JSON值
        switch (config_item->type) {
            case TFW_CONFIG_TYPE_STRING:
                if (config_item->value.string_value != NULL) {
                    json_value = TFW_Json_CreateString(config_item->value.string_value);
                } else {
                    json_value = TFW_Json_CreateString("");
                }
                break;

            case TFW_CONFIG_TYPE_INT:
                json_value = TFW_Json_CreateInt(config_item->value.int_value);
                break;

            case TFW_CONFIG_TYPE_BOOL:
                json_value = TFW_Json_CreateBool(config_item->value.bool_value);
                break;

            case TFW_CONFIG_TYPE_FLOAT:
                json_value = TFW_Json_CreateFloat(config_item->value.float_value);
                break;

            default:
                continue; // Skip invalid types / 跳过无效类型
        }

        if (json_value == NULL) {
            TFW_LOGE_UTILS("Failed to create JSON value for key: %s", key_name);
            continue;
        }

        // Add to JSON object
        // 添加到JSON对象
        int32_t set_result = TFW_Json_SetObjectValue(json_root, key_name, json_value);
        TFW_Json_Free(json_value);

        if (set_result != TFW_SUCCESS) {
            TFW_LOGE_UTILS("Failed to set JSON value for key: %s", key_name);
            return TFW_ERROR;
        }
    }

    return TFW_SUCCESS;
}

/**
 * Load configuration from file
 * 从文件加载配置
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
static int32_t LoadConfigFromFile(void) {
    // Check if configuration file exists
    // 检查配置文件是否存在
    if (!TFW_IsFileExists(g_config_manager.config_path)) {
        TFW_LOGI_UTILS("Configuration file does not exist: %s", g_config_manager.config_path);
        return TFW_ERROR_NOT_FOUND;  // File not found, will create new file
    }

    TFW_LOGI_UTILS("Loading configuration from file: %s", g_config_manager.config_path);

    // Read JSON file content
    // 读取JSON文件内容
    FILE* file = fopen(g_config_manager.config_path, "r");
    if (file == NULL) {
        TFW_LOGE_UTILS("Failed to open configuration file: %s", g_config_manager.config_path);
        return TFW_ERROR_FILE_ERROR;
    }

    // Get file size
    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) {
        fclose(file);
        TFW_LOGE_UTILS("Configuration file is empty: %s", g_config_manager.config_path);
        return TFW_ERROR_FILE_ERROR;
    }

    // Allocate buffer for file content
    // 为文件内容分配缓冲区
    char* json_content = (char*)TFW_Malloc((uint32_t)(file_size + 1));
    if (json_content == NULL) {
        fclose(file);
        TFW_LOGE_UTILS("Failed to allocate memory for configuration file content");
        return TFW_ERROR;
    }

    // Read file content
    // 读取文件内容
    size_t bytes_read = fread(json_content, 1, file_size, file);
    fclose(file);

    if (bytes_read != (size_t)file_size) {
        TFW_Free(json_content);
        TFW_LOGE_UTILS("Failed to read complete configuration file");
        return TFW_ERROR_FILE_ERROR;
    }

    json_content[file_size] = '\0';

    // Parse JSON content
    // 解析JSON内容
    TFW_JsonValue* json_root = NULL;
    int32_t parse_result = TFW_Json_Parse(json_content, &json_root);
    TFW_Free(json_content);

    if (parse_result != TFW_SUCCESS || json_root == NULL) {
        TFW_LOGE_UTILS("Failed to parse configuration file JSON");
        if (json_root) {
            TFW_Json_Free(json_root);
        }
        return TFW_ERROR_FILE_ERROR;
    }

    // Update configuration values from JSON
    // 从JSON更新配置值
    int32_t update_result = UpdateConfigFromJson(json_root);
    TFW_Json_Free(json_root);

    if (update_result != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to update configuration from JSON");
        return TFW_ERROR;
    }

    TFW_LOGI_UTILS("Configuration loaded successfully from file");
    return TFW_SUCCESS;
}

/**
 * Save configuration to file
 * 保存配置到文件
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
static int32_t SaveConfigToFile(void) {
    // Get directory path from config file path
    // 从配置文件路径获取目录路径
    char config_dir[TFW_PATH_LEN_MAX];
    if (TFW_GetFileDirectory(g_config_manager.config_path, config_dir, sizeof(config_dir)) != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to get configuration directory path");
        return TFW_ERROR_FILE_ERROR;
    }

    // Create directory if it doesn't exist
    // 如果目录不存在则创建
    if (!TFW_IsDirectory(config_dir)) {
        TFW_LOGI_UTILS("Creating configuration directory: %s", config_dir);
        if (!TFW_CreateDirectory(config_dir)) {
            TFW_LOGE_UTILS("Failed to create configuration directory: %s", config_dir);
            return TFW_ERROR_FILE_ERROR;
        }
    }

    TFW_LOGI_UTILS("Saving configuration to file: %s", g_config_manager.config_path);

    // Create JSON object from configuration
    // 从配置创建JSON对象
    TFW_JsonValue* json_root = TFW_Json_CreateObject();
    if (json_root == NULL) {
        TFW_LOGE_UTILS("Failed to create JSON object for configuration");
        return TFW_ERROR;
    }

    // Add configuration values to JSON object
    // 将配置值添加到JSON对象
    int32_t json_result = CreateJsonFromConfig(json_root);
    if (json_result != TFW_SUCCESS) {
        TFW_Json_Free(json_root);
        TFW_LOGE_UTILS("Failed to create JSON from configuration");
        return TFW_ERROR;
    }

    // Serialize JSON to string
    // 将JSON序列化为字符串
    char json_buffer[4096]; // Buffer for JSON string / JSON字符串缓冲区
    int32_t serialize_result = TFW_Json_Serialize(json_root, json_buffer, sizeof(json_buffer));
    TFW_Json_Free(json_root);

    if (serialize_result != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to serialize configuration to JSON");
        return TFW_ERROR;
    }

    // Write JSON string to file
    // 将JSON字符串写入文件
    FILE* file = fopen(g_config_manager.config_path, "w");
    if (file == NULL) {
        TFW_LOGE_UTILS("Failed to open configuration file for writing: %s", g_config_manager.config_path);
        return TFW_ERROR_FILE_ERROR;
    }

    size_t json_len = strlen(json_buffer);
    size_t bytes_written = fwrite(json_buffer, 1, json_len, file);
    fclose(file);

    if (bytes_written != json_len) {
        TFW_LOGE_UTILS("Failed to write complete configuration file");
        return TFW_ERROR_FILE_ERROR;
    }

    TFW_LOGI_UTILS("Configuration saved successfully to file");
    return TFW_SUCCESS;
}

/**
 * Load or create configuration file
 * 加载或创建配置文件
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
static int32_t LoadOrCreateConfigFile(void) {
    // Try to load configuration from file
    // 尝试从文件加载配置
    int32_t load_result = LoadConfigFromFile();

    if (load_result == TFW_SUCCESS) {
        // Successfully loaded configuration from file
        // 成功从文件加载配置
        TFW_LOGI_UTILS("Configuration loaded successfully from file");
        g_config_manager.config_loaded = true;
        return TFW_SUCCESS;
    } else if (load_result == TFW_ERROR_NOT_FOUND) {
        // Failed to load from file, create new configuration file
        // 从文件加载失败，创建新的配置文件
        TFW_LOGI_UTILS("Failed to load configuration from file, creating new file");

        if (SaveConfigToFile() != TFW_SUCCESS) {
            TFW_LOGE_UTILS("Failed to create configuration file");
            return TFW_ERROR;
        }

        TFW_LOGI_UTILS("New configuration file created successfully");
        g_config_manager.config_loaded = true;
        return TFW_SUCCESS;
    } else {
        TFW_LOGE_UTILS("Failed to load configuration from file");
        return TFW_ERROR_FILE_ERROR;
    }
}

// ============================================================================
// Public Configuration Management Interface
// 公共配置管理接口
// ============================================================================

int32_t TFW_Config_Init(void) {
    if (g_config_manager.initialized) {
        TFW_LOGI_UTILS("Configuration already initialized");
        return TFW_SUCCESS;  // Already initialized
    }

    // Build complete configuration file path using TFW common defines
    // 使用TFW通用定义构建完整的配置文件路径
    // Format: "runtime/TFW_config.json"
    // 格式："runtime/TFW_config.json"
    if (TFW_Strcpy_S(g_config_manager.config_path, sizeof(g_config_manager.config_path), TFW_CONFIG_FILE_PATH) != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to copy configuration file path");
        return TFW_ERROR;
    }

    if (TFW_Strcat_S(g_config_manager.config_path, sizeof(g_config_manager.config_path), "/") != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to concatenate configuration file path");
        return TFW_ERROR;
    }

    if (TFW_Strcat_S(g_config_manager.config_path, sizeof(g_config_manager.config_path), TFW_CONFIG_FILE_NAME) != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to concatenate configuration file name");
        return TFW_ERROR;
    }

    // Initialize configuration items with default values
    // 使用默认值初始化配置项
    InitializeConfigItems();

    // Load or create configuration file
    // 加载或创建配置文件
    if (LoadOrCreateConfigFile() != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to load or create configuration file");
        return TFW_ERROR_INIT_FAIL;
    }

    g_config_manager.initialized = true;
    TFW_LOGI_UTILS("Configuration initialized successfully");
    return TFW_SUCCESS;
}

int32_t TFW_Config_Deinit(void) {
    if (!g_config_manager.initialized) {
        TFW_LOGI_UTILS("Configuration already deinitialized");
        return TFW_SUCCESS;  // Already deinitialized
    }

    // Save configuration to file before deinitializing
    // 反初始化前保存配置到文件
    if (SaveConfigToFile() != TFW_SUCCESS) {
        TFW_LOGW_UTILS("Failed to save configuration to file during deinit");
        // Continue with deinitialization even if save fails
        // 即使保存失败也继续反初始化
    }

    // Free configuration items memory
    // 释放配置项内存
    FreeConfigItems();

    // Reset manager state
    // 重置管理器状态
    if (TFW_Memset_S(&g_config_manager, sizeof(g_config_manager), 0, sizeof(g_config_manager)) != TFW_SUCCESS) {
        return TFW_ERROR_DEINIT_FAIL;
    }
    TFW_LOGI_UTILS("Configuration deinitialized successfully");
    return TFW_SUCCESS;
}

static int32_t GetValueByKey(TFW_ConfigKey key, void* value, size_t buffer_size) {
    if (value == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_config_manager.initialized) {
        TFW_LOGI_UTILS("Configuration not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (key >= TFW_CONFIG_KEY_COUNT) {
        return TFW_ERROR_INVALID_PARAM;
    }

    TFW_ConfigItem* config_item = &g_config_manager.configs[key];

    // Copy value based on type using direct memory copy
    // 根据类型使用直接内存复制
    switch (config_item->type) {
        case TFW_CONFIG_TYPE_STRING:
            if (config_item->value.string_value == NULL) {
                return TFW_ERROR_INVALID_PARAM;
            }
            {
                // Use TFW safe string function for string type
                // 字符串类型使用TFW安全字符串函数
                if (TFW_Strcpy_S((char*)value, buffer_size, config_item->value.string_value) != TFW_SUCCESS) {
                    TFW_LOGE_UTILS("Failed to copy string value");
                    return TFW_ERROR;
                }
            }
            break;

        case TFW_CONFIG_TYPE_INT:
            if (buffer_size < sizeof(int32_t)) {
                TFW_LOGE_UTILS("Buffer too small for int value");
                return TFW_ERROR_INVALID_PARAM;
            }
            // Direct memory copy for int type
            // 整数类型直接内存复制
            *(int32_t*)value = config_item->value.int_value;
            break;

        case TFW_CONFIG_TYPE_BOOL:
            if (buffer_size < sizeof(bool)) {
                TFW_LOGE_UTILS("Buffer too small for bool value");
    return TFW_ERROR_INVALID_PARAM;
}
            // Direct memory copy for bool type
            // 布尔类型直接内存复制
            *(bool*)value = config_item->value.bool_value;
            break;

        case TFW_CONFIG_TYPE_FLOAT:
            if (buffer_size < sizeof(float)) {
                TFW_LOGE_UTILS("Buffer too small for float value");
        return TFW_ERROR_INVALID_PARAM;
            }
            // Direct memory copy for float type
            // 浮点类型直接内存复制
            *(float*)value = config_item->value.float_value;
            break;

        case TFW_CONFIG_TYPE_INVALID:
        default:
            return TFW_ERROR;
    }

        return TFW_SUCCESS;
    }

static int32_t SetValueByKey(TFW_ConfigKey key, const void* value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_config_manager.initialized) {
        TFW_LOGI_UTILS("Configuration not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGI_UTILS("Invalid key");
        return TFW_ERROR_INVALID_PARAM;
    }

    TFW_ConfigItem* config_item = &g_config_manager.configs[key];

    // Set value based on type using direct memory copy
    // 根据类型使用直接内存复制设置值
    switch (config_item->type) {
        case TFW_CONFIG_TYPE_STRING:
            // Free old string value
            // 释放旧的字符串值
            if (config_item->value.string_value != NULL) {
                TFW_Free((void*)config_item->value.string_value);
            }

            // Allocate and copy new string value using TFW memory management
            // 使用TFW内存管理分配并复制新的字符串值
            config_item->value.string_value = TFW_Strdup((const char*)value);
            if (config_item->value.string_value == NULL) {
                return TFW_ERROR;
            }
            break;

        case TFW_CONFIG_TYPE_INT:
            // Direct memory copy for int type
            // 整数类型直接内存复制
            config_item->value.int_value = *(const int32_t*)value;
            break;

        case TFW_CONFIG_TYPE_BOOL:
            // Direct memory copy for bool type
            // 布尔类型直接内存复制
            config_item->value.bool_value = *(const bool*)value;
            break;

        case TFW_CONFIG_TYPE_FLOAT:
            // Direct memory copy for float type
            // 浮点类型直接内存复制
            config_item->value.float_value = *(const float*)value;
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
    int32_t ret = GetValueByKey(key, value, buffer_size);
    if (ret != TFW_SUCCESS && default_value != NULL) {
        // Use TFW safe string function for default value
        // 使用TFW安全字符串函数处理默认值
        if (TFW_Strcpy_S(value, buffer_size, default_value) != TFW_SUCCESS) {
        return TFW_ERROR;
    }
        return TFW_SUCCESS;
    }
    return ret;
}

int32_t TFW_Config_GetInt(TFW_ConfigKey key, int32_t* value, int32_t default_value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = GetValueByKey(key, value, sizeof(int32_t));
    if (ret != TFW_SUCCESS) {
        *value = default_value;
    }
    return TFW_SUCCESS;
}

int32_t TFW_Config_GetBool(TFW_ConfigKey key, bool* value, bool default_value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = GetValueByKey(key, value, sizeof(bool));
    if (ret != TFW_SUCCESS) {
        *value = default_value;
    }
    return TFW_SUCCESS;
}

int32_t TFW_Config_GetFloat(TFW_ConfigKey key, float* value, float default_value) {
    if (value == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = GetValueByKey(key, value, sizeof(float));
    if (ret != TFW_SUCCESS) {
        *value = default_value;
    }
    return TFW_SUCCESS;
}

int32_t TFW_Config_SetString(TFW_ConfigKey key, const char* value) {
    return SetValueByKey(key, value);
}

int32_t TFW_Config_SetInt(TFW_ConfigKey key, int32_t value) {
    return SetValueByKey(key, &value);
}

int32_t TFW_Config_SetBool(TFW_ConfigKey key, bool value) {
    return SetValueByKey(key, &value);
}

int32_t TFW_Config_SetFloat(TFW_ConfigKey key, float value) {
    return SetValueByKey(key, &value);
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
            TFW_LOGI_UTILS("Buffer too small for key name");
            return TFW_ERROR;
        }
        // Use TFW safe string function
        // 使用TFW安全字符串函数
        if (TFW_Strcpy_S(name, buffer_size, "unknown") != TFW_SUCCESS) {
            TFW_LOGI_UTILS("Failed to copy key name");
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
