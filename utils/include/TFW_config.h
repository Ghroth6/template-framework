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
    TFW_CONFIG_MODULE_CORE = 0,    // 核心模块/Core module
    TFW_CONFIG_MODULE_UTILS,       // 工具模块/Utils module
    TFW_CONFIG_MODULE_SDK,         // SDK模块/SDK module
    TFW_CONFIG_MODULE_SYSTEM       // 系统模块/System module
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
 * @brief 获取指定键的配置项
 * @param key 配置项键枚举值
 * @param item 输出配置项指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigGetItem(TFW_ConfigKey key, TFW_ConfigItem *item);

// 释放单项配置内存
/**
 * @brief 释放通过TFW_ConfigGetItem获取的配置项内存
 * @param item 配置项指针
 */
void TFW_ConfigFreeItem(TFW_ConfigItem* item);

// 设置单项配置
/**
 * @brief 设置指定键的配置项
 * @param key 配置项键枚举值
 * @param item 配置项指针
 * @return 0表示成功，负值表示失败
 */
int32_t TFW_ConfigSetItem(TFW_ConfigKey key, const TFW_ConfigItem *item);

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
