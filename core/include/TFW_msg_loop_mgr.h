#ifndef TFW_MSG_LOOP_MGR_H
#define TFW_MSG_LOOP_MGR_H

#include "TFW_types.h"
#include "TFW_single_instance.h"
#include "TFW_message_loop.h"

namespace TFW {

// 定义回调函数类型
typedef void (*TFW_AsyncCallbackFunc)(void* para);

// 回调信息结构体
typedef struct {
    TFW_Message msg;
    TFW_Handler handler;
    TFW_AsyncCallbackFunc callback;
    void* cbPara;
} TFW_AsyncCallbackInfo;

class TFW_MsgLoopMgr {
    TFW_DECLARE_SINGLE_INSTANCE(TFW_MsgLoopMgr)

private:
    // 内部状态
    bool isInitialized_ = false;

    // 私有方法用于处理异步回调消息
    static void AsyncCallbackHandler(TFW_Message* msg);
    static void FreeAsyncCallbackMessage(TFW_Message* msg);
    static void InitAsyncCallbackMessage(TFW_Message* msg, int32_t what, void* obj, TFW_Handler* handler);
    static void InitAsyncCallbackHandler(TFW_Handler* handler, TFW_Looper* looper);
    TFW_AsyncCallbackInfo* CreateAsyncCallbackInfo(TFW_Looper* looper, TFW_AsyncCallbackFunc callback, void* para);

public:
    // 初始化和去初始化
    int32_t Init();
    int32_t Deinit();
    bool IsInitialized() const;

    // 获取指定类型的消息循环（保留原有接口，但建议使用下面的封装方法）
    TFW_Looper* GetLooper(TFW_LooperType type);

    // 简化的异步回调接口
    int32_t PostAsyncCallback(TFW_LooperType type = TFW_LOOP_TYPE_DEFAULT, TFW_AsyncCallbackFunc callback = nullptr, void* para = nullptr);
    int32_t PostAsyncCallbackDelay(TFW_LooperType type = TFW_LOOP_TYPE_DEFAULT, TFW_AsyncCallbackFunc callback = nullptr, void* para = nullptr, uint64_t delayMillis = 0);
};

} // namespace TFW

#endif // TFW_MSG_LOOP_MGR_H
