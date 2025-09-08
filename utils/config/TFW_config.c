/*
 * TFW_config.c
 *
 * TFW 配置管理模块实现文件
 *
 * 该文件实现了配置管理相关的函数接口
 */

#include "TFW_config.h"
#include "TFW_file.h"
#include "TFW_utils_log.h"
#include "TFW_json.h"
#include "TFW_common_defines.h"
#include "TFW_mem.h"
#include "TFW_errorno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 配置模块内部状态
static TFW_ConfigItem g_config_items[TFW_CONFIG_KEY_COUNT];  // 配置项数组
static bool g_initialized = false;  // 配置模块初始化状态
static TFW_ConfigUpdateCallback g_callbacks[TFW_CONFIG_MODULE_SYSTEM + 1] = {NULL};  // 配置更新回调

// 内部辅助函数声明
static int32_t TFW_ConfigLoadFromFile(const char *file_path);
static int32_t TFW_ConfigCreateDefault(void);
static void TFW_ConfigNotifyUpdate(TFW_ConfigKey key, const TFW_ConfigItem *item);
static const char *TFW_ConfigKeyToString(TFW_ConfigKey key);
static TFW_ConfigKey TFW_ConfigStringToKey(const char *key_str);
static int32_t JsonToConfigItems(cJSON* json_root);
static int32_t ConfigItemsToJson(cJSON* json_root);

// 配置模块初始化
int32_t TFW_ConfigInit(void) {
    // 初始化配置项数组
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        g_config_items[i].key = (TFW_ConfigKey)i;
        g_config_items[i].type = TFW_CONFIG_TYPE_INVALID;
        g_config_items[i].value.string_value = NULL;
    }

    // 尝试从文件加载配置
    char config_file_path[TFW_PATH_LEN_MAX];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", TFW_CONFIG_FILE_PATH, TFW_CONFIG_FILE_NAME);

    int32_t ret = TFW_ConfigLoadFromFile(config_file_path);
    if (ret != TFW_SUCCESS) {
        // 加载失败，创建默认配置
        TFW_LOGW_UTILS("Failed to load config from file, creating default config");
        ret = TFW_ConfigCreateDefault();
        if (ret != TFW_SUCCESS) {
            TFW_LOGE_UTILS("Failed to create default config");
            return ret;
        }
    }

    g_initialized = true;
    TFW_LOGI_UTILS("Config module initialized successfully");
    return TFW_SUCCESS;
}

// 配置模块反初始化
int32_t TFW_ConfigDeinit(void) {
    // 保存配置到文件
    if (g_initialized) {
        TFW_ConfigSave();
    }

    // 释放资源
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        TFW_ConfigFreeItem(&g_config_items[i]);
    }

    g_initialized = false;

    // 清空回调函数
    for (int i = 0; i <= TFW_CONFIG_MODULE_SYSTEM; i++) {
        g_callbacks[i] = NULL;
    }

    TFW_LOGI_UTILS("Config module deInitialized successfully");
    return TFW_SUCCESS;
}

// 注册配置更新回调
int32_t TFW_ConfigRegisterUpdateCallback(TFW_ConfigModule module, TFW_ConfigUpdateCallback callback) {
    if (callback == NULL || module > TFW_CONFIG_MODULE_SYSTEM) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigRegisterUpdateCallback");
        return TFW_ERROR_INVALID_PARAM;
    }

    g_callbacks[module] = callback;
    TFW_LOGI_UTILS("Config update callback registered successfully for module %d", module);
    return TFW_SUCCESS;
}

// 取消注册配置更新回调
int32_t TFW_ConfigUnregisterUpdateCallback(TFW_ConfigModule module) {
    if (module > TFW_CONFIG_MODULE_SYSTEM) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigUnregisterUpdateCallback");
        return TFW_ERROR_INVALID_PARAM;
    }

    // 无论传入的callback是什么，都将对应模块的回调设置为NULL
    // 该实现与设计意图一致：每个模块只支持注册一个回调函数
    g_callbacks[module] = NULL;
    TFW_LOGI_UTILS("Config update callback unregistered successfully for module %d", module);
    return TFW_SUCCESS;
}

