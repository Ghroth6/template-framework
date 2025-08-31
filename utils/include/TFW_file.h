#ifndef TFW_FILE_H
#define TFW_FILE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// File path related function declarations
// File path相关函数声明
// ============================================================================

/**
 * Get file name (remove path)
 * 获取文件名（去掉路径）
 * @param file_path Complete file path / 完整文件路径
 * @param filename Output filename buffer / 输出文件名缓冲区
 * @param buffer_size Buffer size / 缓冲区大小
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_GetFileName(const char* file_path, char* filename, size_t buffer_size);

/**
 * Get file extension
 * 获取文件扩展名
 * @param file_path File path / 文件路径
 * @param extension Output extension buffer / 输出扩展名缓冲区
 * @param buffer_size Buffer size / 缓冲区大小
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_GetFileExtension(const char* file_path, char* extension, size_t buffer_size);

/**
 * Get file directory path
 * 获取文件目录路径
 * @param file_path Complete file path / 完整文件路径
 * @param directory Output directory buffer / 输出目录路径缓冲区
 * @param buffer_size Buffer size / 缓冲区大小
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_GetFileDirectory(const char* file_path, char* directory, size_t buffer_size);

/**
 * Check if file exists
 * 检查文件是否存在
 * @param file_path File path
 * @return 1 for exists, 0 for not exists
 */
int32_t TFW_FileExists(const char* file_path);

/**
 * Check if path is directory
 * 检查路径是否是目录
 * @param path Path
 * @return 1 for directory, 0 for not directory
 */
int32_t TFW_IsDirectory(const char* path);

/**
 * Create directory
 * 创建目录
 * @param path Directory path to create
 * @return 1 for success, 0 for failure
 */
int32_t TFW_CreateDirectory(const char* path);

#ifdef __cplusplus
}
#endif

#endif // TFW_FILE_H
