#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_c_interface.h"
#include "TFW_sdk_log.h"
#include "TFW_cli_common.h"
#include "TFW_common_defines.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    char name[TFW_NORMAL_BUFFER_SIZE_MAX];
    int32_t (*func)(void);
} CliTestFunc;

static int32_t ConfigTest(void)
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

static CliTestFunc g_cliTestFuncs[] = {
    {"config", ConfigTest},
};

#define TEST_NUM (int32_t)(sizeof(g_cliTestFuncs) / sizeof(CliTestFunc))

static void PrintMenu(void)
{
    printf("Please select a test:\n");
    for (int32_t i = 0; i < TEST_NUM; i++) {
        printf("%d. %s\n", i + 1, g_cliTestFuncs[i].name);
    }
    printf("0. Exit\n");
}

static void TestEntry(void)
{
    printf("Welcome to TFW CLI Test!\n");
    printf("This test will demonstrate basic configuration retrieval.\n");
    printf("Press Enter to continue...\n");
    printf("Please Select test mode\n");
    int32_t choice = -1;
    int32_t result = 0;
    while (1) {
        PrintMenu();
        printf("Enter your choice (0-%d): ", TEST_NUM);
        if (TFW_CliReadInteger(&choice) != TFW_SUCCESS || choice < 0 || choice > TEST_NUM) {
            printf("Invalid input. Please enter a number between 0 and %d.\n", TEST_NUM);
            continue;
        }
        if (choice == 0) {
            printf("Exiting...\n");
            break;
        }
        printf("Running test %s...\n", g_cliTestFuncs[choice - 1].name);
        result = g_cliTestFuncs[choice - 1].func();
        if (result != TFW_SUCCESS) {
            printf("Test %s failed.\n", g_cliTestFuncs[choice - 1].name);
            continue;
        }
        printf("Test %s passed.\n", g_cliTestFuncs[choice - 1].name);
    }
}

int main(int argc, char *argv[]) {
    int32_t result = TFW_SUCCESS;
    result = TFW_CoreInit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("cli test TFW_CoreInit failed");
        return result;
    }
    TestEntry();

    result = TFW_CoreDeinit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("cli test TFW_CoreDeinit failed");
        return result;
    }
    TFW_LOGI_SDK("cli test success");
    return TFW_SUCCESS;
}
