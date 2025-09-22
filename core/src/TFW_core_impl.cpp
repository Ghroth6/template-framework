#include "TFW_core_impl.h"
#include "TFW_config_manager.h"
#include "TFW_msg_loop_mgr.h"
#include "TFW_core_log.h"
#include "TFW_errorno.h"
#include "TFW_types.h"
#include <cstring>
#include <cstdlib>

namespace TFW {

TFW_IMPLEMENT_SINGLE_INSTANCE(TFW_Core_Impl)

int32_t TFW_Core_Impl::Init() {
    TFW_LOGI_CORE("TFW_Core_Impl::Init called");

    if (IsInitialized()) {
        TFW_LOGW_CORE("Core already initialized");
        return TFW_ERROR_ALREADY_INIT;
    }

    int32_t result = TFW_ConfigManager::GetInstance().Init();
    if(result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to initialize config manager, error: %d", result);
        TFW_ConfigManager::GetInstance().Deinit();
        return result;
    }
    // 初始化配置管理器
    result = TFW_MsgLoopMgr::GetInstance().Init();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to initialize message loop manager, error: %d", result);
        TFW_MsgLoopMgr::GetInstance().Deinit();
        TFW_ConfigManager::GetInstance().Deinit();
        return result;
    }

    // 设置初始化标志
    isInitialized_ = true;
    TFW_LOGI_CORE("TFW_Core_Impl initialized successfully");

    return TFW_SUCCESS;
}

int32_t TFW_Core_Impl::Deinit() {
    TFW_LOGI_CORE("TFW_Core_Impl::Deinit called");

    if (!IsInitialized()) {
        TFW_LOGI_CORE("Core not initialized");
        return TFW_SUCCESS;
    }

    // 清理配置管理器
    int32_t result = TFW_MsgLoopMgr::GetInstance().Deinit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to deinitialize message loop manager, error: %d", result);
    }

    result = TFW_ConfigManager::GetInstance().Deinit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to deinitialize config manager, error: %d", result);
    }

    // 重置初始化标志
    isInitialized_ = false;
    TFW_LOGI_CORE("TFW_Core_Impl deinitialized successfully");

    return TFW_SUCCESS;
}

bool TFW_Core_Impl::IsInitialized() const {
    return isInitialized_;
}

int32_t TFW_Core_Impl::GetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count) {
    TFW_LOGD_CORE("GetAllConfigItems called");

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().GetAllConfigItems(config_array, count);
}

void TFW_Core_Impl::FreeAllConfigItems(TFW_ConfigItem *config_array) {
    TFW_LOGD_CORE("FreeAllConfigItems called");

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return;
    }

    return TFW_ConfigManager::GetInstance().FreeAllConfigItems(config_array);
}

int32_t TFW_Core_Impl::GetIntConfigValue(TFW_ConfigKey key, int32_t *value) {
    TFW_LOGD_CORE("GetIntConfigValue called, key=%d", key);

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().GetIntConfigValue(key, value);
}

int32_t TFW_Core_Impl::SetIntConfigValue(TFW_ConfigKey key, int32_t value) {
    TFW_LOGD_CORE("SetIntConfigValue called, key=%d, value=%d", key, value);

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().SetIntConfigValue(key, value);
}

int32_t TFW_Core_Impl::GetFloatConfigValue(TFW_ConfigKey key, float *value) {
    TFW_LOGD_CORE("GetFloatConfigValue called, key=%d", key);

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().GetFloatConfigValue(key, value);
}

int32_t TFW_Core_Impl::SetFloatConfigValue(TFW_ConfigKey key, float value) {
    TFW_LOGD_CORE("SetFloatConfigValue called, key=%d, value=%f", key, value);

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().SetFloatConfigValue(key, value);
}

int32_t TFW_Core_Impl::GetBoolConfigValue(TFW_ConfigKey key, bool *value) {
    TFW_LOGD_CORE("GetBoolConfigValue called, key=%d", key);

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().GetBoolConfigValue(key, value);
}

int32_t TFW_Core_Impl::SetBoolConfigValue(TFW_ConfigKey key, bool value) {
    TFW_LOGD_CORE("SetBoolConfigValue called, key=%d, value=%s", key, value ? "true" : "false");

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().SetBoolConfigValue(key, value);
}

int32_t TFW_Core_Impl::GetStringConfigValue(TFW_ConfigKey key, const char **value) {
    TFW_LOGD_CORE("GetStringConfigValue called, key=%d", key);

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().GetStringConfigValue(key, value);
}

int32_t TFW_Core_Impl::SetStringConfigValue(TFW_ConfigKey key, const char *value) {
    TFW_LOGD_CORE("SetStringConfigValue called, key=%d, value=%s", key, value ? value : "null");

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().SetStringConfigValue(key, value);
}

int32_t TFW_Core_Impl::GetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type) {
    TFW_LOGD_CORE("GetConfigValueTypeByKey called, key=%d", key);

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    return TFW_ConfigManager::GetInstance().GetConfigValueTypeByKey(key, type);
}

} // namespace TFW
