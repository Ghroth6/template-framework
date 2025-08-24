#include <iostream>
#include <string>
#include "../../interface/TFW_interface.h"
#include "../../interface/TFW_errorno.h"

using namespace TFW;

int main() {
    std::cout << "=== TFW Simple C++ Example ===" << std::endl;

    try {
        // 初始化TFW核心
        std::cout << "Initializing TFW core..." << std::endl;
        auto interface = CoreInterface::GetInstance();
        int32_t result = interface->Initialize();

        if (result != TFW_SUCCESS) {
            std::cout << "Failed to initialize TFW core, error code: " << result << std::endl;
            return -1;
        }

        std::cout << "TFW core initialized successfully" << std::endl;

        // 使用接口执行操作
        std::cout << "Executing operation via interface..." << std::endl;
        result = interface->ExecuteOperation("test_operation");

        if (result == TFW_SUCCESS) {
            std::cout << "Operation executed successfully" << std::endl;
        } else {
            std::cout << "Operation failed, error code: " << result << std::endl;
        }

        // 测试类型数据操作
        std::cout << "Testing type data operations..." << std::endl;

        // 设置类型数据
        result = interface->SetData(100);
        if (result == TFW_SUCCESS) {
            std::cout << "Type data set successfully" << std::endl;
        } else {
            std::cout << "Failed to set type data, error code: " << result << std::endl;
        }

        // 获取类型数据
        int32_t dataValue = interface->GetData();
        std::cout << "Retrieved type data: " << dataValue << std::endl;

        // 退出TFW核心
        std::cout << "Exiting TFW core..." << std::endl;
        result = interface->Exit();

        if (result == TFW_SUCCESS) {
            std::cout << "TFW core exited successfully" << std::endl;
        } else {
            std::cout << "Failed to exit TFW core, error code: " << result << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return -1;
    }

    std::cout << "=== Example completed ===" << std::endl;

    // Windows下暂停，等待用户按键
#ifdef _WIN32
    std::cout << "\n按任意键继续..." << std::endl;
    system("pause");
#else
    std::cout << "\n按回车键继续..." << std::endl;
    std::cin.get();
#endif

    return 0;
}
