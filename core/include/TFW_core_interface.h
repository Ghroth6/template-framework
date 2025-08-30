#ifndef TFW_CORE_INTERFACE_H
#define TFW_CORE_INTERFACE_H

#include "TFW_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// TFW core module C interface declaration
// ============================================================================

/**
 * initialize TFW core module
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceInit(void);

/**
 * deinitialize TFW core module
 * @return success return TFW_SUCCESS, failure return error code
 */
int32_t TFW_CoreInterfaceDeinit(void);

#ifdef __cplusplus
}
#endif

#endif // TFW_CORE_INTERFACE_H
