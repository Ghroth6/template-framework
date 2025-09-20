#ifndef TFW_COMMON_DEFINES_H
#define TFW_COMMON_DEFINES_H

// 跨平台的未使用变量/函数标记宏
// Cross-platform unused variable/function marker macro

#ifdef _WIN32
#define TFW_UNUSED __pragma(warning(disable : 4505))
#else
#define TFW_UNUSED __attribute__((unused))
#endif

// TODO:编译生成文件独立存放
// Version information / 版本信息
// Note: Update these values manually when releasing new versions
// 包含版本信息（如果构建系统生成了版本文件，则使用生成的版本）
#if __has_include("TFW_build_info.h")
#include "TFW_build_info.h"
#else
// 否则使用默认版本（用于 IDE 代码提示等场景）
// 注意：发布新版本时更新这些值
#define TFW_VERSION_MAJOR 0
#define TFW_VERSION_MINOR 0
#define TFW_VERSION_PATCH 0
#define TFW_VERSION_STRING "0.0.0"
#endif

// Configuration file settings / 配置文件设置
#define TFW_CONFIG_FILE_PATH "runtime"
#define TFW_CONFIG_FILE_NAME "TFW_config.json"

// Buffer size limits / 缓冲区大小限制
#define TFW_NORMAL_BUFFER_SIZE_MAX 256
#define TFW_PATH_LEN_MAX 512
#define TFW_FILENAME_LEN_MAX 256
#define TFW_LOG_MAX_MESSAGE_LENGTH 2048
#define TFW_LOG_LABEL_LEN_MAX 32
#define TFW_MODULE_LABEL_LEN_MAX 32

// Timestamp related buffer sizes / 时间戳相关缓冲区大小
// Format: YYYY-MM-DD HH:MM:SS.mmm (23 characters + null terminator = 24)
// 格式：YYYY-MM-DD HH:MM:SS.mmm (23个字符 + 空终止符 = 24)
#define TFW_TIMESTAMP_LEN_MAX 32        // Maximum timestamp string length / 最大时间戳字符串长度
#define TFW_TIMESTAMP_BUFFER_SIZE 64    // Recommended buffer size for timestamp / 时间戳推荐缓冲区大小

// Configuration key names / 配置键名称
// Note: These strings are used in configuration files and should be consistent
// 注意：这些字符串用于配置文件，应保持一致
#define TFW_CONFIG_NAME_LOGGING_LEVEL "logging.level"
#define TFW_CONFIG_NAME_LOGGING_OUTPUT "logging.output"
#define TFW_CONFIG_NAME_LOGGING_FILE_PATH "logging.file_path"
#define TFW_CONFIG_NAME_LOGGING_FILE_PREFIX "logging.file_prefix"
#define TFW_CONFIG_NAME_LOGGING_MAX_FILE_SIZE "logging.max_file_size"
#define TFW_CONFIG_NAME_LOGGING_MAX_RETENTION_DAYS "logging.max_retention_days"
#define TFW_CONFIG_NAME_RESOURCES_AUTO_UPDATE "resources.auto_update"
#define TFW_CONFIG_NAME_RESOURCES_UPDATE_INTERVAL "resources.update_interval"
#define TFW_CONFIG_NAME_RESOURCES_CACHE_PATH "resources.cache_path"
#define TFW_CONFIG_NAME_SYSTEM_MAX_THREADS "system.max_threads"
#define TFW_CONFIG_NAME_SYSTEM_TIMEOUT "system.timeout"

// Default configuration values / 默认配置值
// Note: Log level configurations now use enum types, no string defaults needed
// 注意：日志等级配置现在使用枚举类型，不需要字符串默认值
#define TFW_CONFIG_DEFAULT_LOGGING_OUTPUT "console"
#define TFW_CONFIG_DEFAULT_LOGGING_FILE_PATH TFW_CONFIG_FILE_PATH"/logs"
#define TFW_CONFIG_DEFAULT_LOGGING_FILE_PREFIX "TFW"
#define TFW_CONFIG_DEFAULT_LOGGING_MAX_FILE_SIZE 10485760
#define TFW_CONFIG_DEFAULT_LOGGING_MAX_RETENTION_DAYS 7
#define TFW_CONFIG_DEFAULT_RESOURCES_AUTO_UPDATE true
#define TFW_CONFIG_DEFAULT_RESOURCES_UPDATE_INTERVAL 3600
#define TFW_CONFIG_DEFAULT_RESOURCES_CACHE_PATH "./cache"
#define TFW_CONFIG_DEFAULT_SYSTEM_MAX_THREADS 10
#define TFW_CONFIG_DEFAULT_SYSTEM_TIMEOUT 30

#endif  // TFW_COMMON_DEFINES_H
