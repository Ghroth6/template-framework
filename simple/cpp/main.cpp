#include <iostream>
#include <string>
#include "../../core/include/TFW_core.h"
#include "../../interface/TFW_interface.h"
#include "../../core/include/TFW_core_log.h"

using namespace TFW;

int main() {
    std::cout << "=== TFW Simple C++ Example ===" << std::endl;

    try {
        // 初始化TFW核心
        std::cout << "Initializing TFW core..." << std::endl;
        TFW_Result result = Core::GetInstance().Initialize();

        if (result != TFW_SUCCESS) {
            std::cout << "Failed to initialize TFW core, error code: " << result << std::endl;
            return -1;
        }

        std::cout << "TFW core initialized successfully" << std::endl;
        TFW_LOGI_CORE("Core initialized successfully");

        // 检查初始化状态
        if (Core::GetInstance().IsInitialized()) {
            std::cout << "Core is initialized" << std::endl;
        } else {
            std::cout << "Core is not initialized" << std::endl;
            return -1;
        }

        // 使用接口执行操作
        std::cout << "Executing operation via interface..." << std::endl;
        auto interface = CoreInterface::GetInstance();
        result = interface->ExecuteOperation("test_operation");

        if (result == TFW_SUCCESS) {
            std::cout << "Operation executed successfully" << std::endl;
            TFW_LOGI_CORE("Operation executed successfully");
        } else {
            std::cout << "Operation failed, error code: " << result << std::endl;
            TFW_LOGE_CORE("Operation failed with error code: %d", result);
        }

        // 设置和获取值
        std::cout << "Setting key-value pair..." << std::endl;
        result = Core::GetInstance().SetValue("test_key", "test_value");

        if (result == TFW_SUCCESS) {
            std::cout << "Value set successfully" << std::endl;
            TFW_LOGI_CORE("Value set successfully");

            // 获取值
            std::string value;
            result = Core::GetInstance().GetValue("test_key", value);

            if (result == TFW_SUCCESS) {
                std::cout << "Retrieved value: " << value << std::endl;
                TFW_LOGI_CORE("Retrieved value: %s", value.c_str());
            } else {
                std::cout << "Failed to get value, error code: " << result << std::endl;
                TFW_LOGE_CORE("Failed to get value, error code: %d", result);
            }
        } else {
            std::cout << "Failed to set value, error code: " << result << std::endl;
            TFW_LOGE_CORE("Failed to set value, error code: %d", result);
        }

        // 退出TFW核心
        std::cout << "Exiting TFW core..." << std::endl;
        result = Core::GetInstance().Exit();

        if (result == TFW_SUCCESS) {
            std::cout << "TFW core exited successfully" << std::endl;
            TFW_LOGI_CORE("Core exited successfully");
        } else {
            std::cout << "Failed to exit TFW core, error code: " << result << std::endl;
            TFW_LOGE_CORE("Failed to exit core, error code: %d", result);
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        TFW_LOGE_CORE("Exception occurred: %s", e.what());
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        TFW_LOGE_CORE("Unknown exception occurred");
        return -1;
    }

    std::cout << "=== Example completed ===" << std::endl;
    return 0;
}
