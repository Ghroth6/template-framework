#include "../include/TFW_core.h"
#include "../interface/TFW_interface.h"
#include "../include/TFW_core_log.h"
#include <string>
#include <unordered_map>

namespace TFW {

Core& Core::GetInstance() {
    static Core instance;
    return instance;
}

ErrorCode Core::Initialize() {
    if (initialized_) {
        return ErrorCode::TFW_ERROR_ALREADY_INIT;
    }

    try {
        // 初始化主类型
        if (mainType_.Initialize()) {
            // 初始化接口
            auto result = CoreInterface::GetInstance()->Initialize();
            if (result == TFW_SUCCESS) {
                initialized_ = true;
                TFW_LOGI_CORE("Core initialized successfully");
                return ErrorCode::TFW_SUCCESS;
            } else {
                TFW_LOGE_CORE("Core interface initialization failed");
                return static_cast<ErrorCode>(result);
            }
        } else {
            TFW_LOGE_CORE("Main type initialization failed");
            return ErrorCode::TFW_ERROR_OPERATION_FAIL;
        }
    } catch (...) {
        TFW_LOGE_CORE("Exception during core initialization");
        return ErrorCode::TFW_ERROR_OPERATION_FAIL;
    }
}

ErrorCode Core::Exit() {
    if (!initialized_) {
        return ErrorCode::TFW_ERROR_NOT_INIT;
    }

    try {
        // 清理主类型
        mainType_.Cleanup();

        // 退出接口
        auto result = CoreInterface::GetInstance()->Exit();
        if (result == TFW_SUCCESS) {
            initialized_ = false;
            TFW_LOGI_CORE("Core exited successfully");
            return ErrorCode::TFW_SUCCESS;
        } else {
            TFW_LOGE_CORE("Core interface exit failed");
            return static_cast<ErrorCode>(result);
        }
    } catch (...) {
        TFW_LOGE_CORE("Exception during core exit");
        return ErrorCode::TFW_ERROR_OPERATION_FAIL;
    }
}

bool Core::IsInitialized() const {
    return initialized_;
}

ErrorCode Core::GetValue(const std::string& key, std::string& value) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return ErrorCode::TFW_ERROR_NOT_INIT;
    }

    try {
        TFW_LOGI_CORE("Getting value for key: %s", key.c_str());

        // 简单的键值对存储
        auto it = keyValueStore_.find(key);
        if (it != keyValueStore_.end()) {
            value = it->second;
            TFW_LOGI_CORE("Value found for key: %s", key.c_str());
            return ErrorCode::TFW_SUCCESS;
        } else {
            TFW_LOGW_CORE("Value not found for key: %s", key.c_str());
            return ErrorCode::TFW_ERROR_NOT_FOUND;
        }
    } catch (...) {
        TFW_LOGE_CORE("Exception during get value: %s", key.c_str());
        return ErrorCode::TFW_ERROR_OPERATION_FAIL;
    }
}

ErrorCode Core::SetValue(const std::string& key, const std::string& value) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return ErrorCode::TFW_ERROR_NOT_INIT;
    }

    try {
        TFW_LOGI_CORE("Setting value for key: %s", key.c_str());

        // 简单的键值对存储
        keyValueStore_[key] = value;

        TFW_LOGI_CORE("Value set successfully for key: %s", key.c_str());
        return ErrorCode::TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during set value: %s", key.c_str());
        return ErrorCode::TFW_ERROR_OPERATION_FAIL;
    }
}

ErrorCode Core::Action(const std::string& action) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return ErrorCode::TFW_ERROR_NOT_INIT;
    }

    try {
        TFW_LOGI_CORE("Executing action: %s", action.c_str());

        // 空实现的动作函数
        // 这里可以根据action参数执行不同的动作
        // 当前只是记录日志并返回成功
        TFW_LOGI_CORE("Action executed successfully: %s", action.c_str());
        return ErrorCode::TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during action execution: %s", action.c_str());
        return ErrorCode::TFW_ERROR_OPERATION_FAIL;
    }
}

} // namespace TFW
