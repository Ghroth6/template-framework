#include "TFW_core.h"
#include "TFW_interface.h"
#include "TFW_core_log.h"
#include <string>
#include <unordered_map>

namespace TFW {

Core& Core::GetInstance() {
    static Core instance;
    return instance;
}

ErrorCode Core::Initialize() {
    if (initialized_) {
        return ErrorCode::StandardError;
    }

    try {
        // 鍒濆鍖栦富绫诲瀷
        if (mainType_.Initialize()) {
            // 鍒濆鍖栨帴鍙?            auto result = CoreInterface::GetInstance().Initialize();
            if (result == ErrorCode::Success) {
                initialized_ = true;
                TFW_LOGI_CORE("Core initialized successfully");
                return ErrorCode::Success;
            } else {
                TFW_LOGE_CORE("Core interface initialization failed");
                return result;
            }
        } else {
            TFW_LOGE_CORE("Main type initialization failed");
            return ErrorCode::StandardError;
        }
    } catch (...) {
        TFW_LOGE_CORE("Exception during core initialization");
        return ErrorCode::StandardError;
    }
}

ErrorCode Core::Exit() {
    if (!initialized_) {
        return ErrorCode::StandardError;
    }

    try {
        // 娓呯悊涓荤被鍨?        mainType_.Cleanup();

        // 閫€鍑烘帴鍙?        auto result = CoreInterface::GetInstance().Exit();
        if (result == ErrorCode::Success) {
            initialized_ = false;
            TFW_LOGI_CORE("Core exited successfully");
            return ErrorCode::Success;
        } else {
            TFW_LOGE_CORE("Core interface exit failed");
            return result;
        }
    } catch (...) {
        TFW_LOGE_CORE("Exception during core exit");
        return ErrorCode::StandardError;
    }
}

bool Core::IsInitialized() const {
    return initialized_;
}

ErrorCode Core::GetValue(const std::string& key, std::string& value) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return ErrorCode::StandardError;
    }

    try {
        TFW_LOGI_CORE("Getting value for key: %s", key.c_str());

        // 绠€鍗曠殑閿€煎瀛樺偍
        auto it = keyValueStore_.find(key);
        if (it != keyValueStore_.end()) {
            value = it->second;
            TFW_LOGI_CORE("Value found for key: %s", key.c_str());
            return ErrorCode::Success;
        } else {
            TFW_LOGW_CORE("Value not found for key: %s", key.c_str());
            return ErrorCode::NotFound;
        }
    } catch (...) {
        TFW_LOGE_CORE("Exception during get value: %s", key.c_str());
        return ErrorCode::StandardError;
    }
}

ErrorCode Core::SetValue(const std::string& key, const std::string& value) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return ErrorCode::StandardError;
    }

    try {
        TFW_LOGI_CORE("Setting value for key: %s", key.c_str());

        // 绠€鍗曠殑閿€煎瀛樺偍
        keyValueStore_[key] = value;

        TFW_LOGI_CORE("Value set successfully for key: %s", key.c_str());
        return ErrorCode::Success;
    } catch (...) {
        TFW_LOGE_CORE("Exception during set value: %s", key.c_str());
        return ErrorCode::StandardError;
    }
}

ErrorCode Core::Action(const std::string& action) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return ErrorCode::StandardError;
    }

    try {
        TFW_LOGI_CORE("Executing action: %s", action.c_str());

        // 绌哄疄鐜扮殑鍔ㄤ綔鍑芥暟
        // 杩欓噷鍙互鏍规嵁action鍙傛暟鎵ц涓嶅悓鐨勫姩浣?        // 褰撳墠鍙槸璁板綍鏃ュ織骞惰繑鍥炴垚鍔?
        TFW_LOGI_CORE("Action executed successfully: %s", action.c_str());
        return ErrorCode::Success;
    } catch (...) {
        TFW_LOGE_CORE("Exception during action execution: %s", action.c_str());
        return ErrorCode::StandardError;
    }
}

} // namespace TFW
