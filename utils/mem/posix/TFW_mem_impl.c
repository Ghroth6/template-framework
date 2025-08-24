#include "../../include/TFW_mem.h"
#include "../../include/TFW_log.h"
#include "../../../interface/TFW_errorno.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// ============================================================================
// POSIX平台内存管理实现
// ============================================================================

// 内存统计信息（线程安全）
static uint64_t g_total_allocated = 0;
static uint64_t g_total_freed = 0;
static uint64_t g_current_used = 0;
static pthread_mutex_t g_mem_stats_mutex = PTHREAD_MUTEX_INITIALIZER;

void* TFW_Malloc(uint32_t size) {
    // 检查内存大小限制
    if (size == 0 || size > TFW_MAX_MALLOC_SIZE) {
        TFW_LOGE_UTILS("Invalid memory size: %u bytes", size);
        return NULL;
    }
    
    // POSIX平台：使用 malloc
    void* ptr = malloc(size);
    if (ptr != NULL) {
        // 更新统计信息（线程安全）
        pthread_mutex_lock(&g_mem_stats_mutex);
        g_total_allocated += size;
        g_current_used += size;
        pthread_mutex_unlock(&g_mem_stats_mutex);
        
        TFW_LOGD_UTILS("Memory allocated: %u bytes at %p", size, ptr);
    } else {
        TFW_LOGE_UTILS("Memory allocation failed: %u bytes", size);
    }
    
    return ptr;
}

void* TFW_Calloc(uint32_t size) {
    // 检查内存大小限制
    if (size == 0 || size > TFW_MAX_MALLOC_SIZE) {
        TFW_LOGE_UTILS("Invalid memory size: %u bytes", size);
        return NULL;
    }
    
    // POSIX平台：使用 calloc
    void* ptr = calloc(1, size);
    if (ptr != NULL) {
        // 更新统计信息（线程安全）
        pthread_mutex_lock(&g_mem_stats_mutex);
        g_total_allocated += size;
        g_current_used += size;
        pthread_mutex_unlock(&g_mem_stats_mutex);
        
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
    
    // POSIX平台：使用 free
    // 注意：这里无法获取释放的内存大小，统计信息可能不准确
    // 在实际应用中，可能需要维护一个内存块信息表
    free(ptr);
    
    TFW_LOGD_UTILS("Memory freed at %p", ptr);
    // 注意：这里无法准确更新统计信息，因为不知道释放的内存大小
}



int32_t TFW_GetMemoryStats(uint64_t* total_allocated, uint64_t* total_freed, uint64_t* current_used) {
    if (total_allocated == NULL || total_freed == NULL || current_used == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }
    
    // 返回当前统计信息（线程安全）
    pthread_mutex_lock(&g_mem_stats_mutex);
    *total_allocated = g_total_allocated;
    *total_freed = g_total_freed;
    *current_used = g_current_used;
    pthread_mutex_unlock(&g_mem_stats_mutex);
    
    return TFW_SUCCESS;
}
