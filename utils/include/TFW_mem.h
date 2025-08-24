#ifndef TFW_MEM_H
#define TFW_MEM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Memory management related definitions
// ============================================================================

// Maximum allocation memory size: 512MB
#define TFW_MAX_MALLOC_SIZE (512 * 1024 * 1024)

// ============================================================================
// Memory management function declarations
// ============================================================================

/**
 * Allocate memory of specified size
 * @param size Memory size to allocate (bytes)
 * @return Returns memory pointer on success, NULL on failure
 */
void* TFW_Malloc(uint32_t size);

/**
 * Allocate and zero memory of specified size
 * @param size Memory size to allocate (bytes)
 * @return Returns memory pointer on success, NULL on failure
 */
void* TFW_Calloc(uint32_t size);

/**
 * Free memory
 * @param ptr Memory pointer to free
 */
void TFW_Free(void* ptr);

/**
 * Get memory usage statistics
 * @param total_allocated Total allocated memory size (output parameter)
 * @param total_freed Total freed memory size (output parameter)
 * @param current_used Current used memory size (output parameter)
 * @return 0 for success, negative value for failure
 */
int32_t TFW_GetMemoryStats(uint64_t* total_allocated, uint64_t* total_freed, uint64_t* current_used);

#ifdef __cplusplus
}
#endif

#endif // TFW_MEM_H
