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


int32_t TFW_CORE_GET_DATA() {

    try {
        auto result = TFW::CoreInterface::GetInstance()->GetData();
        return static_cast<int32_t>(result);
    } catch (...) {
        return TFW_ERROR;
    }
}

int32_t TFW_CORE_SET_DATA(const int32_t data) {

    try {
        auto result = TFW::CoreInterface::GetInstance()->SetData(data);
        return static_cast<int32_t>(result);
    } catch (...) {
        return TFW_ERROR;
    }
}

} // extern "C"
