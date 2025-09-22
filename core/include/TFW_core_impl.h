#ifndef TFW_CORE_IMPL_H
#define TFW_CORE_IMPL_H

#include "TFW_core.h"
#include "TFW_types.h"
#include "TFW_single_instance.h"
#include <map>
#include <string>
#include <memory>

namespace TFW {

// Core实现类
class TFW_Core_Impl : public TFW_Core {
    TFW_DECLARE_SINGLE_INSTANCE(TFW_Core_Impl)

private:
    // 内部状态
    bool isInitialized_ = false;

public:
    // 核心初始化和去初始化
    virtual int32_t Init() override;
    virtual int32_t Deinit() override;
    virtual bool IsInitialized() const override;

    // 配置管理接口
    virtual int32_t GetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count) override;
    virtual void FreeAllConfigItems(TFW_ConfigItem *config_array) override;
    virtual int32_t GetIntConfigValue(TFW_ConfigKey key, int32_t *value) override;
    virtual int32_t SetIntConfigValue(TFW_ConfigKey key, int32_t value) override;
    virtual int32_t GetFloatConfigValue(TFW_ConfigKey key, float *value) override;
    virtual int32_t SetFloatConfigValue(TFW_ConfigKey key, float value) override;
    virtual int32_t GetBoolConfigValue(TFW_ConfigKey key, bool *value) override;
    virtual int32_t SetBoolConfigValue(TFW_ConfigKey key, bool value) override;
    virtual int32_t GetStringConfigValue(TFW_ConfigKey key, const char **value) override;
    virtual int32_t SetStringConfigValue(TFW_ConfigKey key, const char *value) override;
    virtual int32_t GetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type) override;
};

} // namespace TFW

#endif // TFW_CORE_IMPL_H
