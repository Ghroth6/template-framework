#ifndef TFW_CORE_H
#define TFW_CORE_H

#include "../interface/TFW_errorno.h"
#include "../interface/TFW_types.h"
#include <string>
#include <unordered_map>
#include <cstring>

namespace TFW {

// 基础类型类，使用TFW_Type结构体的C++封装
class Type {
public:
    Type() : type_{0, nullptr, nullptr} {}
    ~Type() {
        if (type_.name) {
            delete[] type_.name;
        }
    }

    bool Initialize() {
        type_.id = 1;
        SetName("default_type");
        return true;
    }

    void Cleanup() {
        type_.id = 0;
        type_.data = nullptr;
        if (type_.name) {
            delete[] type_.name;
            type_.name = nullptr;
        }
    }

    int Execute() {
        return 0;
    }

    void SetName(const char* name) {
        if (type_.name) {
            delete[] type_.name;
        }
        if (name) {
            type_.name = new char[strlen(name) + 1];
            strcpy(type_.name, name);
        } else {
            type_.name = nullptr;
        }
    }

private:
    TFW_Type type_;
};

// 核心管理类
class Core {
public:
    static Core& GetInstance();

    // 初始化核心
    ErrorCode Initialize();

    // 退出核心
    ErrorCode Exit();

    // 检查核心是否已初始化
    bool IsInitialized() const;

    // 获取一个值
    ErrorCode GetValue(const std::string& key, std::string& value);

    // 设置一个值
    ErrorCode SetValue(const std::string& key, const std::string& value);

    // 执行一个动作
    ErrorCode Action(const std::string& action);

private:
    Core() = default;
    ~Core() = default;
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    bool initialized_ = false;
    Type mainType_;
    std::unordered_map<std::string, std::string> keyValueStore_;
};

} // namespace TFW

#endif // TFW_CORE_H
