#include "../include/TFW_core.h"
#include "../interface/TFW_interface.h"
#include "../include/TFW_core_log.h"
#include <string>
#include <cstring>

namespace TFW {

Core& Core::GetInstance() {
    static Core instance;
    return instance;
}

int32_t Core::Initialize() {
    if (initialized_) {
        return TFW_ERROR_ALREADY_INIT;
    }

    try {
        // 初始化主类型
        mainType_.id = 1;
        SetData(1);
        
        initialized_ = true;
        TFW_LOGI_CORE("Core initialized successfully");
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during core initialization");
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t Core::Exit() {
    if (!initialized_) {
        return TFW_ERROR_NOT_INIT;
    }

    try {
        // 清理主类型
        mainType_.id = 0;
        mainType_.data = 0;
        initialized_ = false;
        TFW_LOGI_CORE("Core exited successfully");
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during core exit");
        return TFW_ERROR_OPERATION_FAIL;
    }
}

bool Core::IsInitialized() const {
    return initialized_;
}

int32_t Core::Action(const std::string& action) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    try {
        TFW_LOGI_CORE("Executing action: %s", action.c_str());
        TFW_LOGI_CORE("Action executed successfully: %s", action.c_str());
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during action execution: %s", action.c_str());
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t Core::GetData() const {
    return mainType_.data;
}

int32_t Core::SetData(const int32_t data) {
    mainType_.data = data;
    return TFW_SUCCESS;
}

} // namespace TFW
