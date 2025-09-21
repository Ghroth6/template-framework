#ifndef TFW_THREAD_H
#define TFW_THREAD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "TFW_errorno.h"
#include "TFW_utils_log.h"
#include "TFW_timer.h"

typedef uintptr_t TFW_Mutex_t;
typedef uintptr_t TFW_MutexAttr_t;
typedef uintptr_t TFW_Cond_t;
typedef uintptr_t TFW_Thread_t;

// 线程属性结构体
typedef struct {
    const char *name;
    uint64_t stackSize;
    int32_t priority;
} TFW_ThreadAttr;

// 互斥锁类型
typedef enum {
    TFW_MUTEX_NORMAL,
    TFW_MUTEX_RECURSIVE
} TFW_MutexType;

// 互斥锁属性结构体
typedef struct {
    TFW_MutexType type;
} TFW_MutexAttr;

// 基础互斥锁函数
int32_t TFW_MutexAttr_Init(TFW_MutexAttr_t* mutexAttr);
int32_t TFW_Mutex_Init(TFW_Mutex_t* mutex, TFW_MutexAttr_t* mutexAttr);
int32_t TFW_Mutex_Lock_Inner(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Unlock_Inner(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Destroy(TFW_Mutex_t* mutex);

// 条件变量函数
int32_t TFW_Cond_Init(TFW_Cond_t* cond);
int32_t TFW_Cond_Signal(TFW_Cond_t* cond);
int32_t TFW_Cond_Broadcast(TFW_Cond_t* cond);
int32_t TFW_Cond_Wait(TFW_Cond_t* cond, TFW_Mutex_t* mutex, TFW_SysTime* time);
int32_t TFW_Cond_Destroy(TFW_Cond_t* cond);

// 线程属性函数
int32_t TFW_ThreadAttr_Init(TFW_ThreadAttr* attr);

// 线程操作函数
int32_t TFW_Thread_Create(TFW_Thread_t* thread, const TFW_ThreadAttr* attr,
                            void* (*threadEntry)(void*), void* arg);
int32_t TFW_Thread_Join(TFW_Thread_t thread, void** value);
int32_t TFW_Thread_SetName(TFW_Thread_t thread, const char* name);
TFW_Thread_t TFW_Thread_GetSelf(void);

// 内联检查函数
static inline bool TFW_CheckMutexIsNull(const TFW_Mutex_t* mutex) {
    return (mutex == NULL) || ((void*)(*mutex) == NULL);
}

// 安全锁操作函数声明
int32_t TFW_Mutex_Lock_Safe(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Unlock_Safe(TFW_Mutex_t* mutex);

// 宏定义安全锁操作
#define TFW_Mutex_Lock(mutex) TFW_Mutex_Lock_Safe(mutex)
#define TFW_Mutex_Unlock(mutex) TFW_Mutex_Unlock_Safe(mutex)

// ============================================================================
// Process and thread related function declarations
// 进程和线程相关函数声明
// ============================================================================

/**
 * Get current process ID
 * 获取当前进程ID
 * @return Current process ID
 */
int32_t TFW_GetProcessId();

/**
 * Get current thread ID
 * 获取当前线程ID
 * @return Current thread ID
 */
uint64_t TFW_GetThreadId();

#endif  // TFW_THREAD_H
