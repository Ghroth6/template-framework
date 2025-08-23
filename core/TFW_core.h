#ifndef TFW_CORE_H
#define TFW_CORE_H

#include "../interface/TFW_errorno.h"
#include "../interface/TFW_types.h"
#include <string>
#include <unordered_map>

namespace TFW {

// 鍩虹绫诲瀷绫伙紝浣跨敤TFW_Type缁撴瀯浣撶殑C++鍖呰
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

// 鏍稿績绠＄悊绫?
class Core {
public:
    static Core& GetInstance();

    // 鍒濆鍖栨牳蹇?
    ErrorCode Initialize();

    // 閫€鍑烘牳蹇?
    ErrorCode Exit();

    // 妫€鏌ユ牳蹇冩槸鍚﹀凡鍒濆鍖?
    bool IsInitialized() const;

    // 鑾峰彇涓€涓€?
    ErrorCode GetValue(const std::string& key, std::string& value);

    // 璁剧疆涓€涓€?
    ErrorCode SetValue(const std::string& key, const std::string& value);

    // 鎵ц涓€涓姩浣?
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
