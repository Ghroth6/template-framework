#ifndef TFW_MSG_LOOP_MGR_H
#define TFW_MSG_LOOP_MGR_H

#include "TFW_types.h"
#include "TFW_single_instance.h"
#include "TFW_message_loop.h"

namespace TFW {

class TFW_MsgLoopMgr {
    TFW_DECLARE_SINGLE_INSTANCE(TFW_MsgLoopMgr)

private:
    // 内部状态
    bool isInitialized_ = false;

public:
    // 初始化和去初始化
    int32_t Init();
    int32_t Deinit();
    bool IsInitialized() const;

    // 获取指定类型的消息循环
    TFW_Looper* GetLooper(TFW_LooperType type);
};

} // namespace TFW

#endif // TFW_MSG_LOOP_MGR_H
