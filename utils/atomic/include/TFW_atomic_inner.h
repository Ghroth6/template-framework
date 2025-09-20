#ifndef TFW_ATOMIC_INNER_H
#define TFW_ATOMIC_INNER_H

#include "TFW_atomic.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// 32位原子操作内部函数声明
int32_t TFW_AtomicAdd32_Inner(TFW_AtomicInt32* atomic, int32_t value);
int32_t TFW_AtomicSub32_Inner(TFW_AtomicInt32* atomic, int32_t value);
int32_t TFW_AtomicInc32_Inner(TFW_AtomicInt32* atomic);
int32_t TFW_AtomicDec32_Inner(TFW_AtomicInt32* atomic);
bool TFW_AtomicCompareAndSwap32_Inner(TFW_AtomicInt32* atomic, int32_t expected, int32_t desired);
int32_t TFW_AtomicLoad32_Inner(TFW_AtomicInt32* atomic);
void TFW_AtomicStore32_Inner(TFW_AtomicInt32* atomic, int32_t value);

// 64位原子操作内部函数声明
int64_t TFW_AtomicAdd64_Inner(TFW_AtomicInt64* atomic, int64_t value);
int64_t TFW_AtomicSub64_Inner(TFW_AtomicInt64* atomic, int64_t value);
int64_t TFW_AtomicInc64_Inner(TFW_AtomicInt64* atomic);
int64_t TFW_AtomicDec64_Inner(TFW_AtomicInt64* atomic);
bool TFW_AtomicCompareAndSwap64_Inner(TFW_AtomicInt64* atomic, int64_t expected, int64_t desired);
int64_t TFW_AtomicLoad64_Inner(TFW_AtomicInt64* atomic);
void TFW_AtomicStore64_Inner(TFW_AtomicInt64* atomic, int64_t value);

// 指针原子操作内部函数声明
void* TFW_AtomicLoadPtr_Inner(TFW_AtomicPtr* atomic);
void TFW_AtomicStorePtr_Inner(TFW_AtomicPtr* atomic, void* ptr);
bool TFW_AtomicCompareAndSwapPtr_Inner(TFW_AtomicPtr* atomic, void* expected, void* desired);

// 内存屏障操作内部函数声明
void TFW_MemoryBarrier_Inner(void);

#ifdef __cplusplus
}
#endif

#endif /* TFW_ATOMIC_INNER_H */
