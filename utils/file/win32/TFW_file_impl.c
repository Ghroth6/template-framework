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
bool TFW_IsFileExists(const char* file_path) {
    if (file_path == NULL) {
        return false;
    }

    // Windows平台：使用 GetFileAttributes
    // Windows platform: use GetFileAttributes
    DWORD attrs = GetFileAttributesA(file_path);
    return (attrs != INVALID_FILE_ATTRIBUTES) ? true : false;
}

bool TFW_IsDirectory(const char* path) {
    if (path == NULL) {
        return false;
    }

    // Windows平台：使用 GetFileAttributes
    // Windows platform: use GetFileAttributes
    DWORD attrs = GetFileAttributesA(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    return (attrs & FILE_ATTRIBUTE_DIRECTORY) ? true : false;
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

// ============================================================================
// 新增的文件操作函数实现
// Implementation of new file operation functions
// ============================================================================

int32_t TFW_ReadFile(int32_t fd, void *readBuf, uint32_t maxLen) {
    if (readBuf == NULL || maxLen == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ReadFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    HANDLE hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Invalid file handle for TFW_ReadFile");
        return TFW_ERROR;
    }

    DWORD bytesRead = 0;
    if (!ReadFile(hFile, readBuf, maxLen, &bytesRead, NULL)) {
        TFW_LOGE_UTILS("Failed to read from file handle, error code: %lu", GetLastError());
        return TFW_ERROR;
    }

    return (int32_t)bytesRead;
}

int32_t TFW_ReadFullFile(const char *fileName, char *readBuf, uint32_t maxLen) {
    if (fileName == NULL || readBuf == NULL || maxLen == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ReadFullFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Failed to open file %s, error code: %lu", fileName, GetLastError());
        return TFW_ERROR;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        TFW_LOGE_UTILS("Failed to get file size for %s, error code: %lu", fileName, GetLastError());
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    if (fileSize >= maxLen) {
        TFW_LOGE_UTILS("Buffer too small for file %s", fileName);
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    DWORD bytesRead = 0;
    if (!ReadFile(hFile, readBuf, fileSize, &bytesRead, NULL)) {
        TFW_LOGE_UTILS("Failed to read file %s, error code: %lu", fileName, GetLastError());
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    readBuf[bytesRead] = '\0'; // 确保字符串终止
    CloseHandle(hFile);
    return (int32_t)bytesRead;
}

int32_t TFW_WriteFile(const char *fileName, const char *writeBuf, uint32_t len) {
    if (fileName == NULL || writeBuf == NULL || len == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_WriteFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Failed to open file %s, error code: %lu", fileName, GetLastError());
        return TFW_ERROR;
    }

    DWORD bytesWritten = 0;
    if (!WriteFile(hFile, writeBuf, len, &bytesWritten, NULL)) {
        TFW_LOGE_UTILS("Failed to write to file %s, error code: %lu", fileName, GetLastError());
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    CloseHandle(hFile);
    return (int32_t)bytesWritten;
}

int32_t TFW_WriteFileFd(int32_t fd, const char *writeBuf, uint32_t len) {
    if (writeBuf == NULL || len == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_WriteFileFd");
        return TFW_ERROR_INVALID_PARAM;
    }

    HANDLE hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Invalid file handle for TFW_WriteFileFd");
        return TFW_ERROR;
    }

    DWORD bytesWritten = 0;
    if (!WriteFile(hFile, writeBuf, len, &bytesWritten, NULL)) {
        TFW_LOGE_UTILS("Failed to write to file handle, error code: %lu", GetLastError());
        return TFW_ERROR;
    }

    return (int32_t)bytesWritten;
}

int32_t TFW_OpenFile(const char *fileName, int32_t flags) {
    if (fileName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_OpenFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    // 将TFW标志转换为Windows标志
    DWORD desiredAccess = 0;
    DWORD creationDisposition = 0;

    if (flags & TFW_O_RDONLY) desiredAccess |= GENERIC_READ;
    if (flags & TFW_O_WRONLY) desiredAccess |= GENERIC_WRITE;
    if (flags & TFW_O_RDWR) {
        desiredAccess |= GENERIC_READ | GENERIC_WRITE;
    }

    if (flags & TFW_O_CREATE) {
        creationDisposition = OPEN_ALWAYS;
    } else if (flags & TFW_O_TRUNC) {
        creationDisposition = TRUNCATE_EXISTING;
    } else {
        creationDisposition = OPEN_EXISTING;
    }

    HANDLE hFile = CreateFileA(
        fileName,
        desiredAccess,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        creationDisposition,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Failed to open file %s, error code: %lu", fileName, GetLastError());
        return TFW_ERROR;
    }

    // 将HANDLE转换为文件描述符
    int fd = _open_osfhandle((intptr_t)hFile, 0);
    if (fd == -1) {
        TFW_LOGE_UTILS("Failed to convert handle to file descriptor for %s", fileName);
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    return fd;
}

int32_t TFW_OpenFileWithPerms(const char *fileName, int32_t flags, int32_t perms) {
    if (fileName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_OpenFileWithPerms");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows中权限处理与POSIX略有不同，这里简化处理
    // 将TFW标志转换为Windows标志
    DWORD desiredAccess = 0;
    DWORD creationDisposition = 0;

    if (flags & TFW_O_RDONLY) desiredAccess |= GENERIC_READ;
    if (flags & TFW_O_WRONLY) desiredAccess |= GENERIC_WRITE;
    if (flags & TFW_O_RDWR) {
        desiredAccess |= GENERIC_READ | GENERIC_WRITE;
    }

    if (flags & TFW_O_CREATE) {
        creationDisposition = OPEN_ALWAYS;
    } else if (flags & TFW_O_TRUNC) {
        creationDisposition = TRUNCATE_EXISTING;
    } else {
        creationDisposition = OPEN_EXISTING;
    }

    HANDLE hFile = CreateFileA(
        fileName,
        desiredAccess,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        creationDisposition,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Failed to open file %s, error code: %lu", fileName, GetLastError());
        return TFW_ERROR;
    }

    // 将HANDLE转换为文件描述符
    int fd = _open_osfhandle((intptr_t)hFile, 0);
    if (fd == -1) {
        TFW_LOGE_UTILS("Failed to convert handle to file descriptor for %s", fileName);
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    return fd;
}

void TFW_RemoveFile(const char *fileName) {
    if (fileName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_RemoveFile");
        return;
    }

    if (!DeleteFileA(fileName)) {
        TFW_LOGE_UTILS("Failed to remove file %s, error code: %lu", fileName, GetLastError());
    }
}

void TFW_CloseFile(int32_t fd) {
    if (fd < 0) {
        TFW_LOGE_UTILS("Invalid file descriptor for TFW_CloseFile");
        return;
    }

    HANDLE hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Invalid file handle for TFW_CloseFile");
        return;
    }

    if (!CloseHandle(hFile)) {
        TFW_LOGE_UTILS("Failed to close file handle, error code: %lu", GetLastError());
    }
}

int64_t TFW_PreadFile(int32_t fd, void *buf, uint64_t readBytes, uint64_t offset) {
    if (buf == NULL || readBytes == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_PreadFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    HANDLE hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Invalid file handle for TFW_PreadFile");
        return TFW_ERROR;
    }

    // 设置文件指针位置
    LARGE_INTEGER liDistanceToMove;
    liDistanceToMove.QuadPart = offset;
    if (!SetFilePointerEx(hFile, liDistanceToMove, NULL, FILE_BEGIN)) {
        TFW_LOGE_UTILS("Failed to set file pointer, error code: %lu", GetLastError());
        return TFW_ERROR;
    }

    DWORD bytesRead = 0;
    if (!ReadFile(hFile, buf, (DWORD)readBytes, &bytesRead, NULL)) {
        TFW_LOGE_UTILS("Failed to read from file handle, error code: %lu", GetLastError());
        return TFW_ERROR;
    }

    return (int64_t)bytesRead;
}

int64_t TFW_PwriteFile(int32_t fd, const void *buf, uint64_t writeBytes, uint64_t offset) {
    if (buf == NULL || writeBytes == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_PwriteFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    HANDLE hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Invalid file handle for TFW_PwriteFile");
        return TFW_ERROR;
    }

    // 设置文件指针位置
    LARGE_INTEGER liDistanceToMove;
    liDistanceToMove.QuadPart = offset;
    if (!SetFilePointerEx(hFile, liDistanceToMove, NULL, FILE_BEGIN)) {
        TFW_LOGE_UTILS("Failed to set file pointer, error code: %lu", GetLastError());
        return TFW_ERROR;
    }

    DWORD bytesWritten = 0;
    if (!WriteFile(hFile, buf, (DWORD)writeBytes, &bytesWritten, NULL)) {
        TFW_LOGE_UTILS("Failed to write to file handle, error code: %lu", GetLastError());
        return TFW_ERROR;
    }

    return (int64_t)bytesWritten;
}

int32_t TFW_AccessFile(const char *pathName, int32_t mode) {
    if (pathName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_AccessFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    DWORD fileAttributes = GetFileAttributesA(pathName);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        return TFW_ERROR;
    }

    // Windows中权限检查与POSIX略有不同，这里简化处理
    // 文件存在即返回成功
    return TFW_SUCCESS;
}

int32_t TFW_MakeDir(const char *pathName, int32_t mode) {
    if (pathName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_MakeDir");
        return TFW_ERROR_INVALID_PARAM;
    }

    // Windows中权限处理与POSIX略有不同，这里忽略mode参数
    if (!CreateDirectoryA(pathName, NULL)) {
        TFW_LOGE_UTILS("Failed to create directory %s, error code: %lu", pathName, GetLastError());
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

int32_t TFW_GetFileSize(const char *fileName, uint64_t *fileSize) {
    if (fileName == NULL || fileSize == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_GetFileSize");
        return TFW_ERROR_INVALID_PARAM;
    }

    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Failed to open file %s, error code: %lu", fileName, GetLastError());
        return TFW_ERROR;
    }

    LARGE_INTEGER liFileSize;
    if (!GetFileSizeEx(hFile, &liFileSize)) {
        TFW_LOGE_UTILS("Failed to get file size for %s, error code: %lu", fileName, GetLastError());
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    *fileSize = liFileSize.QuadPart;
    CloseHandle(hFile);
    return TFW_SUCCESS;
}

char *TFW_RealPath(const char *path, char *absPath) {
    if (path == NULL || absPath == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_RealPath");
        return NULL;
    }

    DWORD result = GetFullPathNameA(path, MAX_PATH, absPath, NULL);
    if (result == 0) {
        TFW_LOGE_UTILS("Failed to get real path for %s, error code: %lu", path, GetLastError());
        return NULL;
    }

    return absPath;
}

int32_t TFW_ReadFullFileAndSize(const char *fileName, char *readBuf, uint32_t maxLen, int32_t *size) {
    if (fileName == NULL || readBuf == NULL || size == NULL || maxLen == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ReadFullFileAndSize");
        return TFW_ERROR_INVALID_PARAM;
    }

    uint64_t fileSize;
    int32_t ret = TFW_GetFileSize(fileName, &fileSize);
    if (ret != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Failed to get file size for %s", fileName);
        return ret;
    }

    if (fileSize >= maxLen) {
        TFW_LOGE_UTILS("Buffer too small for file %s", fileName);
        return TFW_ERROR;
    }

    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        TFW_LOGE_UTILS("Failed to open file %s, error code: %lu", fileName, GetLastError());
        return TFW_ERROR;
    }

    DWORD bytesRead = 0;
    if (!ReadFile(hFile, readBuf, (DWORD)fileSize, &bytesRead, NULL)) {
        TFW_LOGE_UTILS("Failed to read file %s, error code: %lu", fileName, GetLastError());
        CloseHandle(hFile);
        return TFW_ERROR;
    }

    readBuf[bytesRead] = '\0'; // 确保字符串终止
    *size = (int32_t)bytesRead;
    CloseHandle(hFile);
    return TFW_SUCCESS;
}