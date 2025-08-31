#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "TFW_file.h"
#include "TFW_utils_log.h"
#include "TFW_errorno.h"
#include "TFW_common_defines.h"

// ============================================================================
// Windows平台文件操作实现
// Windows platform file operation implementation
// ============================================================================

int32_t TFW_GetFileName(const char* file_path, char* filename, size_t buffer_size) {
    if (file_path == NULL || filename == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：支持 '/' 和 '\' 分隔符
    // Windows platform: support '/' and '\' separator
    const char* name = strrchr(file_path, '/');
    if (name == NULL) {
        name = strrchr(file_path, '\\');
    }

    if (name == NULL) {
        // 没有路径分隔符，整个字符串就是文件名
        // No path separator, entire string is filename
        name = file_path;
    } else {
        // 跳过路径分隔符
        // Skip path separator
        name++;
    }

    // 检查缓冲区大小
    // Check buffer size
    size_t name_len = strlen(name);
    if (name_len >= buffer_size) {
        return TFW_ERROR;
    }

    // 复制文件名到缓冲区
    // Copy filename to buffer
    strcpy(filename, name);

    return TFW_SUCCESS;
}

int32_t TFW_GetFileExtension(const char* file_path, char* extension, size_t buffer_size) {
    if (file_path == NULL || extension == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // 先获取文件名
    // First get file name
    char filename[TFW_FILENAME_LEN_MAX];
    int32_t ret = TFW_GetFileName(file_path, filename, sizeof(filename));
    if (ret != TFW_SUCCESS) {
        return ret;
    }

    // 查找最后一个点号
    // Find the last dot
    const char* ext = strrchr(filename, '.');
    if (ext == NULL) {
        // 没有扩展名，返回空字符串
        // No extension, return empty string
        if (buffer_size < 1) {
            return TFW_ERROR;
        }
        extension[0] = '\0';
        return TFW_SUCCESS;
    }

    // 检查缓冲区大小
    // Check buffer size
    size_t ext_len = strlen(ext);
    if (ext_len >= buffer_size) {
        return TFW_ERROR;
    }

    // 复制扩展名到缓冲区
    // Copy extension to buffer
    strcpy(extension, ext);
    return TFW_SUCCESS;
}

int32_t TFW_GetFileDirectory(const char* file_path, char* directory, size_t buffer_size) {
    if (file_path == NULL || directory == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows平台：支持 '/' 和 '\' 分隔符
    // Windows platform: support '/' and '\' separator
    const char* last_slash = strrchr(file_path, '/');
    const char* last_backslash = strrchr(file_path, '\\');

    const char* last_sep = NULL;
    if (last_slash && last_backslash) {
        last_sep = (last_slash > last_backslash) ? last_slash : last_backslash;
    } else if (last_slash) {
        last_sep = last_slash;
    } else if (last_backslash) {
        last_sep = last_backslash;
    }

    if (last_sep) {
        // 计算目录路径长度
        // Calculate directory path length
        size_t len = last_sep - file_path;
        if (len >= buffer_size) {
            return TFW_ERROR;
        }

        // 复制目录路径到缓冲区
        // Copy directory path to buffer
        strncpy(directory, file_path, len);
        directory[len] = '\0';
        return TFW_SUCCESS;
    } else {
        // 没有路径分隔符，返回当前目录
        // No path separator, return current directory
        if (buffer_size < 2) {
            return TFW_ERROR;
        }
        strcpy(directory, ".");
        return TFW_SUCCESS;
    }
}

int32_t TFW_FileExists(const char* file_path) {
    if (file_path == NULL) {
        return 0;
    }

    // Windows平台：使用 GetFileAttributes
    // Windows platform: use GetFileAttributes
    DWORD attrs = GetFileAttributesA(file_path);
    return (attrs != INVALID_FILE_ATTRIBUTES) ? 1 : 0;
}

int32_t TFW_IsDirectory(const char* path) {
    if (path == NULL) {
        return 0;
    }

    // Windows平台：使用 GetFileAttributes
    // Windows platform: use GetFileAttributes
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return 0;
    }

    return (attrs & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}

int32_t TFW_CreateDirectory(const char* path) {
    if (path == NULL) {
        return 0;
    }

    // Windows平台：使用 CreateDirectoryA
    // Windows platform: use CreateDirectoryA
    if (CreateDirectoryA(path, NULL)) {
        return 1;
    }

    return 0;
}
