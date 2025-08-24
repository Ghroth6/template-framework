#ifndef TFW_THREAD_H
#define TFW_THREAD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../../interface/TFW_errorno.h"
#include "TFW_utils_log.h"

typedef uintptr_t TFW_Mutex_t;
typedef uintptr_t TFW_MutexAttr_t;

// Basic mutex functions
int32_t TFW_MutexAttr_Init(TFW_MutexAttr_t* mutexAttr);
int32_t TFW_Mutex_Init(TFW_Mutex_t* mutex, TFW_MutexAttr_t* mutexAttr);
int32_t TFW_Mutex_Lock_Inner(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Unlock_Inner(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Destroy(TFW_Mutex_t* mutex);

// Inline check function
static inline bool TFW_CheckMutexIsNull(const TFW_Mutex_t* mutex)
{
    return (mutex == NULL) || ((void *)(*mutex) == NULL);
}

// Safe lock operation function declarations
int32_t TFW_Mutex_Lock_Safe(TFW_Mutex_t* mutex);
int32_t TFW_Mutex_Unlock_Safe(TFW_Mutex_t* mutex);

// Macro definitions for safe lock operations
#define TFW_Mutex_Lock(mutex) TFW_Mutex_Lock_Safe(mutex)
#define TFW_Mutex_Unlock(mutex) TFW_Mutex_Unlock_Safe(mutex)

// ============================================================================
// Process and thread related function declarations
// ============================================================================

/**
 * Get current process ID
 * @return Current process ID
 */
int32_t TFW_GetProcessId();

/**
 * Get current thread ID
 * @return Current thread ID
 */
uint64_t TFW_GetThreadId();

#endif // TFW_THREAD_H
