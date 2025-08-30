/**
 * @file TFW_core_interface.cpp
 * @brief TFW核心模块C++接口实现（直接实现TFW::CoreInterface）
 * @author TFW Team
 * @date 2024
 */

// 1. system headers
#include <iostream>
#include <string>
#include <memory>
#include <mutex>

// 2. project headers
#include "TFW_interface.h"
#include "TFW_c_interface.h"
#include "TFW_types.h"
#include "TFW_errorno.h"
#include "TFW_core_interface.h"

// ============================================================================
// TFW::CoreInterface implementation
// ============================================================================

namespace TFW {

// 全局单例实例，使用unique_ptr管理
static std::unique_ptr<CoreInterface> g_coreInstance = nullptr;
static std::mutex g_coreInterfaceMutex;

CoreInterface::CoreInterface() {
    std::cout << "[TFW_CORE_CPP] CoreInterface constructor called" << std::endl;
    isInitialized_ = false;
}

CoreInterface::~CoreInterface() {
    std::cout << "[TFW_CORE_CPP] CoreInterface destructor called" << std::endl;
    if (isInitialized_) {
        std::cout << "[TFW_CORE_CPP] Warning: Destroying initialized instance" << std::endl;
    }
}

CoreInterface* CoreInterface::GetInstance() {
    std::lock_guard<std::mutex> lock(g_coreInterfaceMutex);

    if (g_coreInstance == nullptr) {
        g_coreInstance = std::unique_ptr<CoreInterface>(new CoreInterface());
    }

    // 安全检查：确保返回的指针有效
    if (g_coreInstance == nullptr) {
        std::cerr << "[TFW_CORE_CPP] Warning: Failed to create core instance" << std::endl;
        return nullptr;
    }

    return g_coreInstance.get();
}

void CoreInterface::DestroyInstance() {
    std::lock_guard<std::mutex> lock(g_coreInterfaceMutex);

    if (g_coreInstance == nullptr) {
        std::cout << "[TFW_CORE_CPP] Core instance already destroyed or not created" << std::endl;
        return;
    }

    // 检查实例状态，如果已初始化，建议先反初始化
    if (g_coreInstance->IsInitialized()) {
        std::cout << "[TFW_CORE_CPP] Warning: Destroying initialized instance, consider calling Deinit first" << std::endl;
        g_coreInstance->Deinit();
    }

    g_coreInstance.reset();  // unique_ptr的reset方法
    std::cout << "[TFW_CORE_CPP] Core instance destroyed via C++ interface" << std::endl;
}

int32_t CoreInterface::Init() {
    if (isInitialized_) {
        std::cout << "[TFW_CORE_CPP] Core already initialized" << std::endl;
        return TFW_SUCCESS;
    }

    std::cout << "[TFW_CORE_CPP] Initializing core..." << std::endl;
    // TODO: 实际的初始化逻辑
    isInitialized_ = true;
    std::cout << "[TFW_CORE_CPP] Core initialized successfully" << std::endl;
    return TFW_SUCCESS;
}

int32_t CoreInterface::Deinit() {
    if (!isInitialized_) {
        std::cout << "[TFW_CORE_CPP] Core not initialized" << std::endl;
        return TFW_SUCCESS;
    }

    std::cout << "[TFW_CORE_CPP] Deinitializing core..." << std::endl;
    // TODO: 实际的清理逻辑
    isInitialized_ = false;
    std::cout << "[TFW_CORE_CPP] Core deinitialized successfully" << std::endl;
    return TFW_SUCCESS;
}

bool CoreInterface::IsInitialized() const {
    return isInitialized_;
}

} // namespace TFW

// C接口实现
extern "C" {

int32_t TFW_CoreInterfaceInit(void) {
    std::cout << "[TFW_CORE_C] TFW_CoreInterfaceInit called" << std::endl;

    TFW::CoreInterface* coreInstance = TFW::CoreInterface::GetInstance();
    if (coreInstance == nullptr) {
        std::cerr << "[TFW_CORE_C] Failed to get core instance" << std::endl;
        return TFW_ERROR_NOT_INIT;
    }

    // 检查是否已经初始化
    if (coreInstance->IsInitialized()) {
        std::cout << "[TFW_CORE_C] Core already initialized, returning success" << std::endl;
        return TFW_SUCCESS;
    }

    // 调用C++实现进行初始化
    int32_t result = coreInstance->Init();
    if (result == TFW_SUCCESS) {
        std::cout << "[TFW_CORE_C] Core initialization successful" << std::endl;
    } else {
        std::cerr << "[TFW_CORE_C] Core initialization failed with error: " << result << std::endl;
    }

    return result;
}

int32_t TFW_CoreInterfaceDeinit(void) {
    std::cout << "[TFW_CORE_C] TFW_CoreInterfaceDeinit called" << std::endl;

    TFW::CoreInterface* coreInstance = TFW::CoreInterface::GetInstance();
    if (coreInstance == nullptr) {
        std::cerr << "[TFW_CORE_C] Failed to get core instance" << std::endl;
        return TFW_ERROR;
    }

    // 调用C++实现进行反初始化
    int32_t result = coreInstance->Deinit();
    if (result == TFW_SUCCESS) {
        std::cout << "[TFW_CORE_C] Core deinitialization successful" << std::endl;
    } else {
        std::cerr << "[TFW_CORE_C] Core deinitialization failed with error: " << result << std::endl;
    }

    // 销毁实例
    TFW::CoreInterface::DestroyInstance();

    return result;
}

} // extern "C"
