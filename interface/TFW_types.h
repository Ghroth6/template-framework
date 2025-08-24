#ifndef TFW_TYPES_H
#define TFW_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 基础类型定义 - C语言兼容
// ============================================================================

// 基础类型模板
typedef struct {
    int32_t id;
    int32_t data;
} TFW_Type;

#ifdef __cplusplus
}
#endif

#endif // TFW_TYPES_H
