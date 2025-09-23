#include "TFW_config_manager.h"
#include "TFW_core_log.h"
#include "TFW_errorno.h"
#include "TFW_config.h"
#include <cstring>
#include <cstdlib>

namespace TFW {

TFW_IMPLEMENT_SINGLE_INSTANCE(TFW_ConfigManager)

int32_t TFW_ConfigManager::Init() {
    TFW_LOGI_CORE("TFW_ConfigManager::Init called");

    if (IsInitialized() != false) {
        TFW_LOGW_CORE("Config manager already initialized");
        return TFW_SUCCESS;
    }

    // 初始化utils配置模块
    int32_t result = TFW_ConfigInit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to initialize utils config module, error: %d", result);
        return result;
    }

    // 设置初始化标志
    isInitialized_ = true;
    TFW_LOGI_CORE("TFW_ConfigManager initialized successfully");

    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::Deinit() {
    TFW_LOGI_CORE("TFW_ConfigManager::Deinit called");

    if (IsInitialized() == false) {
        TFW_LOGI_CORE("Config manager not initialized");
        return TFW_SUCCESS;
    }

    // 反初始化utils配置模块
    int32_t result = TFW_ConfigDeinit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to deinitialize utils config module, error: %d", result);
        return result;
    }

    // 重置初始化标志
    isInitialized_ = false;
    TFW_LOGI_CORE("TFW_ConfigManager deinitialized successfully");

    return TFW_SUCCESS;
}

bool TFW_ConfigManager::IsInitialized() const {
    return isInitialized_;
}

int32_t TFW_ConfigManager::GetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count) {
    TFW_LOGD_CORE("GetAllConfigItems called");

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (config_array == nullptr || count == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for GetAllConfigItems");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t result = TFW_ConfigGetAll(config_array, count);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigGetAll failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("GetAllConfigItems success, count=%u", *count);
    return TFW_SUCCESS;
}

void TFW_ConfigManager::FreeAllConfigItems(TFW_ConfigItem *config_array) {
    TFW_LOGD_CORE("FreeAllConfigItems called");

    if (config_array == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for FreeAllConfigItems");
        return;
    }

    TFW_ConfigFreeAll(config_array);
    TFW_LOGD_CORE("Config items memory freed");
}

int32_t TFW_ConfigManager::GetIntConfigValue(TFW_ConfigKey key, int32_t *value) {
    TFW_LOGD_CORE("GetIntConfigValue called, key=%d", key);

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (value == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for GetIntConfigValue");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t result = TFW_ConfigGetInt(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigGetInt failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("GetIntConfigValue success, key=%d, value=%d", key, *value);
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::SetIntConfigValue(TFW_ConfigKey key, int32_t value) {
    TFW_LOGD_CORE("SetIntConfigValue called, key=%d, value=%d", key, value);

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    int32_t result = TFW_ConfigSetInt(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigSetInt failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("SetIntConfigValue success, key=%d, value=%d", key, value);
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::GetFloatConfigValue(TFW_ConfigKey key, float *value) {
    TFW_LOGD_CORE("GetFloatConfigValue called, key=%d", key);

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (value == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for GetFloatConfigValue");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t result = TFW_ConfigGetFloat(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigGetFloat failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("GetFloatConfigValue success, key=%d, value=%f", key, *value);
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::SetFloatConfigValue(TFW_ConfigKey key, float value) {
    TFW_LOGD_CORE("SetFloatConfigValue called, key=%d, value=%f", key, value);

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    int32_t result = TFW_ConfigSetFloat(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigSetFloat failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("SetFloatConfigValue success, key=%d, value=%f", key, value);
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::GetBoolConfigValue(TFW_ConfigKey key, bool *value) {
    TFW_LOGD_CORE("GetBoolConfigValue called, key=%d", key);

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (value == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for GetBoolConfigValue");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t result = TFW_ConfigGetBool(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigGetBool failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("GetBoolConfigValue success, key=%d, value=%s", key, *value ? "true" : "false");
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::SetBoolConfigValue(TFW_ConfigKey key, bool value) {
    TFW_LOGD_CORE("SetBoolConfigValue called, key=%d, value=%s", key, value ? "true" : "false");

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    int32_t result = TFW_ConfigSetBool(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigSetBool failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("SetBoolConfigValue success, key=%d, value=%s", key, value ? "true" : "false");
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::GetStringConfigValue(TFW_ConfigKey key, const char **value) {
    TFW_LOGD_CORE("GetStringConfigValue called, key=%d", key);

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (value == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for GetStringConfigValue");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t result = TFW_ConfigGetString(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigGetString failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("GetStringConfigValue success, key=%d, value=%s", key, *value ? *value : "null");
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::SetStringConfigValue(TFW_ConfigKey key, const char *value) {
    TFW_LOGD_CORE("SetStringConfigValue called, key=%d, value=%s", key, value ? value : "null");

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (value == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for SetStringConfigValue");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t result = TFW_ConfigSetString(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigSetString failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("SetStringConfigValue success, key=%d, value=%s", key, value);
    return TFW_SUCCESS;
}

int32_t TFW_ConfigManager::GetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type) {
    TFW_LOGD_CORE("GetConfigValueTypeByKey called, key=%d", key);

    if (IsInitialized() == false) {
        TFW_LOGE_CORE("Config manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (type == nullptr) {
        TFW_LOGE_CORE("Invalid parameter for GetConfigValueTypeByKey");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t result = TFW_ConfigGetValueTypeByKey(key, type);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("TFW_ConfigGetValueTypeByKey failed with error: %d", result);
        return result;
    }

    TFW_LOGD_CORE("GetConfigValueTypeByKey success, key=%d, type=%d", key, static_cast<int>(*type));
    return TFW_SUCCESS;
}

} // namespace TFW
