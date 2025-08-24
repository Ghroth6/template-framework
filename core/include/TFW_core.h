#ifndef TFW_CORE_H
#define TFW_CORE_H

#include "../interface/TFW_errorno.h"
#include "../interface/TFW_types.h"
#include <string>
#include <cstring>

namespace TFW {



// 核心管理类
class Core {
public:
    static Core& GetInstance();

    // 初始化核心
    int32_t Initialize();

    // 退出核心
    int32_t Exit();

    // 检查核心是否已初始化
    bool IsInitialized() const;

    // 获取类型数据
    int32_t GetData() const;

    // 设置类型数据
    int32_t SetData(const int32_t data);

    // 执行一个动作
    int32_t Action(const std::string& action);

private:
    Core() = default;
    ~Core() = default;
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    bool initialized_ = false;
    TFW_Type mainType_;
};

} // namespace TFW

#endif // TFW_CORE_H
