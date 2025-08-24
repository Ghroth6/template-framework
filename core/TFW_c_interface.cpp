#include "../interface/TFW_c_interface.h"
#include "TFW_interface.h"
#include "include/TFW_core_log.h"
#include <cstring>
#include <algorithm>

extern "C" {

// ============================================================================
// 核心模块C接口实现
// ============================================================================

int32_t TFW_CORE_INITIALIZE() {
    try {
        auto result = TFW::CoreInterface::GetInstance()->Initialize();
        return static_cast<int32_t>(result);
    } catch (...) {
        return TFW_ERROR;
    }
}

int32_t TFW_CORE_EXIT() {
    try {
        auto result = TFW::CoreInterface::GetInstance()->Exit();
        return static_cast<int32_t>(result);
    } catch (...) {
        return TFW_ERROR;
    }
}

int32_t TFW_CORE_IS_INITIALIZED() {
    try {
        return TFW::CoreInterface::GetInstance()->IsInitialized() ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

int32_t TFW_CORE_GET_VALUE(const char* key, char* value, int32_t maxLen) {
    if (key == nullptr || value == nullptr || maxLen <= 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    try {
        std::string keyStr(key);
        std::string valueStr;

        auto result = TFW::CoreInterface::GetInstance()->GetValue(keyStr, valueStr);
        if (result == TFW_SUCCESS) {
            size_t copyLen = std::min(static_cast<size_t>(maxLen - 1), valueStr.length());
            std::strncpy(value, valueStr.c_str(), copyLen);
            value[copyLen] = '\0';
            return TFW_SUCCESS;
        } else {
            return static_cast<int32_t>(result);
        }
    } catch (...) {
        return TFW_ERROR;
    }
}

int32_t TFW_CORE_SET_VALUE(const char* key, const char* value) {
    if (key == nullptr || value == nullptr) {
        return TFW_ERROR_INVALID_PARAM;
    }

    try {
        std::string keyStr(key);
        std::string valueStr(value);

        auto result = TFW::CoreInterface::GetInstance()->SetValue(keyStr, valueStr);
        return static_cast<int32_t>(result);
    } catch (...) {
        return TFW_ERROR;
    }
}

int32_t TFW_CORE_ACTION(const char* action) {
    if (action == nullptr) {
        return TFW_ERROR_INVALID_PARAM;
    }

    try {
        std::string actionStr(action);
        auto result = TFW::CoreInterface::GetInstance()->ExecuteOperation(actionStr);
        return static_cast<int32_t>(result);
    } catch (...) {
        return TFW_ERROR;
    }
}

} // extern "C"
