#include "TFW_msg_loop_mgr.h"
#include "TFW_core_log.h"
#include "TFW_errorno.h"
#include "TFW_message_loop.h"
#include "TFW_mem.h"
#include <cstring>
#include <cstdlib>

namespace TFW {

TFW_IMPLEMENT_SINGLE_INSTANCE(TFW_MsgLoopMgr)

#define TFW_ASYNC_CALLBACK_HANDLER_NAME "TFW_AsyncHandler"
#define TFW_ASYNC_CALLBACK_MSG_TYPE 0

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

    // 初始化异步回调处理器
    asyncCallbackHandler_.name = const_cast<char*>(TFW_ASYNC_CALLBACK_HANDLER_NAME);
    asyncCallbackHandler_.looper = nullptr;
    asyncCallbackHandler_.HandleMessage = AsyncCallbackHandler;

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

    asyncCallbackHandler_.looper = nullptr;
    asyncCallbackHandler_.HandleMessage = nullptr;

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

// 异步回调处理函数
void TFW_MsgLoopMgr::AsyncCallbackHandler(TFW_Message* msg) {
    TFW_AsyncCallbackInfo* info = nullptr;

    if (msg == nullptr) {
        TFW_LOGE_CORE("Async callback received null message");
        return;
    }

    info = (TFW_AsyncCallbackInfo*)msg->obj;
    if (info == nullptr) {
        TFW_LOGE_CORE("Async callback received null info");
        return;
    }

    if (info->callback == nullptr) {
        TFW_LOGE_CORE("Async callback function is null");
        return;
    }

    // 执行回调函数
    info->callback(info->cbPara);
}

// 释放异步回调消息
void TFW_MsgLoopMgr::FreeAsyncCallbackMessage(TFW_Message* msg) {
    TFW_AsyncCallbackInfo* info = nullptr;

    if (msg == nullptr || msg->obj == nullptr) {
        TFW_LOGE_CORE("Failed: message or callback is null");
        return;
    }

    info = (TFW_AsyncCallbackInfo*)msg->obj;
    TFW_Free(info);
}

// 初始化异步回调消息
void TFW_MsgLoopMgr::InitAsyncCallbackMessage(TFW_Message* msg, int32_t what, void* obj, TFW_Handler* handler) {
    msg->what = what;
    msg->obj = obj;
    msg->handler = handler;
    msg->FreeMessage = FreeAsyncCallbackMessage;
    msg->arg1 = 0;
    msg->arg2 = 0;
    msg->time = 0;
}

// 创建异步回调信息
TFW_AsyncCallbackInfo* TFW_MsgLoopMgr::CreateAsyncCallbackInfo(TFW_Looper* looper,
    TFW_AsyncCallbackFunc callback, void* para) {
    TFW_AsyncCallbackInfo* info = nullptr;

    info = (TFW_AsyncCallbackInfo*)TFW_Calloc(sizeof(TFW_AsyncCallbackInfo));
    if (info == nullptr) {
        TFW_LOGE_CORE("Failed to allocate memory for async callback info");
        return nullptr;
    }
    TFW_Message* msg = (TFW_Message*)TFW_Calloc(sizeof(TFW_Message));
    if (msg == nullptr) {
        TFW_LOGE_CORE("Failed to allocate memory for async callback message");
        TFW_Free(info);
        return nullptr;
    }
    info->msg = msg;
    info->callback = callback;
    info->cbPara = para;
    info->handler = asyncCallbackHandler_;
    info->handler.looper = looper;
    InitAsyncCallbackMessage(info->msg, TFW_ASYNC_CALLBACK_MSG_TYPE, (void*)info, &info->handler);

    return info;
}

// 异步回调辅助函数
int32_t TFW_MsgLoopMgr::PostAsyncCallback(TFW_LooperType type, TFW_AsyncCallbackFunc callback, void* para) {
    TFW_AsyncCallbackInfo* info = nullptr;
    TFW_Looper* looper = nullptr;

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Msg loop manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (callback == nullptr) {
        TFW_LOGE_CORE("Callback function is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    looper = TFW_GetLooper(type);
    if (looper == nullptr) {
        TFW_LOGE_CORE("Failed to get looper for type: %d", type);
        return TFW_ERROR;
    }

    info = CreateAsyncCallbackInfo(looper, callback, para);
    if (info == nullptr) {
        TFW_LOGE_CORE("Failed to create async callback info");
        return TFW_ERROR_MALLOC_ERR;
    }

    // 直接通过looper发送消息
    if (looper->PostMessage != nullptr) {
        looper->PostMessage(looper, info->msg);
    } else {
        TFW_LOGE_CORE("PostMessage function pointer is null for looper type: %d", type);
        TFW_Free(info);
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

// 延迟异步回调辅助函数
int32_t TFW_MsgLoopMgr::PostAsyncCallbackDelay(TFW_LooperType type, TFW_AsyncCallbackFunc callback,
    void* para, uint64_t delayMillis) {
    TFW_AsyncCallbackInfo* info = nullptr;
    TFW_Looper* looper = nullptr;

    if (!IsInitialized()) {
        TFW_LOGE_CORE("Msg loop manager not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    if (callback == nullptr) {
        TFW_LOGE_CORE("Callback function is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    looper = TFW_GetLooper(type);
    if (looper == nullptr) {
        TFW_LOGE_CORE("Failed to get looper for type: %d", type);
        return TFW_ERROR;
    }

    info = CreateAsyncCallbackInfo(looper, callback, para);
    if (info == nullptr) {
        TFW_LOGE_CORE("Failed to create async callback info");
        return TFW_ERROR_MALLOC_ERR;
    }

    // 通过looper发送延迟消息
    if (looper->PostMessageDelay != nullptr) {
        looper->PostMessageDelay(looper, info->msg, delayMillis);
    } else {
        TFW_LOGE_CORE("PostMessageDelay function pointer is null for looper type: %d", type);
        TFW_Free(info);
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

} // namespace TFW
