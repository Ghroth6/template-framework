#ifndef TFW_FILE_H
#define TFW_FILE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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
 * @return true for exists, false for not exists
 */
bool TFW_IsFileExists(const char* file_path);

/**
 * Check if path is directory
 * 检查路径是否是目录
 * @param path Path
 * @return true for directory, false for not directory
 */
bool TFW_IsDirectory(const char* path);

/**
 * Create directory
 * 创建目录
 * @param path Directory path to create
 * @return 1 for success, 0 for failure
 */
int32_t TFW_CreateDirectory(const char* path);

// ============================================================================
// File operation function declarations
// 文件操作函数声明
// ============================================================================

/* 文件操作标志 */
#define TFW_O_RDONLY (00)
#define TFW_O_WRONLY (01)
#define TFW_O_RDWR (02)
#define TFW_O_CREATE (0100)
#define TFW_O_TRUNC (01000)

#define TFW_S_IRUSR (0400)
#define TFW_S_IWUSR (0200)
#define TFW_S_IXUSR (0100)

/* 文件操作函数 */
/**
 * Read data from file descriptor
 * 从文件描述符读取数据
 * @param fd File descriptor / 文件描述符
 * @param readBuf Buffer to store read data / 存储读取数据的缓冲区
 * @param maxLen Maximum length to read / 最大读取长度
 * @return Number of bytes read, negative value on error / 读取的字节数，错误时返回负值
 */
int32_t TFW_ReadFile(int32_t fd, void *readBuf, uint32_t maxLen);

/**
 * Read entire file content
 * 读取整个文件内容
 * @param fileName File name / 文件名
 * @param readBuf Buffer to store file content / 存储文件内容的缓冲区
 * @param maxLen Maximum buffer length / 最大缓冲区长度
 * @return Number of bytes read, negative value on error / 读取的字节数，错误时返回负值
 */
int32_t TFW_ReadFullFile(const char *fileName, char *readBuf, uint32_t maxLen);

/**
 * Write data to file
 * 写入文件
 * @param fileName File name / 文件名
 * @param writeBuf Buffer containing data to write / 包含要写入数据的缓冲区
 * @param len Length of data to write / 要写入的数据长度
 * @return Number of bytes written, negative value on error / 写入的字节数，错误时返回负值
 */
int32_t TFW_WriteFile(const char *fileName, const char *writeBuf, uint32_t len);

/**
 * Write data to file descriptor
 * 向文件描述符写入数据
 * @param fd File descriptor / 文件描述符
 * @param writeBuf Buffer containing data to write / 包含要写入数据的缓冲区
 * @param len Length of data to write / 要写入的数据长度
 * @return Number of bytes written, negative value on error / 写入的字节数，错误时返回负值
 */
int32_t TFW_WriteFileFd(int32_t fd, const char *writeBuf, uint32_t len);

/**
 * Open file
 * 打开文件
 * @param fileName File name / 文件名
 * @param flags Open flags / 打开标志
 * @return File descriptor, negative value on error / 文件描述符，错误时返回负值
 */
int32_t TFW_OpenFile(const char *fileName, int32_t flags);

/**
 * Open file with permissions
 * 带权限打开文件
 * @param fileName File name / 文件名
 * @param flags Open flags / 打开标志
 * @param perms File permissions / 文件权限
 * @return File descriptor, negative value on error / 文件描述符，错误时返回负值
 */
int32_t TFW_OpenFileWithPerms(const char *fileName, int32_t flags, int32_t perms);

/**
 * Remove file
 * 删除文件
 * @param fileName File name / 文件名
 */
void TFW_RemoveFile(const char *fileName);

/**
 * Close file
 * 关闭文件
 * @param fd File descriptor / 文件描述符
 */
void TFW_CloseFile(int32_t fd);

/**
 * Read data from file at offset
 * 带偏移量的文件读取
 * @param fd File descriptor / 文件描述符
 * @param buf Buffer to store read data / 存储读取数据的缓冲区
 * @param readBytes Number of bytes to read / 要读取的字节数
 * @param offset Offset in file / 文件中的偏移量
 * @return Number of bytes read, negative value on error / 读取的字节数，错误时返回负值
 */
int64_t TFW_PreadFile(int32_t fd, void *buf, uint64_t readBytes, uint64_t offset);

/**
 * Write data to file at offset
 * 带偏移量的文件写入
 * @param fd File descriptor / 文件描述符
 * @param buf Buffer containing data to write / 包含要写入数据的缓冲区
 * @param writeBytes Number of bytes to write / 要写入的字节数
 * @param offset Offset in file / 文件中的偏移量
 * @return Number of bytes written, negative value on error / 写入的字节数，错误时返回负值
 */
int64_t TFW_PwriteFile(int32_t fd, const void *buf, uint64_t writeBytes, uint64_t offset);

/**
 * Check file access permissions
 * 检查文件访问权限
 * @param pathName Path name / 路径名
 * @param mode Access mode / 访问模式
 * @return 0 on success, negative value on error / 成功返回0，错误时返回负值
 */
int32_t TFW_AccessFile(const char *pathName, int32_t mode);

/**
 * Create directory
 * 创建目录
 * @param pathName Path name / 路径名
 * @param mode Directory permissions / 目录权限
 * @return 0 on success, negative value on error / 成功返回0，错误时返回负值
 */
int32_t TFW_MakeDir(const char *pathName, int32_t mode);

/**
 * Get file size
 * 获取文件大小
 * @param fileName File name / 文件名
 * @param fileSize Pointer to store file size / 存储文件大小的指针
 * @return 0 on success, negative value on error / 成功返回0，错误时返回负值
 */
int32_t TFW_GetFileSize(const char *fileName, uint64_t *fileSize);

/**
 * Get real path of file
 * 获取文件真实路径
 * @param path Path / 路径
 * @param absPath Buffer to store absolute path / 存储绝对路径的缓冲区
 * @return Pointer to absolute path, NULL on error / 指向绝对路径的指针，错误时返回NULL
 */
char *TFW_RealPath(const char *path, char *absPath);

/**
 * Read entire file content and size
 * 读取整个文件内容并返回文件大小
 * @param fileName File name / 文件名
 * @param readBuf Buffer to store file content / 存储文件内容的缓冲区
 * @param maxLen Maximum buffer length / 最大缓冲区长度
 * @param size Pointer to store file size / 存储文件大小的指针
 * @return 0 on success, negative value on error / 成功返回0，错误时返回负值
 */
int32_t TFW_ReadFullFileAndSize(const char *fileName, char *readBuf, uint32_t maxLen, int32_t *size);

#ifdef __cplusplus
}
#endif

#endif // TFW_FILE_H