// 读取全部配置
int32_t TFW_ConfigGetAll(TFW_ConfigItem **config_array, uint32_t *count) {
    if (config_array == NULL || count == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetAll");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    *count = TFW_CONFIG_KEY_COUNT;
    *config_array = (TFW_ConfigItem *)TFW_Calloc(*count * sizeof(TFW_ConfigItem));
    if (*config_array == NULL) {
        TFW_LOGE_UTILS("Failed to allocate memory for config array");
        return TFW_ERROR;
    }

    // 复制配置项数组
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        (*config_array)[i].key = g_config_items[i].key;
        (*config_array)[i].type = g_config_items[i].type;

        // 根据类型复制值，特别是字符串类型需要深拷贝
        switch (g_config_items[i].type) {
            case TFW_CONFIG_TYPE_STRING:
                if (g_config_items[i].value.string_value != NULL) {
                    (*config_array)[i].value.string_value = TFW_Strdup(g_config_items[i].value.string_value);
                    if ((*config_array)[i].value.string_value == NULL) {
                        TFW_LOGE_UTILS("Failed to allocate memory for string value");
                        // 释放已分配的内存
                        for (int j = 0; j < i; j++) {
                            if ((*config_array)[j].type == TFW_CONFIG_TYPE_STRING &&
                                (*config_array)[j].value.string_value != NULL) {
                                TFW_Free((void*)(*config_array)[j].value.string_value);
                            }
                        }
                        TFW_Free(*config_array);
                        *config_array = NULL;
                        return TFW_ERROR;
                    }
                } else {
                    (*config_array)[i].value.string_value = NULL;
                }
                break;
            case TFW_CONFIG_TYPE_INT:
                (*config_array)[i].value.int_value = g_config_items[i].value.int_value;
                break;
            case TFW_CONFIG_TYPE_BOOL:
                (*config_array)[i].value.bool_value = g_config_items[i].value.bool_value;
                break;
            case TFW_CONFIG_TYPE_FLOAT:
                (*config_array)[i].value.float_value = g_config_items[i].value.float_value;
                break;
            default:
                break;
        }
    }

    return TFW_SUCCESS;
}

// 释放配置数组内存
void TFW_ConfigFreeAll(TFW_ConfigItem *config_array) {
    if (config_array != NULL) {
        // 释放字符串类型的内存
        for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
            if (config_array[i].type == TFW_CONFIG_TYPE_STRING &&
                config_array[i].value.string_value != NULL) {
                TFW_Free((void*)config_array[i].value.string_value);
                config_array[i].value.string_value = NULL;
            }
        }
        TFW_Free(config_array);
    }
}

