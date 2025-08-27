#include "../../include/TFW_file.h"
#include "../../include/TFW_log.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// Windows平台文件操作实现
// ============================================================================

const char* TFW_GetFileName(const char* file_path) {
    if (file_path == NULL) {
        return NULL;
    }

    // Windows平台：支持 '/' 和 '\' 分隔符
    const char* filename = strrchr(file_path, '/');
    if (filename == NULL) {
        filename = strrchr(file_path, '\\');
    }

    return filename ? filename + 1 : file_path;
}

const char* TFW_GetFileExtension(const char* file_path) {
    if (file_path == NULL) {
        return "";
    }

    // 先获取文件名
    const char* filename = TFW_GetFileName(file_path);
    if (filename == NULL) {
        return "";
    }

    // 查找最后一个点号
    const char* ext = strrchr(filename, '.');
    return ext ? ext : "";
}

const char* TFW_GetFileDirectory(const char* file_path) {
    if (file_path == NULL) {
        return NULL;
    }

    // Windows平台：支持 '/' 和 '\' 分隔符
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
        // 创建目录路径的副本
        static char dir_path[1024];
        size_t len = last_sep - file_path;
        if (len < sizeof(dir_path)) {
            strncpy(dir_path, file_path, len);
            dir_path[len] = '\0';
            return dir_path;
        }
    }

    return file_path;
}

int32_t TFW_FileExists(const char* file_path) {
    if (file_path == NULL) {
        return 0;
    }

    // Windows平台：使用 GetFileAttributes
    DWORD attrs = GetFileAttributesA(file_path);
    return (attrs != INVALID_FILE_ATTRIBUTES) ? 1 : 0;
}

int32_t TFW_IsDirectory(const char* path) {
    if (path == NULL) {
        return 0;
    }

    // Windows平台：使用 GetFileAttributes
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
    if (CreateDirectoryA(path, NULL)) {
        return 1;
    }

    return 0;
}
