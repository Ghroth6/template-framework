#include "../interface/TFW_interface.h"
#include "include/TFW_core.h"
#include "include/TFW_core_log.h"
#include <unordered_map>
#include <mutex>

namespace TFW {

// ============================================================================
// CoreInterface 实现
// ============================================================================

CoreInterface* CoreInterface::GetInstance() {
    static CoreInterface instance;
    return &instance;
}

TFW_Result CoreInterface::Initialize() {
    try {
        TFW::Core::GetInstance().Initialize();
        initialized_ = true;  // 设置初始化标志
        TFW_LOGI_CORE("CoreInterface initialized success");
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("CoreInterface initialized failed");
        return TFW_ERROR;
    }
}

TFW_Result CoreInterface::Exit() {
    try {
        TFW::Core::GetInstance().Exit();
        initialized_ = false;  // 重置初始化标志
        return TFW_SUCCESS;
    } catch (...) {
        return TFW_ERROR;
    }
}

bool CoreInterface::IsInitialized() const {
    return initialized_;
}

TFW_Result CoreInterface::ExecuteOperation(const std::string& operation) {
    if (!initialized_) {
        TFW_LOGE_CORE("CoreInterface not initialized");
        return TFW_ERROR;
    }

    try {
        TFW_LOGI_CORE("Executing operation: %s", operation.c_str());
        // 调用Core的实现
        auto result = TFW::Core::GetInstance().Action(operation);
        return static_cast<TFW_Result>(result);
    } catch (...) {
        TFW_LOGE_CORE("Exception during operation execution: %s", operation.c_str());
        return TFW_ERROR;
    }
}

TFW_Result CoreInterface::GetValue(const std::string& key, std::string& value) {
    if (!initialized_) {
        TFW_LOGE_CORE("CoreInterface not initialized");
        return TFW_ERROR;
    }

    try {
        TFW_LOGI_CORE("Getting value for key: %s", key.c_str());
        auto result = TFW::Core::GetInstance().GetValue(key, value);
        return static_cast<TFW_Result>(result);
    } catch (...) {
        TFW_LOGE_CORE("Exception during get value: %s", key.c_str());
        return TFW_ERROR;
    }
}

TFW_Result CoreInterface::SetValue(const std::string& key, const std::string& value) {
    if (!initialized_) {
        TFW_LOGE_CORE("CoreInterface not initialized");
        return TFW_ERROR;
    }

    try {
        TFW_LOGI_CORE("Setting value for key: %s", key.c_str());
        auto result = TFW::Core::GetInstance().SetValue(key, value);
        return static_cast<TFW_Result>(result);
    } catch (...) {
        TFW_LOGE_CORE("Exception during set value: %s", key.c_str());
        return TFW_ERROR;
    }
}



} // namespace TFW