// 读取单项配置
int32_t TFW_ConfigGetItem(TFW_ConfigKey key, TFW_ConfigItem *item) {
    if (item == NULL || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetItem");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    item->key = g_config_items[key].key;
    item->type = g_config_items[key].type;

    // 根据类型复制值
    switch (g_config_items[key].type) {
        case TFW_CONFIG_TYPE_STRING:
            if (g_config_items[key].value.string_value != NULL) {
                item->value.string_value = TFW_Strdup(g_config_items[key].value.string_value);
                if (item->value.string_value == NULL) {
                    TFW_LOGE_UTILS("Failed to allocate memory for string value");
                    return TFW_ERROR;
                }
            } else {
                item->value.string_value = NULL;
            }
            break;
        case TFW_CONFIG_TYPE_INT:
            item->value.int_value = g_config_items[key].value.int_value;
            break;
        case TFW_CONFIG_TYPE_BOOL:
            item->value.bool_value = g_config_items[key].value.bool_value;
            break;
        case TFW_CONFIG_TYPE_FLOAT:
            item->value.float_value = g_config_items[key].value.float_value;
            break;
        default:
            break;
    }

    return TFW_SUCCESS;
}

static int32_t TFW_ConfigSetItemInner(TFW_ConfigKey key, const TFW_ConfigItem *item) {
    // 释放旧值（如果是字符串）
    TFW_ConfigFreeItem(&g_config_items[key]);

    // 复制新值
    g_config_items[key].type = item->type;
    switch (item->type) {
        case TFW_CONFIG_TYPE_STRING:
            if (item->value.string_value != NULL) {
                g_config_items[key].value.string_value = TFW_Strdup(item->value.string_value);
                if (g_config_items[key].value.string_value == NULL) {
                    TFW_LOGE_UTILS("Failed to allocate memory for string value");
                    g_config_items[key].type = TFW_CONFIG_TYPE_INVALID;
                    return TFW_ERROR;
                }
            } else {
                g_config_items[key].value.string_value = NULL;
            }
            break;
        case TFW_CONFIG_TYPE_INT:
            g_config_items[key].value.int_value = item->value.int_value;
            break;
        case TFW_CONFIG_TYPE_BOOL:
            g_config_items[key].value.bool_value = item->value.bool_value;
            break;
        case TFW_CONFIG_TYPE_FLOAT:
            g_config_items[key].value.float_value = item->value.float_value;
            break;
        default:
            TFW_LOGE_UTILS("Unsupported config type: %d", item->type);
            return TFW_ERROR;
    }

    TFW_ConfigNotifyUpdate(key, item);
    return TFW_SUCCESS;
}

// 设置单项配置
int32_t TFW_ConfigSetItem(TFW_ConfigKey key, const TFW_ConfigItem *item) {
    if (item == NULL || key >= TFW_CONFIG_KEY_COUNT || key != item->key) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetItem");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    return TFW_ConfigSetItemInner(key, item);
}

// 读取单项配置（整数）
int32_t TFW_ConfigGetInt(TFW_ConfigKey key, int32_t *value) {
    if (value == NULL || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetInt");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    if (g_config_items[key].type != TFW_CONFIG_TYPE_INT) {
        TFW_LOGW_UTILS("Config key '%d' is not an integer", key);
        return TFW_ERROR;
    }

    *value = g_config_items[key].value.int_value;
    return TFW_SUCCESS;
}

// 设置单项配置（整数）
int32_t TFW_ConfigSetInt(TFW_ConfigKey key, int32_t value) {
    if (key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetInt");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    // 创建临时配置项用于通知
    TFW_ConfigItem config_item;
    config_item.key = key;
    config_item.type = TFW_CONFIG_TYPE_INT;
    config_item.value.int_value = value;

    return TFW_ConfigSetItem(key, &config_item);
}

// 读取单项配置（浮点数）
int32_t TFW_ConfigGetFloat(TFW_ConfigKey key, float *value) {
    if (value == NULL || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetFloat");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    if (g_config_items[key].type != TFW_CONFIG_TYPE_FLOAT) {
        TFW_LOGW_UTILS("Config key '%d' is not a float", key);
        return TFW_ERROR;
    }

    *value = g_config_items[key].value.float_value;
    return TFW_SUCCESS;
}

// 设置单项配置（浮点数）
int32_t TFW_ConfigSetFloat(TFW_ConfigKey key, float value) {
    if (key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetFloat");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    // 创建临时配置项用于通知
    TFW_ConfigItem config_item;
    config_item.key = key;
    config_item.type = TFW_CONFIG_TYPE_FLOAT;
    config_item.value.float_value = value;

    return TFW_ConfigSetItem(key, &config_item);
}

// 读取单项配置（布尔值）
int32_t TFW_ConfigGetBool(TFW_ConfigKey key, bool *value) {
    if (value == NULL || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetBool");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    if (g_config_items[key].type != TFW_CONFIG_TYPE_BOOL) {
        TFW_LOGW_UTILS("Config key '%d' is not a boolean", key);
        return TFW_ERROR;
    }

    *value = g_config_items[key].value.bool_value;
    return TFW_SUCCESS;
}

// 设置单项配置（布尔值）
int32_t TFW_ConfigSetBool(TFW_ConfigKey key, bool value) {
    if (key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetBool");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    // 创建临时配置项用于通知
    TFW_ConfigItem config_item;
    config_item.key = key;
    config_item.type = TFW_CONFIG_TYPE_BOOL;
    config_item.value.bool_value = value;

    return TFW_ConfigSetItem(key, &config_item);
}

// 读取单项配置（字符串）
int32_t TFW_ConfigGetString(TFW_ConfigKey key, char **value) {
    if (value == NULL || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetString");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    if (g_config_items[key].type != TFW_CONFIG_TYPE_STRING) {
        TFW_LOGW_UTILS("Config key '%d' is not a string", key);
        return TFW_ERROR;
    }

    if (g_config_items[key].value.string_value != NULL) {
        *value = TFW_Strdup(g_config_items[key].value.string_value);
        if (*value == NULL) {
            TFW_LOGE_UTILS("Failed to allocate memory for string value");
            return TFW_ERROR;
        }
    } else {
        *value = NULL;
    }

    return TFW_SUCCESS;
}

// 设置单项配置（字符串）
int32_t TFW_ConfigSetString(TFW_ConfigKey key, const char *value) {
    if (key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetString");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    // 创建临时配置项用于通知
    TFW_ConfigItem config_item;
    config_item.key = key;
    config_item.type = TFW_CONFIG_TYPE_STRING;
    config_item.value.string_value = value; // TFW_ConfigSetItem会复制字符串

    return TFW_ConfigSetItem(key, &config_item);
}

// 保存配置到文件
int32_t TFW_ConfigSave(void) {
    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not properly initialized");
        return TFW_ERROR;
    }

    char config_file_path[TFW_PATH_LEN_MAX];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", TFW_CONFIG_FILE_PATH, TFW_CONFIG_FILE_NAME);

    // 创建JSON对象
    cJSON* json_root = cJSON_CreateObject();
    if (json_root == NULL) {
        TFW_LOGE_UTILS("Failed to create JSON object");
        return TFW_ERROR;
    }

    // 将配置项数组转换为JSON对象
    int32_t ret = ConfigItemsToJson(json_root);
    if (ret != TFW_SUCCESS) {
        cJSON_Delete(json_root);
        TFW_LOGE_UTILS("Failed to convert config items to JSON");
        return ret;
    }

    // 序列化JSON对象
    char* json_string = cJSON_Print(json_root);
    cJSON_Delete(json_root);

    if (json_string == NULL) {
        TFW_LOGE_UTILS("Failed to print config to JSON string");
        return TFW_ERROR;
    }

    // 写入文件
    int32_t write_ret = TFW_WriteFile(config_file_path, json_string, strlen(json_string));
    free(json_string);

    if (write_ret < 0) {
        TFW_LOGE_UTILS("Failed to write config to file");
        return TFW_ERROR;
    }

    TFW_LOGI_UTILS("Config saved to file successfully");
    return TFW_SUCCESS;
}

// 从文件重新加载配置
int32_t TFW_ConfigReload(void) {
    if (!g_initialized) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    // 保存当前配置（用于出错时恢复）
    TFW_ConfigItem backup_items[TFW_CONFIG_KEY_COUNT];
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        backup_items[i] = g_config_items[i];
        // 对于字符串类型，需要复制字符串内容
        if (backup_items[i].type == TFW_CONFIG_TYPE_STRING && backup_items[i].value.string_value != NULL) {
            backup_items[i].value.string_value = TFW_Strdup(backup_items[i].value.string_value);
        }
    }

    char config_file_path[TFW_PATH_LEN_MAX];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", TFW_CONFIG_FILE_PATH, TFW_CONFIG_FILE_NAME);

    int32_t ret = TFW_ConfigLoadFromFile(config_file_path);
    if (ret != TFW_SUCCESS) {
        // 加载失败，恢复备份
        for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
            TFW_ConfigFreeItem(&g_config_items[i]);
            g_config_items[i] = backup_items[i];
        }
        TFW_LOGE_UTILS("Failed to reload config from file, restored backup");
    } else {
        // 加载成功，释放备份
        for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
            TFW_ConfigFreeItem(&backup_items[i]);
        }
        TFW_LOGI_UTILS("Config reloaded from file successfully");
    }

    return ret;
}

// 内部辅助函数实现
static int32_t TFW_ConfigLoadFromFile(const char *file_path) {
    if (file_path == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // 检查文件是否存在
    if (!TFW_IsFileExists(file_path)) {
        TFW_LOGW_UTILS("Config file does not exist: %s", file_path);
        return TFW_ERROR;
    }

    // 获取文件大小
    uint64_t file_size;
    int32_t ret = TFW_GetFileSize(file_path, &file_size);
    if (ret != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to get config file size");
        return ret;
    }

    // 分配内存读取文件
    char *file_content = (char *)TFW_Malloc(file_size + 1);
    if (file_content == NULL) {
        TFW_LOGE_UTILS("Failed to allocate memory for file content");
        return TFW_ERROR;
    }

    // 读取文件内容
    int32_t size;
    ret = TFW_ReadFullFileAndSize(file_path, file_content, file_size + 1, &size);
    if (ret != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to read config file");
        TFW_Free(file_content);
        return ret;
    }

    // 解析JSON
    cJSON *json_root = cJSON_Parse(file_content);
    TFW_Free(file_content);

    if (json_root == NULL) {
        TFW_LOGE_UTILS("Failed to parse config file as JSON");
        return TFW_ERROR;
    }

    // 将JSON对象转换为配置项数组
    ret = JsonToConfigItems(json_root);
    cJSON_Delete(json_root);

    if (ret != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to convert JSON to config items");
        return TFW_ERROR;
    }

    TFW_LOGI_UTILS("Config loaded from file successfully");
    return TFW_SUCCESS;
}

static int32_t TFW_ConfigCreateDefault(void) {
    // 设置默认配置项
    TFW_ConfigItem item;

    // Main configuration
    item.key = TFW_CONFIG_MAIN_VERSION;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = 1;
    TFW_ConfigSetItemInner(TFW_CONFIG_MAIN_VERSION, &item);

    item.key = TFW_CONFIG_MAIN_DEBUG;
    item.type = TFW_CONFIG_TYPE_BOOL;
    item.value.bool_value = false;
    TFW_ConfigSetItemInner(TFW_CONFIG_MAIN_DEBUG, &item);

    item.key = TFW_CONFIG_MAIN_LOG_LEVEL;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = 1;
    TFW_ConfigSetItemInner(TFW_CONFIG_MAIN_LOG_LEVEL, &item);

    // Logging configuration
    item.key = TFW_CONFIG_LOGGING_LEVEL;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = 1;
    TFW_ConfigSetItemInner(TFW_CONFIG_LOGGING_LEVEL, &item);

    item.key = TFW_CONFIG_LOGGING_OUTPUT;
    item.type = TFW_CONFIG_TYPE_STRING;
    item.value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_LOGGING_OUTPUT);
    TFW_ConfigSetItemInner(TFW_CONFIG_LOGGING_OUTPUT, &item);
    // 释放临时分配的字符串内存
    TFW_Free((void*)item.value.string_value);

    item.key = TFW_CONFIG_LOGGING_FILE_PATH;
    item.type = TFW_CONFIG_TYPE_STRING;
    item.value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_LOGGING_FILE_PATH);
    TFW_ConfigSetItemInner(TFW_CONFIG_LOGGING_FILE_PATH, &item);
    // 释放临时分配的字符串内存
    TFW_Free((void*)item.value.string_value);

    item.key = TFW_CONFIG_LOGGING_FILE_PREFIX;
    item.type = TFW_CONFIG_TYPE_STRING;
    item.value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_LOGGING_FILE_PREFIX);
    TFW_ConfigSetItemInner(TFW_CONFIG_LOGGING_FILE_PREFIX, &item);
    // 释放临时分配的字符串内存
    TFW_Free((void*)item.value.string_value);

    item.key = TFW_CONFIG_LOGGING_MAX_FILE_SIZE;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = atoi(TFW_CONFIG_DEFAULT_LOGGING_MAX_FILE_SIZE);
    TFW_ConfigSetItemInner(TFW_CONFIG_LOGGING_MAX_FILE_SIZE, &item);

    item.key = TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = atoi(TFW_CONFIG_DEFAULT_LOGGING_MAX_RETENTION_DAYS);
    TFW_ConfigSetItemInner(TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS, &item);

    // Resources configuration
    item.key = TFW_CONFIG_RESOURCES_AUTO_UPDATE;
    item.type = TFW_CONFIG_TYPE_BOOL;
    item.value.bool_value = (strcmp(TFW_CONFIG_DEFAULT_RESOURCES_AUTO_UPDATE, "true") == 0);
    TFW_ConfigSetItemInner(TFW_CONFIG_RESOURCES_AUTO_UPDATE, &item);

    item.key = TFW_CONFIG_RESOURCES_UPDATE_INTERVAL;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = atoi(TFW_CONFIG_DEFAULT_RESOURCES_UPDATE_INTERVAL);
    TFW_ConfigSetItemInner(TFW_CONFIG_RESOURCES_UPDATE_INTERVAL, &item);

    item.key = TFW_CONFIG_RESOURCES_CACHE_PATH;
    item.type = TFW_CONFIG_TYPE_STRING;
    item.value.string_value = TFW_Strdup(TFW_CONFIG_DEFAULT_RESOURCES_CACHE_PATH);
    TFW_ConfigSetItemInner(TFW_CONFIG_RESOURCES_CACHE_PATH, &item);
    // 释放临时分配的字符串内存
    TFW_Free((void*)item.value.string_value);

    // System configuration
    item.key = TFW_CONFIG_SYSTEM_MAX_THREADS;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = atoi(TFW_CONFIG_DEFAULT_SYSTEM_MAX_THREADS);
    TFW_ConfigSetItemInner(TFW_CONFIG_SYSTEM_MAX_THREADS, &item);

    item.key = TFW_CONFIG_SYSTEM_TIMEOUT;
    item.type = TFW_CONFIG_TYPE_INT;
    item.value.int_value = atoi(TFW_CONFIG_DEFAULT_SYSTEM_TIMEOUT);
    TFW_ConfigSetItemInner(TFW_CONFIG_SYSTEM_TIMEOUT, &item);

    TFW_LOGI_UTILS("Default config created successfully");
    return TFW_SUCCESS;
}

