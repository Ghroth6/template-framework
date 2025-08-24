#ifndef TFW_THREAD_H
#define TFW_THREAD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "TFW_errorno.h"
#include "TFW_utils_log.h"

typedef uintptr_t TFW_Mutex_t;
typedef uintptr_t TFW_MutexAttr_t;

// 基础互斥锁函数
int32_t TFW_MutexAttr_Init(TFW_MutexAttr_t* mutexAttr);
int32_t TFW_Mutex_Init(TFW_Mutex_t* mutex, TFW_MutexAttr_t* mutexAttr);
int32_t TFW_Mutex_Lock_Inner(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Unlock_Inner(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Destroy(TFW_Mutex_t* mutex);

// 内联检查函数
static inline bool TFW_CheckMutexIsNull(const TFW_Mutex_t* mutex)
{
    return (mutex == NULL) || ((void *)(*mutex) == NULL);
}

// 安全锁操作函数声明（替代原来的GCC语句表达式宏）
int32_t TFW_Mutex_Lock_Safe(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Unlock_Safe(TFW_Mutex_t* mutex);

// 宏定义的安全锁操作（调用安全函数）
#define TFW_Mutex_Lock(mutex) TFW_Mutex_Lock_Safe(mutex)
#define TFW_Mutex_Unlock(mutex) TFW_Mutex_Unlock_Safe(mutex)

// ============================================================================
// 进程和线程相关函数声明
// ============================================================================

/**
 * 获取当前进程ID
 * @return 当前进程ID
 */
int32_t TFW_GetProcessId();

/**
 * 获取当前线程ID
 * @return 当前线程ID
 */
uint64_t TFW_GetThreadId();

#endif // TFW_THREAD_H
