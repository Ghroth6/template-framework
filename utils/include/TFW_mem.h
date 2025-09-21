#ifndef TFW_MEM_H
#define TFW_MEM_H

#include <stdint.h>
#include <stddef.h>

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
 * Duplicate string using TFW memory management
 * 使用TFW内存管理复制字符串
 * @param src Source string / 源字符串
 * @return Duplicated string pointer, NULL on failure / 复制的字符串指针，失败时返回NULL
 */
char* TFW_Strdup(const char* src);

/**
 * Get memory usage statistics
 * @param total_allocated Total allocated memory size (output parameter)
 * @param total_freed Total freed memory size (output parameter)
 * @param current_used Current used memory size (output parameter)
 * @return 0 for success, negative value for failure
 */
int32_t TFW_GetMemoryStats(uint64_t* total_allocated, uint64_t* total_freed, uint64_t* current_used);

// 安全函数包装器
int32_t TFW_Memset_S(void* dest, size_t destSize, int32_t c, size_t count);
int32_t TFW_Memcpy_S(void* dest, size_t destSize, const void* src, size_t count);
int32_t TFW_Strcpy_S(char* dest, size_t destSize, const char* src);
int32_t TFW_Strcat_S(char* dest, size_t destSize, const char* src);

// 字符串搜索函数的安全替代
/**
 * Find the last occurrence of a character in a string
 * 查找字符串中字符最后一次出现的位置
 * @param str String to search in / 要搜索的字符串
 * @param c Character to search for / 要查找的字符
 * @return Pointer to the last occurrence of c in str, or NULL if not found / 指向最后一次出现位置的指针，未找到则返回NULL
 */
char* TFW_Strrchr(const char* str, int c);

/**
 * Extract tokens from string (thread-safe)
 * 从字符串中提取标记（线程安全）
 * @param str String to tokenize / 要分词的字符串
 * @param delim Delimiter characters / 分隔符字符
 * @param saveptr Pointer to store context between calls / 用于存储调用间上下文的指针
 * @return Next token, or NULL if no more tokens / 下一个标记，如果没有更多标记则返回NULL
 */
char* TFW_Strtok_R(char* str, const char* delim, char** saveptr);

#ifdef __cplusplus
}
#endif

#endif // TFW_MEM_H