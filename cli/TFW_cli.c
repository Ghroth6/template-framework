#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_c_interface.h"
#include "TFW_cli_common.h"
#include "TFW_cli_config.h"
#include <stddef.h>
#include <stdio.h>



static ModuleGetterFunc g_cliTestModule[] = {
    GetConfigModuleInstance,
};
#define CLI_MODULE_NUM (int32_t)(sizeof(g_cliTestModule) / sizeof(ModuleGetterFunc))

static void PrintModule(void)
{
    printf("Select cli test module:\n");
    for (int32_t i = 0; i < CLI_MODULE_NUM; i++) {
        CliTestModule* module = g_cliTestModule[i]();
        printf("%d. %s\n", i + 1, module->name);
        printf("    %s\n", module->description);
    }
    printf("0. Exit\n");
}

static void HandleModuleFunction(int32_t moduleIndex) {
    if (moduleIndex < 0 || moduleIndex >= CLI_MODULE_NUM) {
        printf("Invalid module index, %d\n", moduleIndex);
        return;
    }
    int32_t choice = -1;
    CliTestModule* module = g_cliTestModule[moduleIndex]();
    if (module == NULL || module->functions == NULL || module->func_count <= 0) {
        printf("No functions available in module %s\n", module->name);
        return;
    }
    while (1) {
        printf("Select function:\n");
        for (int32_t i = 0; i < module->func_count; i++) {
            printf("%d. %s\n", i + 1, module->functions[i].name);
        }
        printf("0. Back to module selection\n");
        printf("Enter your choice (0-%d): ", module->func_count);
        if(TFW_CliReadInteger(&choice) != TFW_SUCCESS || choice < 0 || choice > module->func_count) {
            printf("Invalid input. Please enter a number between 0 and %d.\n", module->func_count);
            continue;
        }
        if (choice == 0) {
            break;
        }
        printf("Executing function %s...\n", module->functions[choice - 1].name);
        int32_t result = module->functions[choice - 1].func();
        printf("Function %s executed with result: %d\n", module->functions[choice - 1].name, result);
    }
    printf("Back to module selection\n");
    return;
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
        PrintModule();
        printf("Enter your choice (0-%d): ", CLI_MODULE_NUM);
        if (TFW_CliReadInteger(&choice) != TFW_SUCCESS || choice < 0 || choice > CLI_MODULE_NUM) {
            printf("Invalid input. Please enter a number between 0 and %d.\n", CLI_MODULE_NUM);
            continue;
        }
        if (choice == 0) {
            printf("Exiting...\n");
            break;
        }
        HandleModuleFunction(choice - 1);
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
