#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_c_interface.h"

int main(int argc, char *argv[]) {
    int32_t result = TFW_SUCCESS;
    result = TFW_CoreInit();
    if (result != TFW_SUCCESS) {
        printf("cli test TFW_CoreInit failed\n");
        return result;
    }
    result = TFW_CoreDeinit();
    if (result != TFW_SUCCESS) {
        printf("cli test TFW_CoreDeinit failed\n");
        return result;
    }
    printf("cli test success\n");
    return 0;
}
