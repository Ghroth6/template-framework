/**
 * @file TFW_core_interface.c
 * @brief TFW Core module C interface implementation (calling C++ implementation)
 * @author TFW Team
 * @date 2024
 */

// 1. system headers
#include <stdio.h>
#include <stdlib.h>

// 2. project headers
#include "TFW_c_interface.h"
#include "TFW_types.h"
#include "TFW_errorno.h"
#include "TFW_core_interface.h"

// ============================================================================
// public interface implementation
// ============================================================================

int32_t TFW_CoreInit(void) {
    printf("[TFW_CORE_C] 初始化核心模块...\n");

    // call C++ implementation for initialization
    int32_t result = TFW_CoreInterfaceInit();
    if (result != TFW_SUCCESS) {
        printf("[TFW_CORE_C] 错误：C++初始化失败\n");
        return result;
    }

    printf("[TFW_CORE_C] 核心模块初始化成功\n");
    return TFW_SUCCESS;
}

int32_t TFW_CoreDeinit(void) {
    printf("[TFW_CORE_C] 反初始化核心模块...\n");

    // call C++ implementation for deinitialization
    int32_t result = TFW_CoreInterfaceDeinit();
    if (result != TFW_SUCCESS) {
        printf("[TFW_CORE_C] 错误：C++反初始化失败\n");
        return result;
    }

    printf("[TFW_CORE_C] 核心模块反初始化成功\n");
    return TFW_SUCCESS;
}
