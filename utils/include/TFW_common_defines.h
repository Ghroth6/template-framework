#ifndef TFW_COMMON_DEFINES_H
#define TFW_COMMON_DEFINES_H

// 跨平台的未使用变量/函数标记宏
#ifdef _WIN32
    #define TFW_UNUSED __pragma(warning(disable:4505))
#else
    #define TFW_UNUSED __attribute__((unused))
#endif

#define TFW_CONFIG_FILE_PATH "runtime"
#define TFW_CONFIG_FILE_NAME "TFW_config.json"
#define TFW_PATH_LEN_MAX 512

#endif // TFW_COMMON_DEFINES_H