static void TFW_ConfigNotifyUpdate(TFW_ConfigKey key, const TFW_ConfigItem *item) {
    // 通知所有已注册的回调函数
    for (int i = 0; i <= TFW_CONFIG_MODULE_SYSTEM; i++) {
        if (g_callbacks[i] != NULL) {
            g_callbacks[i](key, item);
        }
    }
}

static const char *TFW_ConfigKeyToString(TFW_ConfigKey key) {
    switch (key) {
        case TFW_CONFIG_MAIN_VERSION:
            return TFW_CONFIG_NAME_MAIN_VERSION;
        case TFW_CONFIG_MAIN_DEBUG:
            return TFW_CONFIG_NAME_MAIN_DEBUG;
        case TFW_CONFIG_MAIN_LOG_LEVEL:
            return TFW_CONFIG_NAME_MAIN_LOG_LEVEL;
        case TFW_CONFIG_LOGGING_LEVEL:
            return TFW_CONFIG_NAME_LOGGING_LEVEL;
        case TFW_CONFIG_LOGGING_OUTPUT:
            return TFW_CONFIG_NAME_LOGGING_OUTPUT;
        case TFW_CONFIG_LOGGING_FILE_PATH:
            return TFW_CONFIG_NAME_LOGGING_FILE_PATH;
        case TFW_CONFIG_LOGGING_FILE_PREFIX:
            return TFW_CONFIG_NAME_LOGGING_FILE_PREFIX;
        case TFW_CONFIG_LOGGING_MAX_FILE_SIZE:
            return TFW_CONFIG_NAME_LOGGING_MAX_FILE_SIZE;
        case TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS:
            return TFW_CONFIG_NAME_LOGGING_MAX_RETENTION_DAYS;
        case TFW_CONFIG_RESOURCES_AUTO_UPDATE:
            return TFW_CONFIG_NAME_RESOURCES_AUTO_UPDATE;
        case TFW_CONFIG_RESOURCES_UPDATE_INTERVAL:
            return TFW_CONFIG_NAME_RESOURCES_UPDATE_INTERVAL;
        case TFW_CONFIG_RESOURCES_CACHE_PATH:
            return TFW_CONFIG_NAME_RESOURCES_CACHE_PATH;
        case TFW_CONFIG_SYSTEM_MAX_THREADS:
            return TFW_CONFIG_NAME_SYSTEM_MAX_THREADS;
        case TFW_CONFIG_SYSTEM_TIMEOUT:
            return TFW_CONFIG_NAME_SYSTEM_TIMEOUT;
        default:
            return "unknown";
    }
}

