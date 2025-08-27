#include "../../include/TFW_file.h"
#include "../../include/TFW_log.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

// ============================================================================
// POSIX平台文件操作实现
// ============================================================================

const char* TFW_GetFileName(const char* file_path) {
    if (file_path == NULL) {
        return NULL;
    }
    
    // POSIX平台：使用 '/' 分隔符
    const char* filename = strrchr(file_path, '/');
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
    
    // POSIX平台：使用 '/' 分隔符
    const char* last_slash = strrchr(file_path, '/');
    
    if (last_slash) {
        // 创建目录路径的副本
        static char dir_path[1024];
        size_t len = last_slash - file_path;
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
    
    // POSIX平台：使用 access 函数
    return (access(file_path, F_OK) == 0) ? 1 : 0;
}

int32_t TFW_IsDirectory(const char* path) {
    if (path == NULL) {
        return 0;
    }
    
    // POSIX平台：使用 stat 函数
    struct stat st;
    if (stat(path, &st) != 0) {
        return 0;
    }
    
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

int32_t TFW_CreateDirectory(const char* path) {
    if (path == NULL) {
        return 0;
    }
    
    // POSIX平台：使用 mkdir 函数
    if (mkdir(path, 0755) == 0) {
        return 1;
    }
    
    return 0;
}
