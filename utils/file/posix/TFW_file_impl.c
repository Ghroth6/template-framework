#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#include "TFW_file.h"
#include "TFW_utils_log.h"
#include "TFW_errorno.h"
#include "TFW_common_defines.h"

// ============================================================================
// POSIX平台文件操作实现
// POSIX platform file operation implementation
// ============================================================================

int32_t TFW_GetFileName(const char* file_path, char* filename, size_t buffer_size) {
    if (file_path == NULL || filename == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // POSIX平台：使用 '/' 分隔符
    // POSIX platform: use '/' separator
    const char* name = strrchr(file_path, '/');

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

    // POSIX平台：使用 '/' 分隔符
    // POSIX platform: use '/' separator
    const char* last_slash = strrchr(file_path, '/');

    if (last_slash) {
        // 计算目录路径长度
        // Calculate directory path length
        size_t len = last_slash - file_path;
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

    // POSIX平台：使用 access 函数
    // POSIX platform: use access function
    return (access(file_path, F_OK) == 0) ? true : false;
}

bool TFW_IsDirectory(const char* path) {
    if (path == NULL) {
        return false;
    }

    // POSIX平台：使用 stat 函数
    // POSIX platform: use stat function
    struct stat st;
    if (stat(path, &st) != 0) {
        return false;
    }

    return S_ISDIR(st.st_mode) ? true : false;
}

int32_t TFW_CreateDirectory(const char* path) {
    if (path == NULL) {
        return 0;
    }

    // POSIX平台：使用 mkdir 函数
    // POSIX platform: use mkdir function
    if (mkdir(path, 0755) == 0) {
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

    ssize_t result = read(fd, readBuf, maxLen);
    if (result == -1) {
        TFW_LOGE_UTILS("Failed to read from file descriptor: %s", strerror(errno));
        return TFW_ERROR;
    }

    return (int32_t)result;
}

int32_t TFW_ReadFullFile(const char *fileName, char *readBuf, uint32_t maxLen) {
    if (fileName == NULL || readBuf == NULL || maxLen == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_ReadFullFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        TFW_LOGE_UTILS("Failed to open file %s: %s", fileName, strerror(errno));
        return TFW_ERROR;
    }

    ssize_t bytesRead = read(fd, readBuf, maxLen - 1); // 保留一个字节用于null终止符
    if (bytesRead == -1) {
        TFW_LOGE_UTILS("Failed to read file %s: %s", fileName, strerror(errno));
        close(fd);
        return TFW_ERROR;
    }

    readBuf[bytesRead] = '\0'; // 确保字符串终止
    close(fd);
    return (int32_t)bytesRead;
}

int32_t TFW_WriteFile(const char *fileName, const char *writeBuf, uint32_t len) {
    if (fileName == NULL || writeBuf == NULL || len == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_WriteFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        TFW_LOGE_UTILS("Failed to open file %s: %s", fileName, strerror(errno));
        return TFW_ERROR;
    }

    ssize_t bytesWritten = write(fd, writeBuf, len);
    if (bytesWritten == -1) {
        TFW_LOGE_UTILS("Failed to write to file %s: %s", fileName, strerror(errno));
        close(fd);
        return TFW_ERROR;
    }

    close(fd);
    return (int32_t)bytesWritten;
}

int32_t TFW_WriteFileFd(int32_t fd, const char *writeBuf, uint32_t len) {
    if (writeBuf == NULL || len == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_WriteFileFd");
        return TFW_ERROR_INVALID_PARAM;
    }

    ssize_t bytesWritten = write(fd, writeBuf, len);
    if (bytesWritten == -1) {
        TFW_LOGE_UTILS("Failed to write to file descriptor: %s", strerror(errno));
        return TFW_ERROR;
    }

    return (int32_t)bytesWritten;
}

int32_t TFW_OpenFile(const char *fileName, int32_t flags) {
    if (fileName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_OpenFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    // 将TFW标志转换为POSIX标志
    int posixFlags = 0;
    if (flags & TFW_O_RDONLY) posixFlags |= O_RDONLY;
    if (flags & TFW_O_WRONLY) posixFlags |= O_WRONLY;
    if (flags & TFW_O_RDWR) posixFlags |= O_RDWR;
    if (flags & TFW_O_CREATE) posixFlags |= O_CREAT;
    if (flags & TFW_O_TRUNC) posixFlags |= O_TRUNC;

    int fd = open(fileName, posixFlags, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        TFW_LOGE_UTILS("Failed to open file %s: %s", fileName, strerror(errno));
        return TFW_ERROR;
    }

    return fd;
}

int32_t TFW_OpenFileWithPerms(const char *fileName, int32_t flags, int32_t perms) {
    if (fileName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_OpenFileWithPerms");
        return TFW_ERROR_INVALID_PARAM;
    }

    // 将TFW标志转换为POSIX标志
    int posixFlags = 0;
    if (flags & TFW_O_RDONLY) posixFlags |= O_RDONLY;
    if (flags & TFW_O_WRONLY) posixFlags |= O_WRONLY;
    if (flags & TFW_O_RDWR) posixFlags |= O_RDWR;
    if (flags & TFW_O_CREATE) posixFlags |= O_CREAT;
    if (flags & TFW_O_TRUNC) posixFlags |= O_TRUNC;

    // 将TFW权限转换为POSIX权限
    mode_t posixPerms = 0;
    if (perms & TFW_S_IRUSR) posixPerms |= S_IRUSR;
    if (perms & TFW_S_IWUSR) posixPerms |= S_IWUSR;
    if (perms & TFW_S_IXUSR) posixPerms |= S_IXUSR;

    int fd = open(fileName, posixFlags, posixPerms);
    if (fd == -1) {
        TFW_LOGE_UTILS("Failed to open file %s: %s", fileName, strerror(errno));
        return TFW_ERROR;
    }

    return fd;
}

void TFW_RemoveFile(const char *fileName) {
    if (fileName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_RemoveFile");
        return;
    }

    if (unlink(fileName) == -1) {
        TFW_LOGE_UTILS("Failed to remove file %s: %s", fileName, strerror(errno));
    }
}

void TFW_CloseFile(int32_t fd) {
    if (fd < 0) {
        TFW_LOGE_UTILS("Invalid file descriptor for TFW_CloseFile");
        return;
    }

    if (close(fd) == -1) {
        TFW_LOGE_UTILS("Failed to close file descriptor: %s", strerror(errno));
    }
}

int64_t TFW_PreadFile(int32_t fd, void *buf, uint64_t readBytes, uint64_t offset) {
    if (buf == NULL || readBytes == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_PreadFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    ssize_t result = pread(fd, buf, readBytes, offset);
    if (result == -1) {
        TFW_LOGE_UTILS("Failed to pread from file descriptor: %s", strerror(errno));
        return TFW_ERROR;
    }

    return (int64_t)result;
}

int64_t TFW_PwriteFile(int32_t fd, const void *buf, uint64_t writeBytes, uint64_t offset) {
    if (buf == NULL || writeBytes == 0) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_PwriteFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    ssize_t result = pwrite(fd, buf, writeBytes, offset);
    if (result == -1) {
        TFW_LOGE_UTILS("Failed to pwrite to file descriptor: %s", strerror(errno));
        return TFW_ERROR;
    }

    return (int64_t)result;
}

int32_t TFW_AccessFile(const char *pathName, int32_t mode) {
    if (pathName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_AccessFile");
        return TFW_ERROR_INVALID_PARAM;
    }

    // 将TFW模式转换为POSIX模式
    int posixMode = 0;
    if (mode & 0x0) posixMode |= F_OK;  // 文件存在检查
    if (mode & 0x2) posixMode |= W_OK;  // 写权限检查
    if (mode & 0x4) posixMode |= R_OK;  // 读权限检查

    if (access(pathName, posixMode) == -1) {
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

int32_t TFW_MakeDir(const char *pathName, int32_t mode) {
    if (pathName == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_MakeDir");
        return TFW_ERROR_INVALID_PARAM;
    }

    // 将TFW权限转换为POSIX权限
    mode_t posixMode = 0;
    if (mode & TFW_S_IRUSR) posixMode |= S_IRUSR;
    if (mode & TFW_S_IWUSR) posixMode |= S_IWUSR;
    if (mode & TFW_S_IXUSR) posixMode |= S_IXUSR;

    if (mkdir(pathName, posixMode) == -1) {
        TFW_LOGE_UTILS("Failed to create directory %s: %s", pathName, strerror(errno));
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

int32_t TFW_GetFileSize(const char *fileName, uint64_t *fileSize) {
    if (fileName == NULL || fileSize == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_GetFileSize");
        return TFW_ERROR_INVALID_PARAM;
    }

    struct stat st;
    if (stat(fileName, &st) == -1) {
        TFW_LOGE_UTILS("Failed to get file size for %s: %s", fileName, strerror(errno));
        return TFW_ERROR;
    }

    *fileSize = (uint64_t)st.st_size;
    return TFW_SUCCESS;
}

char *TFW_RealPath(const char *path, char *absPath) {
    if (path == NULL || absPath == NULL) {
        TFW_LOGE_UTILS("Invalid parameter for TFW_RealPath");
        return NULL;
    }

    char *result = realpath(path, absPath);
    if (result == NULL) {
        TFW_LOGE_UTILS("Failed to get real path for %s: %s", path, strerror(errno));
        return NULL;
    }

    return result;
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

    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        TFW_LOGE_UTILS("Failed to open file %s: %s", fileName, strerror(errno));
        return TFW_ERROR;
    }

    ssize_t bytesRead = read(fd, readBuf, fileSize);
    if (bytesRead == -1) {
        TFW_LOGE_UTILS("Failed to read file %s: %s", fileName, strerror(errno));
        close(fd);
        return TFW_ERROR;
    }

    readBuf[bytesRead] = '\0'; // 确保字符串终止
    *size = (int32_t)bytesRead;
    close(fd);
    return TFW_SUCCESS;
}