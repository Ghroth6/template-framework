#include "TFW_thread.h"

#include "TFW_utils_log.h"

/**
 * 安全的互斥锁加锁操作
 * Safe mutex lock operation
 * @param mutex 互斥锁指针
 * @return 0表示成功，负数表示错误
 */
int32_t TFW_Mutex_Lock_Safe(TFW_Mutex_t* mutex) {
    int32_t ret = TFW_SUCCESS;

    if (TFW_CheckMutexIsNull(mutex)) {
        TFW_LOGD_UTILS("TFW_Mutex_Lock mutex is null");
        ret = TFW_ERROR_INVALID_PARAM;
    } else {
        ret = TFW_Mutex_Lock_Inner(mutex);
        if (ret != 0) {
            TFW_LOGE_UTILS("TFW_Mutex_Lock failed, ret=%d", ret);
            ret = TFW_ERROR_LOCK_FAILED;
        }
    }

    return ret;
}

/**
 * 安全的互斥锁解锁操作
 * Safe mutex unlock operation
 * @param mutex 互斥锁指针
 * @return 0表示成功，负数表示错误
 */
int32_t TFW_Mutex_Unlock_Safe(TFW_Mutex_t* mutex) {
    int32_t ret = TFW_SUCCESS;

    if (TFW_CheckMutexIsNull(mutex)) {
        TFW_LOGE_UTILS("TFW_Mutex_Unlock mutex is null");
        ret = TFW_ERROR_INVALID_PARAM;
    } else {
        ret = TFW_Mutex_Unlock_Inner(mutex);
        if (ret != 0) {
            TFW_LOGE_UTILS("TFW_Mutex_Unlock failed, ret=%d", ret);
            ret = TFW_ERROR_LOCK_FAILED;
        }
    }

    return ret;
}
