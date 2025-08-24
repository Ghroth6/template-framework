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
    TFW_Result Initialize();

    // 退出接口
    TFW_Result Exit();

    // 检查状态接口
    bool IsInitialized() const;

    // 执行操作接口
    TFW_Result ExecuteOperation(const std::string& operation);

    // 获取值接口
    TFW_Result GetValue(const std::string& key, std::string& value);

    // 设置值接口
    TFW_Result SetValue(const std::string& key, const std::string& value);

private:
    CoreInterface() = default;
    ~CoreInterface() = default;
    CoreInterface(const CoreInterface&) = delete;
    CoreInterface& operator=(const CoreInterface&) = delete;

    bool initialized_ = false;
    TFW_Type mainType_;
};



} // namespace TFW

#endif // TFW_INTERFACE_H
