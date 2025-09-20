#include "TFW_atomic.h"
#include "TFW_atomic_inner.h"
#include "TFW_utils_log.h"

// 检查是否支持C11原子操作
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
    #define TFW_USE_C11_ATOMICS 1
#else
    #define TFW_USE_C11_ATOMICS 0
#endif

#if TFW_USE_C11_ATOMICS
    #include <stdatomic.h>
#endif

// 32位原子操作函数实现
int32_t TFW_AtomicAdd32(TFW_AtomicInt32* atomic, int32_t value) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_add(&atomic->value, value) + value;
#else
    // 使用平台特定的实现
    return TFW_AtomicAdd32_Inner(atomic, value);
#endif
}

int32_t TFW_AtomicSub32(TFW_AtomicInt32* atomic, int32_t value) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_sub(&atomic->value, value) - value;
#else
    // 使用平台特定的实现
    return TFW_AtomicSub32_Inner(atomic, value);
#endif
}

int32_t TFW_AtomicInc32(TFW_AtomicInt32* atomic) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_add(&atomic->value, 1) + 1;
#else
    // 使用平台特定的实现
    return TFW_AtomicInc32_Inner(atomic);
#endif
}

int32_t TFW_AtomicDec32(TFW_AtomicInt32* atomic) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_sub(&atomic->value, 1) - 1;
#else
    // 使用平台特定的实现
    return TFW_AtomicDec32_Inner(atomic);
#endif
}

bool TFW_AtomicCompareAndSwap32(TFW_AtomicInt32* atomic, int32_t expected, int32_t desired) {
#if TFW_USE_C11_ATOMICS
    int32_t expected_copy = expected;
    return atomic_compare_exchange_strong(&atomic->value, &expected_copy, desired);
#else
    // 使用平台特定的实现
    return TFW_AtomicCompareAndSwap32_Inner(atomic, expected, desired);
#endif
}

int32_t TFW_AtomicLoad32(TFW_AtomicInt32* atomic) {
#if TFW_USE_C11_ATOMICS
    return atomic_load(&atomic->value);
#else
    // 使用平台特定的实现
    return TFW_AtomicLoad32_Inner(atomic);
#endif
}

void TFW_AtomicStore32(TFW_AtomicInt32* atomic, int32_t value) {
#if TFW_USE_C11_ATOMICS
    atomic_store(&atomic->value, value);
#else
    // 使用平台特定的实现
    TFW_AtomicStore32_Inner(atomic, value);
#endif
}

// 64位原子操作函数实现
int64_t TFW_AtomicAdd64(TFW_AtomicInt64* atomic, int64_t value) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_add(&atomic->value, value) + value;
#else
    // 使用平台特定的实现
    return TFW_AtomicAdd64_Inner(atomic, value);
#endif
}

int64_t TFW_AtomicSub64(TFW_AtomicInt64* atomic, int64_t value) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_sub(&atomic->value, value) - value;
#else
    // 使用平台特定的实现
    return TFW_AtomicSub64_Inner(atomic, value);
#endif
}

int64_t TFW_AtomicInc64(TFW_AtomicInt64* atomic) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_add(&atomic->value, 1) + 1;
#else
    // 使用平台特定的实现
    return TFW_AtomicInc64_Inner(atomic);
#endif
}

int64_t TFW_AtomicDec64(TFW_AtomicInt64* atomic) {
#if TFW_USE_C11_ATOMICS
    return atomic_fetch_sub(&atomic->value, 1) - 1;
#else
    // 使用平台特定的实现
    return TFW_AtomicDec64_Inner(atomic);
#endif
}

bool TFW_AtomicCompareAndSwap64(TFW_AtomicInt64* atomic, int64_t expected, int64_t desired) {
#if TFW_USE_C11_ATOMICS
    int64_t expected_copy = expected;
    return atomic_compare_exchange_strong(&atomic->value, &expected_copy, desired);
#else
    // 使用平台特定的实现
    return TFW_AtomicCompareAndSwap64_Inner(atomic, expected, desired);
#endif
}

int64_t TFW_AtomicLoad64(TFW_AtomicInt64* atomic) {
#if TFW_USE_C11_ATOMICS
    return atomic_load(&atomic->value);
#else
    // 使用平台特定的实现
    return TFW_AtomicLoad64_Inner(atomic);
#endif
}

void TFW_AtomicStore64(TFW_AtomicInt64* atomic, int64_t value) {
#if TFW_USE_C11_ATOMICS
    atomic_store(&atomic->value, value);
#else
    // 使用平台特定的实现
    TFW_AtomicStore64_Inner(atomic, value);
#endif
}

// 指针原子操作实现
void* TFW_AtomicLoadPtr(TFW_AtomicPtr* atomic) {
#if TFW_USE_C11_ATOMICS
    return atomic_load(&atomic->ptr);
#else
    // 使用平台特定的实现
    return TFW_AtomicLoadPtr_Inner(atomic);
#endif
}

void TFW_AtomicStorePtr(TFW_AtomicPtr* atomic, void* ptr) {
#if TFW_USE_C11_ATOMICS
    atomic_store(&atomic->ptr, ptr);
#else
    // 使用平台特定的实现
    TFW_AtomicStorePtr_Inner(atomic, ptr);
#endif
}

bool TFW_AtomicCompareAndSwapPtr(TFW_AtomicPtr* atomic, void* expected, void* desired) {
#if TFW_USE_C11_ATOMICS
    void* expected_copy = expected;
    return atomic_compare_exchange_strong(&atomic->ptr, &expected_copy, desired);
#else
    // 使用平台特定的实现
    return TFW_AtomicCompareAndSwapPtr_Inner(atomic, expected, desired);
#endif
}

// 内存屏障操作实现
void TFW_MemoryBarrier(void) {
#if TFW_USE_C11_ATOMICS
    atomic_thread_fence(memory_order_seq_cst);
#else
    // 使用平台特定的实现
    TFW_MemoryBarrier_Inner();
#endif
}
