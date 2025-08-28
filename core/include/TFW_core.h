#ifndef TFW_CORE_H
#define TFW_CORE_H

#include "../interface/TFW_errorno.h"
#include "../interface/TFW_types.h"
#include "../../utils/include/TFW_config.h"
#include "../../utils/include/TFW_common_defines.h"
#include <string>
#include <cstring>
#include <vector> // Added for std::vector

namespace TFW {

// 配置状态结构（C++包装）
struct ConfigStatus {
    bool loaded;
    uint64_t lastLoadTime;
    uint64_t lastSaveTime;
    size_t totalKeys;
    std::vector<std::string> loadedConfigs;
    
    ConfigStatus() : loaded(false), lastLoadTime(0), lastSaveTime(0), totalKeys(0) {}
    
    // 从C结构转换
    ConfigStatus(const TFW_ConfigStatus& cStatus) {
        loaded = cStatus.loaded;
        lastLoadTime = cStatus.lastLoadTime;
        lastSaveTime = cStatus.lastSaveTime;
        totalKeys = cStatus.totalKeys;
        
        // 不再有多个配置文件，只有一个JSON文件
        loadedConfigs.push_back(TFW_CONFIG_FILE_PATH);
    }
};

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

    // 配置管理接口
    std::string GetConfigValue(const std::string& key, const std::string& defaultValue = "");
    int32_t SetConfigValue(const std::string& key, const std::string& value);
    ConfigStatus GetConfigStatus() const;

private:
    Core() = default;
    ~Core() = default;
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    // 私有配置管理函数
    int32_t LoadConfigs();
    int32_t LoadConfig(const std::string& configName);
    int32_t SaveConfig(const std::string& configName);
    int32_t CreateDefaultConfig(const std::string& configName);

    bool initialized_ = false;
    TFW_Type mainType_;
    std::string configPath_;
};

} // namespace TFW

#endif // TFW_CORE_H
