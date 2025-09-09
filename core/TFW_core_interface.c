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
#include "TFW_core_log.h"

// ============================================================================
// public interface implementation
// ============================================================================

int32_t TFW_CoreInit(void) {
    TFW_LOGI_CORE("init core module...");

    // call C++ implementation for initialization
    int32_t result = TFW_CoreInterfaceInit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: C++ init failed");
        return result;
    }

    TFW_LOGI_CORE("core module init success");
    return TFW_SUCCESS;
}

int32_t TFW_CoreDeinit(void) {
    TFW_LOGI_CORE("deinit core module...");

    // call C++ implementation for deinitialization
    int32_t result = TFW_CoreInterfaceDeinit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: C++ deinit failed");
        return result;
    }

    TFW_LOGI_CORE("core module deinit success");
    return TFW_SUCCESS;
}

int32_t TFW_CoreGetAllConfigItems(TFW_ConfigItem **config_array, uint32_t *count) {
    TFW_LOGI_CORE("get all config items...");
    int32_t result = TFW_CoreInterfaceGetAllConfigItems(config_array, count);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: get all config items failed");
        return result;
    }
    TFW_LOGI_CORE("get all config items success, count=%u", *count);
    return TFW_SUCCESS;
}
void TFW_CoreFreeAllConfigItems(TFW_ConfigItem *config_array) {
    TFW_LOGI_CORE("free all config items...");
    TFW_CoreInterfaceFreeAllConfigItems(config_array);
}
