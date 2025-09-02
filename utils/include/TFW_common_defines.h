#ifndef TFW_COMMON_DEFINES_H
#define TFW_COMMON_DEFINES_H

// 跨平台的未使用变量/函数标记宏
// Cross-platform unused variable/function marker macro

#ifdef _WIN32
#define TFW_UNUSED __pragma(warning(disable : 4505))
#else
#define TFW_UNUSED __attribute__((unused))
#endif

// Version information / 版本信息
// Note: Update these values manually when releasing new versions
// 注意：发布新版本时请手动更新这些值
#define TFW_VERSION_MAJOR 1
#define TFW_VERSION_MINOR 0
#define TFW_VERSION_PATCH 0
#define TFW_VERSION_STRING "1.0.0"

// Configuration file settings / 配置文件设置
#define TFW_CONFIG_FILE_PATH "runtime"
#define TFW_CONFIG_FILE_NAME "TFW_config.json"

// Buffer size limits / 缓冲区大小限制
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
#define TFW_CONFIG_NAME_MAIN_VERSION "main.version"
#define TFW_CONFIG_NAME_MAIN_DEBUG "main.debug"
#define TFW_CONFIG_NAME_MAIN_LOG_LEVEL "main.log_level"
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
#define TFW_CONFIG_DEFAULT_LOGGING_FILE_PATH "./logs"
#define TFW_CONFIG_DEFAULT_LOGGING_FILE_PREFIX "tfw"
#define TFW_CONFIG_DEFAULT_RESOURCES_CACHE_PATH "./cache"

#endif  // TFW_COMMON_DEFINES_H
