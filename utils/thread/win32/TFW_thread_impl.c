#include <process.h>
#include <windows.h>

#include "TFW_errorno.h"
#include "TFW_log.h"
#include "TFW_thread.h"

// ============================================================================
// Windows平台互斥锁实现
// Windows platform mutex implementation
// ============================================================================

int32_t TFW_MutexAttr_Init(TFW_MutexAttr_t* mutexAttr) {
    if (mutexAttr == NULL) {
        TFW_LOGE_UTILS("TFW_MutexAttr_Init mutexAttr is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：互斥锁属性通常不需要特殊处理
    // Windows platform: mutex attribute usually does not need special processing
    *mutexAttr = 0;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Init(TFW_Mutex_t* mutex, TFW_MutexAttr_t* mutexAttr) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Init mutex is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：创建互斥锁
    // Windows platform: create mutex
    HANDLE handle = CreateMutex(NULL, FALSE, NULL);
    if (handle == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Init handle is null");
        return TFW_ERROR;
    }
    *mutex = (TFW_Mutex_t)handle;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Lock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Lock_Inner handle is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：等待互斥锁
    // Windows platform: wait for mutex
    DWORD result = WaitForSingleObject((HANDLE)*mutex, INFINITE);
    if (result == WAIT_OBJECT_0) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR;
    }
}

int32_t TFW_Mutex_Unlock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Lock_Inner handle is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：释放互斥锁
    // Windows platform: release mutex
    BOOL result = ReleaseMutex((HANDLE)*mutex);
    if (result) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR;
    }
}

int32_t TFW_Mutex_Destroy(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        TFW_LOGE_UTILS("TFW_Mutex_Unlock_Inner handle is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：关闭互斥锁句柄
    // Windows platform: close mutex handle
    if (*mutex != 0) {
        BOOL result = CloseHandle((HANDLE)*mutex);
        if (result) {
            *mutex = 0;
            return TFW_SUCCESS;
        } else {
            return TFW_ERROR;
        }
    }
    return TFW_SUCCESS;
}

// ============================================================================
// Windows平台进程和线程ID实现
// Windows platform process and thread ID implementation
// ============================================================================

int32_t TFW_GetProcessId() {
    return (int32_t)_getpid();
}

uint64_t TFW_GetThreadId() {
    return (uint64_t)GetCurrentThreadId();
}
