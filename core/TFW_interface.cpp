#include "../interface/TFW_interface.h"
#include "include/TFW_core.h"

namespace TFW {

// ============================================================================
// CoreInterface 实现
// ============================================================================

CoreInterface* CoreInterface::GetInstance() {
    static CoreInterface instance;
    return &instance;
}

int32_t CoreInterface::Initialize() {
    return static_cast<int32_t>(TFW::Core::GetInstance().Initialize());
}

int32_t CoreInterface::Exit() {
    return static_cast<int32_t>(TFW::Core::GetInstance().Exit());
}

int32_t CoreInterface::ExecuteOperation(const std::string& operation) {
    return static_cast<int32_t>(TFW::Core::GetInstance().Action(operation));
}

int32_t CoreInterface::GetData() {
    return static_cast<int32_t>(TFW::Core::GetInstance().GetData());
}

int32_t CoreInterface::SetData(const int32_t data) {
    return static_cast<int32_t>(TFW::Core::GetInstance().SetData(data));
}


} // namespace TFW
