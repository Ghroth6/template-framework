#ifndef TFW_CORE_H
#define TFW_CORE_H

#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_single_instance.h"
#include <memory>

namespace TFW {

class TFW_Core {
public:
    /**
     * Initialize core interface
     * 初始化核心接口
     *
     * @return Returns TFW_SUCCESS on success, error code on failure
     * @return 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t Init() = 0;

    /**
     * Deinitialize core interface
     * 反初始化核心接口
     *
     * @return Returns TFW_SUCCESS on success, error code on failure
     * @return 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t Deinit() = 0;

    /**
     * Check if the instance is initialized
     * 检查实例是否已初始化
     *
     * @return true means initialized, false means uninitialized
     * @return true表示已初始化，false表示未初始化
     */
    virtual bool IsInitialized() const = 0;

    /**
     * Get all configuration items
     * 获取所有配置项
     *
     * @param config_array Pointer to store the array of configuration items
     * @param count Pointer to store the number of configuration items
     * @return Returns TFW_SUCCESS on success, error code on failure
     * @param config_array 存储配置项数组的指针
     * @param count 存储配置项数量的指针
     * @return 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t GetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count) = 0;

    /**
     * Free all configuration items
     * 释放所有配置项
     *
     * This function releases the memory of the configuration item array allocated by GetAllConfigItems()
     * 该函数释放由GetAllConfigItems()分配的配置项数组的内存
     *
     * @param config_array Pointer to the configuration item array to be freed
     * @param config_array 待释放的配置项数组指针
     * @return Returns TFW_SUCCESS on success, error code on failure
     * @return 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual void FreeAllConfigItems(TFW_ConfigItem *config_array) = 0;

    /**
     * Get integer configuration value by key
     * 根据键获取整数配置值
     *
     * @param key Configuration key to query / 要查询的配置键
     * @param value Pointer to store the retrieved integer value / 存储获取的整数值的指针
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t GetIntConfigValue(TFW_ConfigKey key, int32_t *value) = 0;

    /**
     * Set integer configuration value by key
     * 根据键设置整数配置值
     *
     * @param key Configuration key to set / 要设置的配置键
     * @param value Integer value to set / 要设置的整数值
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t SetIntConfigValue(TFW_ConfigKey key, int32_t value) = 0;

    /**
     * Get float configuration value by key
     * 根据键获取浮点数配置值
     *
     * @param key Configuration key to query / 要查询的配置键
     * @param value Pointer to store the retrieved float value / 存储获取的浮点数值的指针
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t GetFloatConfigValue(TFW_ConfigKey key, float *value) = 0;

    /**
     * Set float configuration value by key
     * 根据键设置浮点数配置值
     *
     * @param key Configuration key to set / 要设置的配置键
     * @param value Float value to set / 要设置的浮点数值
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t SetFloatConfigValue(TFW_ConfigKey key, float value) = 0;

    /**
     * Get boolean configuration value by key
     * 根据键获取布尔配置值
     *
     * @param key Configuration key to query / 要查询的配置键
     * @param value Pointer to store the retrieved boolean value / 存储获取的布尔值的指针
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t GetBoolConfigValue(TFW_ConfigKey key, bool *value) = 0;

    /**
     * Set boolean configuration value by key
     * 根据键设置布尔配置值
     *
     * @param key Configuration key to set / 要设置的配置键
     * @param value Boolean value to set / 要设置的布尔值
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t SetBoolConfigValue(TFW_ConfigKey key, bool value) = 0;

    /**
     * Get string configuration value by key
     * 根据键获取字符串配置值
     *
     * @param key Configuration key to query / 要查询的配置键
     * @param value Pointer to store the retrieved string value pointer / 存储获取的字符串值指针的指针
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t GetStringConfigValue(TFW_ConfigKey key, const char **value) = 0;

    /**
     * Set string configuration value by key
     * 根据键设置字符串配置值
     *
     * @param key Configuration key to set / 要设置的配置键
     * @param value String value to set / 要设置的字符串值
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t SetStringConfigValue(TFW_ConfigKey key, const char *value) = 0;

    /**
     * Get configuration value type by key
     * 根据键获取配置值类型
     *
     * @param key Configuration key to query / 要查询的配置键
     * @param type Pointer to store the retrieved configuration value type / 存储获取的配置值类型的指针
     * @return Returns TFW_SUCCESS on success, error code on failure / 成功返回TFW_SUCCESS，失败返回错误码
     */
    virtual int32_t GetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type) = 0;

protected:
    TFW_Core() = default;
    virtual ~TFW_Core() = default;

private:
    TFW_DECLARE_SINGLE_INSTANCE_BASE(TFW_Core)
};

} // namespace TFW

#endif // TFW_CORE_H
