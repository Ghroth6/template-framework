/*
 * TFW_json.h
 *
 * TFW JSON操作工具头文件
 *
 * 该文件定义了JSON操作相关的函数接口
 */

#ifndef TFW_JSON_H
#define TFW_JSON_H

#include <stdint.h>
#include "cJSON.h"
#include "string.h"
#include "stdbool.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @brief 从JSON对象中获取字符串项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标字符串缓冲区
 * @param targetLen 目标缓冲区长度
 * @return int32_t 成功返回0，失败返回非0值
 */
int32_t GetStringItemByJsonObject(const cJSON *json, const char * const string, char *target, uint32_t targetLen);

/**
 * @brief 从JSON对象中获取字符串项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标字符串缓冲区
 * @param targetLen 目标缓冲区长度
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectStringItem(const cJSON *json, const char * const string, char *target, uint32_t targetLen);

/**
 * @brief 从JSON对象中获取16位无符号整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectNumber16Item(const cJSON *json, const char * const string, uint16_t *target);

/**
 * @brief 从JSON对象中获取32位整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectNumberItem(const cJSON *json, const char * const string, int32_t *target);

/**
 * @brief 从JSON对象中获取32位有符号整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectSignedNumberItem(const cJSON *json, const char * const string, int32_t *target);

/**
 * @brief 从JSON对象中获取64位整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectNumber64Item(const cJSON *json, const char * const string, int64_t *target);

/**
 * @brief 从JSON对象中获取64位有符号整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectSignedNumber64Item(const cJSON *json, const char * const string, int64_t *target);

/**
 * @brief 从JSON对象中获取双精度浮点数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectDoubleItem(const cJSON *json, const char * const string, double *target);

/**
 * @brief 从JSON对象中获取布尔值项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectBoolItem(const cJSON *json, const char * const string, bool *target);

/**
 * @brief 向JSON对象中添加字符串项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param value 字符串值
 * @return bool 成功返回true，失败返回false
 */
bool AddStringToJsonObject(cJSON *json, const char * const string, const char *value);

/**
 * @brief 向JSON对象中添加字符串数组项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param strings 字符串数组
 * @param count 数组元素个数
 * @return bool 成功返回true，失败返回false
 */
bool AddStringArrayToJsonObject(cJSON *json, const char * const string, const char * const *strings, int32_t count);

/**
 * @brief 向JSON对象中添加16位无符号整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param num 数值
 * @return bool 成功返回true，失败返回false
 */
bool AddNumber16ToJsonObject(cJSON *json, const char * const string, uint16_t num);

/**
 * @brief 向JSON对象中添加32位整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param num 数值
 * @return bool 成功返回true，失败返回false
 */
bool AddNumberToJsonObject(cJSON *json, const char * const string, int32_t num);

/**
 * @brief 向JSON对象中添加64位整数项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param num 数值
 * @return bool 成功返回true，失败返回false
 */
bool AddNumber64ToJsonObject(cJSON *json, const char * const string, int64_t num);

/**
 * @brief 向JSON对象中添加布尔值项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param value 布尔值
 * @return bool 成功返回true，失败返回false
 */
bool AddBoolToJsonObject(cJSON *json, const char * const string, bool value);

/**
 * @brief 从JSON对象中获取32位整数项（别名函数）
 * 
 * @param json JSON对象
 * @param string 键名
 * @param target 目标变量指针
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectInt32Item(const cJSON *json, const char * const string, int32_t *target);

/**
 * @brief 从JSON对象中获取动态分配的字符串项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param limit 字符串长度限制
 * @return char* 动态分配的字符串，需要调用者释放内存
 */
char *GetDynamicStringItemByJsonObject(const cJSON * const json, const char * const string, uint32_t limit);

/**
 * @brief 向JSON对象中添加整数数组项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param array 整数数组
 * @param arrayLen 数组长度
 * @return bool 成功返回true，失败返回false
 */
bool AddIntArrayToJsonObject(cJSON *json, const char *string, const int32_t *array, int32_t arrayLen);

/**
 * @brief 从JSON对象中获取整数数组项
 * 
 * @param json JSON对象
 * @param string 键名
 * @param array 目标整数数组
 * @param arrayLen 数组长度
 * @return bool 成功返回true，失败返回false
 */
bool GetJsonObjectIntArrayItem(const cJSON *json, const char *string, int32_t *array, int32_t arrayLen);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TFW_JSON_H */