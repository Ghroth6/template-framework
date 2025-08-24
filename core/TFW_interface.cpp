#include "../interface/TFW_interface.h"
#include "TFW_core_log.h"
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
        initialized_ = true;  // 设置初始化标志
        TFW_LOGI_CORE("CoreInterface initialized");
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during CoreInterface initialization");
        return TFW_ERROR;
    }
}

TFW_Result CoreInterface::Exit() {
    try {
        initialized_ = false;  // 重置初始化标志
        TFW_LOGI_CORE("CoreInterface exited");
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during CoreInterface exit");
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
        // 这里可以添加具体的操作逻辑
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during operation execution: %s", operation.c_str());
        return TFW_ERROR;
    }
}



} // namespace TFW
