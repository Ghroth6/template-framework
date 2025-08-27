#include "../include/TFW_core.h"
#include "../interface/TFW_interface.h"
#include "../include/TFW_core_log.h"
#include "../../utils/include/TFW_config.h"
#include <string>
#include <cstring>

namespace TFW {

Core& Core::GetInstance() {
    static Core instance;
    return instance;
}

int32_t Core::Initialize() {
    TFW_LOGI_CORE("Initializing core");
    if (initialized_) {
        return TFW_ERROR_ALREADY_INIT;
    }

    try {
        // 设置配置路径
        configPath_ = "./runtime";
        
        // 初始化配置管理器
        int32_t configResult = TFW_Config_Initialize(configPath_.c_str());
        if (configResult != TFW_SUCCESS) {
            TFW_LOGE_CORE("Failed to initialize config manager: %d", configResult);
            // 继续运行，使用默认配置
        }
        
        // 自动加载配置
        int32_t loadResult = LoadConfigs();
        if (loadResult != TFW_SUCCESS) {
            TFW_LOGE_CORE("Failed to load configs: %d", loadResult);
            // 继续运行，使用默认配置
        }
        
        // 初始化主类型
        mainType_.id = 1;
        SetData(1);
        
        initialized_ = true;
        TFW_LOGI_CORE("Core initialized successfully");
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during core initialization");
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t Core::Exit() {
    TFW_LOGI_CORE("Exiting core");
    if (!initialized_) {
        return TFW_ERROR_NOT_INIT;
    }

    try {
        // 保存所有配置
        TFW_Config_Exit();

        // 清理主类型
        mainType_.id = 0;
        mainType_.data = 0;
        initialized_ = false;
        TFW_LOGI_CORE("Core exited successfully");
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during core exit");
        return TFW_ERROR_OPERATION_FAIL;
    }
}

bool Core::IsInitialized() const {
    return initialized_;
}

int32_t Core::Action(const std::string& action) {
    if (!initialized_) {
        TFW_LOGE_CORE("Core not initialized");
        return TFW_ERROR_NOT_INIT;
    }

    try {
        TFW_LOGI_CORE("Executing action: %s", action.c_str());
        TFW_LOGI_CORE("Action executed successfully: %s", action.c_str());
        return TFW_SUCCESS;
    } catch (...) {
        TFW_LOGE_CORE("Exception during action execution: %s", action.c_str());
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t Core::GetData() const {
    return mainType_.data;
}

int32_t Core::SetData(const int32_t data) {
    mainType_.data = data;
    return TFW_SUCCESS;
}

// 配置管理接口实现
std::string Core::GetConfigValue(const std::string& key, const std::string& defaultValue) {
    if (!initialized_) {
        return defaultValue;
    }
    
    // 将字符串key转换为枚举key（简化实现）
    // 这里可以根据需要扩展
    if (key == "main.version") {
        char value[512];
        int32_t result = TFW_Config_GetString(TFW_CONFIG_MAIN_VERSION, value, sizeof(value), defaultValue.c_str());
        if (result == TFW_SUCCESS) {
            return std::string(value);
        }
    }
    
    return defaultValue;
}

int32_t Core::SetConfigValue(const std::string& key, const std::string& value) {
    if (!initialized_) {
        return TFW_ERROR_NOT_INIT;
    }
    
    // 将字符串key转换为枚举key（简化实现）
    if (key == "main.version") {
        return TFW_Config_SetString(TFW_CONFIG_MAIN_VERSION, value.c_str());
    }
    
    return TFW_ERROR_INVALID_PARAM;
}

ConfigStatus Core::GetConfigStatus() const {
    ConfigStatus status;
    
    if (initialized_) {
        TFW_ConfigStatus cStatus;
        if (TFW_Config_GetStatus(&cStatus) == TFW_SUCCESS) {
            status = ConfigStatus(cStatus);
        }
    }
    
    return status;
}

// 私有配置管理函数实现
int32_t Core::LoadConfigs() {
    // 现在只有一个配置文件，直接加载
    int32_t result = LoadConfig("");
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("Failed to load config");
        // 尝试创建默认配置
        CreateDefaultConfig("");
    }
    
    return TFW_SUCCESS;
}

int32_t Core::LoadConfig(const std::string& configName) {
    return TFW_Config_LoadConfig();
}

int32_t Core::SaveConfig(const std::string& configName) {
    return TFW_Config_SaveConfig();
}

int32_t Core::CreateDefaultConfig(const std::string& configName) {
    return TFW_Config_CreateDefaultConfig();
}

} // namespace TFW
