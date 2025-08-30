/**
 * @file TFW_types.h
 * @brief TFW Framework Type Definitions
 * @brief TFW框架类型定义
 * @author TFW Team
 * @date 2025
 * @version 1.0.0
 *
 * This file contains basic type definitions for the TFW framework
 * 本文件包含TFW框架的基本类型定义
 * All types are designed to be C language compatible for cross-platform support
 * 所有类型都设计为C语言兼容，以支持跨平台
 */

#ifndef TFW_TYPES_H
#define TFW_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Default types - C language compatible
// 默认类型 - C语言兼容
// ============================================================================

// Basic type template
// 基础类型模板
typedef struct {
    int32_t id;
    int32_t value;
} TFW_Type;

#ifdef __cplusplus
}
#endif

#endif // TFW_TYPES_H
