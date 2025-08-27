#ifndef TFW_CONFIG_H
#define TFW_CONFIG_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 配置键枚举定义
typedef enum {
    // 主配置
    TFW_CONFIG_MAIN_VERSION = 0,
    TFW_CONFIG_MAIN_DEBUG,
    TFW_CONFIG_MAIN_LOG_LEVEL,
    
    // 日志配置
    TFW_CONFIG_LOGGING_LEVEL,
    TFW_CONFIG_LOGGING_OUTPUT,
    TFW_CONFIG_LOGGING_FILE_PATH,
    TFW_CONFIG_LOGGING_FILE_PREFIX,
    TFW_CONFIG_LOGGING_MAX_FILE_SIZE,
    TFW_CONFIG_LOGGING_MAX_RETENTION_DAYS,
    
    // 资源配置
    TFW_CONFIG_RESOURCES_AUTO_UPDATE,
    TFW_CONFIG_RESOURCES_UPDATE_INTERVAL,
    TFW_CONFIG_RESOURCES_CACHE_PATH,
    
    // 系统配置
    TFW_CONFIG_SYSTEM_MAX_THREADS,
    TFW_CONFIG_SYSTEM_TIMEOUT,
    
    // 配置键总数
    TFW_CONFIG_KEY_COUNT
} TFW_ConfigKey;

// 配置值类型
typedef enum {
    TFW_CONFIG_TYPE_STRING = 0,
    TFW_CONFIG_TYPE_INT,
    TFW_CONFIG_TYPE_BOOL,
    TFW_CONFIG_TYPE_FLOAT
} TFW_ConfigValueType;

// 配置值联合体
typedef union {
    const char* string_value;
    int32_t int_value;
    bool bool_value;
    float float_value;
} TFW_ConfigValue;

// 配置项结构
typedef struct {
    TFW_ConfigKey key;
    TFW_ConfigValueType type;
    TFW_ConfigValue value;
} TFW_ConfigItem;

// 配置状态结构
typedef struct {
    bool loaded;                    // 配置是否已加载
    uint64_t lastLoadTime;         // 最后加载时间（Unix时间戳）
    uint64_t lastSaveTime;         // 最后保存时间（Unix时间戳）
    size_t totalKeys;              // 总配置项数量
    char configFilePath[512];      // 配置文件路径
} TFW_ConfigStatus;

// 配置管理接口
int32_t TFW_Config_Initialize(const char* configPath);
int32_t TFW_Config_Exit(void);

// 配置读写接口（类型安全）
int32_t TFW_Config_GetString(TFW_ConfigKey key, char* value, size_t valueSize, const char* defaultValue);
int32_t TFW_Config_GetInt(TFW_ConfigKey key, int32_t* value, int32_t defaultValue);
int32_t TFW_Config_GetBool(TFW_ConfigKey key, bool* value, bool defaultValue);
int32_t TFW_Config_GetFloat(TFW_ConfigKey key, float* value, float defaultValue);

int32_t TFW_Config_SetString(TFW_ConfigKey key, const char* value);
int32_t TFW_Config_SetInt(TFW_ConfigKey key, int32_t value);
int32_t TFW_Config_SetBool(TFW_ConfigKey key, bool value);
int32_t TFW_Config_SetFloat(TFW_ConfigKey key, float value);

// 配置状态查询
int32_t TFW_Config_GetStatus(TFW_ConfigStatus* status);

// 配置文件管理
int32_t TFW_Config_LoadConfig(void);
int32_t TFW_Config_SaveConfig(void);
int32_t TFW_Config_CreateDefaultConfig(void);

// 配置键名称转换（调试用）
const char* TFW_Config_GetKeyName(TFW_ConfigKey key);
TFW_ConfigKey TFW_Config_GetKeyFromName(const char* name);

#ifdef __cplusplus
}
#endif

#endif // TFW_CONFIG_H
