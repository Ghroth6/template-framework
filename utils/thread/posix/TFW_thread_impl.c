#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "TFW_errorno.h"
#include "TFW_thread.h"
#include "TFW_utils_log.h"
#include "TFW_mem.h"

// ============================================================================
// POSIX平台互斥锁实现
// POSIX platform mutex implementation
// ============================================================================

int32_t TFW_MutexAttr_Init(TFW_MutexAttr_t* mutexAttr) {
    if (mutexAttr == NULL) {
        TFW_LOGE_UTILS("TFW_MutexAttr_Init mutexAttr is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：初始化pthread互斥锁属性
    // POSIX platform: initialize pthread mutex attribute
    pthread_mutexattr_t* attr = (pthread_mutexattr_t*)TFW_Malloc(sizeof(pthread_mutexattr_t));
    if (attr == NULL) {
        return TFW_ERROR_MALLOC_ERR;
    }

    int ret = pthread_mutexattr_init(attr);
    if (ret != 0) {
        TFW_Free(attr);
        return TFW_ERROR;
    }

    *mutexAttr = (TFW_MutexAttr_t)attr;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Init(TFW_Mutex_t* mutex, TFW_MutexAttr_t* mutexAttr) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Init mutex is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：创建pthread互斥锁
    // POSIX platform: create pthread mutex
    pthread_mutex_t* mtx = (pthread_mutex_t*)TFW_Malloc(sizeof(pthread_mutex_t));
    if (mtx == NULL) {
        return TFW_ERROR_MALLOC_ERR;
    }

    int ret;
    if (mutexAttr != NULL) {
        pthread_mutexattr_t* attr = (pthread_mutexattr_t*)*mutexAttr;
        ret = pthread_mutex_init(mtx, attr);
    } else {
        ret = pthread_mutex_init(mtx, NULL);
    }

    if (ret != 0) {
        TFW_Free(mtx);
        return TFW_ERROR;
    }

    *mutex = (TFW_Mutex_t)mtx;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Lock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Lock_Inner mutex is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：锁定pthread互斥锁
    // POSIX platform: lock pthread mutex
    pthread_mutex_t* mtx = (pthread_mutex_t*)*mutex;
    int ret = pthread_mutex_lock(mtx);
    if (ret == 0) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR;
    }
}

int32_t TFW_Mutex_Unlock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Unlock_Inner mutex is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：解锁pthread互斥锁
    // POSIX platform: unlock pthread mutex
    pthread_mutex_t* mtx = (pthread_mutex_t*)*mutex;
    int ret = pthread_mutex_unlock(mtx);
    if (ret == 0) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR;
    }
}

int32_t TFW_Mutex_Destroy(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Destroy mutex is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：销毁pthread互斥锁
    // POSIX platform: destroy pthread mutex
    if (*mutex != 0) {
        pthread_mutex_t* mtx = (pthread_mutex_t*)*mutex;
        int ret = pthread_mutex_destroy(mtx);
        TFW_Free(mtx);
        *mutex = 0;

        if (ret == 0) {
            return TFW_SUCCESS;
        } else {
            return TFW_ERROR;
        }
    }
    return TFW_SUCCESS;
}

// ============================================================================
// POSIX平台条件变量实现
// POSIX platform condition variable implementation
// ============================================================================

int32_t TFW_Cond_Init(TFW_Cond_t* cond) {
    if (cond == NULL) {
        TFW_LOGE_UTILS("TFW_Cond_Init cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    pthread_condattr_t attr;
    int32_t ret = pthread_condattr_init(&attr);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Cond_Init pthread_condattr_init failed, ret=%d", ret);
        return TFW_ERROR;
    }

    ret = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Cond_Init pthread_condattr_setclock failed, ret=%d", ret);
        return TFW_ERROR;
    }

    pthread_cond_t* tempCond = (pthread_cond_t*)TFW_Malloc(sizeof(pthread_cond_t));
    if (tempCond == NULL) {
        TFW_LOGE_UTILS("TFW_Cond_Init malloc tempCond failed");
        return TFW_ERROR_MALLOC_ERR;
    }

    ret = pthread_cond_init(tempCond, &attr);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Cond_Init pthread_cond_init failed, ret=%d", ret);
        TFW_Free(tempCond);
        return TFW_ERROR;
    }

    *cond = (TFW_Cond_t)tempCond;
    return TFW_SUCCESS;
}

