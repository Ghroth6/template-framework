#include <process.h>
#include <windows.h>
#include <string.h>

#include "TFW_errorno.h"
#include "TFW_thread.h"
#include "TFW_utils_log.h"

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
// Windows平台条件变量实现
// Windows platform condition variable implementation
// ============================================================================

int32_t TFW_Cond_Init(TFW_Cond_t* cond) {
    if (cond == NULL) {
        TFW_LOGE_UTILS("TFW_Cond_Init cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：创建条件变量
    CONDITION_VARIABLE* cv = (CONDITION_VARIABLE*)TFW_Malloc(sizeof(CONDITION_VARIABLE));
    if (cv == NULL) {
        TFW_LOGE_UTILS("TFW_Cond_Init malloc failed");
        return TFW_ERROR_MALLOC_ERR;
    }

    InitializeConditionVariable(cv);
    *cond = (TFW_Cond_t)cv;
    return TFW_SUCCESS;
}

int32_t TFW_Cond_Signal(TFW_Cond_t* cond) {
    if ((cond == NULL) || ((void*)(*cond) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Signal cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    WakeConditionVariable((CONDITION_VARIABLE*)*cond);
    return TFW_SUCCESS;
}

int32_t TFW_Cond_Broadcast(TFW_Cond_t* cond) {
    if ((cond == NULL) || ((void*)(*cond) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Broadcast cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    WakeAllConditionVariable((CONDITION_VARIABLE*)*cond);
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

    if (time == NULL) {
        // 无限期等待
        BOOL result = SleepConditionVariableCS((CONDITION_VARIABLE*)*cond,
                                                (CRITICAL_SECTION*)*mutex,
                                                INFINITE);
        if (!result) {
            TFW_LOGE_UTILS("TFW_Cond_Wait SleepConditionVariableCS failed");
            return TFW_ERROR;
        }
    } else {
        // 计算超时时间（以毫秒为单位）
        TFW_SysTime now;
        TFW_GetTime(&now);

        int64_t timeout_ms = (time->sec - now.sec) * 1000 + (time->nsec - now.nsec) / 1000000;
        if (timeout_ms < 0) {
            timeout_ms = 0;
        }

        BOOL result = SleepConditionVariableCS((CONDITION_VARIABLE*)*cond,
                                                (CRITICAL_SECTION*)*mutex,
                                                (DWORD)timeout_ms);
        if (!result) {
            if (GetLastError() == ERROR_TIMEOUT) {
                TFW_LOGD_UTILS("TFW_Cond_Wait timeout");
                return TFW_ERROR_TIMEOUT;
            } else {
                TFW_LOGE_UTILS("TFW_Cond_Wait SleepConditionVariableCS failed, error=%lu", GetLastError());
                return TFW_ERROR;
            }
        }
    }

    return TFW_SUCCESS;
}

int32_t TFW_Cond_Destroy(TFW_Cond_t* cond) {
    if ((cond == NULL) || ((void*)(*cond) == NULL)) {
        TFW_LOGE_UTILS("TFW_Cond_Destroy cond is null");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：条件变量不需要显式销毁
    // Windows platform: condition variables do not need explicit destruction
    TFW_Free((void*)*cond);
    *cond = (TFW_Cond_t)NULL;
    return TFW_SUCCESS;
}

// ============================================================================
// Windows平台线程属性实现
// Windows platform thread attribute implementation
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
// Windows平台线程实现
// Windows platform thread implementation
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

    // Windows平台：创建线程
    HANDLE handle = CreateThread(NULL,
                                attr ? attr->stackSize : 0,
                                (LPTHREAD_START_ROUTINE)threadEntry,
                                arg,
                                0,
                                NULL);
    if (handle == NULL) {
        TFW_LOGE_UTILS("TFW_Thread_Create CreateThread failed");
        return TFW_ERROR;
    }

    *thread = (TFW_Thread_t)handle;

    if (attr && attr->name) {
        // Windows平台：设置线程名称
        HRESULT hr = SetThreadDescription(handle, attr->name);
        if (FAILED(hr)) {
            TFW_LOGE_UTILS("TFW_Thread_Create SetThreadDescription failed, hr=0x%08x", hr);
        }
    }

    return TFW_SUCCESS;
}

int32_t TFW_Thread_Join(TFW_Thread_t thread, void** value) {
    if (thread <= 0) {
        TFW_LOGE_UTILS("TFW_Thread_Join thread is invalid");
        return TFW_ERROR_INVALID_PARAM;
    }

    DWORD result = WaitForSingleObject((HANDLE)thread, INFINITE);
    if (result != WAIT_OBJECT_0) {
        TFW_LOGE_UTILS("TFW_Thread_Join WaitForSingleObject failed, result=%lu", result);
        return TFW_ERROR;
    }

    // Windows平台：关闭线程句柄
    CloseHandle((HANDLE)thread);
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

    // Windows平台：设置线程描述
    HRESULT hr = SetThreadDescription((HANDLE)thread, name);
    if (FAILED(hr)) {
        TFW_LOGE_UTILS("TFW_Thread_SetName SetThreadDescription failed, hr=0x%08x", hr);
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

TFW_Thread_t TFW_Thread_GetSelf(void) {
    return (TFW_Thread_t)GetCurrentThread();
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
