#ifndef TFW_FILE_H
#define TFW_FILE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 文件路径相关函数声明
// ============================================================================

/**
 * 获取文件名（去掉路径）
 * @param file_path 完整文件路径
 * @return 文件名（不包含路径）
 */
const char* TFW_GetFileName(const char* file_path);

/**
 * 获取文件扩展名
 * @param file_path 文件路径
 * @return 文件扩展名（包含点号），如果没有扩展名返回空字符串
 */
const char* TFW_GetFileExtension(const char* file_path);

/**
 * 获取文件目录路径
 * @param file_path 完整文件路径
 * @return 目录路径（不包含文件名）
 */
const char* TFW_GetFileDirectory(const char* file_path);

/**
 * 检查文件是否存在
 * @param file_path 文件路径
 * @return 1表示存在，0表示不存在
 */
int32_t TFW_FileExists(const char* file_path);

/**
 * 检查路径是否为目录
 * @param path 路径
 * @return 1表示是目录，0表示不是目录
 */
int32_t TFW_IsDirectory(const char* path);

#ifdef __cplusplus
}
#endif

#endif // TFW_FILE_H
