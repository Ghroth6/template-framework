#ifndef TFW_TYPES_H
#define TFW_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// 鍩虹绫诲瀷瀹氫箟 - C璇█鍏煎
// ============================================================================

// 鍩虹绫诲瀷妯℃澘
typedef struct {
    int id;
    char* name;
    void* data;
} TFW_Type;

#ifdef __cplusplus
}
#endif

#endif // TFW_TYPES_H
