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

// 宏定义的安全锁操作
#define TFW_Mutex_Lock(mutex)                                                        \
({                                                                                   \
    int32_t ret = TFW_SUCCESS;                                                       \
    if (TFW_CheckMutexIsNull(mutex)) {                                               \
        TFW_LOGD_UTILS("TFW_Mutex_Lock mutex is null");                             \
        ret = TFW_ERROR_INVALID_PARAM;                                               \
    } else {                                                                         \
        ret = TFW_Mutex_Lock_Inner(mutex);                                           \
        if (ret != 0) {                                                              \
            TFW_LOGE_UTILS("TFW_Mutex_Lock failed, ret=%d", ret);                   \
            ret = TFW_ERROR_LOCK_FAILED;                                              \
        }                                                                            \
    }                                                                                \
    ret;                                                                             \
})

#define TFW_Mutex_Unlock(mutex)                                                      \
({                                                                                   \
    int32_t ret = TFW_SUCCESS;                                                       \
    if (TFW_CheckMutexIsNull(mutex)) {                                               \
        TFW_LOGE_UTILS("TFW_Mutex_Unlock mutex is null");                           \
        ret = TFW_ERROR_INVALID_PARAM;                                               \
    } else {                                                                         \
        ret = TFW_Mutex_Unlock_Inner(mutex);                                         \
        if (ret != 0) {                                                              \
            TFW_LOGE_UTILS("TFW_Mutex_Unlock failed, ret=%d", ret);                 \
            ret = TFW_ERROR_LOCK_FAILED;                                              \
        }                                                                            \
    }                                                                                \
    ret;                                                                             \
})

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