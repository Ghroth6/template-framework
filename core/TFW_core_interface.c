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

int32_t TFW_CoreGetIntConfigValue(TFW_ConfigKey key, int32_t *value) {
    TFW_LOGI_CORE("get int config value for key %d...", key);
    if (value == NULL) {
        TFW_LOGE_CORE("error: invalid parameter");
        return TFW_ERROR_INVALID_PARAM;
    }
    int32_t result = TFW_CoreInterfaceGetIntConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: get config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreSetIntConfigValue(TFW_ConfigKey key, int32_t value) {
    TFW_LOGI_CORE("set int config value for key %d...", key);
    int32_t result = TFW_CoreInterfaceSetIntConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: set config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreGetBoolConfigValue(TFW_ConfigKey key, bool *value) {
    TFW_LOGI_CORE("get bool config value for key %d...", key);
    if (value == NULL) {
        TFW_LOGE_CORE("error: invalid parameter");
        return TFW_ERROR_INVALID_PARAM;
    }
    int32_t result = TFW_CoreInterfaceGetBoolConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: get config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreSetBoolConfigValue(TFW_ConfigKey key, bool value) {
    TFW_LOGI_CORE("set bool config value for key %d...", key);
    int32_t result = TFW_CoreInterfaceSetBoolConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: set config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreGetFloatConfigValue(TFW_ConfigKey key, float *value) {
    TFW_LOGI_CORE("get float config value for key %d...", key);
    if (value == NULL) {
        TFW_LOGE_CORE("error: invalid parameter");
        return TFW_ERROR_INVALID_PARAM;
    }
    int32_t result = TFW_CoreInterfaceGetFloatConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: get config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreSetFloatConfigValue(TFW_ConfigKey key, float value) {
    TFW_LOGI_CORE("set float config value for key %d...", key);
    int32_t result = TFW_CoreInterfaceSetFloatConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: set config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreGetStringConfigValue(TFW_ConfigKey key, const char **value) {
    TFW_LOGI_CORE("get string config value for key %d...", key);
    if (value == NULL) {
        TFW_LOGE_CORE("error: invalid parameter");
        return TFW_ERROR_INVALID_PARAM;
    }
    int32_t result = TFW_CoreInterfaceGetStringConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: get config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreSetStringConfigValue(TFW_ConfigKey key, const char *value) {
    TFW_LOGI_CORE("set string config value for key %d...", key);
    if (value == NULL) {
        TFW_LOGE_CORE("error: invalid parameter");
        return TFW_ERROR_INVALID_PARAM;
    }
    int32_t result = TFW_CoreInterfaceSetStringConfigValue(key, value);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: set config item failed");
        return result;
    }
    return TFW_SUCCESS;
}

int32_t TFW_CoreGetConfigValueTypeByKey(TFW_ConfigKey key, TFW_ConfigValueType *type) {
    TFW_LOGI_CORE("get config value type for key %d...", key);
    if (type == NULL) {
        TFW_LOGE_CORE("error: invalid parameter");
        return TFW_ERROR_INVALID_PARAM;
    }
    int32_t result = TFW_CoreInterfaceGetConfigValueTypeByKey(key, type);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_CORE("error: get config item type failed");
        return result;
    }
    return TFW_SUCCESS;
}
