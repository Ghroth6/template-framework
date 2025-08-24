#ifndef TFW_MEM_H
#define TFW_MEM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 内存管理相关定义
// ============================================================================

// 最大分配内存大小：512MB
#define TFW_MAX_MALLOC_SIZE (512 * 1024 * 1024)

// ============================================================================
// 内存管理函数声明
// ============================================================================

/**
 * 分配指定大小的内存
 * @param size 要分配的内存大小（字节）
 * @return 分配成功返回内存指针，失败返回NULL
 */
void* TFW_Malloc(uint32_t size);

/**
 * 分配并清零指定大小的内存
 * @param size 要分配的内存大小（字节）
 * @return 分配成功返回内存指针，失败返回NULL
 */
void* TFW_Calloc(uint32_t size);

/**
 * 释放内存
 * @param ptr 要释放的内存指针
 */
void TFW_Free(void* ptr);

/**
 * 获取内存使用统计信息
 * @param total_allocated 总分配内存大小（输出参数）
 * @param total_freed 总释放内存大小（输出参数）
 * @param current_used 当前使用内存大小（输出参数）
 * @return 0表示成功，负数表示失败
 */
int32_t TFW_GetMemoryStats(uint64_t* total_allocated, uint64_t* total_freed, uint64_t* current_used);

#ifdef __cplusplus
}
#endif

#endif // TFW_MEM_H