static TFW_ConfigKey TFW_ConfigStringToKey(const char *key_str) {
    if (key_str == NULL) return TFW_CONFIG_KEY_COUNT;

    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        if (strcmp(key_str, TFW_ConfigKeyToString((TFW_ConfigKey)i)) == 0) {
            return (TFW_ConfigKey)i;
        }
    }

    return TFW_CONFIG_KEY_COUNT;
}

// 将JSON对象转换为配置项数组
static int32_t JsonToConfigItems(cJSON* json_root) {
    if (json_root == NULL || !cJSON_IsObject(json_root)) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // 遍历JSON对象的所有键值对
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json_root) {
        TFW_ConfigKey key = TFW_ConfigStringToKey(item->string);

        if (key >= TFW_CONFIG_KEY_COUNT) {
            // 未知的配置键，跳过
            continue;
        }

        // 根据JSON值类型设置配置项
        TFW_ConfigItem config_item;
        config_item.key = key;

        if (cJSON_IsString(item)) {
            config_item.type = TFW_CONFIG_TYPE_STRING;
            config_item.value.string_value = TFW_Strdup(item->valuestring);
            if (config_item.value.string_value == NULL) {
                TFW_LOGE_UTILS("Failed to allocate memory for string value");
                return TFW_ERROR;
            }
        } else if (cJSON_IsNumber(item)) {
            config_item.type = TFW_CONFIG_TYPE_INT;
            config_item.value.int_value = item->valueint;
        } else if (cJSON_IsBool(item)) {
            config_item.type = TFW_CONFIG_TYPE_BOOL;
            config_item.value.bool_value = cJSON_IsTrue(item);
        } else if (cJSON_IsNumber(item)) {
            // cJSON中浮点数和整数都是Number类型，这里统一处理为整数
            config_item.type = TFW_CONFIG_TYPE_INT;
            config_item.value.int_value = item->valueint;
        } else {
            // 不支持的类型，设置为无效
            config_item.type = TFW_CONFIG_TYPE_INVALID;
        }

        // 更新配置项数组
        TFW_ConfigFreeItem(&g_config_items[key]);
        g_config_items[key] = config_item;
    }

    return TFW_SUCCESS;
}

