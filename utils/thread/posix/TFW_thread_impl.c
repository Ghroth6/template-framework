#include "../../include/TFW_thread.h"
#include "../../include/TFW_log.h"
#include "../../../interface/TFW_errorno.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// ============================================================================
// POSIX平台互斥锁实现
// ============================================================================

int32_t TFW_MutexAttr_Init(TFW_MutexAttr_t* mutexAttr) {
    if (mutexAttr == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：初始化pthread互斥锁属性
    pthread_mutexattr_t* attr = (pthread_mutexattr_t*)malloc(sizeof(pthread_mutexattr_t));
    if (attr == NULL) {
        return TFW_ERROR_NO_MEMORY;
    }
    
    int ret = pthread_mutexattr_init(attr);
    if (ret != 0) {
        free(attr);
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    *mutexAttr = (TFW_MutexAttr_t)attr;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Init(TFW_Mutex_t* mutex, TFW_MutexAttr_t* mutexAttr) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：创建pthread互斥锁
    pthread_mutex_t* mtx = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (mtx == NULL) {
        return TFW_ERROR_NO_MEMORY;
    }
    
    int ret;
    if (mutexAttr != NULL) {
        pthread_mutexattr_t* attr = (pthread_mutexattr_t*)*mutexAttr;
        ret = pthread_mutex_init(mtx, attr);
    } else {
        ret = pthread_mutex_init(mtx, NULL);
    }
    
    if (ret != 0) {
        free(mtx);
        return TFW_ERROR_OPERATION_FAIL;
    }
    
    *mutex = (TFW_Mutex_t)mtx;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Lock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：锁定pthread互斥锁
    pthread_mutex_t* mtx = (pthread_mutex_t*)*mutex;
    int ret = pthread_mutex_lock(mtx);
    if (ret == 0) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t TFW_Mutex_Unlock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：解锁pthread互斥锁
    pthread_mutex_t* mtx = (pthread_mutex_t*)*mutex;
    int ret = pthread_mutex_unlock(mtx);
    if (ret == 0) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t TFW_Mutex_Destroy(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：销毁pthread互斥锁
    if (*mutex != 0) {
        pthread_mutex_t* mtx = (pthread_mutex_t*)*mutex;
        int ret = pthread_mutex_destroy(mtx);
        free(mtx);
        *mutex = 0;
        
        if (ret == 0) {
            return TFW_SUCCESS;
        } else {
            return TFW_ERROR_OPERATION_FAIL;
        }
    }
    return TFW_SUCCESS;
}

// ============================================================================
// POSIX平台进程和线程ID实现
// ============================================================================

int32_t TFW_GetProcessId() {
    return (int32_t)getpid();
}

uint64_t TFW_GetThreadId() {
    return (uint64_t)pthread_self();
}
