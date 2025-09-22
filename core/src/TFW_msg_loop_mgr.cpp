#include "TFW_msg_loop_mgr.h"
#include "TFW_core_log.h"
#include "TFW_errorno.h"
#include "TFW_message_loop.h"
#include <cstring>
#include <cstdlib>

namespace TFW {

TFW_IMPLEMENT_SINGLE_INSTANCE(TFW_MsgLoopMgr)

int32_t TFW_MsgLoopMgr::Init() {
    TFW_LOGI_CORE("TFW_MsgLoopMgr::Init called");

    if (IsInitialized() != false) {
        TFW_LOGW_CORE("Msg loop manager already initialized");
        return TFW_SUCCESS;
    }

    // 初始化utils层的消息循环管理模块
    int32_t result = TFW_LooperInit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to initialize looper module");
        return TFW_ERROR;
    }

    // 设置初始化标志
    isInitialized_ = true;
    TFW_LOGI_CORE("TFW_MsgLoopMgr initialized successfully");

    return TFW_SUCCESS;
}

int32_t TFW_MsgLoopMgr::Deinit() {
    TFW_LOGI_CORE("TFW_MsgLoopMgr::Deinit called");

    if (IsInitialized() == false) {
        TFW_LOGI_CORE("Msg loop manager not initialized");
        return TFW_SUCCESS;
    }

    // 清理资源
    TFW_LooperDeinit();
    TFW_LOGI_CORE("Message loop module deinitialized");

    // 重置初始化标志
    isInitialized_ = false;
    TFW_LOGI_CORE("TFW_MsgLoopMgr deinitialized successfully");

    return TFW_SUCCESS;
}

bool TFW_MsgLoopMgr::IsInitialized() const {
    return isInitialized_;
}

TFW_Looper* TFW_MsgLoopMgr::GetLooper(TFW_LooperType type) {
    if (!IsInitialized()) {
        TFW_LOGE_CORE("Msg loop manager not initialized");
        return nullptr;
    }

    // 通过utils层获取指定类型的消息循环
    TFW_Looper* looper = TFW_GetLooper(type);
    if (looper == nullptr) {
        TFW_LOGE_CORE("Failed to get looper for type: %d", type);
    }

    return looper;
}

} // namespace TFW
