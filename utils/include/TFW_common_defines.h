#ifndef TFW_COMMON_DEFINES_H
#define TFW_COMMON_DEFINES_H

// 跨平台的未使用变量/函数标记宏
#ifdef _WIN32
    #define TFW_UNUSED __pragma(warning(disable:4505))
#else
    #define TFW_UNUSED __attribute__((unused))
#endif

#define TFW_CONFIG_FILE_PATH "TFW_config.json"

#endif // TFW_COMMON_DEFINES_H
