#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

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
// POSIX平台进程和线程ID实现
// POSIX platform process and thread ID implementation
// ============================================================================

int32_t TFW_GetProcessId() {
    return (int32_t)getpid();
}

uint64_t TFW_GetThreadId() {
    return (uint64_t)pthread_self();
}
