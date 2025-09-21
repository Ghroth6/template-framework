#ifndef TFW_ATOMIC_H
#define TFW_ATOMIC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// 原子整型类型定义
typedef struct {
    volatile int32_t value;
} TFW_AtomicInt32;

typedef struct {
    volatile int64_t value;
} TFW_AtomicInt64;


// 32位原子操作函数
int32_t TFW_AtomicAdd32(TFW_AtomicInt32* atomic, int32_t value);
int32_t TFW_AtomicSub32(TFW_AtomicInt32* atomic, int32_t value);
int32_t TFW_AtomicInc32(TFW_AtomicInt32* atomic);
int32_t TFW_AtomicDec32(TFW_AtomicInt32* atomic);
bool TFW_AtomicCompareAndSwap32(TFW_AtomicInt32* atomic, int32_t expected, int32_t desired);
int32_t TFW_AtomicLoad32(TFW_AtomicInt32* atomic);
void TFW_AtomicStore32(TFW_AtomicInt32* atomic, int32_t value);

// 64位原子操作函数
int64_t TFW_AtomicAdd64(TFW_AtomicInt64* atomic, int64_t value);
int64_t TFW_AtomicSub64(TFW_AtomicInt64* atomic, int64_t value);
int64_t TFW_AtomicInc64(TFW_AtomicInt64* atomic);
int64_t TFW_AtomicDec64(TFW_AtomicInt64* atomic);
bool TFW_AtomicCompareAndSwap64(TFW_AtomicInt64* atomic, int64_t expected, int64_t desired);
int64_t TFW_AtomicLoad64(TFW_AtomicInt64* atomic);
void TFW_AtomicStore64(TFW_AtomicInt64* atomic, int64_t value);

// 指针原子操作
typedef struct {
    void* volatile ptr;
} TFW_AtomicPtr;

void* TFW_AtomicLoadPtr(TFW_AtomicPtr* atomic);
void TFW_AtomicStorePtr(TFW_AtomicPtr* atomic, void* ptr);
bool TFW_AtomicCompareAndSwapPtr(TFW_AtomicPtr* atomic, void* expected, void* desired);

// 内存屏障操作
void TFW_MemoryBarrier(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TFW_ATOMIC_H */
