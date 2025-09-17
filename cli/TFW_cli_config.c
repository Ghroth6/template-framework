#include "TFW_cli_config.h"
#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_c_interface.h"


static int32_t ShowAllConfig(void)
{
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

static CliTestFunc g_cliConfigTestFuncs[] = {
    {"ShowAllConfig", ShowAllConfig},
};

#define TEST_NUM (int32_t)(sizeof(g_cliConfigTestFuncs) / sizeof(CliTestFunc))

static CliTestModule g_configTestModule = {
    .name = "config",
    .description = "Configuration Test Module",
    .functions = g_cliConfigTestFuncs,
    .func_count = TEST_NUM,
};

ModuleGetterFunc GetConfigModuleInstance(void)
{
    return &g_configTestModule;
}
