#include "../../include/TFW_thread.h"
#include "../../include/TFW_log.h"
#include "../../../interface/TFW_errorno.h"
#include <windows.h>
#include <process.h>

// ============================================================================
// Windows平台互斥锁实现
// ============================================================================

int32_t TFW_MutexAttr_Init(TFW_MutexAttr_t* mutexAttr) {
    if (mutexAttr == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    // Windows平台：互斥锁属性通常不需要特殊处理
    *mutexAttr = 0;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Init(TFW_Mutex_t* mutex, TFW_MutexAttr_t* mutexAttr) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：创建互斥锁
    HANDLE handle = CreateMutex(NULL, FALSE, NULL);
    if (handle == NULL) {
        return TFW_ERROR_OPERATION_FAIL;
    }
    *mutex = (TFW_Mutex_t)handle;
    return TFW_SUCCESS;
}

int32_t TFW_Mutex_Lock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：等待互斥锁
    DWORD result = WaitForSingleObject((HANDLE)*mutex, INFINITE);
    if (result == WAIT_OBJECT_0) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t TFW_Mutex_Unlock_Inner(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：释放互斥锁
    BOOL result = ReleaseMutex((HANDLE)*mutex);
    if (result) {
        return TFW_SUCCESS;
    } else {
        return TFW_ERROR_OPERATION_FAIL;
    }
}

int32_t TFW_Mutex_Destroy(TFW_Mutex_t* mutex) {
    if (mutex == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：关闭互斥锁句柄
    if (*mutex != 0) {
        BOOL result = CloseHandle((HANDLE)*mutex);
        if (result) {
            *mutex = 0;
            return TFW_SUCCESS;
        } else {
            return TFW_ERROR_OPERATION_FAIL;
        }
    }
    return TFW_SUCCESS;
}

// ============================================================================
// Windows平台进程和线程ID实现
// ============================================================================

int32_t TFW_GetProcessId() {
    return (int32_t)_getpid();
}

uint64_t TFW_GetThreadId() {
    return (uint64_t)GetCurrentThreadId();
}
