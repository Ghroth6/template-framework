#include "TFW_config.h"
#include "TFW_common_defines.h"
#include "TFW_file.h"
#include "TFW_timer.h"
#include "TFW_thread.h"
#include "TFW_log.h"
#include "TFW_errorno.h"
#include "../../core/include/TFW_core_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 配置管理器结构
typedef struct {
    char configPath[512];
    TFW_ConfigItem* configs;
    size_t configCount;
    TFW_ConfigStatus status;
    TFW_Mutex_t configMutex;
    bool initialized;
} ConfigManager;

// 全局配置管理器实例
static ConfigManager g_configManager = {0};

// 内部函数声明
static int32_t ParseJsonConfig(const char* filePath);
static int32_t WriteJsonConfig(const char* filePath);
static void FreeConfigManager(void);
static int32_t CreateDirectories(const char* path);
static const char* GetDefaultValue(TFW_ConfigKey key);
static TFW_ConfigValueType GetKeyType(TFW_ConfigKey key);

// 配置键名称映射表
static const char* g_configKeyNames[] = {
    "main.version", "main.debug", "main.log_level",
    "logging.level", "logging.output", "logging.file_path", "logging.file_prefix", "logging.max_file_size", "logging.max_retention_days",
    "resources.auto_update", "resources.update_interval", "resources.cache_path",
    "system.max_threads", "system.timeout"
};