// 将配置项数组转换为JSON对象
static int32_t ConfigItemsToJson(cJSON* json_root) {
    if (json_root == NULL || !cJSON_IsObject(json_root)) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // 遍历配置项数组
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        TFW_ConfigItem* item = &g_config_items[i];
        if (item->type == TFW_CONFIG_TYPE_INVALID) {
            // 跳过无效的配置项
            continue;
        }

        const char* key_str = TFW_ConfigKeyToString(item->key);
        cJSON* json_value = NULL;

        // 根据配置项类型创建JSON值
        switch (item->type) {
            case TFW_CONFIG_TYPE_STRING:
                json_value = cJSON_CreateString(item->value.string_value);
                break;

            case TFW_CONFIG_TYPE_INT:
                json_value = cJSON_CreateNumber(item->value.int_value);
                break;

            case TFW_CONFIG_TYPE_BOOL:
                json_value = cJSON_CreateBool(item->value.bool_value);
                break;

            case TFW_CONFIG_TYPE_FLOAT:
                json_value = cJSON_CreateNumber(item->value.float_value);
                break;

            default:
                // 不支持的类型，跳过
                continue;
        }

        if (json_value == NULL) {
            TFW_LOGE_UTILS("Failed to create JSON value for key: %s", key_str);
            continue;
        }

        // 添加到JSON对象
        cJSON_AddItemToObject(json_root, key_str, json_value);
    }

    return TFW_SUCCESS;
}

// 释放单项配置项内存
void TFW_ConfigFreeItem(TFW_ConfigItem* item) {
    if (item == NULL) return;

    if (item->type == TFW_CONFIG_TYPE_STRING && item->value.string_value != NULL) {
        TFW_Free((void*)item->value.string_value);
        item->value.string_value = NULL;
    }

    item->type = TFW_CONFIG_TYPE_INVALID;
}
