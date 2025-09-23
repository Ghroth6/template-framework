#ifndef TFW_CONFIG_MANAGER_H
#define TFW_CONFIG_MANAGER_H

#include "TFW_types.h"
#include "TFW_single_instance.h"
#include <map>

namespace TFW {

class TFW_ConfigManager {
    TFW_DECLARE_SINGLE_INSTANCE(TFW_ConfigManager)

private:
    // 内部状态
    bool isInitialized_ = false;

public:
    // 初始化和去初始化
    int32_t Init();
    int32_t Deinit();
    bool IsInitialized() const;

    // 配置管理接口
    int32_t GetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count);
    void FreeAllConfigItems(TFW_ConfigItem *config_array);
    int32_t GetIntConfigValue(TFW_ConfigKey key, int32_t *value);
    int32_t SetIntConfigValue(TFW_ConfigKey key, int32_t value);
    int32_t GetFloatConfigValue(TFW_ConfigKey key, float *value);
    int32_t SetFloatConfigValue(TFW_ConfigKey key, float value);
    int32_t GetBoolConfigValue(TFW_ConfigKey key, bool *value);
    int32_t SetBoolConfigValue(TFW_ConfigKey key, bool value);
    int32_t GetStringConfigValue(TFW_ConfigKey key, const char **value);
    int32_t SetStringConfigValue(TFW_ConfigKey key, const char *value);
    int32_t GetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type);
};

} // namespace TFW

#endif // TFW_CONFIG_MANAGER_H
