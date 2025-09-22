#include "TFW_core_interface.h"
#include "TFW_core_log.h"
#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_common_defines.h"
#include "TFW_config.h"
#include "TFW_core.h"

static TFW::TFW_Core* g_coreImplInstance = nullptr;

// C接口实现
extern "C" {

int32_t TFW_CoreInterfaceInit(void) {
    TFW_LOGI_CORE("TFW_CoreInterfaceInit called");

    if (g_coreImplInstance == nullptr) {
        g_coreImplInstance = &TFW::TFW_Core::GetInstance();
    }

    // 检查是否已经初始化
    if (g_coreImplInstance->IsInitialized()) {
        TFW_LOGI_CORE("Core already initialized, returning success");
        return TFW_SUCCESS;
    }

    // 调用C++实现进行初始化
    int32_t result = g_coreImplInstance->Init();
    if (result == TFW_SUCCESS) {
        TFW_LOGI_CORE("Core initialization successful");
    } else {
        TFW_LOGE_CORE("Core initialization failed with error: %d", result);
    }

    return result;
}

int32_t TFW_CoreInterfaceDeinit(void) {
    TFW_LOGI_CORE("TFW_CoreInterfaceDeinit called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现进行反初始化
    int32_t result = g_coreImplInstance->Deinit();
    if (result == TFW_SUCCESS) {
        TFW_LOGI_CORE("Core deinitialization successful");
    } else {
        TFW_LOGE_CORE("Core deinitialization failed with error: %d", result);
    }

    return result;
}

int32_t TFW_CoreInterfaceGetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count) {
    TFW_LOGD_CORE("TFW_CoreInterfaceGetAllConfigItems called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现获取配置项
    int32_t result = g_coreImplInstance->GetAllConfigItems(config_array, count);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to get all config items, error: %d", result);
    }
    return result;
}

void TFW_CoreInterfaceFreeAllConfigItems(TFW_ConfigItem *config_array) {
    TFW_LOGD_CORE("TFW_CoreInterfaceFreeAllConfigItems called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return;
    }

    // 调用C++实现释放配置项
    g_coreImplInstance->FreeAllConfigItems(config_array);
    return;
}

int32_t TFW_CoreInterfaceGetIntConfigValue(TFW_ConfigKey key, int32_t *value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceGetIntConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现获取整数配置值
    return g_coreImplInstance->GetIntConfigValue(key, value);
}

int32_t TFW_CoreInterfaceSetIntConfigValue(TFW_ConfigKey key, int32_t value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceSetIntConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现设置整数配置值
    return g_coreImplInstance->SetIntConfigValue(key, value);
}

int32_t TFW_CoreInterfaceGetFloatConfigValue(TFW_ConfigKey key, float *value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceGetFloatConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现获取浮点数配置值
    return g_coreImplInstance->GetFloatConfigValue(key, value);
}

int32_t TFW_CoreInterfaceSetFloatConfigValue(TFW_ConfigKey key, float value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceSetFloatConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现设置浮点数配置值
    return g_coreImplInstance->SetFloatConfigValue(key, value);
}

int32_t TFW_CoreInterfaceGetBoolConfigValue(TFW_ConfigKey key, bool *value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceGetBoolConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现获取布尔配置值
    return g_coreImplInstance->GetBoolConfigValue(key, value);
}

int32_t TFW_CoreInterfaceSetBoolConfigValue(TFW_ConfigKey key, bool value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceSetBoolConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现设置布尔配置值
    return g_coreImplInstance->SetBoolConfigValue(key, value);
}

int32_t TFW_CoreInterfaceGetStringConfigValue(TFW_ConfigKey key, const char **value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceGetStringConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现获取字符串配置值
    return g_coreImplInstance->GetStringConfigValue(key, value);
}

int32_t TFW_CoreInterfaceSetStringConfigValue(TFW_ConfigKey key, const char *value) {
    TFW_LOGD_CORE("TFW_CoreInterfaceSetStringConfigValue called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }
    return g_coreImplInstance->SetStringConfigValue(key, value);
}

int32_t TFW_CoreInterfaceGetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type) {
    TFW_LOGD_CORE("TFW_CoreInterfaceGetValueTypeByKey called");

    if (g_coreImplInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 调用C++实现获取配置值类型
    return g_coreImplInstance->GetConfigValueTypeByKey(key, type);
}

}  // extern "C"
