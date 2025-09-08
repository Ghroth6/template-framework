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
static cJSON *g_config_root = NULL;           // 配置根对象
static TFW_ConfigUpdateCallback g_callbacks[TFW_CONFIG_MODULE_SYSTEM + 1] = {NULL};  // 配置更新回调

// 内部辅助函数声明
static int32_t TFW_ConfigLoadFromFile(const char *file_path);
static int32_t TFW_ConfigCreateDefault(void);
static void TFW_ConfigNotifyUpdate(TFW_ConfigKey key, const TFW_ConfigItem *item);
static const char *TFW_ConfigKeyToString(TFW_ConfigKey key);
static TFW_ConfigKey TFW_ConfigStringToKey(const char *key_str);
static int32_t TFW_ConfigSetDefault(TFW_ConfigKey key, const char *default_value, TFW_ConfigValueType type);

// 配置模块初始化
int32_t TFW_ConfigInit(void) {
    // 释放已有资源
    if (g_config_root != NULL) {
        cJSON_Delete(g_config_root);
        g_config_root = NULL;
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

    TFW_LOGI_UTILS("Config module initialized successfully");
    return TFW_SUCCESS;
}

// 配置模块反初始化
int32_t TFW_ConfigDeinit(void) {
    // 保存配置到文件
    if (g_config_root != NULL) {
        TFW_ConfigSave();
    }

    // 释放资源
    if (g_config_root != NULL) {
        cJSON_Delete(g_config_root);
        g_config_root = NULL;
    }

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
int32_t TFW_ConfigUnregisterUpdateCallback(TFW_ConfigModule module, TFW_ConfigUpdateCallback callback) {
    if (module > TFW_CONFIG_MODULE_SYSTEM) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigUnregisterUpdateCallback");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_callbacks[module] == callback) {
        g_callbacks[module] = NULL;
        TFW_LOGI_UTILS("Config update callback unregistered successfully for module %d", module);
        return TFW_SUCCESS;
    }

    TFW_LOGW_UTILS("Callback not found or mismatch");
    return TFW_ERROR;
}

// 读取全部配置
int32_t TFW_ConfigGetAll(TFW_ConfigItem **config_array, uint32_t *count) {
    if (config_array == NULL || count == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetAll");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    *count = TFW_CONFIG_KEY_COUNT;
    *config_array = (TFW_ConfigItem *)TFW_Calloc(*count * sizeof(TFW_ConfigItem));
    if (*config_array == NULL) {
        TFW_LOGE_UTILS("Failed to allocate memory for config array");
        return TFW_ERROR;
    }

    // 遍历所有可能的配置键
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        TFW_ConfigItem item;
        item.key = (TFW_ConfigKey)i;

        cJSON *json_item = cJSON_GetObjectItemCaseSensitive(g_config_root, TFW_ConfigKeyToString(item.key));
        if (json_item == NULL) {
            // 配置项不存在，设置为无效类型
            item.type = TFW_CONFIG_TYPE_INVALID;
        } else if (cJSON_IsString(json_item)) {
            item.type = TFW_CONFIG_TYPE_STRING;
            item.value.string_value = json_item->valuestring;
        } else if (cJSON_IsNumber(json_item)) {
            item.type = TFW_CONFIG_TYPE_INT;
            item.value.int_value = json_item->valueint;
        } else if (cJSON_IsBool(json_item)) {
            item.type = TFW_CONFIG_TYPE_BOOL;
            item.value.bool_value = cJSON_IsTrue(json_item) ? true : false;
        } else {
            item.type = TFW_CONFIG_TYPE_INVALID;
        }

        (*config_array)[i] = item;
    }

    return TFW_SUCCESS;
}

// 读取单项配置
int32_t TFW_ConfigGetItem(TFW_ConfigKey key, TFW_ConfigItem *item) {
    if (item == NULL || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetItem");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    item->key = key;

    cJSON *json_item = cJSON_GetObjectItemCaseSensitive(g_config_root, TFW_ConfigKeyToString(key));
    if (json_item == NULL) {
        // 配置项不存在，设置为无效类型
        item->type = TFW_CONFIG_TYPE_INVALID;
        return TFW_ERROR;
    }

    if (cJSON_IsString(json_item)) {
        item->type = TFW_CONFIG_TYPE_STRING;
        item->value.string_value = json_item->valuestring;
    } else if (cJSON_IsNumber(json_item)) {
        item->type = TFW_CONFIG_TYPE_INT;
        item->value.int_value = json_item->valueint;
    } else if (cJSON_IsBool(json_item)) {
        item->type = TFW_CONFIG_TYPE_BOOL;
        item->value.bool_value = cJSON_IsTrue(json_item) ? true : false;
    } else {
        item->type = TFW_CONFIG_TYPE_INVALID;
    }

    return TFW_SUCCESS;
}

// 设置单项配置
int32_t TFW_ConfigSetItem(TFW_ConfigKey key, const TFW_ConfigItem *item) {
    if (item == NULL || key >= TFW_CONFIG_KEY_COUNT || key != item->key) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetItem");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON *json_item = NULL;
    switch (item->type) {
        case TFW_CONFIG_TYPE_STRING:
            json_item = cJSON_CreateString(item->value.string_value);
            break;
        case TFW_CONFIG_TYPE_INT:
            json_item = cJSON_CreateNumber(item->value.int_value);
            break;
        case TFW_CONFIG_TYPE_BOOL:
            json_item = cJSON_CreateBool(item->value.bool_value);
            break;
        default:
            TFW_LOGE_UTILS("Unsupported config type: %d", item->type);
            return TFW_ERROR;
    }

    if (json_item == NULL) {
        TFW_LOGE_UTILS("Failed to create cJSON item");
        return TFW_ERROR;
    }

    cJSON_ReplaceItemInObject(g_config_root, TFW_ConfigKeyToString(key), json_item);
    TFW_ConfigNotifyUpdate(key, item);
    return TFW_SUCCESS;
}

// 读取单项配置（整数）
int32_t TFW_ConfigGetInt(const char *key, int32_t *value) {
    if (key == NULL || value == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetInt");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON *item = cJSON_GetObjectItemCaseSensitive(g_config_root, key);
    if (item == NULL || !cJSON_IsNumber(item)) {
        TFW_LOGW_UTILS("Config key '%s' not found or not a number", key);
        return TFW_ERROR;
    }

    *value = item->valueint;
    return TFW_SUCCESS;
}

// 设置单项配置（整数）
int32_t TFW_ConfigSetInt(TFW_ConfigKey key, int32_t value) {
    if (key < TFW_CONFIG_MAIN_VERSION || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetInt");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON *item = cJSON_CreateNumber(value);
    if (item == NULL) {
        TFW_LOGE_UTILS("Failed to create cJSON number item");
        return TFW_ERROR;
    }

    cJSON_ReplaceItemInObject(g_config_root, TFW_ConfigKeyToString(key), item);

    // 创建临时配置项用于通知
    TFW_ConfigItem config_item;
    config_item.key = key;
    config_item.type = TFW_CONFIG_TYPE_INT;
    config_item.value.int_value = value;
    TFW_ConfigNotifyUpdate(key, &config_item);
    return TFW_SUCCESS;
}

// 读取单项配置（浮点数）
int32_t TFW_ConfigGetDouble(const char *key, double *value) {
    if (key == NULL || value == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetDouble");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON *item = cJSON_GetObjectItemCaseSensitive(g_config_root, key);
    if (item == NULL || !cJSON_IsNumber(item)) {
        TFW_LOGW_UTILS("Config key '%s' not found or not a number", key);
        return TFW_ERROR;
    }

    *value = item->valuedouble;
    return TFW_SUCCESS;
}

// 设置单项配置（浮点数）
int32_t TFW_ConfigSetDouble(TFW_ConfigKey key, double value) {
    if (key < TFW_CONFIG_MAIN_VERSION || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetDouble");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON *item = cJSON_CreateNumber(value);
    if (item == NULL) {
        TFW_LOGE_UTILS("Failed to create cJSON number item");
        return TFW_ERROR;
    }

    cJSON_ReplaceItemInObject(g_config_root, TFW_ConfigKeyToString(key), item);

    // 创建临时配置项用于通知
    TFW_ConfigItem config_item;
    config_item.key = key;
    config_item.type = TFW_CONFIG_TYPE_FLOAT;
    config_item.value.float_value = value;
    TFW_ConfigNotifyUpdate(key, &config_item);
    return TFW_SUCCESS;
}

// 读取单项配置（布尔值）
int32_t TFW_ConfigGetBool(const char *key, bool *value) {
    if (key == NULL || value == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigGetBool");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON *item = cJSON_GetObjectItemCaseSensitive(g_config_root, key);
    if (item == NULL || !cJSON_IsBool(item)) {
        TFW_LOGW_UTILS("Config key '%s' not found or not a boolean", key);
        return TFW_ERROR;
    }

    *value = cJSON_IsTrue(item) ? true : false;
    return TFW_SUCCESS;
}

// 设置单项配置（布尔值）
int32_t TFW_ConfigSetBool(TFW_ConfigKey key, bool value) {
    if (key < TFW_CONFIG_MAIN_VERSION || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetBool");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON *item = cJSON_CreateBool(value);
    if (item == NULL) {
        TFW_LOGE_UTILS("Failed to create cJSON boolean item");
        return TFW_ERROR;
    }

    cJSON_ReplaceItemInObject(g_config_root, TFW_ConfigKeyToString(key), item);

    // 创建临时配置项用于通知
    TFW_ConfigItem config_item;
    config_item.key = key;
    config_item.type = TFW_CONFIG_TYPE_BOOL;
    config_item.value.bool_value = value;
    TFW_ConfigNotifyUpdate(key, &config_item);
    return TFW_SUCCESS;
}


// 删除配置项
int32_t TFW_ConfigRemoveKey(const char *key) {
    if (key == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigRemoveKey");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    cJSON_DeleteItemFromObject(g_config_root, key);
    return TFW_SUCCESS;
}

// 保存配置到文件
int32_t TFW_ConfigSave(void) {
    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not properly initialized");
        return TFW_ERROR;
    }

    char *json_string = cJSON_Print(g_config_root);
    if (json_string == NULL) {
        TFW_LOGE_UTILS("Failed to print config to JSON string");
        return TFW_ERROR;
    }

    char config_file_path[TFW_PATH_LEN_MAX];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", TFW_CONFIG_FILE_PATH, TFW_CONFIG_FILE_NAME);

    int32_t ret = TFW_WriteFile(config_file_path, json_string, strlen(json_string));
    TFW_Free(json_string);

    if (ret < 0) {
        TFW_LOGE_UTILS("Failed to write config to file");
        return TFW_ERROR;
    }

    TFW_LOGI_UTILS("Config saved to file successfully");
    return TFW_SUCCESS;
}

// 从文件重新加载配置
int32_t TFW_ConfigReload(void) {
    char config_file_path[TFW_PATH_LEN_MAX];
    snprintf(config_file_path, sizeof(config_file_path), "%s/%s", TFW_CONFIG_FILE_PATH, TFW_CONFIG_FILE_NAME);
    return TFW_ConfigLoadFromFile(config_file_path);
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
    cJSON *config = cJSON_Parse(file_content);
    TFW_Free(file_content);

    if (config == NULL) {
        TFW_LOGE_UTILS("Failed to parse config file as JSON");
        return TFW_ERROR;
    }

    // 更新全局配置
    if (g_config_root != NULL) {
        cJSON_Delete(g_config_root);
    }
    g_config_root = config;

    TFW_LOGI_UTILS("Config loaded from file successfully");
    return TFW_SUCCESS;
}

static int32_t TFW_ConfigCreateDefault(void) {
    if (g_config_root != NULL) {
        cJSON_Delete(g_config_root);
    }

    g_config_root = cJSON_CreateObject();
    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Failed to create default config object");
        return TFW_ERROR;
    }

    // 设置默认配置项
    TFW_ConfigSetDefault(TFW_CONFIG_MAIN_VERSION, TFW_CONFIG_DEFAULT_MAIN_VERSION, TFW_CONFIG_TYPE_INT);
    TFW_ConfigSetDefault(TFW_CONFIG_MAIN_DEBUG, TFW_CONFIG_DEFAULT_MAIN_DEBUG, TFW_CONFIG_TYPE_BOOL);
    TFW_ConfigSetDefault(TFW_CONFIG_MAIN_LOG_LEVEL, TFW_CONFIG_DEFAULT_MAIN_LOG_LEVEL, TFW_CONFIG_TYPE_INT);
    TFW_ConfigSetDefault(TFW_CONFIG_LOGGING_LEVEL, TFW_CONFIG_DEFAULT_LOGGING_LEVEL, TFW_CONFIG_TYPE_INT);
    TFW_ConfigSetDefault(TFW_CONFIG_LOGGING_OUTPUT, TFW_CONFIG_DEFAULT_LOGGING_OUTPUT, TFW_CONFIG_TYPE_STRING);
    TFW_ConfigSetDefault(TFW_CONFIG_LOGGING_FILE_PATH, TFW_CONFIG_DEFAULT_LOGGING_FILE_PATH, TFW_CONFIG_TYPE_STRING);
    TFW_ConfigSetDefault(TFW_CONFIG_LOGGING_FILE_PREFIX, TFW_CONFIG_DEFAULT_LOGGING_FILE_PREFIX, TFW_CONFIG_TYPE_STRING);
    TFW_ConfigSetDefault(TFW_CONFIG_LOGGING_MAX_FILE_SIZE, TFW_CONFIG_DEFAULT_LOGGING_MAX_FILE_SIZE, TFW_CONFIG_TYPE_INT);
    TFW_ConfigSetDefault(TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS, TFW_CONFIG_DEFAULT_LOGGING_MAX_RETENTION_DAYS, TFW_CONFIG_TYPE_INT);
    TFW_ConfigSetDefault(TFW_CONFIG_RESOURCES_AUTO_UPDATE, TFW_CONFIG_DEFAULT_RESOURCES_AUTO_UPDATE, TFW_CONFIG_TYPE_BOOL);
    TFW_ConfigSetDefault(TFW_CONFIG_RESOURCES_UPDATE_INTERVAL, TFW_CONFIG_DEFAULT_RESOURCES_UPDATE_INTERVAL, TFW_CONFIG_TYPE_INT);
    TFW_ConfigSetDefault(TFW_CONFIG_RESOURCES_CACHE_PATH, TFW_CONFIG_DEFAULT_RESOURCES_CACHE_PATH, TFW_CONFIG_TYPE_STRING);
    TFW_ConfigSetDefault(TFW_CONFIG_SYSTEM_MAX_THREADS, TFW_CONFIG_DEFAULT_SYSTEM_MAX_THREADS, TFW_CONFIG_TYPE_INT);
    TFW_ConfigSetDefault(TFW_CONFIG_SYSTEM_TIMEOUT, TFW_CONFIG_DEFAULT_SYSTEM_TIMEOUT, TFW_CONFIG_TYPE_INT);

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

static int32_t TFW_ConfigSetDefault(TFW_ConfigKey key, const char *default_value, TFW_ConfigValueType type) {
    if (default_value == NULL || key >= TFW_CONFIG_KEY_COUNT) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ConfigSetDefault");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (g_config_root == NULL) {
        TFW_LOGE_UTILS("Config module not initialized");
        return TFW_ERROR;
    }

    // 根据类型创建配置项
    cJSON *item = NULL;
    switch (type) {
        case TFW_CONFIG_TYPE_STRING:
            item = cJSON_CreateString(default_value);
            break;
        case TFW_CONFIG_TYPE_INT: {
            int32_t val = atoi(default_value);
            item = cJSON_CreateNumber(val);
            break;
        }
        case TFW_CONFIG_TYPE_BOOL: {
            bool val = (strcmp(default_value, "true") == 0) ? true : false;
            item = cJSON_CreateBool(val);
            break;
        }
        default:
            TFW_LOGE_UTILS("Unsupported config type: %d", type);
            return TFW_ERROR;
    }

    if (item == NULL) {
        TFW_LOGE_UTILS("Failed to create cJSON item");
        return TFW_ERROR;
    }

    cJSON_AddItemToObject(g_config_root, TFW_ConfigKeyToString(key), item);
    return TFW_SUCCESS;
}
