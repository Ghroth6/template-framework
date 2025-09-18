#include "TFW_cli_config.h"
#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_c_interface.h"


static int32_t ShowAllConfig(void) {
    int32_t result = TFW_SUCCESS;
    int32_t configNum = 0;
    TFW_ConfigItem *configArray = NULL;
    result = TFW_CoreGetAllConfigItems(&configArray, (uint32_t*)&configNum);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("TFW_CoreGetAllConfigItem failed, result = %d", result);
        return TFW_ERROR;
    }
    printf("config num = %d\n", configNum);
    for (int32_t i = 0; i < configNum; i++) {
        printf("config[%d]: key=%d, type=%d, value=", i, configArray[i].key, configArray[i].type);
        switch (configArray[i].type) {
            case TFW_CONFIG_TYPE_STRING:
                printf("%s", configArray[i].value.string_value);
                break;
            case TFW_CONFIG_TYPE_INT:
                printf("%d", configArray[i].value.int_value);
                break;
            case TFW_CONFIG_TYPE_BOOL:
                printf("%s", configArray[i].value.bool_value ? "true" : "false");
                break;
            case TFW_CONFIG_TYPE_FLOAT:
                printf("%f", configArray[i].value.float_value);
                break;
            default:
                break;
        }
        printf("\n");
    }
    TFW_CoreFreeAllConfigItems(configArray);
    return TFW_SUCCESS;
}

static int32_t GetSpecificConfig(void) {
    int32_t result = TFW_SUCCESS;
    int32_t keyInput;
    TFW_ConfigValueType type;

    printf("Enter config key (0-%d): ", TFW_CONFIG_KEY_COUNT - 1);
    result = TFW_CliReadInteger(&keyInput);
    if (result != TFW_SUCCESS || keyInput < 0 || keyInput >= TFW_CONFIG_KEY_COUNT) {
        printf("Error: Invalid config key\n");
        return TFW_ERROR_INVALID_PARAM;
    }

    TFW_ConfigKey key = (TFW_ConfigKey)keyInput;

    // 获取配置项类型
    result = TFW_CoreGetConfigValueTypeByKey(key, &type);
    if (result != TFW_SUCCESS) {
        printf("Error: Failed to get config type for key %d\n", key);
        return result;
    }

    printf("Config key %d has type %d, value: ", key, type);

    // 根据类型调用对应的获取接口
    switch (type) {
        case TFW_CONFIG_TYPE_INT: {
            int32_t intValue;
            result = TFW_CoreGetIntConfigValue(key, &intValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to get integer config value\n");
                return result;
            }
            printf("%d\n", intValue);
            break;
        }

        case TFW_CONFIG_TYPE_FLOAT: {
            float floatValue;
            result = TFW_CoreGetFloatConfigValue(key, &floatValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to get float config value\n");
                return result;
            }
            printf("%f\n", floatValue);
            break;
        }

        case TFW_CONFIG_TYPE_BOOL: {
            bool boolValue;
            result = TFW_CoreGetBoolConfigValue(key, &boolValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to get boolean config value\n");
                return result;
            }
            printf("%s\n", boolValue ? "true" : "false");
            break;
        }

        case TFW_CONFIG_TYPE_STRING: {
            const char* stringValue;
            result = TFW_CoreGetStringConfigValue(key, &stringValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to get string config value\n");
                return result;
            }
            printf("%s\n", stringValue ? stringValue : "(null)");
            // 注意：根据接口文档，stringValue是内部数据的引用，不应释放内存
            break;
        }

        default:
            printf("Error: Unsupported config type %d\n", type);
            return TFW_ERROR_INVALID_PARAM;
    }

    return TFW_SUCCESS;
}

static int32_t SetSpecificConfig(void) {
    int32_t result = TFW_SUCCESS;
    int32_t keyInput;
    TFW_ConfigValueType type;

    printf("Enter config key (0-%d): ", TFW_CONFIG_KEY_COUNT - 1);
    result = TFW_CliReadInteger(&keyInput);
    if (result != TFW_SUCCESS || keyInput < 0 || keyInput >= TFW_CONFIG_KEY_COUNT) {
        printf("Error: Invalid config key\n");
        return TFW_ERROR_INVALID_PARAM;
    }

    TFW_ConfigKey key = (TFW_ConfigKey)keyInput;

    // 使用TFW_CoreGetConfigValueTypeByKey获取配置项类型，而不是获取所有配置
    result = TFW_CoreGetConfigValueTypeByKey(key, &type);
    if (result != TFW_SUCCESS) {
        printf("Error: Failed to get config type for key %d\n", key);
        return result;
    }

    printf("Config key %d has type %d\n", key, type);

    // 根据类型调用对应的设置接口
    switch (type) {
        case TFW_CONFIG_TYPE_INT: {
            int32_t intValue;
            printf("Enter integer value: ");
            result = TFW_CliReadInteger(&intValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to read integer value\n");
                return result;
            }
            result = TFW_CoreSetIntConfigValue(key, intValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to set integer config value\n");
                return result;
            }
            printf("Successfully set integer config value to %d\n", intValue);
            break;
        }

        case TFW_CONFIG_TYPE_FLOAT: {
            float floatValue;
            printf("Enter float value: ");
            result = TFW_CliReadFloat(&floatValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to read float value\n");
                return result;
            }
            result = TFW_CoreSetFloatConfigValue(key, floatValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to set float config value\n");
                return result;
            }
            printf("Successfully set float config value to %f\n", floatValue);
            break;
        }

        case TFW_CONFIG_TYPE_BOOL: {
            int32_t boolInput;
            printf("Enter boolean value (0 for false, 1 for true): ");
            result = TFW_CliReadInteger(&boolInput);
            if (result != TFW_SUCCESS || (boolInput != 0 && boolInput != 1)) {
                printf("Error: Failed to read boolean value, must be 0 or 1\n");
                return TFW_ERROR_INVALID_PARAM;
            }
            bool boolValue = (boolInput != 0);
            result = TFW_CoreSetBoolConfigValue(key, boolValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to set boolean config value\n");
                return result;
            }
            printf("Successfully set boolean config value to %s\n", boolValue ? "true" : "false");
            break;
        }

        case TFW_CONFIG_TYPE_STRING: {
            char stringValue[256];
            printf("Enter string value: ");
            result = TFW_CliReadString(stringValue, sizeof(stringValue));
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to read string value\n");
                return result;
            }
            result = TFW_CoreSetStringConfigValue(key, stringValue);
            if (result != TFW_SUCCESS) {
                printf("Error: Failed to set string config value\n");
                return result;
            }
            printf("Successfully set string config value to %s\n", stringValue);
            break;
        }

        default:
            printf("Error: Unsupported config type %d\n", type);
            return TFW_ERROR_INVALID_PARAM;
    }

    return TFW_SUCCESS;
}

static CliTestFunc g_cliConfigTestFuncs[] = {
    {"ShowAllConfig", ShowAllConfig},
    {"GetSpecificConfig", GetSpecificConfig},
    {"SetSpecificConfig", SetSpecificConfig},
};

#define TEST_NUM (int32_t)(sizeof(g_cliConfigTestFuncs) / sizeof(CliTestFunc))

static CliTestModule g_configTestModule = {
    .name = "config",
    .description = "Configuration Test Module",
    .functions = g_cliConfigTestFuncs,
    .func_count = TEST_NUM,
};

CliTestModule* GetConfigModuleInstance(void) {
    return &g_configTestModule;
}