int32_t TFW_Cond_Signal(TFW_Cond_t* cond) {
    if ((cond == NULL) || ((void*)(*cond) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Signal cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = pthread_cond_signal((pthread_cond_t*)*cond);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Cond_Signal failed, ret=%d", ret);
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

int32_t TFW_Cond_Broadcast(TFW_Cond_t* cond) {
    if ((cond == NULL) || ((void*)(*cond) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Broadcast cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = pthread_cond_broadcast((pthread_cond_t*)*cond);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Cond_Broadcast failed, ret=%d", ret);
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

int32_t TFW_Cond_Wait(TFW_Cond_t* cond, TFW_Mutex_t* mutex, TFW_SysTime* time) {
    if ((cond == NULL) || ((void*)(*cond) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Wait cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    if ((mutex == NULL) || ((void*)(*mutex) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Wait mutex is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret;
    if (time == NULL) {
        ret = pthread_cond_wait((pthread_cond_t*)*cond, (pthread_mutex_t*)*mutex);
        if (ret != 0) {
            TFW_LOGE_UTILS("TFW_Cond_Wait failed, ret=%d", ret);
            return TFW_ERROR;
        }
    } else {
        struct timespec tv;
        tv.tv_sec = time->sec;
        tv.tv_nsec = time->nsec;
        ret = pthread_cond_timedwait((pthread_cond_t*)*cond, (pthread_mutex_t*)*mutex, &tv);
        if (ret == ETIMEDOUT) {
            TFW_LOGD_UTILS("TFW_Cond_Wait timeout, ret=%d", ret);
            return TFW_ERROR_TIMEOUT;
        }

        if (ret != 0) {
            TFW_LOGE_UTILS("TFW_Cond_Wait failed, ret=%d", ret);
            return TFW_ERROR;
        }
    }

    return TFW_SUCCESS;
}

int32_t TFW_Cond_Destroy(TFW_Cond_t* cond) {
    if ((cond == NULL) || ((void*)(*cond) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Destroy cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = pthread_cond_destroy((pthread_cond_t*)*cond);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Cond_Destroy failed, ret=%d", ret);
        TFW_Free((void*)*cond);
        *cond = (TFW_Cond_t)NULL;
        return TFW_ERROR;
    }

    TFW_Free((void*)*cond);
    *cond = (TFW_Cond_t)NULL;
    return TFW_SUCCESS;
}

// ============================================================================
// POSIX平台线程属性实现
// POSIX platform thread attribute implementation
// ============================================================================

int32_t TFW_ThreadAttr_Init(TFW_ThreadAttr* attr) {
    if (attr == NULL) {
        TFW_LOGE_UTILS("TFW_ThreadAttr_Init attr is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    attr->name = NULL;
    attr->stackSize = 0;
    attr->priority = 0;
    return TFW_SUCCESS;
}

// ============================================================================
// POSIX平台线程实现
// POSIX platform thread implementation
// ============================================================================

int32_t TFW_Thread_Create(TFW_Thread_t* thread, const TFW_ThreadAttr* attr,
                         void* (*threadEntry)(void*), void* arg) {
    if (thread == NULL) {
        TFW_LOGE_UTILS("TFW_Thread_Create thread is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (threadEntry == NULL) {
        TFW_LOGE_UTILS("TFW_Thread_Create threadEntry is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret;
    if (attr == NULL) {
        ret = pthread_create((pthread_t*)thread, NULL, threadEntry, arg);
        if (ret != 0) {
            TFW_LOGE_UTILS("TFW_Thread_Create failed, ret=%d", ret);
            return TFW_ERROR;
        }
    } else {
        pthread_attr_t pthreadAttr;
        ret = pthread_attr_init(&pthreadAttr);
        if (ret != 0) {
            TFW_LOGE_UTILS("TFW_Thread_Create pthread_attr_init failed, ret=%d", ret);
            return TFW_ERROR;
        }

        if (attr->stackSize != 0) {
            ret = pthread_attr_setstacksize(&pthreadAttr, attr->stackSize);
            if (ret != 0) {
                TFW_LOGE_UTILS("TFW_Thread_Create pthread_attr_setstacksize failed, ret=%d", ret);
                return TFW_ERROR;
            }
        }

        ret = pthread_create((pthread_t*)thread, &pthreadAttr, threadEntry, arg);
        if (ret != 0) {
            TFW_LOGE_UTILS("TFW_Thread_Create pthread_create failed, ret=%d", ret);
            return TFW_ERROR;
        }

        if (attr->name != NULL) {
            ret = TFW_Thread_SetName(*thread, attr->name);
            if (ret != 0) {
                TFW_LOGE_UTILS("TFW_Thread_Create TFW_Thread_SetName failed, ret=%d", ret);
            }
        }
    }

    return TFW_SUCCESS;
}

int32_t TFW_Thread_Join(TFW_Thread_t thread, void** value) {
    if (thread <= 0) {
        TFW_LOGE_UTILS("TFW_Thread_Join thread is invalid");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = pthread_join((pthread_t)thread, value);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Thread_Join failed, ret=%d", ret);
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

int32_t TFW_Thread_SetName(TFW_Thread_t thread, const char* name) {
    if (thread <= 0) {
        TFW_LOGE_UTILS("TFW_Thread_SetName thread is invalid");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (name == NULL) {
        TFW_LOGE_UTILS("TFW_Thread_SetName name is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // 限制线程名称长度
    if (strlen(name) >= 16) {
        TFW_LOGE_UTILS("TFW_Thread_SetName name length >= 16");
        return TFW_ERROR_INVALID_PARAM;
    }

    int32_t ret = pthread_setname_np((pthread_t)thread, name);
    if (ret != 0) {
        TFW_LOGE_UTILS("TFW_Thread_SetName failed, ret=%d", ret);
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

TFW_Thread_t TFW_Thread_GetSelf(void) {
    return (TFW_Thread_t)pthread_self();
}

// ============================================================================
// POSIX平台进程和线程ID实现
// POSIX platform process and thread ID implementation
// ============================================================================

int32_t TFW_GetProcessId() {
    return (int32_t)getpid();
}

uint64_t TFW_GetThreadId() {
    return (uint64_t)pthread_self();
}