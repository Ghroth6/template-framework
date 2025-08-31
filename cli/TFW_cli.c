#include "TFW_errorno.h"
#include "TFW_types.h"
#include "TFW_c_interface.h"
#include "TFW_sdk_log.h"

int main(int argc, char *argv[]) {
    int32_t result = TFW_SUCCESS;
    result = TFW_CoreInit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("cli test TFW_CoreInit failed");
        return result;
    }
    result = TFW_CoreDeinit();
    if (result != TFW_SUCCESS) {
        TFW_LOGE_SDK("cli test TFW_CoreDeinit failed");
        return result;
    }
    TFW_LOGI_SDK("cli test success");
    return 0;
}
