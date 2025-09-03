/**
 * @file TFW_json.h
 * @brief TFW JSON Processing Module
 * @brief TFW JSON处理模块
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file provides JSON processing functionality for the TFW framework
 * 本文件为TFW框架提供JSON处理功能
 * Supports parsing and serialization of JSON data
 * 支持JSON数据的解析和序列化
 */

#ifndef TFW_JSON_H
#define TFW_JSON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// JSON Data Types
// JSON数据类型
// ============================================================================

/**
 * JSON value types
 * JSON值类型
 */
typedef enum {
    TFW_JSON_TYPE_NULL = 0,    // null / 空值
    TFW_JSON_TYPE_BOOL,        // boolean / 布尔值
    TFW_JSON_TYPE_INT,         // integer / 整数
    TFW_JSON_TYPE_FLOAT,       // float / 浮点数
    TFW_JSON_TYPE_STRING,      // string / 字符串
    TFW_JSON_TYPE_OBJECT,      // object / 对象
    TFW_JSON_TYPE_ARRAY,       // array / 数组
    TFW_JSON_TYPE_INVALID      // invalid / 无效类型
} TFW_JsonType;

/**
 * JSON value structure
 * JSON值结构
 */
typedef struct TFW_JsonValue {
    TFW_JsonType type;         // Value type / 值类型
    union {
        bool bool_value;       // Boolean value / 布尔值
        int64_t int_value;     // Integer value / 整数值
        double float_value;    // Float value / 浮点值
        char* string_value;    // String value / 字符串值
        struct {
            struct TFW_JsonValue* items;  // Array items / 数组项
            size_t count;                 // Item count / 项数量
        } array;
        struct {
            char** keys;                  // Object keys / 对象键
            struct TFW_JsonValue* values; // Object values / 对象值
            size_t count;                 // Key-value count / 键值对数量
        } object;
    } value;
} TFW_JsonValue;

// ============================================================================
// JSON Processing Interface
// JSON处理接口
// ============================================================================

/**
 * Parse JSON string to JSON value
 * 解析JSON字符串为JSON值
 * @param json_string JSON string to parse / 要解析的JSON字符串
 * @param result Output JSON value / 输出JSON值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Json_Parse(const char* json_string, TFW_JsonValue** result);

/**
 * Serialize JSON value to string
 * 将JSON值序列化为字符串
 * @param json_value JSON value to serialize / 要序列化的JSON值
 * @param output Output string buffer / 输出字符串缓冲区
 * @param buffer_size Buffer size / 缓冲区大小
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Json_Serialize(const TFW_JsonValue* json_value, char* output, size_t buffer_size);

/**
 * Free JSON value memory
 * 释放JSON值内存
 * @param json_value JSON value to free / 要释放的JSON值
 */
void TFW_Json_Free(TFW_JsonValue* json_value);

/**
 * Get JSON value type
 * 获取JSON值类型
 * @param json_value JSON value / JSON值
 * @return JSON value type / JSON值类型
 */
TFW_JsonType TFW_Json_GetType(const TFW_JsonValue* json_value);

/**
 * Get JSON object value by key
 * 根据键获取JSON对象值
 * @param json_value JSON object value / JSON对象值
 * @param key Key to search / 要搜索的键
 * @return JSON value pointer, NULL if not found / JSON值指针，未找到返回NULL
 */
const TFW_JsonValue* TFW_Json_GetObjectValue(const TFW_JsonValue* json_value, const char* key);

/**
 * Set JSON object value by key
 * 根据键设置JSON对象值
 * @param json_value JSON object value / JSON对象值
 * @param key Key to set / 要设置的键
 * @param value Value to set / 要设置的值
 * @return TFW_SUCCESS on success, negative value on error / 成功返回TFW_SUCCESS，失败返回负值
 */
int32_t TFW_Json_SetObjectValue(TFW_JsonValue* json_value, const char* key, const TFW_JsonValue* value);

/**
 * Create JSON value from primitive types
 * 从基本类型创建JSON值
 */
TFW_JsonValue* TFW_Json_CreateNull(void);
TFW_JsonValue* TFW_Json_CreateBool(bool value);
TFW_JsonValue* TFW_Json_CreateInt(int64_t value);
TFW_JsonValue* TFW_Json_CreateFloat(double value);
TFW_JsonValue* TFW_Json_CreateString(const char* value);
TFW_JsonValue* TFW_Json_CreateObject(void);
TFW_JsonValue* TFW_Json_CreateArray(void);

/**
 * Get primitive values from JSON value
 * 从JSON值获取基本类型值
 */
bool TFW_Json_GetBool(const TFW_JsonValue* json_value, bool default_value);
int64_t TFW_Json_GetInt(const TFW_JsonValue* json_value, int64_t default_value);
double TFW_Json_GetFloat(const TFW_JsonValue* json_value, double default_value);
const char* TFW_Json_GetString(const TFW_JsonValue* json_value, const char* default_value);

#ifdef __cplusplus
}
#endif

#endif // TFW_JSON_H
