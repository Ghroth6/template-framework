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

    // 执行简单操作
    printf("Executing simple action...\n");
    result = TFW_CORE_ACTION("test_action");

    if (result == TFW_SUCCESS) {
        printf("Action executed successfully\n");
    } else {
        printf("Action failed, error code: %d\n", result);
    }

    // 测试类型数据操作（通过接口层）
    printf("Testing type data operations...\n");
    printf("Note: GetValue and SetValue methods have been removed\n");
    printf("Type data operations are now handled directly in Core class\n");

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
