#include "TFW_atomic_inner.h"
#include "TFW_utils_log.h"

#include <windows.h>

// Windows平台使用Interlocked函数实现原子操作
int32_t TFW_AtomicAdd32_Inner(TFW_AtomicInt32* atomic, int32_t value) {
    return InterlockedAdd((LONG volatile*)&atomic->value, value);
}

int32_t TFW_AtomicSub32_Inner(TFW_AtomicInt32* atomic, int32_t value) {
    return InterlockedAdd((LONG volatile*)&atomic->value, -value);
}

int32_t TFW_AtomicInc32_Inner(TFW_AtomicInt32* atomic) {
    return InterlockedIncrement((LONG volatile*)&atomic->value);
}

int32_t TFW_AtomicDec32_Inner(TFW_AtomicInt32* atomic) {
    return InterlockedDecrement((LONG volatile*)&atomic->value);
}

bool TFW_AtomicCompareAndSwap32_Inner(TFW_AtomicInt32* atomic, int32_t expected, int32_t desired) {
    return InterlockedCompareExchange((LONG volatile*)&atomic->value, desired, expected) == expected;
}

int32_t TFW_AtomicLoad32_Inner(TFW_AtomicInt32* atomic) {
    return InterlockedCompareExchange((LONG volatile*)&atomic->value, 0, 0);
}

void TFW_AtomicStore32_Inner(TFW_AtomicInt32* atomic, int32_t value) {
    InterlockedExchange((LONG volatile*)&atomic->value, value);
}

int64_t TFW_AtomicAdd64_Inner(TFW_AtomicInt64* atomic, int64_t value) {
    return InterlockedAdd64((LONG64 volatile*)&atomic->value, value);
}

int64_t TFW_AtomicSub64_Inner(TFW_AtomicInt64* atomic, int64_t value) {
    return InterlockedAdd64((LONG64 volatile*)&atomic->value, -value);
}

int64_t TFW_AtomicInc64_Inner(TFW_AtomicInt64* atomic) {
    return InterlockedIncrement64((LONG64 volatile*)&atomic->value);
}

int64_t TFW_AtomicDec64_Inner(TFW_AtomicInt64* atomic) {
    return InterlockedDecrement64((LONG64 volatile*)&atomic->value);
}

bool TFW_AtomicCompareAndSwap64_Inner(TFW_AtomicInt64* atomic, int64_t expected, int64_t desired) {
    return InterlockedCompareExchange64((LONG64 volatile*)&atomic->value, desired, expected) == expected;
}

int64_t TFW_AtomicLoad64_Inner(TFW_AtomicInt64* atomic) {
    return InterlockedCompareExchange64((LONG64 volatile*)&atomic->value, 0, 0);
}

void TFW_AtomicStore64_Inner(TFW_AtomicInt64* atomic, int64_t value) {
    InterlockedExchange64((LONG64 volatile*)&atomic->value, value);
}

void* TFW_AtomicLoadPtr_Inner(TFW_AtomicPtr* atomic) {
    return InterlockedCompareExchangePointer((PVOID volatile*)&atomic->ptr, NULL, NULL);
}

void TFW_AtomicStorePtr_Inner(TFW_AtomicPtr* atomic, void* ptr) {
    InterlockedExchangePointer((PVOID volatile*)&atomic->ptr, ptr);
}

bool TFW_AtomicCompareAndSwapPtr_Inner(TFW_AtomicPtr* atomic, void* expected, void* desired) {
    return InterlockedCompareExchangePointer((PVOID volatile*)&atomic->ptr, desired, expected) == expected;
}

void TFW_MemoryBarrier_Inner(void) {
    MemoryBarrier();
}
