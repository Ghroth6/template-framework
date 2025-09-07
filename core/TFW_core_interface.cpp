/**
 * @file TFW_core_interface.cpp
 * @brief TFW核心模块C++接口实现（直接实现TFW::CoreInterface）
 * @author TFW Team
 * @date 2024
 */

// 1. system headers
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

// 2. project headers
#include "TFW_c_interface.h"
#include "TFW_core_interface.h"
#include "TFW_core_log.h"
#include "TFW_errorno.h"
#include "TFW_interface.h"
#include "TFW_types.h"
#include "TFW_common_defines.h"

// ============================================================================
// TFW::CoreInterface implementation
// ============================================================================

namespace TFW {

// 全局单例实例，使用unique_ptr管理
static std::unique_ptr<CoreInterface> g_coreInstance = nullptr;
static std::mutex g_coreInterfaceMutex;

CoreInterface::CoreInterface() {
    TFW_LOGI_CORE("CoreInterface constructor called");
    isInitialized_ = false;
}

CoreInterface::~CoreInterface() {
    TFW_LOGI_CORE("CoreInterface destructor called");
    if (isInitialized_) {
        TFW_LOGW_CORE("Warning: Destroying initialized instance");
    }
}

CoreInterface* CoreInterface::GetInstance() {
    std::lock_guard<std::mutex> lock(g_coreInterfaceMutex);

    if (g_coreInstance == nullptr) {
        g_coreInstance = std::unique_ptr<CoreInterface>(new CoreInterface());
    }

    // 安全检查：确保返回的指针有效
    if (g_coreInstance == nullptr) {
        TFW_LOGE_CORE("Warning: Failed to create core instance");
        return nullptr;
    }

    return g_coreInstance.get();
}

void CoreInterface::DestroyInstance() {
    std::lock_guard<std::mutex> lock(g_coreInterfaceMutex);

    if (g_coreInstance == nullptr) {
        TFW_LOGW_CORE("Core instance already destroyed or not created");
        return;
    }

    // 检查实例状态，如果已初始化，建议先反初始化
    if (g_coreInstance->IsInitialized()) {
        TFW_LOGW_CORE("Warning: Destroying initialized instance, consider calling Deinit first");
        g_coreInstance->Deinit();
    }

    g_coreInstance.reset();  // unique_ptr的reset方法
    TFW_LOGI_CORE("Core instance destroyed via C++ interface");
}

int32_t CoreInterface::Init() {
    TFW_LOGI_CORE("Initializing core...");
    if (isInitialized_) {
        TFW_LOGW_CORE("Core already initialized");
        return TFW_SUCCESS;
    }

    // TODO: 实际的初始化逻辑

    isInitialized_ = true;
    TFW_LOGI_CORE("Core initialized successfully");
    return TFW_SUCCESS;
}

int32_t CoreInterface::Deinit() {
    TFW_LOGI_CORE("DeInitializing core...");
    if (!isInitialized_) {
        TFW_LOGI_CORE("Core not initialized");
        return TFW_SUCCESS;
    }

    // TODO: 实际的清理逻辑

    isInitialized_ = false;
    TFW_LOGI_CORE("Core deInitialized successfully");
    return TFW_SUCCESS;
}

bool CoreInterface::IsInitialized() const {
    return isInitialized_;
}

}  // namespace TFW

// C接口实现
extern "C" {

int32_t TFW_CoreInterfaceInit(void) {
    TFW_LOGI_CORE("TFW_CoreInterfaceInit called");

    TFW::CoreInterface* coreInstance = TFW::CoreInterface::GetInstance();
    if (coreInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR_NOT_INIT;
    }

    // 检查是否已经初始化
    if (coreInstance->IsInitialized()) {
        TFW_LOGI_CORE("Core already initialized, returning success");
        return TFW_SUCCESS;
    }

    // 调用C++实现进行初始化
    int32_t result = coreInstance->Init();
    if (result == TFW_SUCCESS) {
        TFW_LOGI_CORE("Core initialization successful");
    } else {
        TFW_LOGE_CORE("Core initialization failed with error: %d", result);
    }

    return result;
}

int32_t TFW_CoreInterfaceDeinit(void) {
    TFW_LOGI_CORE("TFW_CoreInterfaceDeinit called");

    TFW::CoreInterface* coreInstance = TFW::CoreInterface::GetInstance();
    if (coreInstance == nullptr) {
        TFW_LOGE_CORE("Failed to get core instance");
        return TFW_ERROR;
    }

    // 调用C++实现进行反初始化
    int32_t result = coreInstance->Deinit();
    if (result == TFW_SUCCESS) {
        TFW_LOGI_CORE("Core deinitialization successful");
    } else {
        TFW_LOGE_CORE("Core deinitialization failed with error: %d", result);
    }

    // 销毁实例
    TFW::CoreInterface::DestroyInstance();

    return result;
}

}  // extern "C"
