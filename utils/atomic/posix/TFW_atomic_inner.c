#include "TFW_atomic_inner.h"
#include "TFW_utils_log.h"

// POSIX平台使用GCC内置函数实现原子操作
int32_t TFW_AtomicAdd32_Inner(TFW_AtomicInt32* atomic, int32_t value) {
    return __atomic_fetch_add(&atomic->value, value, __ATOMIC_SEQ_CST) + value;
}

int32_t TFW_AtomicSub32_Inner(TFW_AtomicInt32* atomic, int32_t value) {
    return __atomic_fetch_sub(&atomic->value, value, __ATOMIC_SEQ_CST) - value;
}

int32_t TFW_AtomicInc32_Inner(TFW_AtomicInt32* atomic) {
    return __atomic_fetch_add(&atomic->value, 1, __ATOMIC_SEQ_CST) + 1;
}

int32_t TFW_AtomicDec32_Inner(TFW_AtomicInt32* atomic) {
    return __atomic_fetch_sub(&atomic->value, 1, __ATOMIC_SEQ_CST) - 1;
}

bool TFW_AtomicCompareAndSwap32_Inner(TFW_AtomicInt32* atomic, int32_t expected, int32_t desired) {
    return __atomic_compare_exchange_n(&atomic->value, &expected, desired, false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

int32_t TFW_AtomicLoad32_Inner(TFW_AtomicInt32* atomic) {
    return __atomic_load_n(&atomic->value, __ATOMIC_SEQ_CST);
}

void TFW_AtomicStore32_Inner(TFW_AtomicInt32* atomic, int32_t value) {
    __atomic_store_n(&atomic->value, value, __ATOMIC_SEQ_CST);
}

int64_t TFW_AtomicAdd64_Inner(TFW_AtomicInt64* atomic, int64_t value) {
    return __atomic_fetch_add(&atomic->value, value, __ATOMIC_SEQ_CST) + value;
}

int64_t TFW_AtomicSub64_Inner(TFW_AtomicInt64* atomic, int64_t value) {
    return __atomic_fetch_sub(&atomic->value, value, __ATOMIC_SEQ_CST) - value;
}

int64_t TFW_AtomicInc64_Inner(TFW_AtomicInt64* atomic) {
    return __atomic_fetch_add(&atomic->value, 1, __ATOMIC_SEQ_CST) + 1;
}

int64_t TFW_AtomicDec64_Inner(TFW_AtomicInt64* atomic) {
    return __atomic_fetch_sub(&atomic->value, 1, __ATOMIC_SEQ_CST) - 1;
}

bool TFW_AtomicCompareAndSwap64_Inner(TFW_AtomicInt64* atomic, int64_t expected, int64_t desired) {
    return __atomic_compare_exchange_n(&atomic->value, &expected, desired, false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

int64_t TFW_AtomicLoad64_Inner(TFW_AtomicInt64* atomic) {
    return __atomic_load_n(&atomic->value, __ATOMIC_SEQ_CST);
}

void TFW_AtomicStore64_Inner(TFW_AtomicInt64* atomic, int64_t value) {
    __atomic_store_n(&atomic->value, value, __ATOMIC_SEQ_CST);
}

void* TFW_AtomicLoadPtr_Inner(TFW_AtomicPtr* atomic) {
    return __atomic_load_n(&atomic->ptr, __ATOMIC_SEQ_CST);
}

void TFW_AtomicStorePtr_Inner(TFW_AtomicPtr* atomic, void* ptr) {
    __atomic_store_n(&atomic->ptr, ptr, __ATOMIC_SEQ_CST);
}

bool TFW_AtomicCompareAndSwapPtr_Inner(TFW_AtomicPtr* atomic, void* expected, void* desired) {
    return __atomic_compare_exchange_n(&atomic->ptr, &expected, desired, false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

void TFW_MemoryBarrier_Inner(void) {
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
}
