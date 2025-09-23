/*
 * TFW_config.h
 *
 * TFW 配置管理模块头文件
 *
 * 该文件定义了配置管理相关的函数接口
 */

#ifndef TFW_CONFIG_H
#define TFW_CONFIG_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "TFW_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// 配置模块类型枚举
typedef enum {
    TFW_CONFIG_MODULE_LOG = 0,      // 核心模块/Core module
    TFW_CONFIG_MODULE_MAX           // 模块类型枚举最大值，请勿修改/Module type enumeration maximum value, please do not modify
} TFW_ConfigModule;

// 配置更新回调函数类型
// @param key 配置项键枚举值
// @param item 配置项指针
typedef void (*TFW_ConfigUpdateCallback)(TFW_ConfigKey key, const TFW_ConfigItem *item);

// 配置模块初始化
/**
 * @brief 初始化配置模块
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigInit(void);

// 配置模块反初始化
/**
 * @brief 反初始化配置模块
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigDeinit(void);

// 注册配置更新回调
/**
 * @brief 注册配置更新回调函数
 * @param module 模块类型
 * @param callback 回调函数指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigRegisterUpdateCallback(TFW_ConfigModule module, TFW_ConfigUpdateCallback callback);

// 取消注册配置更新回调
/**
 * @brief 取消注册配置更新回调函数
 * @param module 模块类型
 * @param callback 回调函数指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigUnregisterUpdateCallback(TFW_ConfigModule module);

// 读取全部配置
/**
 * @brief 获取全部配置
 * @param config_array 配置项数组指针
 * @param count 配置项数量指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigGetAll(TFW_ConfigItem **config_array, uint32_t *count);

// 释放配置数组内存
/**
 * @brief 释放通过TFW_ConfigGetAll获取的配置数组内存
 * @param config_array 配置项数组指针
 */
void TFW_ConfigFreeAll(TFW_ConfigItem *config_array);

// 读取单项配置
/**
 * @brief 获取指定键的配置项（只读引用方式）
 * @param key 配置项键枚举值
 * @param item 输出配置项指针
 * @return 0表示成功，负值表示失败
 * @note 返回的配置项为内部数据的引用，调用者需要立即使用或复制，
 *       避免在多线程环境中数据被修改导致的问题
 * @note 对于字符串类型的配置项，调用者不应尝试释放其内存
 */
int32_t TFW_ConfigGetItem(TFW_ConfigKey key, TFW_ConfigItem *item);

// 设置单项配置
/**
 * @brief 设置指定键的配置项
 * @param key 配置项键枚举值
 * @param item 配置项指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigSetItem(const TFW_ConfigItem * const item);

// 读取单项配置（整数）
/**
 * @brief 获取指定键的整数配置值
 * @param key 配置项键枚举值
 * @param value 输出整数值指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigGetInt(TFW_ConfigKey key, int32_t *value);

// 设置单项配置（整数）
/**
 * @brief 设置指定键的整数配置值
 * @param key 配置项键枚举值
 * @param value 整数值
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigSetInt(TFW_ConfigKey key, int32_t value);

// 读取单项配置（浮点数）
/**
 * @brief 获取指定键的浮点数配置值
 * @param key 配置项键枚举值
 * @param value 输出浮点数值指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigGetFloat(TFW_ConfigKey key, float *value);

// 设置单项配置（浮点数）
/**
 * @brief 设置指定键的浮点数配置值
 * @param key 配置项键枚举值
 * @param value 浮点数值
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigSetFloat(TFW_ConfigKey key, float value);

// 读取单项配置（布尔值）
/**
 * @brief 获取指定键的布尔配置值
 * @param key 配置项键枚举值
 * @param value 输出布尔值指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigGetBool(TFW_ConfigKey key, bool *value);

// 设置单项配置（布尔值）
/**
 * @brief 设置指定键的布尔配置值
 * @param key 配置项键枚举值
 * @param value 布尔值
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigSetBool(TFW_ConfigKey key, bool value);

// 读取单项配置（字符串）
/**
 * @brief 获取指定键的字符串配置值（只读引用方式）
 * @param key 配置项键枚举值
 * @param value 输出字符串值指针（内部数据引用，不应释放内存）
 * @return 0表示成功，负值表示失败
 * @note 返回的字符串为内部数据的引用，调用者需要立即使用或复制，
 *       避免在多线程环境中数据被修改导致的问题
 * @note 调用者不应尝试释放返回的字符串内存
 */
int32_t TFW_ConfigGetString(TFW_ConfigKey key, const char **value);

// 设置单项配置（字符串）
/**
 * @brief 设置指定键的字符串配置值
 * @param key 配置项键枚举值
 * @param value 字符串值
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigSetString(TFW_ConfigKey key, const char *value);

/**
 * @brief 根据配置键获取配置值的类型
 * @param key 配置项键枚举值
 * @param type 输出参数，返回配置值的类型
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigGetValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type);

// 保存配置到文件
/**
 * @brief 保存配置到文件
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigSave(void);

// 从文件重新加载配置
/**
 * @brief 从文件重新加载配置
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigReload(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TFW_CONFIG_H */
