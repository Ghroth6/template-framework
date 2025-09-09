#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_c_interface.h"
#include "TFW_sdk_log.h"
#include <stddef.h>
#include <stdio.h>

int32_t ConfigTest(void)
{
    int32_t result = TFW_SUCCESS;
    int32_t configNum = 0;
    TFW_ConfigItem *configArray = NULL;
    result = TFW_CoreGetAllConfigItems(&configArray, (uint32_t*)&configNum);
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("TFW_CoreGetAllConfigItem failed, result = %d", result);
        return -1;
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
    return 0;
}

int main(int argc, char *argv[]) {
    int32_t result = TFW_SUCCESS;
    result = TFW_CoreInit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("cli test TFW_CoreInit failed");
        return result;
    }
    result = ConfigTest();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("cli test ConfigTest failed");
    }

    result = TFW_CoreDeinit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("cli test TFW_CoreDeinit failed");
        return result;
    }
    TFW_LOGI_SDK("cli test success");
    return 0;
}