// 配置管理接口实现
int32_t TFW_Config_Initialize(const char* configPath) {
    if (g_configManager.initialized) {
        return TFW_ERROR_ALREADY_INIT;
    }
    
    if (!configPath || strlen(configPath) == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    // 初始化互斥锁
    TFW_MutexAttr_t mutexAttr;
    if (TFW_MutexAttr_Init(&mutexAttr) != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to initialize mutex attributes");
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    if (TFW_Mutex_Init(&g_configManager.configMutex, &mutexAttr) != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to initialize config mutex");
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    // 设置配置路径
    strncpy(g_configManager.configPath, configPath, sizeof(g_configManager.configPath) - 1);
    g_configManager.configPath[sizeof(g_configManager.configPath) - 1] = '\0';
    
    // 创建配置目录
    if (CreateDirectories(configPath) != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to create config directory: %s", configPath);
        // 继续运行，使用当前目录
    }
    
    // 初始化配置数组
    g_configManager.configs = malloc(TFW_CONFIG_KEY_COUNT * sizeof(TFW_ConfigItem));
    if (!g_configManager.configs) {
        TFW_LOGE_CORE("Failed to allocate config array");
        TFW_Mutex_Destroy(&g_configManager.configMutex);
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    // 初始化所有配置项
    memset(g_configManager.configs, 0, TFW_CONFIG_KEY_COUNT * sizeof(TFW_ConfigItem));
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        g_configManager.configs[i].key = (TFW_ConfigKey)i;
        g_configManager.configs[i].type = GetKeyType((TFW_ConfigKey)i);
        
        // 设置默认值
        const char* defaultValue = GetDefaultValue((TFW_ConfigKey)i);
        if (defaultValue) {
            g_configManager.configs[i].value.string_value = strdup(defaultValue);
        }
    }
    
    g_configManager.configCount = TFW_CONFIG_KEY_COUNT;
    
    // 初始化状态
    memset(&g_configManager.status, 0, sizeof(g_configManager.status));
    g_configManager.status.loaded = false;
    strncpy(g_configManager.status.configFilePath, configPath, sizeof(g_configManager.status.configFilePath) - 1);
    
    // 先标记为已初始化，这样后续操作可以正常进行
    g_configManager.initialized = true;
    
    // 尝试加载配置文件
    int32_t result = TFW_Config_LoadConfig();
    if (result != TFW_SUCCESS) {
        TFW_LOGI_CORE("Failed to load config, creating default");
        // 创建默认配置
        TFW_Config_CreateDefaultConfig();
        // 标记为已加载
        g_configManager.status.loaded = true;
        g_configManager.status.lastLoadTime = (uint64_t)TFW_GetTimestamp();
    } else {
        // 配置加载成功
        g_configManager.status.loaded = true;
        g_configManager.status.lastLoadTime = (uint64_t)TFW_GetTimestamp();
    }
    
    TFW_LOGI_CORE("Config manager initialized successfully");
    return TFW_SUCCESS;
}

// 简化的配置接口实现
int32_t TFW_Config_Exit(void) {
    if (!g_configManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    // 保存配置
    TFW_Config_SaveConfig();
    
    // 释放资源
    FreeConfigManager();
    
    // 销毁互斥锁
    TFW_Mutex_Destroy(&g_configManager.configMutex);
    
    g_configManager.initialized = false;
    TFW_LOGI_CORE("Config manager exited successfully");
    return TFW_SUCCESS;
}

// 简化的字符串配置接口
int32_t TFW_Config_GetString(TFW_ConfigKey key, char* value, size_t valueSize, const char* defaultValue) {
    if (!g_configManager.initialized || !value || valueSize == 0 || key >= TFW_CONFIG_KEY_COUNT) {
        if (defaultValue && value) {
            strncpy(value, defaultValue, valueSize - 1);
            value[valueSize - 1] = '\0';
        }
        return TFW_ERROR_INVALID_PARAM;
    }
    
    TFW_Mutex_Lock(&g_configManager.configMutex);
    
    TFW_ConfigItem* config = &g_configManager.configs[key];
    if (config->value.string_value && strlen(config->value.string_value) > 0) {
        strncpy(value, config->value.string_value, valueSize - 1);
        value[valueSize - 1] = '\0';
        TFW_Mutex_Unlock(&g_configManager.configMutex);
        return TFW_SUCCESS;
    }
    
    // 使用默认值
    if (defaultValue) {
        strncpy(value, defaultValue, valueSize - 1);
        value[valueSize - 1] = '\0';
    } else {
        value[0] = '\0';
    }
    
    TFW_Mutex_Unlock(&g_configManager.configMutex);
    return TFW_SUCCESS;
}

int32_t TFW_Config_SetString(TFW_ConfigKey key, const char* value) {
    if (!g_configManager.initialized || !value || key >= TFW_CONFIG_KEY_COUNT) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    TFW_Mutex_Lock(&g_configManager.configMutex);
    
    TFW_ConfigItem* config = &g_configManager.configs[key];
    if (config->type == TFW_CONFIG_TYPE_STRING) {
        if (config->value.string_value) {
            free((void*)config->value.string_value);
        }
        config->value.string_value = strdup(value);
        
        g_configManager.status.lastSaveTime = (uint64_t)TFW_GetTimestamp();
        TFW_LOGI_CORE("Config updated: %s = %s", g_configKeyNames[key], value);
        
        TFW_Mutex_Unlock(&g_configManager.configMutex);
        return TFW_SUCCESS;
    }
    
    TFW_Mutex_Unlock(&g_configManager.configMutex);
    return TFW_ERROR_INVALID_PARAM;
}

// 其他类型接口（简化实现）
int32_t TFW_Config_GetInt(TFW_ConfigKey key, int32_t* value, int32_t defaultValue) {
    if (!value || key >= TFW_CONFIG_KEY_COUNT) {
        *value = defaultValue;
        return TFW_ERROR_INVALID_PARAM;
    }
    *value = defaultValue;
    return TFW_SUCCESS;
}

int32_t TFW_Config_GetBool(TFW_ConfigKey key, bool* value, bool defaultValue) {
    if (!value || key >= TFW_CONFIG_KEY_COUNT) {
        *value = defaultValue;
        return TFW_ERROR_INVALID_PARAM;
    }
    *value = defaultValue;
    return TFW_SUCCESS;
}

int32_t TFW_Config_GetFloat(TFW_ConfigKey key, float* value, float defaultValue) {
    if (!value || key >= TFW_CONFIG_KEY_COUNT) {
        *value = defaultValue;
        return TFW_ERROR_INVALID_PARAM;
    }
    *value = defaultValue;
    return TFW_SUCCESS;
}

int32_t TFW_Config_SetInt(TFW_ConfigKey key, int32_t value) {
    if (key >= TFW_CONFIG_KEY_COUNT) return TFW_ERROR_INVALID_PARAM;
    return TFW_SUCCESS;
}

int32_t TFW_Config_SetBool(TFW_ConfigKey key, bool value) {
    if (key >= TFW_CONFIG_KEY_COUNT) return TFW_ERROR_INVALID_PARAM;
    return TFW_SUCCESS;
}

int32_t TFW_Config_SetFloat(TFW_ConfigKey key, float value) {
    if (key >= TFW_CONFIG_KEY_COUNT) return TFW_ERROR_INVALID_PARAM;
    return TFW_SUCCESS;
}

int32_t TFW_Config_GetStatus(TFW_ConfigStatus* status) {
    if (!g_configManager.initialized || !status) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    TFW_Mutex_Lock(&g_configManager.configMutex);
    memcpy(status, &g_configManager.status, sizeof(TFW_ConfigStatus));
    TFW_Mutex_Unlock(&g_configManager.configMutex);
    
    return TFW_SUCCESS;
}

int32_t TFW_Config_LoadConfig(void) {
    if (!g_configManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    char filePath[512];
    snprintf(filePath, sizeof(filePath), "%s/TFW_config.json", g_configManager.configPath);
    
    if (!TFW_FileExists(filePath)) {
        TFW_LOGI_CORE("Config file not found: %s", filePath);
        return TFW_ERROR_NOT_FOUND;
    }
    
    int32_t result = ParseJsonConfig(filePath);
    if (result == TFW_SUCCESS) {
        g_configManager.status.lastLoadTime = (uint64_t)TFW_GetTimestamp();
        TFW_LOGI_CORE("Config loaded successfully");
    }
    
    return result;
}

int32_t TFW_Config_SaveConfig(void) {
    if (!g_configManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    char filePath[512];
    snprintf(filePath, sizeof(filePath), "%s/TFW_config.json", g_configManager.configPath);
    
    int32_t result = WriteJsonConfig(filePath);
    if (result == TFW_SUCCESS) {
        g_configManager.status.lastSaveTime = (uint64_t)TFW_GetTimestamp();
        TFW_LOGI_CORE("Config saved successfully");
    }
    
    return result;
}

int32_t TFW_Config_CreateDefaultConfig(void) {
    if (!g_configManager.initialized) {
        return TFW_ERROR_NOT_INIT;
    }
    
    TFW_Mutex_Lock(&g_configManager.configMutex);
    
    // 设置默认值
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        const char* defaultValue = GetDefaultValue((TFW_ConfigKey)i);
        if (defaultValue) {
            g_configManager.configs[i].value.string_value = strdup(defaultValue);
        }
    }
    
    g_configManager.status.totalKeys = TFW_CONFIG_KEY_COUNT;
    
    TFW_Mutex_Unlock(&g_configManager.configMutex);
    
    // 不在这里保存，由调用者决定是否保存
    return TFW_SUCCESS;
}

// 配置键名称转换
const char* TFW_Config_GetKeyName(TFW_ConfigKey key) {
    if (key >= TFW_CONFIG_KEY_COUNT) {
        return "unknown";
    }
    return g_configKeyNames[key];
}

TFW_ConfigKey TFW_Config_GetKeyFromName(const char* name) {
    if (!name) return TFW_CONFIG_KEY_COUNT;
    
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        if (strcmp(g_configKeyNames[i], name) == 0) {
            return (TFW_ConfigKey)i;
        }
    }
    return TFW_CONFIG_KEY_COUNT;
}

// 内部函数实现
static int32_t ParseJsonConfig(const char* filePath) {
    // 简化的JSON解析，只支持基本的键值对
    FILE* file = fopen(filePath, "r");
    if (!file) {
        return TFW_ERROR_NOT_FOUND;
    }
    
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        // 去除行尾换行符
        size_t len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
            len--;
        }
        
        // 跳过空行和注释
        if (len == 0 || line[0] == '{' || line[0] == '}' || line[0] == '[' || line[0] == ']') {
            continue;
        }
        
        // 解析键值对 "key": "value" 或 "key": value
        char* colonPos = strchr(line, ':');
        if (colonPos) {
            *colonPos = '\0';
            char* key = line;
            char* value = colonPos + 1;
            
            // 去除引号和空格
            while (*key && (*key == '"' || *key == ' ' || *key == '\t')) key++;
            char* keyEnd = key + strlen(key) - 1;
            while (keyEnd > key && (*keyEnd == '"' || *keyEnd == ' ' || *keyEnd == '\t')) *keyEnd-- = '\0';
            
            while (*value && (*value == ' ' || *value == '\t')) value++;
            char* valueEnd = value + strlen(value) - 1;
            while (valueEnd > value && (*valueEnd == ' ' || *valueEnd == '\t' || *valueEnd == ',')) *valueEnd-- = '\0';
            if (*valueEnd == '"') *valueEnd = '\0';
            if (*value == '"') value++;
            
            if (strlen(key) > 0) {
                TFW_ConfigKey configKey = TFW_Config_GetKeyFromName(key);
                if (configKey < TFW_CONFIG_KEY_COUNT) {
                    TFW_Config_SetString(configKey, value);
                }
            }
        }
    }
    
    fclose(file);
    return TFW_SUCCESS;
}

static int32_t WriteJsonConfig(const char* filePath) {
    FILE* file = fopen(filePath, "w");
    if (!file) {
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    fprintf(file, "{\n");
    
    for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
        TFW_ConfigItem* config = &g_configManager.configs[i];
        
        fprintf(file, "  \"%s\": ", g_configKeyNames[i]);
        
        // 目前所有配置都作为字符串处理，简化实现
        if (config->value.string_value && strlen(config->value.string_value) > 0) {
            fprintf(file, "\"%s\"", config->value.string_value);
        } else {
            fprintf(file, "\"\"");
        }
        
        if (i < TFW_CONFIG_KEY_COUNT - 1) {
            fprintf(file, ",");
        }
        fprintf(file, "\n");
    }
    
    fprintf(file, "}\n");
    fclose(file);
    return TFW_SUCCESS;
}

static void FreeConfigManager(void) {
    if (g_configManager.configs) {
        for (int i = 0; i < TFW_CONFIG_KEY_COUNT; i++) {
            if (g_configManager.configs[i].type == TFW_CONFIG_TYPE_STRING && 
                g_configManager.configs[i].value.string_value) {
                free((void*)g_configManager.configs[i].value.string_value);
            }
        }
        free(g_configManager.configs);
        g_configManager.configs = NULL;
    }
    
    g_configManager.configCount = 0;
}

static int32_t CreateDirectories(const char* path) {
    if (TFW_FileExists(path)) {
        return TFW_SUCCESS;
    }
    
    // 使用统一的目录创建接口
    if (TFW_CreateDirectory(path)) {
        return TFW_SUCCESS;
    }
    
    return TFW_ERROR_OPERATION_FAIL;
}

static const char* GetDefaultValue(TFW_ConfigKey key) {
    switch (key) {
        case TFW_CONFIG_MAIN_VERSION: return "1.0.0";
        case TFW_CONFIG_MAIN_DEBUG: return "false";
        case TFW_CONFIG_MAIN_LOG_LEVEL: return "INFO";
        case TFW_CONFIG_LOGGING_LEVEL: return "INFO";
        case TFW_CONFIG_LOGGING_OUTPUT: return "console";
        case TFW_CONFIG_LOGGING_FILE_PATH: return "logs/";
        case TFW_CONFIG_LOGGING_FILE_PREFIX: return "TFW_log";
        case TFW_CONFIG_LOGGING_MAX_FILE_SIZE: return "1024000"; // 1MB
        case TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS: return "7";
        case TFW_CONFIG_RESOURCES_AUTO_UPDATE: return "true";
        case TFW_CONFIG_RESOURCES_UPDATE_INTERVAL: return "3600";
        case TFW_CONFIG_RESOURCES_CACHE_PATH: return "cache/";
        case TFW_CONFIG_SYSTEM_MAX_THREADS: return "4";
        case TFW_CONFIG_SYSTEM_TIMEOUT: return "30";
        default: return "";
    }
}

static TFW_ConfigValueType GetKeyType(TFW_ConfigKey key) {
    switch (key) {
        case TFW_CONFIG_MAIN_VERSION:
        case TFW_CONFIG_MAIN_LOG_LEVEL:
        case TFW_CONFIG_LOGGING_LEVEL:
        case TFW_CONFIG_LOGGING_OUTPUT:
        case TFW_CONFIG_LOGGING_FILE_PATH:
        case TFW_CONFIG_LOGGING_FILE_PREFIX:
        case TFW_CONFIG_LOGGING_MAX_FILE_SIZE:
        case TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS:
        case TFW_CONFIG_RESOURCES_CACHE_PATH:
            return TFW_CONFIG_TYPE_STRING;
            
        case TFW_CONFIG_MAIN_DEBUG:
        case TFW_CONFIG_RESOURCES_AUTO_UPDATE:
            return TFW_CONFIG_TYPE_BOOL;
            
        case TFW_CONFIG_RESOURCES_UPDATE_INTERVAL:
        case TFW_CONFIG_SYSTEM_MAX_THREADS:
        case TFW_CONFIG_SYSTEM_TIMEOUT:
            return TFW_CONFIG_TYPE_INT;
            
        default:
            return TFW_CONFIG_TYPE_STRING;
    }
}
