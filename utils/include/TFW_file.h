#ifndef TFW_FILE_H
#define TFW_FILE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// File path related function declarations
// ============================================================================

/**
 * Get file name (remove path)
 * @param file_path Complete file path
 * @return File name (without path)
 */
const char* TFW_GetFileName(const char* file_path);

/**
 * Get file extension
 * @param file_path File path
 * @return File extension (with dot), returns empty string if no extension
 */
const char* TFW_GetFileExtension(const char* file_path);

/**
 * Get file directory path
 * @param file_path Complete file path
 * @return Directory path (without filename)
 */
const char* TFW_GetFileDirectory(const char* file_path);

/**
 * Check if file exists
 * @param file_path File path
 * @return 1 for exists, 0 for not exists
 */
int32_t TFW_FileExists(const char* file_path);

/**
 * Check if path is directory
 * @param path Path
 * @return 1 for directory, 0 for not directory
 */
int32_t TFW_IsDirectory(const char* path);

#ifdef __cplusplus
}
#endif

#endif // TFW_FILE_H
