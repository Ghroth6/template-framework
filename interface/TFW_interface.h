#ifndef TFW_INTERFACE_H
#define TFW_INTERFACE_H

#include "TFW_errorno.h"
#include "TFW_types.h"
#include <string>

namespace TFW {

// 核心接口类
class CoreInterface {
public:
    static CoreInterface* GetInstance();

    // 初始化接口
    int32_t Initialize();

    // 退出接口
    int32_t Exit();

    // 执行操作接口
    int32_t ExecuteOperation(const std::string& operation);

    int32_t GetData();

    int32_t SetData(const int32_t data);

private:
    CoreInterface() = default;
    ~CoreInterface() = default;
    CoreInterface(const CoreInterface&) = delete;
    CoreInterface& operator=(const CoreInterface&) = delete;
};



} // namespace TFW

#endif // TFW_INTERFACE_H
