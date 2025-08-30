/**
 * @file TFW_interface.h
 * @brief TFW Framework Core Interface Definition
 * @brief TFW框架核心接口定义
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * Important Notes:
 * 重要说明：
 * 1. Memory Management: GetInstance() returns raw pointer, caller needs to ensure the object is not destroyed during use
 *    1. 内存管理：GetInstance()返回原始指针，调用者需要确保对象在使用期间不被销毁
 * 2. Pointer Validation: It is recommended to validate the pointer before use to avoid accessing destroyed objects
 *    2. 指针验证：使用前建议验证指针是否有效，避免访问已销毁的对象
 * 3. Lifecycle Management: It is recommended to manage object lifecycle in the same thread to avoid multi-thread competition
 *    3. 生命周期管理：建议在同一线程中管理对象的生命周期，避免多线程竞争
 * 4. Usage Order: First call GetInstance() to get the instance, then call DestroyInstance() to destroy after use
 *    4. 使用顺序：先调用GetInstance()获取实例，使用完毕后调用DestroyInstance()销毁
 *
 * Usage Example:
 * 使用示例：
 * ```cpp
 * // Get instance
 * // 获取实例
 * auto instance = TFW::CoreInterface::GetInstance();
 * if (instance != nullptr) {
 *     // Use instance
 *     // 使用实例
 *     instance->Init();
 *     // ... business logic ...
 *     // ... 业务逻辑 ...
 *     instance->Deinit();
 *
 *     // Destroy instance
 *     // 销毁实例
 *     TFW::CoreInterface::DestroyInstance();
 * }
 * ```
 */

#ifndef TFW_INTERFACE_H
#define TFW_INTERFACE_H

#include "TFW_errorno.h"
#include "TFW_types.h"
#include <string>
#include <memory>

namespace TFW {

// Core interface class
// 核心接口类
class CoreInterface {
public:
    /**
     * Constructor
     * 构造函数
     *
     * Note: It is not recommended to directly call the constructor to create an instance
     * 注意：不建议直接调用构造函数创建实例
     * Please use the GetInstance() static method to get the singleton instance
     * 请使用 GetInstance() 静态方法获取单例实例
     */
    CoreInterface();

    /**
     * Destructor
     * 析构函数
     *
     * Note: It is not recommended to directly call the destructor to destroy an instance
     * 注意：不建议直接调用析构函数销毁实例
     * Please use the DestroyInstance() static method to destroy the singleton instance
     * 请使用 DestroyInstance() 静态方法销毁单例实例
     */
    ~CoreInterface();

    /**
     * Get singleton instance
     * 获取单例实例
     *
     * @return Pointer to CoreInterface instance (raw pointer)
     * @return 指向CoreInterface实例的原始指针
     *
     * Important Notes:
     * 注意事项：
     * 1. Returns a raw pointer, does not manage memory lifecycle
     *    1. 返回的是原始指针，不管理内存生命周期
     * 2. Caller needs to ensure the object is not destroyed during use
     *    2. 调用者需要确保对象在使用期间不被销毁
     * 3. It is recommended to check if the pointer is valid before use
     *    3. 建议在使用前检查指针是否有效
     * 4. External synchronization protection is required in multi-threaded environments
     *    4. 多线程环境下需要外部同步保护
     */
    static CoreInterface* GetInstance();

    /**
     * Destroy singleton instance
     * 销毁单例实例
     *
     * Important Notes:
     * 注意事项：
     * 1. After calling, pointers previously obtained through GetInstance() will become invalid
     *    1. 调用后，之前通过GetInstance()获取的指针将变为无效
     * 2. Using these invalid pointers will cause undefined behavior
     *    2. 使用这些无效指针将导致未定义行为
     * 3. It is recommended to ensure no other code is using the instance before destruction
     *    3. 建议在销毁前确保没有其他代码正在使用该实例
     * 4. After destruction, if you need to use it again, you need to call GetInstance() again
     *    4. 销毁后如需使用，需要重新调用GetInstance()
     */
    static void DestroyInstance();

    /**
     * Initialize core interface
     * 初始化核心接口
     *
     * @return Returns TFW_SUCCESS on success, error code on failure
     * @return 成功返回TFW_SUCCESS，失败返回错误码
     *
     * Important Notes:
     * 注意事项：
     * 1. Can only be called once, repeated calls will return an error
     *    1. 只能调用一次，重复调用将返回错误
     * 2. Ensure the instance has been obtained through GetInstance() before calling
     *    2. 调用前确保实例已通过GetInstance()获取
     */
    int32_t Init();

    /**
     * Deinitialize core interface
     * 反初始化核心接口
     *
     * @return Returns TFW_SUCCESS on success, error code on failure
     * @return 成功返回TFW_SUCCESS，失败返回错误码
     *
     * Important Notes:
     * 注意事项：
     * 1. Can only be called on initialized instances
     *    1. 只能对已初始化的实例调用
     * 2. After calling, the instance state becomes uninitialized
     *    2. 调用后实例状态变为未初始化
     * 3. Can call Init() again to reinitialize
     *    3. 可以重新调用Init()进行初始化
     */
    int32_t Deinit();

    /**
     * Check if the instance is initialized
     * 检查实例是否已初始化
     *
     * @return true means initialized, false means uninitialized
     * @return true表示已初始化，false表示未初始化
     *
     * Important Notes:
     * 注意事项：
     * 1. It is recommended to check this status before use
     *    1. 使用前建议先检查此状态
     * 2. Calling other methods on uninitialized instances may return errors
     *    2. 未初始化的实例调用其他方法可能返回错误
     */
    bool IsInitialized() const;

private:
    // Disable copy constructor and assignment operator
    // 禁用拷贝构造和赋值操作
    CoreInterface(const CoreInterface&) = delete;
    CoreInterface& operator=(const CoreInterface&) = delete;

    // Initialization status flag
    // 初始化状态标志
    mutable bool isInitialized_ = false;
};

} // namespace TFW

#endif // TFW_INTERFACE_H
