#include <stdio.h>
#include <stdlib.h>
#include "../../interface/TFW_c_interface.h"
#include "../../interface/TFW_errorno.h"

int main() {
    printf("=== TFW Simple C Example ===\n");

    // 初始化TFW核心
    printf("Initializing TFW core...\n");
    int result = TFW_CORE_INITIALIZE();

    if (result != TFW_SUCCESS) {
        printf("Failed to initialize TFW core, error code: %d\n", result);
        return -1;
    }

    printf("TFW core initialized successfully\n");

    // 检查初始化状态
    int isInit = TFW_CORE_IS_INITIALIZED();
    if (isInit) {
        printf("Core is initialized\n");
    } else {
        printf("Core is not initialized\n");
        return -1;
    }

    // 执行简单操作
    printf("Executing simple action...\n");
    result = TFW_CORE_ACTION("test_action");

    if (result == TFW_SUCCESS) {
        printf("Action executed successfully\n");
    } else {
        printf("Action failed, error code: %d\n", result);
    }

    // 设置和获取值
    printf("Setting key-value pair...\n");
    result = TFW_CORE_SET_VALUE("test_key", "test_value");

    if (result == TFW_SUCCESS) {
        printf("Value set successfully\n");

        // 获取值
        char value[256];
        result = TFW_CORE_GET_VALUE("test_key", value, sizeof(value));

        if (result == TFW_SUCCESS) {
            printf("Retrieved value: %s\n", value);
        } else {
            printf("Failed to get value, error code: %d\n", result);
        }
    } else {
        printf("Failed to set value, error code: %d\n", result);
    }

    // 退出TFW核心
    printf("Exiting TFW core...\n");
    result = TFW_CORE_EXIT();

    if (result == TFW_SUCCESS) {
        printf("TFW core exited successfully\n");
    } else {
        printf("Failed to exit TFW core, error code: %d\n", result);
    }

    printf("=== Example completed ===\n");
    return 0;
}
