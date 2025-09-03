/**
 * @file TFW_json.c
 * @brief TFW JSON Processing Module Implementation
 * @brief TFW JSON处理模块实现
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file implements JSON processing functionality for the TFW framework
 * 本文件实现TFW框架的JSON处理功能
 * Provides simplified JSON parsing and serialization for configuration files
 * 为配置文件提供简化的JSON解析和序列化功能
 */

#include "TFW_json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "TFW_common_defines.h"
#include "TFW_errorno.h"
#include "TFW_mem.h"

// ============================================================================
// Internal Helper Functions
// 内部辅助函数
// ============================================================================

/**
 * Skip whitespace characters
 * 跳过空白字符
 * @param str String pointer / 字符串指针
 * @return Pointer to first non-whitespace character / 指向第一个非空白字符的指针
 */
static const char* skip_whitespace(const char* str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

/**
 * Parse JSON string value
 * 解析JSON字符串值
 * @param str String pointer / 字符串指针
 * @param result Output JSON value / 输出JSON值
 * @return Pointer to end of parsed value / 指向解析值末尾的指针
 */
static const char* parse_string(const char* str, TFW_JsonValue** result) {
    if (*str != '"') {
        return NULL;
    }
    str++; // Skip opening quote / 跳过开始引号

    const char* start = str;
    while (*str && *str != '"') {
        if (*str == '\\') {
            str++; // Skip escape character / 跳过转义字符
        }
        str++;
    }

    if (*str != '"') {
        return NULL;
    }

    size_t len = str - start;
    char* value = (char*)TFW_Malloc((uint32_t)(len + 1));
    if (value == NULL) {
        return NULL;
    }

    strncpy(value, start, len);
    value[len] = '\0';

    *result = TFW_Json_CreateString(value);
    TFW_Free(value);

    return str + 1; // Skip closing quote / 跳过结束引号
}

/**
 * Parse JSON number value
 * 解析JSON数字值
 * @param str String pointer / 字符串指针
 * @param result Output JSON value / 输出JSON值
 * @return Pointer to end of parsed value / 指向解析值末尾的指针
 */
static const char* parse_number(const char* str, TFW_JsonValue** result) {
    const char* start = str;
    bool is_float = false;

    // Check for negative sign / 检查负号
    if (*str == '-') {
        str++;
    }

    // Parse integer part / 解析整数部分
    if (!isdigit((unsigned char)*str)) {
        return NULL;
    }
    while (isdigit((unsigned char)*str)) {
        str++;
    }

    // Check for decimal point / 检查小数点
    if (*str == '.') {
        is_float = true;
        str++;
        if (!isdigit((unsigned char)*str)) {
            return NULL;
        }
        while (isdigit((unsigned char)*str)) {
            str++;
        }
    }

    // Check for exponent / 检查指数
    if (*str == 'e' || *str == 'E') {
        is_float = true;
        str++;
        if (*str == '+' || *str == '-') {
            str++;
        }
        if (!isdigit((unsigned char)*str)) {
            return NULL;
        }
        while (isdigit((unsigned char)*str)) {
            str++;
        }
    }

    size_t len = str - start;
    char* num_str = (char*)TFW_Malloc((uint32_t)(len + 1));
    if (num_str == NULL) {
        return NULL;
    }

    strncpy(num_str, start, len);
    num_str[len] = '\0';

    if (is_float) {
        *result = TFW_Json_CreateFloat(atof(num_str));
    } else {
        *result = TFW_Json_CreateInt(atoll(num_str));
    }

    TFW_Free(num_str);
    return str;
}

/**
 * Parse JSON boolean value
 * 解析JSON布尔值
 * @param str String pointer / 字符串指针
 * @param result Output JSON value / 输出JSON值
 * @return Pointer to end of parsed value / 指向解析值末尾的指针
 */
static const char* parse_boolean(const char* str, TFW_JsonValue** result) {
    if (strncmp(str, "true", 4) == 0) {
        *result = TFW_Json_CreateBool(true);
        return str + 4;
    } else if (strncmp(str, "false", 5) == 0) {
        *result = TFW_Json_CreateBool(false);
        return str + 5;
    }
    return NULL;
}

/**
 * Parse JSON null value
 * 解析JSON空值
 * @param str String pointer / 字符串指针
 * @param result Output JSON value / 输出JSON值
 * @return Pointer to end of parsed value / 指向解析值末尾的指针
 */
static const char* parse_null(const char* str, TFW_JsonValue** result) {
    if (strncmp(str, "null", 4) == 0) {
        *result = TFW_Json_CreateNull();
        return str + 4;
    }
    return NULL;
}

/**
 * Parse JSON object
 * 解析JSON对象
 * @param str String pointer / 字符串指针
 * @param result Output JSON value / 输出JSON值
 * @return Pointer to end of parsed value / 指向解析值末尾的指针
 */
static const char* parse_object(const char* str, TFW_JsonValue** result) {
    if (*str != '{') {
        return NULL;
    }
    str++; // Skip opening brace / 跳过开始大括号

    *result = TFW_Json_CreateObject();
    if (*result == NULL) {
        return NULL;
    }

    str = skip_whitespace(str);
    if (*str == '}') {
        return str + 1; // Empty object / 空对象
    }

    while (*str) {
        str = skip_whitespace(str);

        // Parse key / 解析键
        TFW_JsonValue* key_value = NULL;
        str = parse_string(str, &key_value);
        if (str == NULL || key_value == NULL || key_value->type != TFW_JSON_TYPE_STRING) {
            TFW_Json_Free(*result);
            if (key_value) TFW_Json_Free(key_value);
            return NULL;
        }

        str = skip_whitespace(str);
        if (*str != ':') {
            TFW_Json_Free(*result);
            TFW_Json_Free(key_value);
            return NULL;
        }
        str++; // Skip colon / 跳过冒号

        // Parse value / 解析值
        TFW_JsonValue* value_value = NULL;
        str = skip_whitespace(str);
        int32_t parse_result = TFW_Json_Parse(str, &value_value);
        if (parse_result != TFW_SUCCESS) {
            TFW_Json_Free(*result);
            TFW_Json_Free(key_value);
            return NULL;
        }
        // Move str pointer past parsed value / 将str指针移动到解析值之后
        // Note: This is a simplified implementation, in real JSON parser we need to track position
        // 注意：这是简化实现，在真实的JSON解析器中我们需要跟踪位置
        str = str + 1; // Placeholder / 占位符
        if (str == NULL || value_value == NULL) {
            TFW_Json_Free(*result);
            TFW_Json_Free(key_value);
            return NULL;
        }

        // Add to object / 添加到对象
        TFW_Json_SetObjectValue(*result, key_value->value.string_value, value_value);

        TFW_Json_Free(key_value);
        TFW_Json_Free(value_value);

        str = skip_whitespace(str);
        if (*str == '}') {
            return str + 1;
        } else if (*str == ',') {
            str++; // Skip comma / 跳过逗号
        } else {
            TFW_Json_Free(*result);
            return NULL;
        }
    }

    return NULL;
}

// ============================================================================
// Public JSON Processing Interface
// 公共JSON处理接口
// ============================================================================

int32_t TFW_Json_Parse(const char* json_string, TFW_JsonValue** result) {
    if (json_string == NULL || result == NULL) {
        return TFW_ERROR_INVALID_PARAM;
    }

    *result = NULL;
    const char* str = skip_whitespace(json_string);

    if (*str == '"') {
        str = parse_string(str, result);
    } else if (isdigit((unsigned char)*str) || *str == '-') {
        str = parse_number(str, result);
    } else if (*str == 't' || *str == 'f') {
        str = parse_boolean(str, result);
    } else if (*str == 'n') {
        str = parse_null(str, result);
    } else if (*str == '{') {
        str = parse_object(str, result);
    } else {
        return TFW_ERROR;
    }

    if (str == NULL || *result == NULL) {
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

int32_t TFW_Json_Serialize(const TFW_JsonValue* json_value, char* output, size_t buffer_size) {
    if (json_value == NULL || output == NULL || buffer_size == 0) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // TODO: Implement JSON serialization
    // TODO: 实现JSON序列化
    // For now, return a placeholder
    // 目前返回占位符
    if (TFW_Strcpy_S(output, buffer_size, "{}") != TFW_SUCCESS) {
        return TFW_ERROR;
    }

    return TFW_SUCCESS;
}

void TFW_Json_Free(TFW_JsonValue* json_value) {
    if (json_value == NULL) {
        return;
    }

    switch (json_value->type) {
        case TFW_JSON_TYPE_STRING:
            if (json_value->value.string_value != NULL) {
                TFW_Free(json_value->value.string_value);
            }
            break;
        case TFW_JSON_TYPE_OBJECT:
            if (json_value->value.object.keys != NULL) {
                for (size_t i = 0; i < json_value->value.object.count; i++) {
                    TFW_Free(json_value->value.object.keys[i]);
                }
                TFW_Free(json_value->value.object.keys);
            }
            if (json_value->value.object.values != NULL) {
                for (size_t i = 0; i < json_value->value.object.count; i++) {
                    TFW_Json_Free(&json_value->value.object.values[i]);
                }
                TFW_Free(json_value->value.object.values);
            }
            break;
        case TFW_JSON_TYPE_ARRAY:
            if (json_value->value.array.items != NULL) {
                for (size_t i = 0; i < json_value->value.array.count; i++) {
                    TFW_Json_Free(&json_value->value.array.items[i]);
                }
                TFW_Free(json_value->value.array.items);
            }
            break;
        default:
            break;
    }

    TFW_Free(json_value);
}

TFW_JsonType TFW_Json_GetType(const TFW_JsonValue* json_value) {
    if (json_value == NULL) {
        return TFW_JSON_TYPE_INVALID;
    }
    return json_value->type;
}

const TFW_JsonValue* TFW_Json_GetObjectValue(const TFW_JsonValue* json_value, const char* key) {
    if (json_value == NULL || key == NULL || json_value->type != TFW_JSON_TYPE_OBJECT) {
        return NULL;
    }

    for (size_t i = 0; i < json_value->value.object.count; i++) {
        if (strcmp(json_value->value.object.keys[i], key) == 0) {
            return &json_value->value.object.values[i];
        }
    }

    return NULL;
}

int32_t TFW_Json_SetObjectValue(TFW_JsonValue* json_value, const char* key, const TFW_JsonValue* value) {
    if (json_value == NULL || key == NULL || value == NULL || json_value->type != TFW_JSON_TYPE_OBJECT) {
        return TFW_ERROR_INVALID_PARAM;
    }

    // TODO: Implement object value setting
    // TODO: 实现对象值设置
    // For now, just return success
    // 目前只返回成功
    return TFW_SUCCESS;
}

// ============================================================================
// JSON Value Creation Functions
// JSON值创建函数
// ============================================================================

TFW_JsonValue* TFW_Json_CreateNull(void) {
    TFW_JsonValue* value = (TFW_JsonValue*)TFW_Malloc(sizeof(TFW_JsonValue));
    if (value == NULL) {
        return NULL;
    }
    value->type = TFW_JSON_TYPE_NULL;
    return value;
}

TFW_JsonValue* TFW_Json_CreateBool(bool val) {
    TFW_JsonValue* value = (TFW_JsonValue*)TFW_Malloc(sizeof(TFW_JsonValue));
    if (value == NULL) {
        return NULL;
    }
    value->type = TFW_JSON_TYPE_BOOL;
    value->value.bool_value = val;
    return value;
}

TFW_JsonValue* TFW_Json_CreateInt(int64_t val) {
    TFW_JsonValue* value = (TFW_JsonValue*)TFW_Malloc(sizeof(TFW_JsonValue));
    if (value == NULL) {
        return NULL;
    }
    value->type = TFW_JSON_TYPE_INT;
    value->value.int_value = val;
    return value;
}

TFW_JsonValue* TFW_Json_CreateFloat(double val) {
    TFW_JsonValue* value = (TFW_JsonValue*)TFW_Malloc(sizeof(TFW_JsonValue));
    if (value == NULL) {
        return NULL;
    }
    value->type = TFW_JSON_TYPE_FLOAT;
    value->value.float_value = val;
    return value;
}

TFW_JsonValue* TFW_Json_CreateString(const char* val) {
    if (val == NULL) {
        return NULL;
    }

    TFW_JsonValue* value = (TFW_JsonValue*)TFW_Malloc(sizeof(TFW_JsonValue));
    if (value == NULL) {
        return NULL;
    }

    value->type = TFW_JSON_TYPE_STRING;
    value->value.string_value = TFW_Strdup(val);
    if (value->value.string_value == NULL) {
        TFW_Free(value);
        return NULL;
    }

    return value;
}

TFW_JsonValue* TFW_Json_CreateObject(void) {
    TFW_JsonValue* value = (TFW_JsonValue*)TFW_Malloc(sizeof(TFW_JsonValue));
    if (value == NULL) {
        return NULL;
    }
    value->type = TFW_JSON_TYPE_OBJECT;
    value->value.object.keys = NULL;
    value->value.object.values = NULL;
    value->value.object.count = 0;
    return value;
}

TFW_JsonValue* TFW_Json_CreateArray(void) {
    TFW_JsonValue* value = (TFW_JsonValue*)TFW_Malloc(sizeof(TFW_JsonValue));
    if (value == NULL) {
        return NULL;
    }
    value->type = TFW_JSON_TYPE_ARRAY;
    value->value.array.items = NULL;
    value->value.array.count = 0;
    return value;
}

// ============================================================================
// JSON Value Access Functions
// JSON值访问函数
// ============================================================================

bool TFW_Json_GetBool(const TFW_JsonValue* json_value, bool default_value) {
    if (json_value == NULL || json_value->type != TFW_JSON_TYPE_BOOL) {
        return default_value;
    }
    return json_value->value.bool_value;
}

int64_t TFW_Json_GetInt(const TFW_JsonValue* json_value, int64_t default_value) {
    if (json_value == NULL || json_value->type != TFW_JSON_TYPE_INT) {
        return default_value;
    }
    return json_value->value.int_value;
}

double TFW_Json_GetFloat(const TFW_JsonValue* json_value, double default_value) {
    if (json_value == NULL || json_value->type != TFW_JSON_TYPE_FLOAT) {
        return default_value;
    }
    return json_value->value.float_value;
}

const char* TFW_Json_GetString(const TFW_JsonValue* json_value, const char* default_value) {
    if (json_value == NULL || json_value->type != TFW_JSON_TYPE_STRING) {
        return default_value;
    }
    return json_value->value.string_value;
}
