#include "../../include/TFW_mem.h"
#include "../../include/TFW_log.h"
#include "../../../interface/TFW_errorno.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Windows platform memory management implementation
// ============================================================================

// Memory statistics (thread-safe)
static volatile uint64_t g_total_allocated = 0;
static volatile uint64_t g_total_freed = 0;
static volatile uint64_t g_current_used = 0;

void* TFW_Malloc(uint32_t size) {
    // Check memory size limit
    if (size == 0 || size > TFW_MAX_MALLOC_SIZE) {
        TFW_LOGE_UTILS("Invalid memory size: %u bytes", size);
        return NULL;
    }

    // Windows platform: use malloc
    void* ptr = malloc(size);
    if (ptr != NULL) {
        // Update statistics
        InterlockedAdd64((LONG64*)&g_total_allocated, size);
        InterlockedAdd64((LONG64*)&g_current_used, size);
        TFW_LOGD_UTILS("Memory allocated: %u bytes at %p", size, ptr);
    } else {
        TFW_LOGE_UTILS("Memory allocation failed: %u bytes", size);
    }

    return ptr;
}

void* TFW_Calloc(uint32_t size) {
    // Check memory size limit
    if (size == 0 || size > TFW_MAX_MALLOC_SIZE) {
        TFW_LOGE_UTILS("Invalid memory size: %u bytes", size);
        return NULL;
    }

    // Windows platform: use calloc
    void* ptr = calloc(1, size);
    if (ptr != NULL) {
        // Update statistics
        InterlockedAdd64((LONG64*)&g_total_allocated, size);
        InterlockedAdd64((LONG64*)&g_current_used, size);
        TFW_LOGD_UTILS("Memory allocated and zeroed: %u bytes at %p", size, ptr);
    } else {
        TFW_LOGE_UTILS("Memory allocation failed: %u bytes", size);
    }

    return ptr;
}

void TFW_Free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // Windows platform: use free
    // Note: Cannot get freed memory size here, statistics may be inaccurate
    // In practical applications, may need to maintain a memory block info table
    free(ptr);

    TFW_LOGD_UTILS("Memory freed at %p", ptr);
    // Note: Cannot accurately update statistics here, as freed memory size is unknown
}

char* TFW_Strdup(const char* src) {
    if (src == NULL) {
        TFW_LOGE_UTILS("TFW_Strdup: source string is NULL");
        return NULL;
    }

    size_t len = strlen(src);
    char* dest = (char*)TFW_Malloc((uint32_t)(len + 1));
    if (dest == NULL) {
        TFW_LOGE_UTILS("TFW_Strdup: memory allocation failed for %zu bytes", len + 1);
        return NULL;
    }

    strcpy(dest, src);
    TFW_LOGD_UTILS("String duplicated: '%s' at %p", src, dest);
    return dest;
}



int32_t TFW_GetMemoryStats(uint64_t* total_allocated, uint64_t* total_freed, uint64_t* current_used) {
    if (total_allocated == NULL || total_freed == NULL || current_used == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // 返回当前统计信息
    *total_allocated = g_total_allocated;
    *total_freed = g_total_freed;
    *current_used = g_current_used;

    return TFW_SUCCESS;
}

// 安全函数包装器实现
int32_t TFW_Memset_S(void* dest, size_t destSize, int32_t c, size_t count) {
    if (dest == NULL || destSize == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (count > destSize) {
        return TFW_ERROR_INVALID_PARAM;  // 长度不满足时应该报错停止
    }

    memset(dest, c, count);
    return TFW_SUCCESS;
}

int32_t TFW_Memcpy_S(void* dest, size_t destSize, const void* src, size_t count) {
    if (dest == NULL || src == NULL || destSize == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    if (count > destSize) {
        return TFW_ERROR_INVALID_PARAM;  // 长度不满足时应该报错停止
    }

    memcpy(dest, src, count);
    return TFW_SUCCESS;
}

int32_t TFW_Strcpy_S(char* dest, size_t destSize, const char* src) {
    if (dest == NULL || src == NULL || destSize == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    size_t srcLen = strlen(src);
    if (srcLen >= destSize) {
        return TFW_ERROR_INVALID_PARAM;  // 长度不满足时应该报错停止
    }

    memcpy(dest, src, srcLen);
    dest[srcLen] = '\0';
    return TFW_SUCCESS;
}

int32_t TFW_Strcat_S(char* dest, size_t destSize, const char* src) {
    if (dest == NULL || src == NULL || destSize == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    size_t destLen = strlen(dest);
    size_t srcLen = strlen(src);

    if (destLen + srcLen >= destSize) {
        return TFW_ERROR_INVALID_PARAM;  // 长度不满足时应该报错停止
    }

    memcpy(dest + destLen, src, srcLen);
    return TFW_SUCCESS;
}
